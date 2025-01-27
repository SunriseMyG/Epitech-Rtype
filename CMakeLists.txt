cmake_minimum_required(VERSION 3.15)
project(RType LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Ajout du chemin pour find_package
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Configuration de Conan
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake OPTIONAL)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

# Configuration de la compilation
if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
endif()

# Recherche des packages
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
find_package(Boost REQUIRED COMPONENTS system serialization unit_test_framework)
find_package(nlohmann_json REQUIRED)

# Configuration du client
file(GLOB_RECURSE CLIENT_SOURCES "client/*.cpp")
file(GLOB_RECURSE CLIENT_HEADERS "client/*.hpp")
add_executable(r-type_client ${CLIENT_SOURCES} ${CLIENT_HEADERS})

# Include directories for client
target_include_directories(r-type_client PRIVATE ${Boost_INCLUDE_DIRS})

# Linking libraries for client
target_link_libraries(r-type_client PRIVATE 
    sfml-system 
    sfml-window 
    sfml-graphics 
    sfml-network 
    sfml-audio 
    Boost::system 
    Boost::serialization 
    nlohmann_json::nlohmann_json 
    pthread)

# Configuration du serveur
file(GLOB_RECURSE SERVER_SOURCES "server/*.cpp")
file(GLOB_RECURSE SERVER_HEADERS "server/*.hpp")
add_executable(r-type_server ${SERVER_SOURCES} ${SERVER_HEADERS})


# Include directories for server
target_include_directories(r-type_server PRIVATE ${Boost_INCLUDE_DIRS})

if (WIN32)
    target_link_libraries(r-type_server PRIVATE ws2_32 mswsock stdc++fs)
endif()

# Linking libraries for server
target_link_libraries(r-type_server PRIVATE 
    Boost::system 
    Boost::serialization 
    nlohmann_json::nlohmann_json)

target_link_libraries(r-type_server PRIVATE Boost::system Boost::serialization nlohmann_json::nlohmann_json)
target_include_directories(r-type_server PRIVATE ${Boost_INCLUDE_DIRS})

# Admin panel
if(WIN32)
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/admin/Stream/admin.rc")
else()
    set(APP_ICON_RESOURCE_WINDOWS "")
endif()

file(GLOB_RECURSE COMMON_SOURCES "admin/*.cpp")
file(GLOB_RECURSE COMMON_HEADERS "admin/*.hpp")
add_executable(r-type_admin-panel ${COMMON_SOURCES} ${COMMON_HEADERS} ${APP_ICON_RESOURCE_WINDOWS})

target_link_libraries(r-type_admin-panel PRIVATE
    sfml-system
    sfml-window
    sfml-graphics
    sfml-network
    sfml-audio
    Boost::system
    Boost::serialization
    nlohmann_json::nlohmann_json)

# Configuration des tests unitaires
enable_testing()

# Ajout des tests pour le serveur
file(GLOB_RECURSE TEST_SOURCES "tests/server/*.cpp")
file(GLOB_RECURSE SERVER_TEST_SOURCES "server/*.cpp")
file(GLOB_RECURSE SERVER_TEST_HEADERS "server/*.hpp")
list(FILTER SERVER_TEST_SOURCES EXCLUDE REGEX "server/main.cpp")
add_executable(r-type_server_test ${TEST_SOURCES} ${SERVER_TEST_SOURCES})

# Include directories for tests
target_include_directories(r-type_server_test PRIVATE ${Boost_INCLUDE_DIRS})

if(WIN32)
    target_link_libraries(r-type_server_test PRIVATE 
        ws2_32 
        mswsock
        stdc++fs)
endif()

# Linking libraries for tests
target_link_libraries(r-type_server_test PRIVATE 
    Boost::system 
    Boost::serialization 
    nlohmann_json::nlohmann_json 
    pthread
)

# Ajout des tests pour le client
file(GLOB_RECURSE CLIENT_TEST_SOURCES "tests/client/*.cpp")
file(GLOB_RECURSE CLIENT_TEST_HEADERS "tests/client/*.hpp")
add_executable(r-type_client_test ${CLIENT_TEST_SOURCES} ${CLIENT_TEST_HEADERS} ${TEST_CLIENT_SOURCES})

if(WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DNOMINMAX)
    add_definitions(-D_WIN32_WINNT=0x0601)
    target_compile_definitions(r-type_client_test PRIVATE 
        _WINSOCK_DEPRECATED_NO_WARNINGS
    )
endif()

# Include directories for tests
target_include_directories(r-type_client_test PRIVATE ${Boost_INCLUDE_DIRS})

# Linking libraries for tests
target_link_libraries(r-type_client_test PRIVATE 
    Boost::system 
    Boost::serialization 
    nlohmann_json::nlohmann_json 
    pthread
)

if (WIN32)
    target_link_libraries(r-type_client_test PRIVATE ws2_32 mswsock stdc++fs)
endif()

add_test(NAME r-type_client_test COMMAND r-type_client_test)

