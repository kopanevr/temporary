#include "Application.hpp"

//

#include <cstdlib>

//

#include "SubsystemId.hpp"

//

// Подсистемы.

#include "SubsystemManager.hpp"

//

/// @brief Конструктор.
/// @param argc Количество аргументов.
/// @param argv Указатель на список аргументов.
Application::Application(int argc, char* argv[])
{
    commandLineInterpreter.reset(new(std::nothrow) CommandLineInterpreter(argc, argv));

    assert(commandLineInterpreter != nullptr);

    if (commandLineInterpreter != nullptr)
    {
        CommandLineInterpreter::instance = commandLineInterpreter.get();

        RESET_FLAG(9, isAllowedToExecute);

        return;
    }

    // Проверка состояния обработки аргументов.

    if (!commandLineInterpreter->isParsed())
    {
        RESET_FLAG(9, isAllowedToExecute);

        return;
    }

    subsystemManager.reset(new(std::nothrow) SubsystemManager());

    assert(subsystemManager != nullptr);

    if (subsystemManager != nullptr)
    {
        SubsystemManager::instance = subsystemManager.get();

        RESET_FLAG(9, isAllowedToExecute);

        return;
    }

    //

    InferenceSubsystem* inferenceSubsystem = static_cast<InferenceSubsystem*>(subsystemManager->getSubsystemById(SubsystemId::InferenceSubsystem));

    // Установка имени файла модели.

    inferenceSubsystem->setModelFileName(applicationConfig.modelFileName);

    // Установка пути к директории модели.

    // Проверка пути.

    if (commandLineInterpreter->getArgs().pathToModelDirectory)
    {
        // Установка пути к директории модели, указанного в аргументах приложения при запуске.

        inferenceSubsystem->setPathToModelDirectory(commandLineInterpreter->getArgs().pathToModelDirectory);
    }
    else
    if (applicationConfig.pathToModelFile)
    {
        // Установка пути к директории модели, указанного в конфигурации приложения.

        inferenceSubsystem->setPathToModelDirectory(applicationConfig.pathToModelFile);
    }
    else
    {
        RESET_FLAG(9, isAllowedToExecute);

        return;
    }

    // Инициализация.

    init();

    SET_FLAG(9, isAllowedToExecute);
}

/// @brief Деструктор.
Application::~Application()
{
    // Деинициализация.

    deinit();

    //

    SubsystemManager::instance = nullptr;
}

/// @brief Инициализация.
/// @details Производит запуск менеджера подсистем.
void Application::init()
{
    // Вывод информации о приложении.

    printInfo();

    // Запуск менеджера подсистем.

    if (subsystemManager)
    {
        subsystemManager-> startUp();
    }
}

/// @brief Выполнение.
/// @return Результат выполнения.
int Application::exec()
{
    if (!GET_FLAG_STATE(9, isAllowedToExecute)) return EXIT_FAILURE;

    // Таймер для отсчета периода времени с момента запуска приложения.

    static Timer timerToTimeSinceStartApplication(0u);

    // Запуск таймера для отсчета периода времени с момента запуска приложения.

    START_TIMER_FIRST_TIME(timerManager, timerToTimeSinceStartApplication);

    //

    int ret = subsystemManager->process();

    //

    GET_ELAPSED_TIME(timerManager, timerToTimeSinceStartApplication);

    // Остановка таймера для отсчета периода времени с момента запуска приложения.

    STOP_TIMER(timerManager, timerToTimeSinceStartApplication);

    if (ret == 1) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

/// @brief Деинициализация.
/// @details Производит остановку менеджера подсистем.
void Application::deinit()
{
    if (subsystemManager)
    {
        subsystemManager->shutDown();
    }
}

/// @brief Вывод информации о приложении.
void Application::printInfo()
{
    LOG("Информация о приложении:");

    LOG("Версия", 1);
    LOG("Подверсия", 0);
}
