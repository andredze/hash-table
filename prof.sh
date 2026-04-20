perf record -g ./run
perf report -g "graph,0.5,caller" -Mintel