1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

   > **Answer**: The value `fork` provides is that the function helps create a new process like a copy of the process of the original parent which can then be used to run what the original parent was running while the parent could run other processes. This can allow multiple tasks to run at once independently at the same time without conflicts.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

   > **Answer**: If the fork() systme call fails it would mean that it was unable to create a copy of the original process, like memory issues or not having enough space. In my implementation, I returned an error message to stderr if the system call for fork() fails.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

   > **Answer**: Execvp() finds the command to execute like ls or cd by searching through the directories given that it is part of the PATH environment variable.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

   > **Answer**: The purpose of calling wait() in the parent process after forking is to allow the child process to finish executing. If we did not call it, there are possibilities where there are conflicts with commands being ran not in order.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

   > **Answer**: The WEXITSTATUS() provides the information of the child exit status. This is important because it shows us that the child finished executing and what happened to the child when it was processing like if it was successful or it failed.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

   > **Answer**: My implementation of build_cmd_buff() handles quoted arguments by checking where in the command has quotation marks, and when they are detected, it would not parse the white spaces or change what is inside of those quotation marks. The token helps move through the string to check if there is or isn't a quotation mark in order to know when to parse and not parse white spaces depending on seeing a quotation mark. This is necessary because we want to correctly parse white spaces that we know we wouldn't want there and to keep white spaces we purposely put there using quotations.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

   > **Answer**: Changes I made to parsing logic compared to the previous assignment was dealing with knowing when to parse the white spaces instead of just parsing whitespaces whenever we detected them. There was unexpected challenges in refactoring my old code since I had to figure out what logic worked with being able to know when to parse and when not to parse not that there was quotation mark commands that would keep the white spaces.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

  > **Answer**: The purpose of signals in a Linux system is to deliver notifications and determine how the process behaves when the signal is delivered. They differ from other forms of interprocess communication since signals don't transfer data but is used remotely unlike some other forms of IPCs like sockets or message queues.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

  > **Answer**: SIGKILL is typically used to immediately terminate a process even if it is in the middle of running. SIGTERM is typically used similarly to SIGKILL except that the signal can be handled rather than just immediately ending the process. SIGINT is typically also used for stopping the process but by using a key input (CTRL + C) while it is still in the middle of running but can also be ignored like SIGKILL and is usually used to stop a command or script that is running.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

  > **Answer**: When a process receives SIGSTOP, the system stops the process in that moment and is later resumed when it recieves another signal to continue (SIGCONT). It can not be caught or ignored like SIGINT because SIGSTOP is designed to forcefully terminate or stop a process regardless of what state it is in or what permission it has.
