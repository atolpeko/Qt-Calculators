cmake_minimum_required(VERSION 3.19)
project(Calculator)

set(CMAKE_CXX_STANDARD 14)

#Auto invocation of the Qt meta-object compiler
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(HEADERS application.hpp calculators.hpp presenter.hpp)
set(SOURCE main.cpp application.cpp calculators.cpp presenter.cpp)

#Qt setup
set(QT_VERSION 6)
set(REQUIRED_LIBS Core Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt6::Core Qt6::Gui Qt6::Widgets)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCE})

#Qt libraries
find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})
