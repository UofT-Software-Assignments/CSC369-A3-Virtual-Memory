instructions for running my analysis program are found at the top of analysis.py

# sim.c runs tables

## Trace File: page-simpleloop.ref

### memsize=50

|  Algorithm | hit rate | hit count | Miss count | Overall eviction count | Clean eviction count | Dirty eviction count |
| :--------: | :------: | :-------: | :--------: | :--------------------: | :------------------: | :------------------: |
|    FIFO    |  22.4822 |    759    |     2617   |          2567          |          45          |         2522         |
|    CLOCK   |  25.1481 |    849    |     2527   |          2477          |          0           |         2477         |
|    LRU     |  25.2073 |    851    |     2525   |          2475          |          0           |         2475         |

## memsize=100

|  Algorithm | hit rate | hit count | Miss count | Overall eviction count | Clean eviction count | Dirty eviction count |
| :--------: | :------: | :-------: | :--------: | :--------------------: | :------------------: | :------------------: |
|    FIFO    |  23.7855 |    803    |     2573   |          2473          |          23          |         2450         |
|    CLOCK   |  25.1481 |    849    |     2527   |          2427          |          0           |         2427         |
|    LRU     |  25.2073 |    851    |     2525   |          2425          |          0           |         2425         |



## Trace File: page-matmul.ref 

### memsize=50

|  Algorithm | hit rate | hit count | Miss count | Overall eviction count | Clean eviction count | Dirty eviction count |
| :--------: | :------: | :-------: | :--------: | :--------------------: | :------------------: | :------------------: |
|    FIFO    |  62.9391 |  1913768  |  1126896   |        1126846         |        1104455       |          22391       |
|    CLOCK   |  65.7665 |  1999739  |  1040925   |        1040875         |        1039912       |          963         |
|    LRU     |  65.7665 |  1999739  |  1040925   |        1040875         |        1039914       |          961         |

### memsize=100

|  Algorithm | hit rate | hit count | Miss count | Overall eviction count | Clean eviction count | Dirty eviction count |
| :--------: | :------: | :-------: | :--------: | :--------------------: | :------------------: | :------------------: |
|    FIFO    |  64.3709 |  1957302  |  1083362   |        1083262         |        1071710       |          11552       |
|    CLOCK   |  67.0594 |  2039051  |  1001613   |        1001513         |        1000552       |          961         |
|    LRU     |  66.9061 |  2034389  |  1006275   |        1006175         |        1005215       |          960         |


## Trace File: page-blocked.ref  

### memsize=50

|  Algorithm | hit rate | hit count | Miss count | Overall eviction count | Clean eviction count | Dirty eviction count |
| :--------: | :------: | :-------: | :--------: | :--------------------: | :------------------: | :------------------: |
|    FIFO    |  99.8245 |  3507538  |  6166      |         6116           |         4121         |         1995         |
|    CLOCK   |  99.8473 |  3508339  |  5365      |         5315           |         3039         |         2276         |
|    LRU     |  99.8618 |  3508848  |  4856      |         4806           |         2604         |         2202         |

### memsize=100

|  Algorithm | hit rate | hit count | Miss count | Overall eviction count | Clean eviction count | Dirty eviction count |
| :--------: | :------: | :-------: | :--------: | :--------------------: | :------------------: | :------------------: |
|    FIFO    |  99.8817 |  3509549  |  4155      |         4055           |         2738         |         1317         |
|    CLOCK   |  99.8866 |  3509721  |  3983      |         3883           |         2570         |         1313         |
|    LRU     |  99.8971 |  3510089  |  3615      |         3515           |         2568         |         947          |


##One paragraph comparing the various algorithms in terms of the results you see in the tables.

In general, OPT is always the best for the obvious fact that it knows the future and thus makes the best decisions in the scope of the 
entire liftime of a process. LRU and clock were always very similar hit rates with LRU usually being slightly better, this is due to the fact that
clock is meant to be a computationally efficient approximation to LRU, and this is hoped to achieve the performance of LRU, although on one run clock
was actually slightly better, which is possible since the least recently used isnt garunteed to be the best page to evict and so occasionally clock will
will make a better eviction decision that isnt the least recently used. FIFO always was the worst of the algorithms, this makes sense since clock and LRU
are already forms of FIFO with improvements made and so shouldnt perform any worse. none of the algorithms performed well in the matmul trace, this 
is because the data used in matmul is randomly and dynamically generated and thus a lack of temporal locality meant there was not much to be done. in 
simpleloop, the algorithms could only achieve 22-25% hit rate, this is because the loop loops over data in different pages, and so the only reused page is for the
loop instructions themself. the aglorithms were able to make good use of the locality inherent in the page-blocked.ref trace and performed very well,
but I beleive the 99+ hit rate says more about the trace than it does the algorithms.

====================================================================================================================================================


A table showing the hits and misses of your three traces on LRU, FIFO, and CLOCK.

## trace1.ref results, memsize = 8

|  Algorithm | hit rate | hit count | Miss count |
| :--------: | :------: | :-------: | :--------: |
|  FIFO      |    0     |     0     |     32     |
|  CLOCK     |    0     |     0     |     32     |
|  LRU       |    0     |     0     |     32     |
|  OPT       |    0.031 |     1     |     31     |

## trace2.ref results, memsize = 8

|  Algorithm | hit rate | hit count | Miss count |
| :--------: | :------: | :-------: | :--------: |
|  FIFO      |  8.8235  |     3     |     31     |
|  CLOCK     |  8.8235  |     3     |     31     |
|  LRU       |  8.8235  |     3     |     31     |
|  OPT       |  8.8235  |     3     |     31     |

## trace3.ref results, memsize = 8

|  Algorithm | hit rate | hit count | Miss count |
| :--------: | :------: | :-------: | :--------: |
|  FIFO      |    0     |     0     |     36     |
|  CLOCK     |    0     |     0     |     36     |
|  LRU       |    0     |     0     |     36     |
|  OPT       |    66.67 |     24    |     12     |