# Project Build Documentation

## Prerequisites

Before starting, make sure you have the following tools and libraries installed on your system:

### Development Tools
- **CMake**: Project configuration tool.
- **Make**: Build tool.
- **g++**: C++ compiler.

### Dependencies
- **SFML**: For graphics, sound, and event handling.
- **Boost**: For thread and network operations.
- **nlohmann/json**: For JSON file management.

## Installing Dependencies

### On Linux

1. Open a terminal.
2. Run the installation script:
   ```bash
   ./build.sh
   ```

This script will check and install the necessary development tools and dependencies (OpenGL, GLUT, GLU, SFML, Boost, nlohmann/json).

### On Windows

1. Insalling prerequisites:
  - **Strawberry Pearl**
    - Download Strawberry Perl from https://strawberryperl.com/
    - Run the installer and follow the instructions
    - Verify installation: `perl --version`
  - **Python**
    - Download Python from https://www.python.org/downloads/
    - During installation, check "Add Python to PATH"
    - Verify installation: `python --version`
  - **Pip (via Python)**
    - Open a terminal (PowerShell or CMD)
    - Run: `py -m pip install --upgrade pip`
    - Verify installation: `pip --version`
  - **Conan (via Pip)**
    - Open a terminal
    - Run: `pip install conan`
    - Verify installation: `conan --version`
  - **CMake**
    - Download CMake from https://cmake.org/download/
    - During installation, select "Add CMake to system PATH"
    - Verify installation: `cmake --version`

2. Environment Setup:
  - Restart your terminal for PATH changes to take effect
  - Verify that all tools are accessible via command line


## Project Build

1. Clone the project repository :
   ```bash
   git clone <REPOSITORY_URL>
   cd <DIRECTORY_NAME>
   ```

2. Create a build directory :
   ```bash
   mkdir build
   cd build
   ```

3. Run CMake to configure the project :
   ```bash
   cmake ..
   ```

4. Compile the project :
   ```bash
   make
   ```

## Running the Project

After compilation, you can run the generated executables :

- For the client :
  ```bash
  ./r-type_client [ip] [port]
  ```

- For the server :
  ```bash
  ./r-type_server [port]
  ```

## Troubleshooting

- If you encounter linking errors, ensure that all necessary libraries are installed and properly linked in the `CMakeLists.txt` file.
- For Windows users, ensure that library paths are correctly configured in your environment.

## Contributing

If you want to contribute to this project, please follow these steps :

1. Fork the project.
2. Create a new branch for your feature :
   ```bash
   git checkout -b my-new-feature
   ```
3. Make your changes and commit them :
   ```bash
   git commit -m "[ADD] Implement new feature"
   git commit -m "[FIX] Correct a bug in feature"
   git commit -m "[UPDATE] Improve performance of feature"
   ```
4. Push your changes :
   ```bash
   git push origin my-new-feature
   ```
5. Open a Pull Request.

## Report a bug

If you encounter any issues or bugs while using this project, please follow these steps to report them:

1. **Check for existing issues**: Before reporting a new bug, please check the [issue tracker](https://github.com/EpitechPromo2027/B-CPP-500-NCE-5-2-rtype-steven.deffontaine/issues) to see if the issue has already been reported. If it has, you can add any additional information you have as a comment.

2. **Open a new issue**: If the issue has not been reported, open a new issue in the [issue tracker](https://github.com/EpitechPromo2027/B-CPP-500-NCE-5-2-rtype-steven.deffontaine/issues/new?template=Blank+issue).

3. **Provide detailed information**: When reporting a bug, please include as much detail as possible to help us understand and reproduce the issue. Include the following information:
   - A clear and descriptive title for the issue.
   - A detailed description of the problem, including steps to reproduce the issue.
   - The expected behavior and what actually happens.
   - Screenshots or code snippets, if applicable.
   - Information about your environment, such as operating system, compiler version, and any relevant configuration details.

4. **Submit the issue**: Once you have provided all the necessary information, submit the issue. We will review it and get back to you as soon as possible.

Thank you for helping us improve this project!

## License

This project is licensed under the MIT License. See the `LICENSE` file for more details.

**[Epitech] Technical Documentation © 2024**

## Liens Utiles

- [Project Architecture Documentation](../architecture/README.md)
- [Client Documentation](../client/README.md)
- [Comparative Study of Graphics Libraries](../comparative%20studies/README.md)
- [Feature Addition for Developers](../feature%20addition%20for%20devs/README.md)
- [Server Protocol Documentation](../server%20protocol/README.md)
- [Server Implementation Documentation](../server%20implementation/README.md)
