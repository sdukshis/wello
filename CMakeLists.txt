cmake_minimum_required(VERSION 2.8.7)

project(wello CXX)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(${PROJECT_NAME}_SRC
	src/wello.cpp
)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC})

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
