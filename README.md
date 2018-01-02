<img src="https://www.holbertonschool.com/assets/holberton-logo-1cc451260ca3cd297def53f2250a9794810667c7ca7b5fa5879a569a457bf16f.png" alt="Holberton logo">

my_shell Project
================

The purpose of this project is to create a simplified clone of the BASH shell. With restrictions in place to
encourage a deeper understanding of Unix and the C programming language.

C Requirements
--------------
* All programs are compiled Ubuntu 14.04 LTS using gcc 4.8.4 (C90) using the flags -Wall -Werror -Wextra -pedantic.
```
    gcc -Wall -Werror -Wextra -pedantic
```

* Allowed functions:
```
    access (man 2 access)
    chdir (man 2 chdir)
    close (man 2 close)
    closedir (man 3 closedir)
    execve (man 2 execve)
    exit (man 3 exit)
    fork (man 2 fork)
    free (man 3 free)
    fstat (man 2 fstat)
    getcwd (man 3 getcwd)
    getline (man 3 getline)
    kill (man 2 kill)
    lstat (man 2 lstat)
    malloc (man 3 malloc)
    open (man 2 open)
    opendir (man 3 opendir)
    perror (man 3 perror)
    read (man 2 read)
    readdir (man 3 readdir)
    signal (man 2 signal)
    stat (man 2 stat)
    strtok (man 3 strtok)
    wait (man 2 wait)
    waitpid (man 2 waitpid)
    wait3 (man 2 wait3)
    wait4 (man 2 wait4)
    write (man 2 write)
    _exit (man 2 _exit)
```

Description
-----------
The simple_shell implements a language that has flow control constructs, a macro facility that provides a variety of 
features in addition to data storage, along with built in history and line editing capabilities. It incorporates many 
features to aid interactive use. Commands can be typed directly to the running shell or can be put into a file and 
the file can be executed directly by the shell.

Usage
-----
To use my_shell, compile and run, then give it commands. my_shell commands follow the following syntax: ```command [arguments]```.

Author
------
All assignments completed by me, [Daniel Bacon](https://github.com/dfbacon)
