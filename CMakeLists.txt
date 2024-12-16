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

set(CMAKE_C_LINK_EXECUTABLE
        "${LLVM_LINK_EXECUTABLE} <LINK_FLAGS> -S <OBJECTS> -o <TARGET>.ll <LINK_LIBRARIES>")

function(add_escape_analysis_target target_name source_file
        analysis_pass tsan_ea_option)
        add_executable(${target_name} ${source_file})

        target_compile_options(${target_name} PRIVATE -O0 -S -emit-llvm
                -Wno-unused-command-line-argument
                -Wno-return-stack-address
                -Xclang -disable-O0-optnone
        )

        add_custom_command(TARGET ${target_name} POST_BUILD
                COMMAND ${OPT_EXECUTABLE}
                -S ${target_name}.ll
                -o ${target_name}_mem2reg.ll
                COMMAND ${OPT_EXECUTABLE}
                -S -passes='print<${analysis_pass}>'
                -debug-only=ea
                ${target_name}_mem2reg.ll -disable-output
                DEPENDS ${target_name}.ll
                COMMENT "Running ${analysis_pass} Printer"
        )
#-passes=mem2reg

        # Run Escape Analysis Printer
#        add_custom_command(TARGET ${target_name} POST_BUILD
#                COMMAND ${CLANG_EXECUTABLE}
#                -S -emit-llvm -fsanitize=thread
#                -O1 -fno-inline-functions
#                -mllvm ${tsan_ea_option}
#                -mllvm -debug-only=tsan
#                -Wno-return-stack-address
#                ../${source_file}
#                -o ${target_name}_TSan_instr.ll
#                COMMENT "Running TSan"
#        )
endfunction()

add_escape_analysis_target(
        EscapeAnalysisTest EscapeAnalysisTest.c
        escape-analysis -tsan-use-escape-analysis)
add_escape_analysis_target(
        EscapeAnalysisGlobalTest EscapeAnalysisGlobalTest.c
        escape-analysis-global -tsan-use-escape-analysis-global)

####################################################################################

add_executable(TSanTest TSanTest.cpp)

set_target_properties(TSanTest PROPERTIES CXX_COMPILER ${CLANG_CXX_EXECUTABLE})

set(CMAKE_CXX_LINK_EXECUTABLE
        "${LLVM_LINK_EXECUTABLE} <LINK_FLAGS> -S <OBJECTS> -o <TARGET>.ll <LINK_LIBRARIES>")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mllvm -debug-only=tsan -mllvm -debug-only=ea")

target_compile_options(TSanTest PRIVATE
        #-O2
        -S -emit-llvm
        -o TSanTest_instr.ll
        -fsanitize=thread
        -mllvm -tsan-use-escape-analysis-global
        -Wno-unused-command-line-argument
        -Wno-return-stack-address
        -Xclang -disable-O0-optnone
)

#-S -passes=mem2reg EscapeAnalysisTest.ll
#-mllvm -stats
#-mllvm -debug-only=tsan,tsan-ea
