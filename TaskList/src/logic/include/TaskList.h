#include "logic_export.h"
#include <QObject>
#include "Task.h"

class LOGIC_EXPORT TaskList : public QObject
{
    Q_OBJECT
public:
    size_t size() const;
    const Task& getTask(size_t index);


public slots:
    void deleteTask(int index);
    void addTask(Task task);
    void incrementTaskTimers();
    void readFile(QString path);
    void saveFile(QString path);
private:
    QVector<Task> m_tasks;
};
