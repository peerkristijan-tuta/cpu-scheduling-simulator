1. Usage instruction

Run the program. Then, enter either 1 for first-come, first-serve, 2 for shortest-job-first, 3 for priority, 4 for round-robin (each process equally gets the same CPU time and if it can't finish in that time, then it will need to join the end of the queue), and 5 for priority with round-robin for equal priority tasks

2. 

Program information

The program's root function is main. It uses a struct called profile to keep track of the state of each thread that it launches, which represent a task. There are 5 tasks, outlined here:

T1, 4, 4
T2, 2, 5
T3, 3, 5
T4, 3, 3
T5, 10, 2

The first column is the task name, the second the priority level (the higher the better), and the third the CPU burst (the amount of time that it takes for the task to complete as measured in seconds). Each thread runs the execute method. In the execute method, the CPU sleeps for the amount of time that it is allowed by field 'allowance' in its given struct instance until it sleeps for an amount of time corresponding to its it's struct 'burst' field. For all algorithms except round-robin, main gives each task the same allowance as its CPU burst in effect allowing each task to finish in one go because when a task is scheduled, it runs until it's completed. However, for tasks under round-robin, main assigns an allowance of 1 second, thereby requiring them to queue up several times until they're finished. 
