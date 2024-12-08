cmake_minimum_required(VERSION 3.29)

project(aoc2024)

set(CMAKE_CXX_STANDARD 23)

file(GLOB DAY_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/src "src/day*")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/opt/homebrew/Cellar/llvm/19.1.4/lib/c++ -Wl,-rpath,/opt/homebrew/Cellarr/llvm/19.1.4/lib/c++")


foreach(day_dir ${DAY_DIRS})
    string(REGEX MATCH "day[0-9]+" target_name ${day_dir})

    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/${day_dir})
        add_executable(${target_name}
            src/${day_dir}/main.cpp
            src/${day_dir}/solution.cpp
        )

        target_sources(${target_name}
                PUBLIC FILE_SET modules TYPE CXX_MODULES FILES ${CMAKE_SOURCE_DIR}/src/utils/utils.ixx)

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

