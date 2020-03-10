#include "usermanagement.h"

UserManagement::UserManagement() {}

UserManagement::~UserManagement()
{
    if (isInit)
    {
        closeDB();
        delete userModel;
    }
}

void UserManagement::init()
{
    if (QSqlDatabase::contains(ConnectionName))
    {
        db = QSqlDatabase::database(ConnectionName);
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE", ConnectionName);
        db.setDatabaseName(DBName);
        db.setUserName(DBUserName);
        db.setPassword(DBPassword);
    }
    if (!openDB())
    {
        return;
    }

    if (!isTableExist("user"))
    {
        QSqlQuery createTable(db);
        auto res = createTable.prepare(
            "create table user(name varchar(120) primary key, password varchar(120), authority int)");
        if (!res)
        {
            qCritical() << createTable.lastError().text();
        }
        if (!createTable.exec())
        {
            qCritical() << createTable.lastError().text();
            return;
        }
    }

    if (!hasUser("Admin"))
    {
        _addUser("Admin", "SilicolAdmin", Admin);
    }
    userModel = new MySqlTableModel(nullptr, db);
    userModel->setTable("user");

    userModel->select();
    isInit = true;
}

void UserManagement::addUser(QString userName, QString password, UserManagement::Authority authority)
{
    if (currentAuthority() != Admin)
    {
        UIOperation::getIns()->showMessage(tr("Authority Error"), tr("Only Admin can add user!"), MsgBoxIcon::Error,
                                           OkBtn);
        return;
    }
    int autho = static_cast<int>(authority);
    if (autho >= Admin || autho <= None)
    {

        UIOperation::getIns()->showMessage(tr("Authority Error"), tr("Please select correct authority!"),
                                           MsgBoxIcon::Error, OkBtn);
        return;
    }
    if (userName.length() < MinUserNameLen)
    {
        UIOperation::getIns()->showMessage(tr("UserName Error"),
                                           tr("User name was too short! Min length: %1").arg(MinUserNameLen),
                                           MsgBoxIcon::Error, OkBtn);
        return;
    }
    if (password.length() < 6)
    {
        UIOperation::getIns()->showMessage(tr("Password Error"),
                                           tr("Password was too short! Min length: %1").arg(MinPasswordLen),
                                           MsgBoxIcon::Error, OkBtn);
        return;
    }
    if (hasUser(userName))
    {
        UIOperation::getIns()->showMessage(tr("UserName Error"), tr("User name already existed!"), MsgBoxIcon::Error,
                                           OkBtn);
        return;
    }
    _addUser(userName, password, authority);
}

void UserManagement::removeUser(QString userName)
{
    if (currentAuthority() != Admin)
    {
        UIOperation::getIns()->showMessage(tr("Authority Error"), tr("Only Admin can remove user!"), MsgBoxIcon::Error,
                                           OkBtn);
        return;
    }

    QString password;
    Authority authority;
    if (!getUserInfo(userName, password, authority))
    {
        return;
    }
    if (authority == Admin)
    {
        UIOperation::getIns()->showMessage(tr("UserName Error"), tr("Can not remove Admin account!"), MsgBoxIcon::Error,
                                           OkBtn);
        return;
    }
    QSqlQuery remove(db);
    remove.prepare("delete from user where name=?");
    remove.addBindValue(userName);
    if (!remove.exec())
    {
        qCritical() << tr("Remove user %1 failed: ").arg(userName) << remove.lastError();
        return;
    }
    userModel->select();
}

bool UserManagement::changePassword(QString userName,
                                    QString oldPassword,
                                    QString newPassword,
                                    QString newPasswordRepeat)
{
    if (newPassword != newPasswordRepeat)
    {
        UIOperation::getIns()->showMessage(tr("Password Error"), tr("Repeat password error!"), MsgBoxIcon::Error,
                                           OkBtn);
        return false;
    }
    if (newPassword.length() < 6)
    {
        UIOperation::getIns()->showMessage(tr("Password Error"),
                                           tr("Password was too short! Min length: %1").arg(MinPasswordLen),
                                           MsgBoxIcon::Error, OkBtn);
        return false;
    }
    if (!verifyUserPsw(userName, oldPassword))
    {
        return false;
    }
    QSqlQuery changePsw(db);
    changePsw.prepare("update user set password=? where name=?");
    changePsw.addBindValue(cryptograph(newPassword));
    changePsw.addBindValue(userName);
    if (!changePsw.exec())
    {
        UIOperation::getIns()->showMessage(tr("Change password failed"), changePsw.lastError().text(),
                                           MsgBoxIcon::Error, OkBtn);
        return false;
    }
    return true;
}

bool UserManagement::login(QString userName, QString password)
{
    QString queriedPsw;
    Authority queriedAuthrity;
    if (getUserInfo(userName, queriedPsw, queriedAuthrity))
    {
        if (queriedPsw != cryptograph(password))
        {
            UIOperation::getIns()->showMessage(tr("Password Error"), tr("Please input correct password"),
                                               MsgBoxIcon::Error, OkBtn);
            return false;
        }
        setCurrentUserName(userName);
        setCurrentAuthority(queriedAuthrity);
        setHasLogin(true);
        return true;
    }
    return false;
}

void UserManagement::logout()
{
    setCurrentUserName("");
    setCurrentAuthority(None);
    setHasLogin(false);
}

void UserManagement::_addUser(QString userName, QString password, UserManagement::Authority authority)
{
    QSqlQuery sAddUser(db);
    sAddUser.prepare("insert into user(name, password, authority) values(?,?,?)");
    sAddUser.addBindValue(userName);
    sAddUser.addBindValue(cryptograph(password));
    sAddUser.addBindValue(static_cast<int>(authority));
    if (!sAddUser.exec())
    {
        qCritical() << tr("Add user %1 failed: ").arg(userName) << sAddUser.lastError();
        return;
    }
    userModel->select();
}

bool UserManagement::hasUser(QString userName)
{
    QString queriedPsw;
    Authority queriedAuthrity;
    return getUserInfo(userName, queriedPsw, queriedAuthrity, false);
}

bool UserManagement::isTableExist(QString tableName)
{
    QSqlQuery isTableExist(db);
    if (!isTableExist.prepare("select count(*) from sqlite_master where type='table' and name=?"))
    {
        qCritical() << isTableExist.lastError();
        return false;
    }
    isTableExist.addBindValue(tableName);
    if (!isTableExist.exec())
    {
        qCritical() << isTableExist.lastError();
        return false;
    }
    if (isTableExist.next())
    {
        return isTableExist.value(0).toInt() != 0;
    }
    else
    {
        return false;
    }
}

bool UserManagement::verifyUserPsw(QString userName, QString password)
{
    QString queriedPsw;
    Authority queriedAuthrity;
    if (getUserInfo(userName, queriedPsw, queriedAuthrity))
    {
        if (queriedPsw != cryptograph(password))
        {
            UIOperation::getIns()->showMessage(tr("Password Error"), tr("Please input correct password"),
                                               MsgBoxIcon::Error, OkBtn);
            return false;
        }
        return true;
    }
    return false;
}

bool UserManagement::getUserInfo(QString userName,
                                 QString &password,
                                 UserManagement::Authority &authority,
                                 bool showMsgBoxAsUserDidNotExist)
{
    QSqlQuery query(db);
    auto res = query.prepare("select * from user where name=?");
    if (!res)
    {
        qCritical() << query.lastError();
    }
    query.addBindValue(userName);
    if (!query.exec())
    {
        UIOperation::getIns()->showMessage(tr("Query user %1 failed").arg(userName), query.lastError().text(),
                                           MsgBoxIcon::Error, OkBtn);
        return false;
    }
    if (query.next())
    {
        password = query.value("password").toString();
        authority = static_cast<Authority>(query.value("authority").toInt());
        return true;
    }
    else
    {
        if (showMsgBoxAsUserDidNotExist)
        {
            UIOperation::getIns()->showMessage(tr("UserName Error"), tr("User %1 did not exist").arg(userName),
                                               MsgBoxIcon::Error, OkBtn);
        }
        return false;
    }
}
