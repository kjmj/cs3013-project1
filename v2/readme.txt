This project simulates a shell environment and allows a user to enter and execute commands.
The program will execute infinitely until the user exits the program (control + c) or "e"

The program will allow a user to add up to 50 additional commands

To make the project, run "make all"
To run it, type "./mc2"

If you want to pipe in commands from a file:
./mc2 < file.txt

Data Structure & Algorithm
To store the list of background process, we used a global static array to store the processes.
The background process are created as a struct process id, numInQueue, commandName, startTime.
We store the process into the array when fork() a background process. We remove certain process
in the array by moving every element after that process forward by one position when the process
is completed.