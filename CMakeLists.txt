cmake_minimum_required(VERSION 3.22)
project(EscapeAnalysisTest C)

set(CMAKE_C_STANDARD 17)

add_executable(EscapeAnalysisTest main.c NewTests.cpp)

set(CMAKE_C_LINK_EXECUTABLE
        "/home/ap/dev/llvm-project/llvm/build/bin/llvm-link \
    <LINK_FLAGS> -S <OBJECTS> -o <TARGET>.ll <LINK_LIBRARIES>")

target_compile_options(EscapeAnalysisTest PRIVATE -O0 -S -emit-llvm
        -Wno-unused-command-line-argument -Wno-return-stack-address
        -Xclang -disable-O0-optnone
        )

# Run Escape Analysis Printer
add_custom_command(TARGET EscapeAnalysisTest POST_BUILD
        COMMAND /home/ap/dev/llvm-project/llvm/build/bin/opt
                -S EscapeAnalysisTest.ll
                -o EscapeAnalysisTest_mem2reg.ll
        COMMAND /home/ap/dev/llvm-project/llvm/build/bin/opt
                -S -passes='print<escape-analysis>'
                EscapeAnalysisTest_mem2reg.ll -disable-output
                -debug-only=escape-analysis
        DEPENDS EscapeAnalysisTest.ll
        COMMENT "Running Escape Analysis Printer"
)

# Run Escape Analysis Printer
add_custom_command(TARGET EscapeAnalysisTest POST_BUILD
        COMMAND /home/ap/dev/llvm-project/llvm/build/bin/clang
        -S -emit-llvm -fsanitize=thread
        -mllvm -tsan-use-escape-analysis
        -mllvm -debug-only=tsan,tsan-ea
        ../main.c
        -o EscapeAnalysisTest_TSan_instr.ll
        COMMENT "Running TSan"
)

#-S -passes=mem2reg EscapeAnalysisTest.ll
