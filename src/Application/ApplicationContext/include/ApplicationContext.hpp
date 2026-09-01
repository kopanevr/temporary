#pragma once

/// @brief
struct ApplicationContext final
{
    ApplicationContext* getInstance()
    {
        static ApplicationContext instance = {};

        return &instance;
    }
};
