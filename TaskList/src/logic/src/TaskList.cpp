#include "TaskList.h"
#include <fstream>
void TaskList::addTask(Task task)
{
    m_tasks.push_back(task);
}

size_t TaskList::size() const
{
    return m_tasks.size();
}

const Task& TaskList::getTask(size_t index)
{
    return m_tasks.at(index);
}

void TaskList::incrementTaskTimers()
{
    for (int i = 0; i < m_tasks.size(); ++i)
    {
        m_tasks[i].addTime(Task::seconds(1));
    };
}

void TaskList::readFile(QString path)
{
    std::ifstream ifstream(path.toStdString());
    while(!ifstream.eof())
    {
        std::string name;
        int priority;
        Task::seconds::rep timer;
        char buf[128];
        ifstream.getline(buf, 128, ';');
        name = buf;
        if(ifstream.eof())
            return;
        ifstream.getline(buf, 128, ';');
        priority = QString(buf).toInt();

        ifstream.getline(buf, 128, '\n');
        timer = QString(buf).toLongLong();

        Task task(name, priority);
        task.setTimer(Task::seconds(timer));
        m_tasks.push_back(task);

    }
}

void TaskList::saveFile(QString path)
{
    std::ofstream ofstream(path.toStdString());
    for(auto& i : m_tasks)
    {
        ofstream << i.getName() << ";" << i.getPriority() << ";" << i.getTimer().count() << "\n";
    }
}

void TaskList::deleteTask(int index)
{
    m_tasks.removeAt(index);
}
