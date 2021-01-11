cmake_minimum_required(VERSION 3.15.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)
#set(CMAKE_VERBOSE_MAKEFILE on)

project(Dwarf_Fortress_Clone CXX)

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "x64")
endif()
message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64"
     OR "${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86"))
    message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
endif()

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)

set(PROJECT_NAME Dwarf_Fortress_Clone)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Entity.h"
    "Inventory.h"
    "Item.h"
    "Map.h"
    "View.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Header_Files__Entities
    "Monster.h"
    "Player.h"
)
source_group("Header Files\\Entities" FILES ${Header_Files__Entities})

set(Header_Files__Items
    "Armor.h"
    "Consumable.h"
    "Weapon.h"
)
source_group("Header Files\\Items" FILES ${Header_Files__Items})

set(Source_Files
    "Dwarf_Fortress_Clone.cpp"
    "Entity.cpp"
    "Inventory.cpp"
    "Item.cpp"
    "Map.cpp"
    "View.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(Source_Files__Entities
    "Monster.cpp"
    "Player.cpp"
)
source_group("Source Files\\Entities" FILES ${Source_Files__Entities})

set(Source_Files__Items
    "Armor.cpp"
    "Consumable.cpp"
    "Weapon.cpp"
)
source_group("Source Files\\Items" FILES ${Source_Files__Items})

set(ALL_FILES
    ${Header_Files}
    ${Header_Files__Entities}
    ${Header_Files__Items}
    ${Source_Files}
    ${Source_Files__Entities}
    ${Source_Files__Items}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

set(ROOT_NAMESPACE DwarfFortressClone)

################################################################################
# Compile and link options
################################################################################

target_compile_options(${PROJECT_NAME} PRIVATE -std=c++17 -O0 -g)
target_link_libraries(${PROJECT_NAME} PRIVATE -lncurses)

