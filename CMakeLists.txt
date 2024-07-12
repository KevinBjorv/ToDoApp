project(ToDoApp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(ToDoApp
    src/main.cpp
    src/Database.cpp
    src/MenuManager.cpp
    src/ToDoApp.cpp
    src/Entry.cpp
)

find_package(nlohmann_json 3.2.0 REQUIRED)
target_link_libraries(ToDoApp PRIVATE nlohmann_json::nlohmann_json)


