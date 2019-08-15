#pragma once
#include "logic_export.h"

#include <QString>
#include <chrono>

class LOGIC_EXPORT Task
{
public:

    using seconds = std::chrono::seconds;
    inline std::string getName() const
    {        return m_name;    }

    inline int getPriority() const
    {        return m_priority;    }

    inline seconds getTimer() const
    {        return m_timeElapsed;    }

    Task(std::string name,int priority) : m_name(name), m_priority(priority), m_timeElapsed(0){}
    Task(Task&&) = default;
    Task(const Task&) = default;
    Task& operator=(Task&&) = default;
    Task& operator=(const Task&) = default;
    void setTimer(seconds s)
    {   m_timeElapsed = s;    }
    inline void addTime(seconds secs)
    {    m_timeElapsed += secs;       }

private:
    std::string m_name;
    int m_priority;
    seconds m_timeElapsed;
};
