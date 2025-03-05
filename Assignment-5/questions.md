1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

My implementation ensures that all child processes complete before the shell continues accepting user input by using waitpid() to ensure that the parent waits for all the child processes. If I forgot to call waitpid() on all the child processes, random out of order things could cause problems when running piped commands.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

It is necessary to close unused pipe ends after calling dup2() because we don't want to wait indefinitely for inputs and leave pipes open to cause processes to unintentionally read or write something wrong.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

I think cd is implemented as a built-in rather than an external command because external processes cannot be changed without the need to change the parent process as well. Challenges that would arise is that the directory changes would only be held in the child processes if cd were implemented as an external process.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

Instead of using a fixed number of piped commands with CMD_MAX, I think I could modify my implementation by using malloc() to allocate more memory which would allow more commands. I think the trade-offs I would need to consider is the management of all the allocated memories and to expect memory leaks or problems with allowing more commands than a fixed number.
