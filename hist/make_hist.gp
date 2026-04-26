set boxwidth 1.0 relative
set style data histograms
set style histogram cluster
set style fill solid 1.0 border rgb "#021a4d"

set yrange [0:*]
set xrange [-25:5013]

set tmargin 7
set title "Hash distribution plot of ".ARGV[2]." hash function" font ",30"

# remove legend
set key off

set terminal pngcairo color size 3200, 1000
set output ARGV[1]

set bmargin at screen 0.12 
set lmargin at screen 0.06 

set ytics font ",20"
set xtics font ",20"

set xlabel 'Bucket index' font ",25" offset 0, -1
set ylabel 'Number of elements in bucket' font ",20" offset -5, 0

plot 'hist/data.dat' using 1 with histograms fc rgb "#030865" lc rgb "#000f2f" lw 0.65

unset output
show output