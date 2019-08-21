#include "Task.h"

#include<QVariant>


void Task::setTitle(const QString& title)
{
    m_title = title;
}
void Task::setPriority(int priority)
{
    m_priority = priority;
}
void Task::setTimer(seconds s)
{
    m_timeElapsed = s;
}
void Task::setState(State state)
{
    m_state = state;
}
void Task::addTime(seconds secs)
{
    m_timeElapsed += secs;
}

void Task::write(QJsonObject &json) const
{
    json["title"] = m_title;
    json["priority"] = m_priority;
    json["timer"] = m_timeElapsed.count();
    json["state"] = (int)m_state;
}

void Task::read(const QJsonObject &json)
{
  m_title = json["title"].toString();
  m_priority = json["priority"].toInt();
  m_timeElapsed = seconds(json["timer"].toVariant().toLongLong());
  m_state = (State)json["state"].toInt();
  if(m_state == State::ACTIVE)
      m_state = State::PAUSED;
}
