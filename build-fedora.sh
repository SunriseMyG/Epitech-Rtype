#!/bin/bash

# Fonction pour installer les dépendances système sous Fedora
install_system_dependencies() {
    echo "Installation des dépendances système pour Fedora..."
    sudo dnf update -y
    sudo dnf install libX11-devel libX11-xcb-devel libfontenc-devel libICE-devel libSM-devel libXau-devel libXaw-devel \
    libXcomposite-devel libXcursor-devel libXdamage-devel libXfixes-devel libXi-devel libXinerama-devel libXkbfile-devel \
    libXmu-devel libXpm-devel libXres-devel libXt-devel libXtst-devel libXv-devel libXxf86vm-devel libxcb-devel \
    libxcb-glx-devel libxcb-render-devel libxcb-render-util-devel libxcb-xkb-devel xcb-util-devel \
    xcb-util-image-devel xcb-util-keysyms-devel xcb-util-wm-devel xcb-util-cursor-devel boost-devel

}

# Configuration Conan
configure_conan_profile() {
    echo "Configuration du profil Conan..."
    if [ ! -f "$HOME/.conan2/profiles/default" ]; then
        conan profile detect || { echo "Erreur lors de la configuration du profil Conan."; exit 1; }
        echo "Profil Conan configuré."
    else
        echo "Profil Conan déjà configuré."
    fi
}

# Préparation du répertoire build
mkdir -p build || { echo "Erreur lors de la création du dossier build."; exit 1; }

# Installation des dépendances et build
install_system_dependencies

if [ ! -d "venv" ]; then
    python3 -m venv venv
fi

source venv/bin/activate || { echo "Impossible d'activer l'environnement virtuel."; exit 1; }
pip install --upgrade pip
pip install conan
which conan || { echo "Conan n'est pas installé dans l'environnement virtuel."; exit 1; }

configure_conan_profile

# Installer les dépendances avec les nouveaux générateurs
conan install . --output-folder=build --build=missing -c tools.system.package_manager:mode=install -g CMakeDeps -g CMakeToolchain

# Configurer CMake avec le fichier toolchain
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=17 -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake
cmake --build build

cp build/r-type_server . || { echo "Erreur lors de la copie du serveur."; exit 1; }
cp build/r-type_client . || { echo "Erreur lors de la copie du client."; exit 1; }
cp build/r-type_admin-panel . || { echo "Erreur lors de la copie du panel d'administration."; exit 1; }
cp build/r-type_server_test ./tests/ || { echo "Erreur lors de la copie de l'éditeur."; exit 1; }
cp build/r-type_client_test ./tests/ || { echo "Erreur lors de la copie de l'éditeur."; exit 1; }

echo "Compilation terminée pour Fedora."