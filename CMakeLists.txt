cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_PATH /home/guilherme/pico/pico-sdk/)

#Name project
set(ProjectName motorProject)

#Set board to the Wireless version
set(PICO_BOARD pico)

# Import those libraries
include(/home/guilherme/pico/pico-sdk/external/pico_sdk_import.cmake)
#include(/home/guilherme/FreeRTOSv202212.01/FreeRTOS/Source/portable/ThirdParty/GCC/RP2040/pico_sdk_import.cmake)
include(/home/guilherme/FreeRTOSv202212.01/FreeRTOS/Source/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

#Define project
project(${ProjectName} CXX C ASM)

#Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(${ProjectName}
        main.cpp
        motor.cpp
        motor.hpp
)

include_directories(${CMAKE_SOURCE_DIR})

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${PROJECT_SOURCE_DIR}/lib
)

add_library(motor
        motor.cpp
        motor.hpp
        main.cpp
)

target_link_libraries(motor
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
        pico_stdlib
        hardware_pwm
        hardware_adc
)

target_include_directories(motor PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

#Link libraries
target_link_libraries(${ProjectName}
    FreeRTOS-Kernel-Heap4
    pico_stdlib
    motor
)

#Enable USB Uart output
pico_enable_stdio_usb(${ProjectName} TRUE)
pico_enable_stdio_uart(${ProjectName} FALSE)

#Add extra outputs
pico_add_extra_outputs(${ProjectName})
