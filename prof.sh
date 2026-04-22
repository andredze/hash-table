perf record -e cycles -g ./run
perf report -n --show-total-period --children -g "graph,0.5,callee" -Mintel