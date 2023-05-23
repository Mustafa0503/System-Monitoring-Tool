***********************************how to run the program***************************************************
step 1: make
step 2: ./a3
***********************************fork and pipe************************************************************

because it uses the fork() system call in a for-loop to start many child processes, concurrent programming is recommended. According to the values of j, it produces three child processes altogether, each with a distinct task (0, 1, and 2).

In this context, concurrency refers to a program's capacity to carry out many activities concurrently, such as child processes that the operating system may schedule to run concurrently.


The following examples of concurrent activity in the code:

You generate three child processes in the for-loop using the int values j=0, j3, and j++.
For each value of j, the fork() method is used to start a new process that is a duplicate of the parent process. The brand-new process, also known as a child process, runs separately.
Functions:
1. printSysOnly():This function takes no argument and it’s a void function, it simply prints the information about the system, using struct utsname to access system name, version, release……


2. printUserOnly(): This function also don't have any argument as well as return values, it access the file of current user in the system using utmp and use built in function such as 
setutent() and getutent() to get access, and loop through the file print out all the information that is required.


3.printSystem(): this function is more complicated, in order to avoid redundancy I have written this function such a way that I can use the same function for different command, since each
command sort of needs to access the same information sometimes, I don't need to rewrite the functionality for each function. it takes many arguments I kept first two argument only for future
development, and rest of them is just to indicate if the commands are being called, such as sf(--system), gf(--graphics or --g), seq(--sequential), samples, and tdelay note that there is 
no short cut for commands other that graphics(-g) since it was not required, basically the whole function is a loop within that loop I've accessed the memory(physical and virtual) using struct 
called sysinfo to access all kinds of memeory(used memory = total memory - free memory), and virtual memeory is equal to memory + swap given by the instructos. for each iteration I used the
struct to access new memory usage and if the command line has "--graphics" argument I will also print out the differences from each time, and display |:@ or |#* depending on positive or 
negative change and number of :'s and #'s according to the differences(note that I defined numbers of # : as ceil((curVirMem-preVirMem)*1000) this is how I defined it Ta's told me we can
define it however we want if there is a clear indication of differences. I display each samples for given time (if not given then 1 sec), the conversion of bytes to gb is divide(1024^3)
according to stack overflow, and depending on the command I print the regular version or graphical version of the output. And since we have multiple samples, and display each at a time so
I made an array to reserve space for each and store the memory usage for each index according to the iteration and display it. For number of cores, I used sysconf which was confirmed by 
marcelo it should give me the right numbers, and I also used cat /proc/cpuinfo to check the number of cores and I got the same number. For Cpu calculation I used the formula that was
given by Marcelo during the office hours((t2-i2)-(t1-i1))/(t1-i1)*100, in mathlab I usually get percentage around 0.0001~0.001 if there are more people testing it I would get significantly 
higher percentage (0.1~0.5) which will have many numbers of |'s if it was tested in mathlab should get a reasonable number of |'s please note that when I tested it I defined the number|'s 
according to the percentages that I got, it might print many |'s depending on what server and how many people are using that server. And by definition of the formula, it is being initialized.
as 0.00% and increase or decrease accordingly. And I take the previous iteration value of Cpu as t1 and calculate it from there, the formula was confirmed by Marcelo and I defined the number
of |'s (ceil((cpu*10000)/2)) if there are many |'s when its being tested then please make sure change *10000 part accordingly, when I tested it in math lab this would give me around 3-7|'s
so I defined it that way. And depending on arguments(--user) I will print user sessions accordingly, I did not make new function for this part because it seems like a redundant.
4.digits(): This function checks if whole string is int returns 1 if so and 0 otherwise. And it takes string(pointer) as an argument. If the argument has --sequential I don't clear the screen
otherwise it is the same thing as regular commands(Marcelo said for sequential I can print all the previous memory usage).
5.main(): For the main method I looped the arguments and make special case for when there is no argument(argc==1), otherwise I will get all the commands and set the command indication to 1
and for second for loop(I could put them in same loop but that seems too complicated) I get the positional arguments it can be with samples and tdelay or not flag is indicated it will still
get the values for samples and tdelay and pass it to printSystem() function. and I have if statement to see which flag is indicated and printSystem() will take care of 3 cases(--system, 
--sequential, --graphical) and second if statement is for when there is only positional argument like (./a.out 1 2), and I also kept the valid argument if there is no any valid argument I do
nothing, and for last if statement its only for command --user which only prints the user sessions and system information.
Commands:
--system: It prints out the memory usage, number of cores, default tdelay, default sample(10), and system information.
--user: it print out the user sessions and system information only.
--sequential: same thing as --system but this time I do not refresh the screen. And I can also print out all the previos memory usage according to Marcelo same as --system.
--graphics: it prints out the --system with graphics and I defined the (#) and (:) ceil((curVirMem-preVirMem)*1000) and (|) (ceil((cpu*10000)/2)) NOTE FOR CPU CHANGE*10000 PART DEPENDING ON 
THE SERVER IT MIGHT PRINT MANY |'S IF IT IS NOT CONNECTED TO MATBLAB
--samples=N: set the number of samples accordingly(detail explaination of getting it is in comments of the code)
--tdelay=N: set the seconds of leep accordingly(detail explaination of getting it is in comments of the code)
--g: same as --graphics
********************************************************************************IMPORTANT NOTES*******************************************************************************************

*Formula for CPU: ((t2-i2)-(t1-i1))/(t1-i1)*100 given by Marcelo
***--sequential: according to Marcelo I can also print all the previos iteration of memory usage same as --system
*forula for number of (:)(#)= ceil((curVirMem-preVirMem)*1000)----------------(|)=(ceil((cpu*10000)/2))
****please note that I defined number of (:)(#)(|) based on mathlab and my formula, if there are too many(|) you are testing it in different server or too many people using it kindly change
the *10000 part in (|) = (ceil((cpu*10000)/2)) so you will get better visualization eventhough it will still print out the correcr number of (|) but its just too many.

**just incase the graphic cpu goes to 100%it because you used different server than mine, just in case if it happens please connect to different host.