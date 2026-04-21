# # isolate core 2 from regular tasks
# isolcpus=2

# # isolate core 2 from clock interrupts
# nohz_full=2

# limit processor's frequency
sudo cpupower frequency-set --max 2.80GHz
sudo cpupower frequency-set --min 2.80GHz

# turn on performance for all CPU
for f in /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor; do
  echo performance | sudo tee "$f"
done

# disable turbo as it is less stable
echo 1 | sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo

# run benchmark
hyperfine --warmup 7 --runs 25 "taskset -c 2 ./run" --export-markdown assets/benchmark.md

# disable cpu limit
sudo cpupower frequency-set -g performance
