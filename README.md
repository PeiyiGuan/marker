# What is Marker ?

Marker is an C application that evaluates two programs connected by a pipe with restricted maximum excecution time of 3 seconds. It will provide **stdin** of the first program with data from file ```test.in``` and record the **stdout** of the second program to file ```test.out```. The **stderr** of both program will be recorded in file ```test.err1``` and ```test.err2``` respectively.


# How to run it ?
marker accepts at least 3 arguments as for example:
```./marker ls sampleDir -p- wc -c```

## During execution
marker will start a process running ```ls``` with argument ```sampleDir``` and then start program ```wc``` with argument ```-c```.
If one or both process are still running after 3 seconds,then living child is sent a SIGKILL signal and an error message: 
```marker:At least one process did not finish```

note: ```-p-``` works as an separator.

## What I learned

* Interprocess communication implementation.
* Working with ```fork``` system call to create child process in Linux.
* Working with ```pipe``` system call to create pipe line for data transmission.
* Familiarity with Linux System calls like ```kill(2)```,```sigaction(2)```,```wait(2)```,```pipe(2)```,```fork(2)```,```read(2)```,```write(2)```,```open(2)```,```close(2)```,```signal(7)```,```erroro(3)```,```dup(2)```,```dup2(2)```.where the numbers are represented as:
 > 1  Executable programs or shell commands
        2   System calls (functions provided by the kernel)
         3   Library calls (functions within program libraries)
       4   Special files (usually found in /dev)
       5   File formats and conventions eg /etc/passwd
       6   Games
       7   Miscellaneous (including macro packages and conventions), e.g. man(7), groff(7)
       8   System administration commands (usually only for root)
       9   Kernel routines [Non standard]

## License
[MIT](https://choosealicense.com/licenses/mit/)
