cmake_minimum_required(VERSION 3.20)

project(mspm0c1104_example LANGUAGES C ASM)

set(PROJECT_NAME mspm0c1104_template)
set(PROJECT_TARGET ${PROJECT_NAME}.out)

set(MSPM0_SDK_INSTALL_DIR "" CACHE PATH "Path to the TI MSPM0 SDK root")

set(_sdk_candidates)

if(MSPM0_SDK_INSTALL_DIR)
    list(APPEND _sdk_candidates "${MSPM0_SDK_INSTALL_DIR}")
endif()

if(DEFINED ENV{MSPM0_SDK_INSTALL_DIR} AND NOT "$ENV{MSPM0_SDK_INSTALL_DIR}" STREQUAL "")
    list(APPEND _sdk_candidates "$ENV{MSPM0_SDK_INSTALL_DIR}")
endif()

list(APPEND _sdk_candidates
    "${CMAKE_CURRENT_LIST_DIR}/../../../../mspm0-sdk"
    "$ENV{HOME}/Documents/mspm-/mspm0-sdk"
    "/usr/local/mspm0-sdk"
)

if(WIN32)
    list(APPEND _sdk_candidates
        "$ENV{USERPROFILE}/Documents/mspm-/mspm0-sdk"
        "$ENV{USERPROFILE}/Documents/Texas Instruments/mspm0-sdk"
        "C:/ti/mspm0-sdk"
    )
endif()

set(_resolved_sdk_dir "")
foreach(candidate IN LISTS _sdk_candidates)
    file(TO_CMAKE_PATH "${candidate}" candidate_path)
    if(EXISTS "${candidate_path}/source/ti/driverlib")
        set(_resolved_sdk_dir "${candidate_path}")
        break()
    endif()
endforeach()

if(NOT _resolved_sdk_dir)
    message(FATAL_ERROR
        "Unable to locate the TI MSPM0 SDK. Set MSPM0_SDK_INSTALL_DIR or the MSPM0_SDK_INSTALL_DIR environment variable."
    )
endif()

set(MSPM0_SDK_INSTALL_DIR "${_resolved_sdk_dir}" CACHE PATH "Path to the TI MSPM0 SDK root" FORCE)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXECUTABLE_SUFFIX ".out")

set(PROJECT_SOURCES
    src/main.c
    libs/config/ti_msp_dl_config.c
    startup_mspm0c110x_gcc.c
)

add_executable(${PROJECT_TARGET} ${PROJECT_SOURCES})

target_compile_definitions(${PROJECT_TARGET} PRIVATE DeviceFamily_MSPM0C110X __MSPM0C1104__ ARM_MATH_CM0)
target_include_directories(${PROJECT_TARGET} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/libs/config
    ${MSPM0_SDK_INSTALL_DIR}/source
    ${MSPM0_SDK_INSTALL_DIR}/source/third_party/CMSIS/Core/Include
    ${MSPM0_SDK_INSTALL_DIR}/source/third_party/CMSIS/DSP/Include
)
target_compile_options(${PROJECT_TARGET} PRIVATE
    -O2 -mcpu=cortex-m0plus -march=armv6-m -mthumb -mfloat-abi=soft
    -ffunction-sections -fdata-sections -g -gstrict-dwarf -Wall
)
target_link_directories(${PROJECT_TARGET} PRIVATE
    ${MSPM0_SDK_INSTALL_DIR}/source/third_party/CMSIS/DSP/lib/gcc/m0p
    ${MSPM0_SDK_INSTALL_DIR}/source/ti/driverlib/lib/gcc/m0p/mspm0g1x0x_g3x0x
)
target_link_options(${PROJECT_TARGET} PRIVATE
    -T${CMAKE_CURRENT_LIST_DIR}/device_linker.lds -march=armv6-m -mthumb -static
    -Wl,--gc-sections -Wl,-Map,${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.map
    --specs=nano.specs --specs=nosys.specs
)
target_link_libraries(${PROJECT_TARGET} PRIVATE -l:arm_cortexM0l_math.a -l:driverlib.a gcc c m)

set_target_properties(${PROJECT_TARGET} PROPERTIES OUTPUT_NAME ${PROJECT_NAME} RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output)

find_program(CMAKE_SIZE arm-none-eabi-size)
find_program(CMAKE_OBJCOPY arm-none-eabi-objcopy)
find_program(PYOCD_EXECUTABLE pyocd)

if(NOT CMAKE_SIZE)
    message(FATAL_ERROR "arm-none-eabi-size was not found in PATH.")
endif()

if(NOT CMAKE_OBJCOPY)
    message(FATAL_ERROR "arm-none-eabi-objcopy was not found in PATH.")
endif()

add_custom_command(TARGET ${PROJECT_TARGET} POST_BUILD COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_TARGET}> VERBATIM)
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/output/${PROJECT_NAME}.hex
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/output
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_TARGET}> ${CMAKE_CURRENT_BINARY_DIR}/output/${PROJECT_NAME}.hex
    DEPENDS ${PROJECT_TARGET}
    VERBATIM
)
add_custom_target(hex ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/output/${PROJECT_NAME}.hex)

if(PYOCD_EXECUTABLE)
    add_custom_target(flash
        COMMAND ${PYOCD_EXECUTABLE} flash -t mspm0c1104 --connect under-reset -O pack.debug_sequences.disabled_sequences=ResetSystem,ResetHWReset,ResetCatchSet,ResetProcessor ${CMAKE_CURRENT_BINARY_DIR}/output/${PROJECT_NAME}.hex
        DEPENDS hex USES_TERMINAL VERBATIM
    )
    add_custom_target(reset
        COMMAND ${PYOCD_EXECUTABLE} reset -t mspm0c1104 -O pack.debug_sequences.disabled_sequences=ResetSystem,ResetHWReset,ResetCatchSet,ResetProcessor
        USES_TERMINAL VERBATIM
    )
else()
    add_custom_target(flash
        COMMAND ${CMAKE_COMMAND} -E echo "pyocd not found in PATH. Install pyocd and reconfigure."
        COMMAND ${CMAKE_COMMAND} -E false
        DEPENDS hex VERBATIM
    )
    add_custom_target(reset
        COMMAND ${CMAKE_COMMAND} -E echo "pyocd not found in PATH. Install pyocd and reconfigure."
        COMMAND ${CMAKE_COMMAND} -E false
        VERBATIM
    )
endif()

message(STATUS "Using MSPM0 SDK at: ${MSPM0_SDK_INSTALL_DIR}")
