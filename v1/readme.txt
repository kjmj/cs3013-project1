------- Overview -------
This project simulates a shell environment and allows a user to enter and execute commands.
The program will execute infinitely until the user exits the program (control + c) or "e"

------- Limitations -------
The program will allow a user to add up to 50 additional commands

------- Making and Running the Project -------
To make the project, run "make all"
To run it, type "./mc1"

If you want to pipe in commands from a file:
./mc1 < file.txt

------- Test Cases -------
We did substantial testing on this version, both from user input and piping from a file. We tried to hit
all of the edge cases, such as bad input, bad paths, no input, and running dependant commands in a certain order.

Here is a sample log:

kenneth@kenneth-VirtualBox:~/Documents/cs3103/cs3013-project1/v1$ ./mc1 < commands.txt
===== Mid-Day Commander, v1 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
Option?: a

-- Add a command --
Command to add?: ls -a -al
Okay, added with ID 3!

===== Mid-Day Commander, v1 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. ls -a -al : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
Option?: 0

-- Who Am I? --
kenneth

-- Statistics ---
Elapsed Time: 1.23 milliseconds
Page Faults: 90
Page Faults (reclaimed): 0

===== Mid-Day Commander, v1 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. ls -a -al : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
Option?: 1

-- Last Logins --
kenneth  tty7         :0               Fri Jan 25 16:12    gone - no logout
reboot   system boot  4.4.162kjmorton  Fri Jan 25 16:11   still running
kenneth  tty7         :0               Thu Jan 24 19:12 - down   (20:51)
reboot   system boot  4.4.162kjmorton  Thu Jan 24 19:10 - 16:03  (20:52)
kenneth  tty7         :0               Thu Jan 24 09:01 - crash  (10:09)
reboot   system boot  4.4.162kjmorton  Thu Jan 24 08:59 - 16:03 (1+07:04)
kenneth  tty7         :0               Wed Jan 23 14:41 - crash  (18:17)
reboot   system boot  4.4.162kjmorton  Wed Jan 23 14:40 - 16:03 (2+01:23)
kenneth  tty7         :0               Tue Jan 22 21:10 - down   (17:20)
reboot   system boot  4.4.162kjmorton  Tue Jan 22 21:08 - 14:31  (17:22)
kenneth  tty7         :0               Tue Jan 22 15:22 - crash  (05:46)
reboot   system boot  4.4.162kjmorton  Tue Jan 22 15:08 - 14:31  (23:23)
kenneth  tty7         :0               Fri Jan 18 21:28 - crash (3+17:40)
reboot   system boot  4.4.162kjmorton  Fri Jan 18 21:20 - 14:31 (4+17:10)
kenneth  tty7         :0               Fri Jan 18 15:06 - down   (03:03)
reboot   system boot  4.4.162kjmorton  Fri Jan 18 15:01 - 18:09  (03:07)
kenneth  tty7         :0               Thu Jan 17 19:55 - down   (00:55)
reboot   system boot  4.4.162kjmorton  Thu Jan 17 19:53 - 20:50  (00:57)
kenneth  tty7         :0               Thu Jan 17 15:48 - crash  (04:04)
reboot   system boot  4.4.162kjmorton  Thu Jan 17 15:46 - 20:50  (05:03)
kenneth  tty7         :0               Wed Jan 16 09:06 - down  (1+00:10)
reboot   system boot  4.4.162kjmorton  Wed Jan 16 09:04 - 09:17 (1+00:12)
kenneth  tty7         :0               Tue Jan 15 16:16 - crash  (16:47)
reboot   system boot  4.4.162kjmorton  Tue Jan 15 16:15 - 09:17 (1+17:02)
kenneth  tty7         :0               Tue Jan 15 15:01 - down   (01:10)
reboot   system boot  4.4.162kjmorton  Tue Jan 15 14:59 - 16:12  (01:12)
kenneth  tty7         :0               Sun Jan 13 20:07 - down   (00:00)
reboot   system boot  4.4.162kjmorton  Sun Jan 13 20:05 - 20:08  (00:02)
kenneth  tty7         :0               Sun Jan 13 20:04 - down   (00:00)
reboot   system boot  4.4.162kjmorton  Sun Jan 13 20:02 - 20:05  (00:02)
reboot   system boot  4.4.162kjmorton  Sun Jan 13 20:00 - 20:05  (00:04)
kenneth  tty7         :0               Sun Jan 13 19:41 - crash  (00:19)
reboot   system boot  4.4.162kjmorton  Sun Jan 13 19:41 - 20:05  (00:23)
kenneth  tty7         :0               Sun Jan 13 18:43 - down   (00:18)
reboot   system boot  4.4.162kjmorton  Sun Jan 13 18:43 - 19:02  (00:18)
kenneth  tty7         :0               Sun Jan 13 16:11 - down   (02:30)
reboot   system boot  4.4.0-141-generi Sun Jan 13 16:11 - 18:42  (02:31)
kenneth  tty7         :0               Sun Jan 13 16:04 - down   (00:06)
reboot   system boot  4.4.0-141-generi Sun Jan 13 16:04 - 16:10  (00:06)
kenneth  tty7         :0               Sun Jan 13 16:03 - down   (00:00)
reboot   system boot  4.4.0-141-generi Sun Jan 13 16:03 - 16:04  (00:00)
kenneth  tty7         :0               Sun Jan 13 15:58 - down   (00:03)
reboot   system boot  4.4.0-141-generi Sun Jan 13 15:58 - 16:02  (00:04)
kenneth  tty7         :0               Sun Jan 13 15:38 - crash  (00:20)
reboot   system boot  4.4.0-31-generic Sun Jan 13 15:38 - 16:02  (00:24)
kenneth  tty7         :0               Sun Jan 13 15:36 - down   (00:01)
reboot   system boot  4.4.0-31-generic Sun Jan 13 15:36 - 15:37  (00:01)

wtmp begins Sun Jan 13 15:36:11 2019

-- Statistics ---
Elapsed Time: 1.85 milliseconds
Page Faults: 112
Page Faults (reclaimed): 0

===== Mid-Day Commander, v1 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. ls -a -al : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
Option?: 2

-- Directory Listing --
Arguments?: -al -l
Path?: /

total 104
drwxr-xr-x  23 root root  4096 Jan 13 19:57 .
drwxr-xr-x  23 root root  4096 Jan 13 19:57 ..
drwxr-xr-x   2 root root  4096 Jan 13 15:42 bin
drwxr-xr-x   3 root root  4096 Jan 16 14:25 boot
drwxrwxr-x   2 root root  4096 Jan 13 15:33 cdrom
drwxr-xr-x  18 root root  3880 Jan 25 16:12 dev
drwxr-xr-x 134 root root 12288 Jan 25 16:12 etc
drwxr-xr-x   3 root root  4096 Jan 13 15:33 home
lrwxrwxrwx   1 root root    33 Jan 13 15:52 initrd.img -> boot/initrd.img-4.4.0-141-generic
lrwxrwxrwx   1 root root    32 Jan 13 15:34 initrd.img.old -> boot/initrd.img-4.4.0-31-generic
drwxr-xr-x  22 root root  4096 Jan 13 15:41 lib
drwx------   2 root root 16384 Jan 13 15:31 lost+found
drwxr-xr-x   3 root root  4096 Jan 13 16:00 media
drwxr-xr-x   2 root root  4096 Jul 19  2016 mnt
drwxr-xr-x   4 root root  4096 Jan 13 16:06 opt
dr-xr-xr-x 196 root root     0 Jan 25 16:11 proc
drwx------   3 root root  4096 Jul 19  2016 root
drwxr-xr-x  25 root root   820 Jan 25 16:17 run
drwxr-xr-x   2 root root 12288 Jan 13 16:01 sbin
drwxr-xr-x   2 root root  4096 Jan 13 15:45 snap
drwxr-xr-x   2 root root  4096 Jul 19  2016 srv
dr-xr-xr-x  13 root root     0 Jan 25 19:34 sys
drwxrwxrwt  10 root root  4096 Jan 25 19:39 tmp
drwxr-xr-x  11 root root  4096 Jul 19  2016 usr
drwxr-xr-x  14 root root  4096 Jul 19  2016 var
lrwxrwxrwx   1 root root    30 Jan 13 15:52 vmlinuz -> boot/vmlinuz-4.4.0-141-generic
lrwxrwxrwx   1 root root    29 Jan 13 15:34 vmlinuz.old -> boot/vmlinuz-4.4.0-31-generic

-- Statistics ---
Elapsed Time: 2.34 milliseconds
Page Faults: 134
Page Faults (reclaimed): 0

===== Mid-Day Commander, v1 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. ls -a -al : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
Option?: c

-- Change Directory --
New Directory?: /home

===== Mid-Day Commander, v1 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. ls -a -al : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
Option?: p

-- Current Directory --
Directory: /home

===== Mid-Day Commander, v1 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. ls -a -al : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
Option?: 3

total 12
drwxr-xr-x  3 root    root    4096 Jan 13 15:33 .
drwxr-xr-x 23 root    root    4096 Jan 13 19:57 ..
drwxr-xr-x 23 kenneth kenneth 4096 Jan 25 16:13 kenneth

-- Statistics ---
Elapsed Time: 4.33 milliseconds
Page Faults: 133
Page Faults (reclaimed): 0

===== Mid-Day Commander, v1 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. ls -a -al : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
Option?: a

-- Add a command --
Command to add?: ls -al --color=auto
Okay, added with ID 4!

===== Mid-Day Commander, v1 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. ls -a -al : User added command
   4. ls -al --color=auto : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
Option?: 4

total 12
drwxr-xr-x  3 root    root    4096 Jan 13 15:33 .
drwxr-xr-x 23 root    root    4096 Jan 13 19:57 ..
drwxr-xr-x 23 kenneth kenneth 4096 Jan 25 16:13 kenneth

-- Statistics ---
Elapsed Time: 4.44 milliseconds
Page Faults: 135
Page Faults (reclaimed): 0

===== Mid-Day Commander, v1 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. ls -a -al : User added command
   4. ls -al --color=auto : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
Option?: 2

-- Directory Listing --
Arguments?:
Path?:

kenneth

-- Statistics ---
Elapsed Time: 0.47 milliseconds
Page Faults: 98
Page Faults (reclaimed): 0

===== Mid-Day Commander, v1 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. ls -a -al : User added command
   4. ls -al --color=auto : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
Option?: 0

-- Who Am I? --
kenneth

-- Statistics ---
Elapsed Time: 1.14 milliseconds
Page Faults: 91
Page Faults (reclaimed): 0

===== Mid-Day Commander, v1 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. ls -a -al : User added command
   4. ls -al --color=auto : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
Option?: 2

-- Directory Listing --
Arguments?: --color=auto
Path?:

kenneth

-- Statistics ---
Elapsed Time: 1.24 milliseconds
Page Faults: 98
Page Faults (reclaimed): 0

===== Mid-Day Commander, v1 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. ls -a -al : User added command
   4. ls -al --color=auto : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
Option?: 0

-- Who Am I? --
kenneth

-- Statistics ---
Elapsed Time: 1.11 milliseconds
Page Faults: 90
Page Faults (reclaimed): 0

===== Mid-Day Commander, v1 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. ls -a -al : User added command
   4. ls -al --color=auto : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
Option?: 2

-- Directory Listing --
Arguments?:
Path?: /home

kenneth

-- Statistics ---
Elapsed Time: 1.14 milliseconds
Page Faults: 97
Page Faults (reclaimed): 0

===== Mid-Day Commander, v1 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. ls -a -al : User added command
   4. ls -al --color=auto : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
Option?: 0

-- Who Am I? --
kenneth

-- Statistics ---
Elapsed Time: 0.95 milliseconds
Page Faults: 91
Page Faults (reclaimed): 0

===== Mid-Day Commander, v1 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. ls -a -al : User added command
   4. ls -al --color=auto : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
Option?: badInput

Invalid input, MDC does not have a command associated with the input "badInput".

===== Mid-Day Commander, v1 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. ls -a -al : User added command
   4. ls -al --color=auto : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
Option?: 12

Invalid input, MDC does not have a command associated with the input "12".

===== Mid-Day Commander, v1 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. ls -a -al : User added command
   4. ls -al --color=auto : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
Option?: 2

-- Directory Listing --
Arguments?:
Path?:

kenneth

-- Statistics ---
Elapsed Time: 4.34 milliseconds
Page Faults: 99
Page Faults (reclaimed): 0

===== Mid-Day Commander, v1 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. ls -a -al : User added command
   4. ls -al --color=auto : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
Option?: more bad input

Invalid input, MDC does not have a command associated with the input "more bad input".

===== Mid-Day Commander, v1 =====
G’day, Commander! What command would you like to run?
   0. whoami  : Prints out the result of the whoamicommand
   1. last    : Prints out the result of the last command
   2. ls      : Prints out the result of a listing on a user-specified path
   3. ls -a -al : User added command
   4. ls -al --color=auto : User added command
   a. add command : Adds a new command to the menu.
   c. change directory : Changes process working directory
   e. exit : Leave Mid-Day Commander
   p. pwd : Prints working directory
Option?: e

Logging you out, Commander.
