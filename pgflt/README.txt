The pgflt program demonstrates the importance of using several tricks to
avoid page faults and memory allocation during critical runtime. It displays
the amount of time taken and generated page faults when performing various
tasks. This clearly shows the expense due to page faults.

Running the program without any (or invalid) arguments shows the usage
help. The program argument specifies which rt tweak should be performed
and if the runtime tests should be performed.

The tests can be performed on any Linux system (embedded, desktop, server,
with or without PREEMPT_RT). The results will be the same.

The following is a summary of the most important results:

running the test without any rt tweaks
   - main setup very fast
   - first big malloc+access very expensive
   - second big malloc+access very expensive
   - first big stack very expensive
$ sudo ./pgflt 0x10
options: 0x10
      5715 ns     0 faults : main setup
  17466150 ns  2561 faults : testfunc_malloc (0)
  14963662 ns  2560 faults : testfunc_malloc (1)
   2517772 ns     0 faults : testfunc_malloc (2)
   2577208 ns     0 faults : testfunc_malloc (3)
   6113558 ns  1847 faults : testfunc_deepstack (0)
    390386 ns     0 faults : testfunc_deepstack (1)
    321146 ns     0 faults : testfunc_deepstack (2)
    317585 ns     0 faults : testfunc_deepstack (3)

running the test with all rt tweaks
   - main setup very slow (does it really matter?)
   - no expensive malloc+access calls
   - no expensive big stacks
$ sudo ./pgflt 0x1f
options: 0x1f
  31276098 ns  7083 faults : main setup
   2657466 ns     0 faults : testfunc_malloc (0)
   2519973 ns     0 faults : testfunc_malloc (1)
   2543506 ns     0 faults : testfunc_malloc (2)
   2750682 ns     0 faults : testfunc_malloc (3)
    526604 ns     0 faults : testfunc_deepstack (0)
    324543 ns     0 faults : testfunc_deepstack (1)
    318580 ns     0 faults : testfunc_deepstack (2)
    317265 ns     0 faults : testfunc_deepstack (3)
