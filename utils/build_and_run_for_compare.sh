PROG=s

clang++ -S -emit-llvm -g  -fsanitize=thread ${PROG}.cpp -o ${PROG}_orig.ll
clang++ -S -emit-llvm -g  -fsanitize=thread -mllvm -tsan-use-escape-analysis-global ${PROG}.cpp -o ${PROG}_new.ll

echo "Orig"
clang++ -c ${PROG}_orig.ll && clang++ -fsanitize=thread ${PROG}_orig.o -o ${PROG}_orig && ./${PROG}_orig

echo "New"
clang++ -c ${PROG}_new.ll && clang++ -fsanitize=thread ${PROG}_new.o -o ${PROG}_new && ./${PROG}_new