# for s-tui
sudo modprobe msr
rm cpu-info/cpu-data_$1.csv
sudo ~/.local/bin/s-tui --csv --csv-file cpu-info/cpu-data_$1.csv --no-mouse
