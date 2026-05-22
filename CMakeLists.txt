cmake_minimum_required(VERSION 3.10)

project(Valideur)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

# QR EMITTER
#add_executable(emitter src/emitter.cpp)

#target_link_libraries(
 #   emitter
#    qrencode
##)

# QR RECEIVER
add_executable(receiver src/receiver.cpp)

target_link_libraries(
    receiver
    ${OpenCV_LIBS}
)

# backend file géneration de code Qr Text format 
add_executable(backend
    src/backend.cpp
)

add_executable( validateur_test
    src/valideur_receiver.cpp
)