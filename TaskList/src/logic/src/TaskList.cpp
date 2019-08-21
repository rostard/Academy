#include "TaskList.h"
#include <fstream>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>



TaskModel::TaskModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void TaskModel::addTask(const Task &task)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_tasks << task;
    endInsertRows();
}

int TaskModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_tasks.count();
}

QVariant TaskModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_tasks.count())
        return QVariant();

    const Task &task = m_tasks[index.row()];
    switch(role)
    {
    case TitleRole:
        return task.getTitle();
    case PriorityRole:
        return task.getPriority();
    case StateRole:
        return (int)task.getState();
    case TimerRole:
        return task.getTimer().count();
    default:
        return QVariant();
    }

}

QVariantMap TaskModel::get(int row) const
{
    auto& task = m_tasks.at(row);
    return{{"title", task.getTitle()}, {"priority", task.getPriority()}, {"timer", task.getTimer().count()}, {"timerState", (int)task.getState()}};
}

void TaskModel::append(const QString &title, int priority)
{
    int row =  m_tasks.count();

    beginInsertRows(QModelIndex(), row, row);
    m_tasks.insert(row, Task(title, priority));
    endInsertRows();
}


void TaskModel::set(int row, const QString &title, int priority)
{

    m_tasks[row].setTitle(title);
    m_tasks[row].setPriority(priority);
    dataChanged(index(row, 0), index(row, 0), {TitleRole, PriorityRole});
}

void TaskModel::pause(int row)
{
    m_tasks[row].setState(Task::State::PAUSED);
    dataChanged(index(row, 0), index(row, 0), {StateRole});
}

void TaskModel::run(int row)
{
    if(m_tasks[row].getState() == Task::State::ACTIVE)
        m_tasks[row].setState(Task::State::PAUSED);
    else {
                m_tasks[row].setState(Task::State::ACTIVE);
    }
    dataChanged(index(row, 0), index(row, 0), {StateRole});
}

void TaskModel::finish(int row)
{
    m_tasks[row].setState(Task::State::FINISHED);
    dataChanged(index(row, 0), index(row, 0), {StateRole});
}

void TaskModel::reset(int row)
{
    m_tasks[row].setState(Task::State::NEW);
    m_tasks[row].setTimer(Task::seconds(0));
    dataChanged(index(row, 0), index(row, 0), {StateRole, TimerRole});
}

QHash<int, QByteArray> TaskModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[PriorityRole] = "priority";
    roles[StateRole] = "timerState";
    roles[TimerRole] = "timer";
    return roles;
}

void TaskModel::remove(int row)
{
    if (row < 0 || row >= m_tasks.count())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    m_tasks.removeAt(row);
    endRemoveRows();
}

void TaskModel::incrementTaskTimers()
{
    for (int i = 0; i < m_tasks.size(); ++i)
    {
        if(m_tasks[i].getState() == Task::State::ACTIVE)
        {
            m_tasks[i].addTime(Task::seconds(1));
            dataChanged(index(i, 0), index(i, 0), {TimerRole});
        }
    }
}

void TaskModel::readFile(QString path)
{
    QFile loadFile(path);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    QJsonObject obj = loadDoc.object();
    QJsonArray levelArray =obj["tasks"].toArray();
    m_tasks.clear();
    m_tasks.reserve(levelArray.size());
    for (int levelIndex = 0; levelIndex < levelArray.size(); ++levelIndex) {
        QJsonObject levelObject = levelArray[levelIndex].toObject();
        Task level;
        level.read(levelObject);
        m_tasks.append(level);
    }

    return;
}

void TaskModel::saveFile(QString path)
{
    QFile saveFile(path);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QJsonArray levelArray;
    QJsonObject gameObject;
    for(auto& task : m_tasks){
        task.write(gameObject);
        levelArray.append(gameObject);
    }
    QJsonObject obj;
    obj["tasks"] = levelArray;
    QJsonDocument saveDoc(obj);
    saveFile.write(saveDoc.toJson());
}
