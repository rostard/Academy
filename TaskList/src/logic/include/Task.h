#pragma once
#include "logic_export.h"

#include <QString>
#include <chrono>
#include <QJsonObject>

class LOGIC_EXPORT Task
{
public:
    enum class State{
        NEW,
        ACTIVE,
        PAUSED,
        FINISHED
    };
    using seconds = std::chrono::seconds;
    inline QString getTitle() const
    {        return m_title;    }

    inline int getPriority() const
    {        return m_priority;    }

    inline State getState() const
    {        return m_state;    }

    inline seconds getTimer() const
    {        return m_timeElapsed;    }

    Task(QString name = " ",int priority = 2) : m_title(name), m_priority(priority), m_state(State::NEW), m_timeElapsed(0){}
    Task(Task&&) = default;
    Task(const Task&) = default;
    Task& operator=(Task&&) = default;
    Task& operator=(const Task&) = default;

    void setTitle(const QString& title);

    void setPriority(int priority);

    void setTimer(seconds s);

    void setState(State state);

    void addTime(seconds secs);

    void write(QJsonObject& json) const;
    void read(const QJsonObject &json);
private:
    QString m_title;
    int m_priority;
    State m_state;
    seconds m_timeElapsed;
};
