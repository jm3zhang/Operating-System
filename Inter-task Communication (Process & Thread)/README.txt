This src file contains the c code files for producer and consumer pattern on multi-threads and multi-process, and also include the report file.

File List:
Process.c
thread_produce.c
ece254_lab3_report.docx

To run on multi Process:
$ gcc Process.c -lm -lrt -o produce
$ ./produce <N> <B> <P> <C>

To run on multi Threads:
$ gcc thread_produce.c -o produce -lm -lpthread
$ ./produce <N> <B> <P> <C>


N - the number of integers the producers should produce in total
B - the buffer size
P - the number of producers
C - the number of consumersß