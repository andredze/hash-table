valgrind --tool=callgrind --dump-instr=yes --collect-jumps=yes --callgrind-out-file=callgrind.out ./run
kcachegrind callgrind.out
# perf record -e cycles -g ./run
# perf report -n --show-total-period --children -g "graph,0.5,callee" -Mintel