input_file = ARG1

basename = substr(input_file, 1, strlen(input_file) - 4)


set datafile separator ","
set grid
set key top left


time_seconds(x) = x * 2


set terminal pngcairo size 1200,600 enhanced font "Verdana,14"
set output sprintf("%s-temp.png", basename)

set title sprintf("Temperature of Core 2 (%s)", basename)
set xlabel "Time (seconds)"
set ylabel "Temperature (°C)"

plot input_file using (time_seconds($0)):7 with lines lw 2 lc rgb "#E15759" title "Core 2 Temp"