from sys import argv
from os import path, remove, walk
from shutil import rmtree, copy2
from subprocess import run
import os

def execute(args):
    if (not (path.exists("conanfile.py") or
             path.exists("conanfile.txt"))) or \
            len(args) > 2:
        profile = "-h"
    elif len(args) == 2:
        profile = "-pr=" + args[1]
    else:
        profile = ""

    print("Profile list: -h, gcc-debug, default")
    print("Profile selected: ", profile if len(profile) > 0 else "default")

    match profile:
        # release or debug mode:
        case "-pr=gcc-debug" | "":
            build_type = "Debug" if profile == "-pr=gcc-debug" else "Release"
            output_folder = f"build/{build_type.lower()}"
            toolchain_file = f"{output_folder}/conan_toolchain.cmake"

            # Clean the build folder
            if path.exists(output_folder):
                rmtree(output_folder)

            # Conan install
            conan_command = [
                "conan", "install", ".", "--build=missing",
                f"--output-folder={output_folder}", "-g", "CMakeToolchain"
            ]
            if profile:
                conan_command.append(profile)

            print("Running Conan install...")
            run(conan_command, check=True)

            print(f"CMakeToolchain generated in {toolchain_file}")

            # CMake configure
            cmake_configure_command = [
                "cmake", "-S", ".", "-B", output_folder,
                f"-DCMAKE_BUILD_TYPE={build_type}",
                "-DCMAKE_CXX_STANDARD=17",
                f"-DCMAKE_TOOLCHAIN_FILE={toolchain_file}",
                "-DCMAKE_C_COMPILER=gcc",  # Ou "cl" pour Visual Studio
                "-DCMAKE_CXX_COMPILER=g++",  # Ou "cl" pour Visual Studio
                "-G", "Ninja",  # Spécifiez Ninja comme générateur
            ]
            print("Configuring project with CMake...")
            run(cmake_configure_command, check=True)

            # CMake build
            cmake_build_command = ["cmake", "--build", output_folder]
            print("Building project with CMake...")
            run(cmake_build_command, check=True)

            # Copier les fichiers .exe qui commencent par "r-type" du dossier build en récursif dans le dossier où est exécuté le script
            for root, _, files in walk(output_folder):
                for file in files:
                    if file.endswith(".exe") and file.startswith("r-type"):
                        exe_path = os.path.join(root, file)
                        copy2(exe_path, ".")
                        print(f"Copied {os.path.basename(exe_path)} to current directory")

        case "-h" | "--help" | _:
            print(""" 
                Usage:
                    NOTE: A conanfile.py or conanfile.txt file should be present within the directory
                    
                    --h or -help    usage instructions
                    gcc-debug       compiles using gcc in debug mode
                    default(no arguments) default compilation is gcc in release mode
                """)

if __name__ == "__main__":
    if not path.exists("C:/Strawberry"):
        print("Strawberry is not installed. Please install it before running this script.")
        exit(1)

    try:
        run(["conan", "--version"], check=True)
    except FileNotFoundError:
        print("Conan is not installed. Please install it before running this script.")
        exit(1)

    # Supprime les fichiers .exe qui commencent par "r-type" dans le dossier courant
    for root, _, files in walk("."):
        for file in files:
            if file.endswith(".exe") and file.startswith("r-type"):
                exe_path = os.path.join(root, file)
                remove(exe_path)
                print(f"Removed {exe_path}")

    print("Building project...")
    try:
        execute(argv)
        print("Build completed successfully!")
    except Exception as e:
        print(f"An error occurred during the build: {e}")
