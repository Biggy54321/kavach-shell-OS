# Shell interpreter (Operating Systems)
# MIS : 111703041
# Name : Bhaskar Pardeshi
# Year : TY

### Input redirection

+ Usage : cmd <infile
+ If multiple input files are given to the same program than the last
  file is taken into consideration, i.e.
  cmd <infile1 <infile2 <infile3, here infile3 will be used to redirect
  the input

### Output redirection

+ Usage : cmd >outfile
+ If multiple output files are given to the same program than the last
  file is taken into consideration, i.e.
  cmd >outfile1 >outfile2 >outfile3, here outfile3 will be used to redirect
  the output

### Pipes

+ Usage : cmd (<infile)* (>outfile)* (| cmd (<infile)* (>outfile)*)*
  (Its funny how, I have written a grammar using regular expression)
+ Multiple pipes are supported in this shell

### Background process

+ Usage : (cmd) &
+ Specifying & at the very end of the (piped) command will move the
  cmd to the background
+ & is not supported in between (potential update)

### Signal handling

+ It is made sure that the signals are directed to foreground process group only (as the fork-execed process form a new group every time)

+ SIGINT

    - Pressing ctrl-c while in prompt prints new prompt
    - Pressing ctrl-c while in command execution will terminate the command

+ SIGTSTP

    - Pressing ctrl-z while in prompt ignores the signal
    - Pressing ctrl-z while in command execution will suspend the command

+ Few more signals (SIGCHLD, SIGCONT, SIGTTOU, SIGTTIN) are used

### Job handling

+ The process groups can be switched to foreground if suspended or in background using the <fg pid> command. Specifying pid of a process will move the group in which that pid lies to the foreground of the controlling terminal
+ The process groups can be switched to background if suspended using the <bg pid> command. Specifying pid of process will move the group in which the pid lies to the background
+ The suspended or background process groups can be viewed using <jobs> command

### Built-ins

+ cd (change directory)
+ fg (foreground switch)
+ bg (background switch)
+ jobs (print jobs)

### Miscellaneous

+ Pressing ctrl-d on blank prompt will exit the shell program
+ Error handling is not so good
+ Its completely colorless shell (as systems people should never enter the darkness of UI)
+ The shell is named "kavach"