cmake_minimum_required(VERSION 3.29)

project(aoc2024)

set(CMAKE_CXX_STANDARD 23)

list(APPEND CMAKE_MODULE_PATH "$ENV{HOME}/.cmake/modules")
include(GCC14Config)

# Find all directories starting with "day" in src directory
file(GLOB DAY_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/src "src/day*")

# Add executable for each day directory
foreach(day_dir ${DAY_DIRS})
    # Extract just the "dayX" part from "src/dayX"
    string(REGEX MATCH "day[0-9]+" target_name ${day_dir})
    
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/${day_dir})
        add_executable(${target_name} 
            src/${day_dir}/main.cpp
            src/${day_dir}/solution.cpp)

        # Add utils header directory
        target_include_directories(${target_name} 
            PUBLIC 
            ${CMAKE_SOURCE_DIR}/src)
            
        # Add custom targets for running with data and test inputs
        add_custom_target(run_${target_name}
            COMMAND ${target_name} ${CMAKE_CURRENT_SOURCE_DIR}/src/${day_dir}/data/data.txt
            DEPENDS ${target_name}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Running ${target_name} with data input"
        )
        
        add_custom_target(test_${target_name}
            COMMAND ${target_name} ${CMAKE_CURRENT_SOURCE_DIR}/src/${day_dir}/data/test.txt
            DEPENDS ${target_name}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Running ${target_name} with test input"
        )
    endif()
endforeach()

# Add convenience targets to run all tests or all solutions
add_custom_target(run)
add_custom_target(test)

# Add dependencies for the convenience targets
foreach(day_dir ${DAY_DIRS})
    get_filename_component(target_name ${day_dir} NAME)
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${day_dir})
        add_dependencies(run run_${target_name})
        add_dependencies(test test_${target_name})
    endif()
endforeach()

