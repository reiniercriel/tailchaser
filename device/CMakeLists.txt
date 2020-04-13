cmake_minimum_required(VERSION 2.8)
project( TailChaser )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_package( Threads REQUIRED )
find_package( OpenCV REQUIRED )
add_executable( TailChaser
                    main.cpp
                    motiondetector/motiondetector.cpp
                    application/tailchaser.cpp)

add_definitions(-std=c++0x)# -m64)

target_link_libraries( TailChaser ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})
