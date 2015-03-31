#set size 5.0/3, 1
set term postscript eps enhanced
set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set output "results.eps"
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically
set title ""
set xlabel "Erlang" font ",18"
set ylabel "Blocking probability" font ",18"
#set xr [0:80]
#set yr [0:0.2]

set logscale y

set key font ",18"
set xtics font ", 18" 
set ytics font ", 18" 

set key spacing 3 top left noinvert box
#set key width +5

set style line 1 lt 1 pt 1
set style line 2 lt 1 pt 2
set style line 3 lt 1 pt 3
set style line 4 lt 2 pt 1
set style line 5 lt 2 pt 2
set style line 6 lt 2 pt 3
set style line 7 lt 4 pt 1
set style line 8 lt 4 pt 2
set style line 9 lt 4 pt 3


plot "normal-lambda1-results.txt"    using ($1):($2) ls 1 title ' Normal, {/Symbol l}_1 ' with linespoints, \
        "normal-lambda2-results.txt"    using ($1):($2) ls 2 title ' Normal, {/Symbol l}_2 ' with linespoints, \
        "normal-lambda3-results.txt"    using ($1):($2) ls 3 title ' Normal, {/Symbol l}_3 ' with linespoints, \
        "poisson-lambda1-results.txt"    using ($1):($2) ls 4 title ' Poisson, {/Symbol l}_1 ' with linespoints, \
        "poisson-lambda2-results.txt"    using ($1):($2) ls 5 title ' Poisson, {/Symbol l}_2 ' with linespoints, \
        "poisson-lambda3-results.txt"    using ($1):($2) ls 6 title ' Poisson, {/Symbol l}_3 ' with linespoints, \
        "pareto-lambda1-results.txt"    using ($1):($2) ls 7 title ' Pareto, {/Symbol l}_1 ' with linespoints, \
        "pareto-lambda2-results.txt"    using ($1):($2) ls 8 title ' Pareto, {/Symbol l}_2 ' with linespoints, \
        "pareto-lambda3-results.txt"    using ($1):($2) ls 9 title ' Pareto, {/Symbol l}_3 ' with linespoints
