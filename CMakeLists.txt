cmake_minimum_required(VERSION 3.10)
project(Application VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#

option(CUSTOM_CONFIGURATION_ENABLE_INFERENCE_PROFILING "Включить профилирование вывода при отладке." OFF)
option(CUSTOM_CONFIGURATION_ENABLE_OUTPUT_MODEL_INFO "Включить вывод информации о модели при отладке." ON)

#

add_subdirectory(${CMAKE_SOURCE_DIR}/src/Application)
