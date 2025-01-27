#!/bin/bash

# Fonction pour installer les dépendances système sous Linux
install_system_dependencies() {
    echo "Installation des dépendances système pour Linux..."
    sudo apt-get update
    sudo apt-get install -y libudev-dev libx11-dev libx11-xcb-dev libfontenc-dev libice-dev libsm-dev libxau-dev libxaw7-dev \
        libxcomposite-dev libxcursor-dev libxdamage-dev libxfixes-dev libxi-dev libxinerama-dev libxkbfile-dev libxmu-dev \
        libxmuu-dev libxpm-dev libxres-dev libxss-dev libxt-dev libxtst-dev libxv-dev libxxf86vm-dev libxcb-glx0-dev \
        libxcb-render0-dev libxcb-render-util0-dev libxcb-xkb-dev libxcb-icccm4-dev libxcb-image0-dev libxcb-keysyms1-dev \
        libxcb-randr0-dev libxcb-shape0-dev libxcb-sync-dev libxcb-xfixes0-dev libxcb-xinerama0-dev libxcb-dri3-dev uuid-dev \
        libxcb-cursor-dev libxcb-dri2-0-dev libxcb-present-dev libxcb-composite0-dev libxcb-ewmh-dev libxcb-res0-dev \
        libboost-all-dev
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
cp build/r-type_admin-panel . || { echo "Erreur lors de la copie du panel administrateur."; exit 1; }
cp build/r-type_server_test ./tests/ || { echo "Erreur lors de la copie de l'éditeur."; exit 1; }
cp build/r-type_client_test ./tests/ || { echo "Erreur lors de la copie de l'éditeur."; exit 1; }

cp build/r-type_admin-panel . || { echo "Erreur lors de la copie du panel administrateur."; exit 1; }


echo "Compilation terminée pour Linux."
