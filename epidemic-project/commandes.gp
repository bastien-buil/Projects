set title "Stats evolution"
set ylabel "number of inhabitants"
set xlabel "number of turn"
set terminal png
set output "evolution_stats.png"
plot "evolution.txt" using 1:2 title "alive" with lines, "evolution.txt" using 1:3 title "ill" with lines, "evolution.txt" using 1:4 title "dead" with lines, "evolution.txt" using 1:5 title "burnt" with lines
