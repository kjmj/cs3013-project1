------- Overview -------
This project simulates a shell environment and allows a user to enter and execute commands.
The program will execute infinitely until the user exits the program (control + c) or "e"

------- Limitations -------
The program will allow a user to add up to 50 additional commands
Additionally, the program can support up to 100 background processes
Background processes that have completed will print out to the terminal after the user answers the original prompt

------- Making and Running the Project -------
To make the project, run "make all"
To run it, type "./mc2"

If you want to pipe in commands from a file:
./mc2 < file.txt

------- Data Structure & Algorithm -------
To store the list of background process, we used a global static array to store the processes.
The background process are stored as a struct storing process id, numInQueue, commandName, startTime.
When we fork a new background process, we store that struct information in the array. Once the background child
has returned, we print its statistics and remove it from the array.

------- Test Cases -------
We tested our code thoroughly to ensure that it worked on all sorts of cases. We tried to cover all of the edge cases,
from bad input to trying to exit before the background tasks have completed. v1 has a test case demonstrating alot of those
edge cases, while the test case in this file demonstrates the edge cases regarding background processes.

We took care to test on out of order commands. An example of this can be found in our sample input below.

Here is some sample input when piped in from a file:

===== Mid-Day Commander, v2 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
   r. running processes : Print list of running processes
Option?: a

-- Add a command --
Command to add?: sleep 3 &
Okay, added with ID 3!

===== Mid-Day Commander, v2 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. sleep 3 & : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
   r. running processes : Print list of running processes
Option?: a

-- Add a command --
Command to add?: sleep 5 &
Okay, added with ID 4!

===== Mid-Day Commander, v2 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. sleep 3 & : User added command
   4. sleep 5 & : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
   r. running processes : Print list of running processes
Option?: a

-- Add a command --
Command to add?: sleep 2
Okay, added with ID 5!

===== Mid-Day Commander, v2 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. sleep 3 & : User added command
   4. sleep 5 & : User added command
   5. sleep 2 : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
   r. running processes : Print list of running processes
Option?: r

-- Background Processes --

===== Mid-Day Commander, v2 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. sleep 3 & : User added command
   4. sleep 5 & : User added command
   5. sleep 2 : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
   r. running processes : Print list of running processes
Option?: 4

-- Command: sleep 5 & --
[1] 7325

===== Mid-Day Commander, v2 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. sleep 3 & : User added command
   4. sleep 5 & : User added command
   5. sleep 2 : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
   r. running processes : Print list of running processes
Option?: r

-- Background Processes --
Background Process Number: [1]
Process ID: 7325
Command Name: sleep 5 &


===== Mid-Day Commander, v2 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. sleep 3 & : User added command
   4. sleep 5 & : User added command
   5. sleep 2 : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
   r. running processes : Print list of running processes
Option?: 5


-- Statistics ---
Elapsed Time: 2014.55 milliseconds
Page Faults: 64
Page Faults (reclaimed): 0

===== Mid-Day Commander, v2 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. sleep 3 & : User added command
   4. sleep 5 & : User added command
   5. sleep 2 : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
   r. running processes : Print list of running processes
Option?: 3

-- Command: sleep 3 & --
[2] 7330

===== Mid-Day Commander, v2 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. sleep 3 & : User added command
   4. sleep 5 & : User added command
   5. sleep 2 : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
   r. running processes : Print list of running processes
Option?: r

-- Background Processes --
Background Process Number: [1]
Process ID: 7325
Command Name: sleep 5 &

Background Process Number: [2]
Process ID: 7330
Command Name: sleep 3 &


===== Mid-Day Commander, v2 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. sleep 3 & : User added command
   4. sleep 5 & : User added command
   5. sleep 2 : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
   r. running processes : Print list of running processes
Option?: 3

-- Command: sleep 3 & --
[3] 7331

===== Mid-Day Commander, v2 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. sleep 3 & : User added command
   4. sleep 5 & : User added command
   5. sleep 2 : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
   r. running processes : Print list of running processes
Option?: 0

-- Who Am I? --
kenneth

-- Statistics ---
Elapsed Time: 2.45 milliseconds
Page Faults: 86
Page Faults (reclaimed): 0

===== Mid-Day Commander, v2 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. sleep 3 & : User added command
   4. sleep 5 & : User added command
   5. sleep 2 : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
   r. running processes : Print list of running processes
Option?: 2

-- Directory Listing --
Arguments?: -l
Path?: /home

total 4
drwxr-xr-x 23 kenneth kenneth 4096 Jan 25 16:13 kenneth

-- Statistics ---
Elapsed Time: 7.00 milliseconds
Page Faults: 130
Page Faults (reclaimed): 0

===== Mid-Day Commander, v2 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. sleep 3 & : User added command
   4. sleep 5 & : User added command
   5. sleep 2 : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
   r. running processes : Print list of running processes
Option?: 5


-- Statistics ---
Elapsed Time: 2004.76 milliseconds
Page Faults: 64
Page Faults (reclaimed): 0

===== Mid-Day Commander, v2 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. sleep 3 & : User added command
   4. sleep 5 & : User added command
   5. sleep 2 : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
   r. running processes : Print list of running processes
Option?: 5


-- Statistics ---
Elapsed Time: 2001.95 milliseconds
Page Faults: 64
Page Faults (reclaimed): 0

===== Mid-Day Commander, v2 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. sleep 3 & : User added command
   4. sleep 5 & : User added command
   5. sleep 2 : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
   r. running processes : Print list of running processes
Option?: -- Job Complete [1] --
Process ID: 7325
[ Output ]

-- Statistics ---
Elapsed Time: 6032.23 milliseconds
Page Faults: 65
Page Faults (reclaimed): 0

-- Job Complete [2] --
Process ID: 7330
[ Output ]

-- Statistics ---
Elapsed Time: 4017.27 milliseconds
Page Faults: 62
Page Faults (reclaimed): 0

-- Job Complete [3] --
Process ID: 7331
[ Output ]

-- Statistics ---
Elapsed Time: 4017.03 milliseconds
Page Faults: 62
Page Faults (reclaimed): 0

r

-- Background Processes --

===== Mid-Day Commander, v2 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. sleep 3 & : User added command
   4. sleep 5 & : User added command
   5. sleep 2 : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
   r. running processes : Print list of running processes
Option?: e

Logging you out, Commander.

