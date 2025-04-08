cmake_minimum_required(VERSION 3.16) 

project("Data_Structure_Visualization")
    
# file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
# file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/raylib.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/raygui/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Textures
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Fonts
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(
     ${PROJECT_NAME}
     src/main.cpp 
     src/DataStructure.cpp 
     src/SinglyLinkedList.cpp
     src/Menu.cpp
     src/NewAnimation.cpp
     src/Button.cpp 
     src/Node.cpp 
     src/Textbox.cpp 
     src/SetOfAnimation.cpp
     src/Presentation.cpp
     src/Graph.cpp
     src/Trie.cpp 
     src/AVL.cpp
     src/AVLNode.cpp 
     )

add_subdirectory("include/raylib")

target_link_libraries(${PROJECT_NAME} PUBLIC raylib)