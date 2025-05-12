#pragma once

#include "thread/spin_lock.hpp"
#include <iostream>

class Progress
{
    public:
        explicit Progress(size_t total, size_t step = 1)
            : m_total(total), m_current(0), m_percent(0), m_last_percent(0), m_step(step)
        {
        }

        ~Progress()
        {
            printf("\rProgress: 100%%\n");
        }

        void update(size_t count)
        {
            m_current += count;
            m_percent = 100 * static_cast<size_t>(m_current )/ static_cast<size_t>(m_total );
            if (m_percent - m_last_percent >= m_step || m_percent == 100)
            {
                m_last_percent = m_percent;
                std::cout << "\rProgress: " << m_percent << "%";
            }
        }

    private:
        size_t m_total, m_current;
        size_t m_percent, m_last_percent, m_step;
        SpinLock m_lock;
};