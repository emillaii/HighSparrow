#include "usermanagement.h"

UserManagement::UserManagement()
{
    setCurrentUserName("None");
}

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
        auto res = createTable.prepare("create table user(name varchar(120) primary key, password varchar(120), authority int)");
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

int UserManagement::userCount() const
{
    QSqlQuery sqlStatement("select * from user", db);
    sqlStatement.exec();
    int countOfUser = 0;
    while (sqlStatement.next())
    {
        countOfUser++;
    }
    return countOfUser;
}

void UserManagement::addUser(QString userName, QString password, UserManagement::Authority authority)
{
    if(userCount() >= 5)
    {
        SI::ui.showMessage(tr("User Number Error"), tr("Maximum 5 user accounts!"), MsgBoxIcon::Error, SI::ui.Ok);
        return;
    }
    if(currentAuthority() != Admin)
    {
        SI::ui.showMessage(tr("Authority Error"), tr("Only Admin can add user!"), MsgBoxIcon::Error, SI::ui.Ok);
        return;
    }
    int autho = static_cast<int>(authority);
    if (autho >= Admin || autho <= None)
    {
        SI::ui.showMessage(tr("Authority Error"), tr("Please select correct authority!"), MsgBoxIcon::Error, SI::ui.Ok);
        return;
    }
    if (userName.length() < MinUserNameLen)
    {
        SI::ui.showMessage(tr("UserName Error"), tr("User name was too short! Min length: %1").arg(MinUserNameLen),
                           MsgBoxIcon::Error, SI::ui.Ok);
        return;
    }
    if (password.length() < 6)
    {
        SI::ui.showMessage(tr("Password Error"), tr("Password was too short! Min length: %1").arg(MinPasswordLen),
                           MsgBoxIcon::Error, SI::ui.Ok);
        return;
    }
    if (hasUser(userName))
    {
        SI::ui.showMessage(tr("UserName Error"), tr("User name already existed!"), MsgBoxIcon::Error, SI::ui.Ok);
        return;
    }
    _addUser(userName, password, authority);
}

void UserManagement::removeUser(QString userName)
{
    if (currentAuthority() != Admin)
    {
        SI::ui.showMessage(tr("Authority Error"), tr("Only Admin can remove user!"), MsgBoxIcon::Error, SI::ui.Ok);
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
        SI::ui.showMessage(tr("UserName Error"), tr("Can not remove Admin account!"), MsgBoxIcon::Error, SI::ui.Ok);
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
        SI::ui.showMessage(tr("Password Error"), tr("Repeat password error!"), MsgBoxIcon::Error, SI::ui.Ok);
        return false;
    }
    if (newPassword.length() < 6)
    {
        SI::ui.showMessage(tr("Password Error"), tr("Password was too short! Min length: %1").arg(MinPasswordLen),
                           MsgBoxIcon::Error, SI::ui.Ok);
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
        SI::ui.showMessage(tr("Change password failed"), changePsw.lastError().text(), MsgBoxIcon::Error, SI::ui.Ok);
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
            SI::ui.showMessage(tr("Password Error"), tr("Please input correct password"), MsgBoxIcon::Error, SI::ui.Ok);
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
    setCurrentUserName("None");
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
            SI::ui.showMessage(tr("Password Error"), tr("Please input correct password"), MsgBoxIcon::Error, SI::ui.Ok);
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
        SI::ui.showMessage(tr("Query user %1 failed").arg(userName), query.lastError().text(), MsgBoxIcon::Error,
                           SI::ui.Ok);
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
            SI::ui.showMessage(tr("UserName Error"), tr("User %1 did not exist").arg(userName), MsgBoxIcon::Error,
                               SI::ui.Ok);
        }
        return false;
    }
}
