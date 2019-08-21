#include "logic_export.h"
#include <QAbstractListModel>
#include "Task.h"

class LOGIC_EXPORT TaskModel : public QAbstractListModel
{
    Q_OBJECT
public:

    enum TaskRoles {
        TitleRole = Qt::UserRole + 1,
        PriorityRole,
        StateRole,
        TimerRole
    };

    TaskModel(QObject *parent = nullptr);

    void addTask(const Task &task);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    const Task& getTask(size_t index);

    Q_INVOKABLE QVariantMap get(int row) const;
    Q_INVOKABLE void append(const QString &title, int priority);
    Q_INVOKABLE void set(int row, const QString &title, int priority);
    Q_INVOKABLE void pause(int row);
    Q_INVOKABLE void run(int row);
    Q_INVOKABLE void finish(int row);
    Q_INVOKABLE void reset(int row);
    Q_INVOKABLE void remove(int row);

public slots:
    void incrementTaskTimers();
    void readFile(QString path);
    void saveFile(QString path);
protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QVector<Task> m_tasks;
};
