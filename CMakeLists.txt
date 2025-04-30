cmake_minimum_required(VERSION 3.10)
project(app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавление исходных файлов
# set(SOURCES "src/main.cpp")
file(GLOB SOURCES "src/*.cpp")

# Создание исполняемого файла
add_executable(${PROJECT_NAME} ${SOURCES})

# Подключение заголовочных файлов
target_include_directories(${PROJECT_NAME} PRIVATE include/)

# Подключение библиотек (если нужны)
# find_package(Boost REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE Boost::boost)

# add_custom_target(build-and-run
#     COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --config Release
#     COMMAND ${CMAKE_BINARY_DIR}/Release/MyApp.exe
#     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
# )