cmake_minimum_required(VERSION 2.8)
project( TailChaser )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_package( Threads REQUIRED )
find_package( OpenCV REQUIRED )
find_package(AWSSDK REQUIRED COMPONENTS s3)

add_executable( TailChaser
                    main.cpp
                    motiondetector/motiondetector.cpp
                    application/tailchaser.cpp
                    storagemanager/bucket.cpp)

add_definitions(-std=c++0x)

target_link_libraries( TailChaser   ${OpenCV_LIBS}
                                    ${CMAKE_THREAD_LIBS_INIT}
                                    ${AWSSDK_LINK_LIBRARIES})
