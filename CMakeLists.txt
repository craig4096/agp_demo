cmake_minimum_required(VERSION 3.27.3)

project(agp_demo)

find_package(GLEW REQUIRED)
find_package(SFML COMPONENTS audio REQUIRED)
find_package(GLUT REQUIRED)

file(GLOB SRC_FILES src/*.cpp src/*.c)
add_executable(${PROJECT_NAME} ${SRC_FILES})

file(GLOB_RECURSE RES_FILES res)

target_link_libraries(${PROJECT_NAME} GLEW::GLEW sfml-audio GLUT::GLUT)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res/ $<TARGET_FILE_DIR:agp_demo>)
