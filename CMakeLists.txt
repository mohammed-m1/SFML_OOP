cmake_minimum_required(VERSION 3.16...3.30)
project(SFMLPROJECT)

set(CMAKE_CXX_STANDARD 17)

# Tell SFML we want to use static libraries (.a)
set(SFML_STATIC_LIBRARIES TRUE)

# Point to SFML's CMake folder
set(SFML_DIR "C:/SFML/lib/cmake/SFML")

# Find the correct components
find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

# Add your main file
add_executable(SFMLPROJECT 
    main.cpp
    ballista.cpp
    building.cpp
    enemy.cpp
    farm.cpp
    goblin.cpp
    mage_tower.cpp
    ogre.cpp
    player.cpp
    skeleton.cpp
    wave.cpp
)

# Link SFML libraries
target_link_libraries(SFMLPROJECT PRIVATE SFML::Graphics SFML::Window SFML::System)


# Make a folder called build if it isnt there already
#  go to terminal and go to the folder build if it is there
# use smake --build .
#  ./SFMLPROJECT.exe and then it should run and the gsame will be duisplayeds...