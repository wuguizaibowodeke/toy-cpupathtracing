#pragma once

#include <chrono>
#include <string>
#include <iostream>

#define PROFILE(name) Profile __profile(name);

class Profile
{
public:
    Profile(const std::string &name)
        : m_name(name),
          m_start_time(std::chrono::high_resolution_clock::now())
    {
    }

    ~Profile()
    {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - m_start_time).count();
        std::cout << m_name << " took " << duration << " ms" << std::endl;
    }


private:
    std::string m_name;
    std::chrono::high_resolution_clock::time_point m_start_time;
};
