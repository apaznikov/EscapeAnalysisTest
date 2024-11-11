cmake_minimum_required(VERSION 3.22)
project(EscapeAnalysisTest C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

####################################################################################

#list(APPEND CMAKE_PROGRAM_PATH "/Users/ap/CLionProjects/llvm-project/llvm/build/bin")
list(APPEND CMAKE_PROGRAM_PATH "/home/ap/dev/llvm-project/llvm/build/bin")

find_program(LLVM_LINK_EXECUTABLE llvm-link)
find_program(OPT_EXECUTABLE opt)
find_program(CLANG_EXECUTABLE clang)
find_program(CLANG_CXX_EXECUTABLE clang++)

# Check if the programs were found
if(NOT LLVM_LINK_EXECUTABLE OR NOT OPT_EXECUTABLE OR NOT CLANG_EXECUTABLE)
        message(FATAL_ERROR "Could not find llvm-link, opt, or clang. Please adjust your PATH or provide the correct paths.")
endif()

message(STATUS "Found clang: ${CLANG_EXECUTABLE}")
message(STATUS "Found clang++: ${CLANG_CXX_EXECUTABLE}")
message(STATUS "Found opt: ${OPT_EXECUTABLE}")
message(STATUS "Found llvm-link: ${LLVM_LINK_EXECUTABLE}")

####################################################################################

add_executable(EscapeAnalysisTest main.c)

set(CMAKE_C_LINK_EXECUTABLE
        "${LLVM_LINK_EXECUTABLE} <LINK_FLAGS> -S <OBJECTS> -o <TARGET>.ll <LINK_LIBRARIES>")
#"/home/ap/dev/llvm-project/llvm/build/bin/llvm-link \

target_compile_options(EscapeAnalysisTest PRIVATE -O0 -S -emit-llvm
        -Wno-unused-command-line-argument -Wno-return-stack-address
        -Xclang -disable-O0-optnone
        )

# Run Escape Analysis Printer
add_custom_command(TARGET EscapeAnalysisTest POST_BUILD
        COMMAND ${OPT_EXECUTABLE}
                -passes=sroa
                -S EscapeAnalysisTest.ll
                -o EscapeAnalysisTest_mem2reg.ll
        COMMAND  ${OPT_EXECUTABLE}
                -S -passes='print<escape-analysis>'
                -debug-only=ea
                EscapeAnalysisTest_mem2reg.ll -disable-output
                -debug-only=escape-analysis
        DEPENDS EscapeAnalysisTest.ll
        COMMENT "Running Escape Analysis Printer"
)

# Run Escape Analysis Printer
add_custom_command(TARGET EscapeAnalysisTest POST_BUILD
        COMMAND ${CLANG_EXECUTABLE}
        -S -emit-llvm -fsanitize=thread
        -O2
        -mllvm -tsan-use-escape-analysis
        -mllvm -debug-only=tsan
        ../main.c
        -o EscapeAnalysisTest_TSan_instr.ll
        COMMENT "Running TSan"
)

####################################################################################

add_executable(TSanTest TSanTest.cpp)

set_target_properties(TSanTest PROPERTIES CXX_COMPILER ${CLANG_CXX_EXECUTABLE})

set(CMAKE_CXX_LINK_EXECUTABLE
        "${LLVM_LINK_EXECUTABLE} <LINK_FLAGS> -S <OBJECTS> -o <TARGET>.ll <LINK_LIBRARIES>")

target_compile_options(TSanTest PRIVATE
        -O0 -S -emit-llvm
        -o TSanTest_instr.ll
        -fsanitize=thread
        -mllvm -tsan-use-escape-analysis
        -Wno-unused-command-line-argument -Wno-return-stack-address
        -Xclang -disable-O0-optnone
)

#-S -passes=mem2reg EscapeAnalysisTest.ll
#-mllvm -stats
#-mllvm -debug-only=tsan,tsan-ea
