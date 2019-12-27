#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QStack>
#include "../loging/rollbackfile.h"

class CommandInterface
{
public:
    virtual bool execute() = 0;
    virtual bool unExecute() = 0;
    virtual QString getExecutionLog() = 0;
    virtual QString getUnExecutionLog() = 0;

    virtual ~CommandInterface()
    {

    }
};


class ConfigManager: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int commandCount READ commandCount WRITE setCommandCount NOTIFY commandCountChanged)
    Q_PROPERTY(int undoCommandCount READ undoCommandCount WRITE setUndoCommandCount NOTIFY undoCommandCountChanged)

public:
    ConfigManager()
    {
        configChangedLog.init("ConfigChangeLogs", "configChangeLog", 1024 * 1024 * 3, 10, true);
    }

    bool executeCommand(CommandInterface* command)
    {
        if(undoCommandStack.count() > 0){
            while (undoCommandStack.count() > 0) {
                delete undoCommandStack.pop();
            }
            setUndoCommandCount(undoCommandStack.count());
        }

        if(command->execute()){
            configChangedLog.appendLine(command->getExecutionLog());
            commandStack.push(command);
            setCommandCount(commandStack.count());
            return true;
        }else {
            return false;
        }
    }

    Q_INVOKABLE void undo()
    {
        if(commandStack.count() > 0){
            CommandInterface* lastCommand = commandStack.pop();
            if(lastCommand->unExecute()){
                configChangedLog.appendLine(lastCommand->getUnExecutionLog());
                undoCommandStack.push(lastCommand);
                setCommandCount(commandStack.count());
                setUndoCommandCount(undoCommandStack.count());
            }else {
                commandStack.push(lastCommand);
            }
        }
    }

    Q_INVOKABLE void redo()
    {
        if(undoCommandStack.count() > 0){
            CommandInterface* lastUndoCommand = undoCommandStack.pop();
            if(lastUndoCommand->execute()){
                configChangedLog.appendLine(lastUndoCommand->getExecutionLog());
                commandStack.push(lastUndoCommand);
                setCommandCount(commandStack.count());
                setUndoCommandCount(undoCommandStack.count());
            }else {
                undoCommandStack.push(lastUndoCommand);
            }
        }
    }

    int commandCount() const
    {
        return m_commandCount;
    }

    int undoCommandCount() const
    {
        return m_undoCommandCount;
    }

    ~ConfigManager()
    {
        while (commandStack.count() > 0) {
            delete commandStack.pop();
        }
        while (undoCommandStack.count() > 0) {
            delete undoCommandStack.pop();
        }
    }

public slots:
    void setCommandCount(int commandCount)
    {
        if (m_commandCount == commandCount)
            return;

        m_commandCount = commandCount;
        emit commandCountChanged(m_commandCount);
    }

    void setUndoCommandCount(int undoCommandCount)
    {
        if (m_undoCommandCount == undoCommandCount)
            return;

        m_undoCommandCount = undoCommandCount;
        emit undoCommandCountChanged(m_undoCommandCount);
    }

signals:
    void commandCountChanged(int commandCount);

    void undoCommandCountChanged(int undoCommandCount);

private:
    QStack<CommandInterface*> commandStack;
    QStack<CommandInterface*> undoCommandStack;
    int m_commandCount;
    int m_undoCommandCount;
    RollbackFile configChangedLog;
};

#endif // CONFIGMANAGER_H
