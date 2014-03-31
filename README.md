piper
=====

Simple C program that cross-connects stdin and stdout of two processes, almost certainly a duplicate on github.
Piper is a simple C program that works with pipes which I want to have on
github.


```
make piper && ./piper

piper executable will print documentation if called without params.

  piper:

    A.stdout  =>  B.stdin
 +---+             +---+
 | A |             | B |
 +---+             +---+
    A.stdin   <=  B.stdout

 (ascii art is like a hundred words)

./piper ./A ./B
```
