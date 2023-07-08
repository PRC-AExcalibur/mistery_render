#pragma once

#include <iostream>
#include <chrono>
#include <iomanip>

namespace mistery_render
{

template <class T>
inline bool TestExpect(T source, T expect, std::string test_name)
{
    bool eq = mistery_render::m_math::IsEqual(source, expect);
    if (eq)
    {
        std::cout << test_name << " : -> passed\n";
    }
    else
    {
        std::cout << test_name << " : -> failed\n";
        std::cout <<"    expect:\n" << expect <<"\n";
        std::cout <<"    result:\n" << source <<"\n";
    }
    std::cout<<"------------------------------\n";
    return eq;
}

// s = 0, ms = 1, us = 2, ns = 3
inline double NowTime(int mode = 1) 
{
    auto now = std::chrono::system_clock::now();
    auto duration_since_epoch = now.time_since_epoch();
    int64_t time_now = 0;

    switch (mode)
    {
    case 0:
        time_now = std::chrono::duration_cast<std::chrono::seconds>(duration_since_epoch).count();
        break;
    case 1:
        time_now = std::chrono::duration_cast<std::chrono::milliseconds>(duration_since_epoch).count();
        break;
    case 2:
        time_now = std::chrono::duration_cast<std::chrono::microseconds>(duration_since_epoch).count();
        break;
    case 3:
        time_now = std::chrono::duration_cast<std::chrono::nanoseconds>(duration_since_epoch).count();
        break;
    default:
        break;
    }

    return static_cast<double>(time_now);
}

}