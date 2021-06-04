# Simple Shell
This is a simple Unix shell written in C.

## How to use:
This shell uses `Readline` library to inputs and to have a history for used commands.

So first install `Readline` using the package manager of your distribution (example here for Ubuntu):
```
apt-get install libreadline-dev
```
Then compile the source file `shell.c` using `gcc` compiler with the following flag
```
gcc shell.c -lreadline -o shell
```
After compiling, run the shell by typing (in the same directory):
```
./shell
```
To exit press `CTRL+c`

## Resources used:
- Operating Systems: Internals and Design Principles by William Stallings
- [Tutorial - Write a Shell in C](https://brennan.io/2015/01/16/write-a-shell-in-c/)
- [Tutorial to code a simple shell in C](https://medium.com/swlh/tutorial-to-code-a-simple-shell-in-c-9405b2d3533e)
- [GNU Readline Library](https://tiswww.case.edu/php/chet/readline/rltop.html#Documentation) 

