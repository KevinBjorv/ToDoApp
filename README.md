# ToDoApp

A simple To-Do application implemented in C++ that allows users to manage their tasks effectively. This application includes features such as adding, editing, removing tasks, and marking them as complete.

## Project Structure
ToDoApp/
├── .git/
├── .vs/
├── Resources/
│ └── Entries.json
├── ToDoApp/
├── vcpkg/
├── x64/
├── .gitattributes
├── .gitignore
├── CMakeLists.txt
├── Colors.h
├── Database.cpp
├── Database.h
├── Entry.cpp
├── Entry.h
├── MenuManager.cpp
├── MenuManager.h
├── README.md
├── ToDoApp.cpp
├── ToDoApp.sln
├── ToDoApp.vcxproj
├── ToDoApp.vcxproj.filters
├── ToDoApp.vcxproj.user
└── UtilityFunctions.h

## Features

- **Add Task**: Add new tasks to the to-do list.
- **Edit Task**: Edit details of existing tasks.
- **Remove Task**: Remove tasks from the to-do list.
- **Display Tasks**: Display all tasks with their details.
- **Settings Menu**: Adjust application settings.

## Requirements

- **CMake**: Version 3.10 or higher
- **C++ Compiler**: Supporting C++17 standard
- **nlohmann/json**: JSON library for C++ (https://github.com/nlohmann/json)

## Build Instructions

1. **Install CMake**: Ensure you have CMake installed on your system. You can download it from the [CMake website](https://cmake.org/download/) or install it via your package manager.
2. **Clone the Repository**: Clone this repository to your local machine.
3. **Create Build Directory**: Navigate to the project directory and create a build directory.
    ```sh
    mkdir build && cd build
    ```
4. **Run CMake**: Configure the project using CMake.
    ```sh
    cmake ..
    ```
5. **Build the Project**: Compile the project.
    ```sh
    cmake --build .
    ```

## Usage

After building the project, you will find the executable in the `build` directory. Run the executable to start the To-Do application.

## Command Line Options
- Add Task: Follow the on-screen prompts to add a new task.
- Edit Task: Choose an existing task to edit its details.
- Remove Task: Select a task to remove it from the list.
- Settings Menu: Access and modify application settings.

## Contributing
Contributions are welcome! Please fork this repository and submit pull requests with your changes. Ensure your code follows the existing coding style and includes appropriate documentation and tests.

## Acknowledgements
nlohmann/json: JSON library for C++
CMake: Build system
All contributors and maintainers of the libraries and tools used in this project.

## Contact
For any inquiries or issues, please open an issue on the Github repository, or contact the project maintainer directly

```sh
./ToDoApp
