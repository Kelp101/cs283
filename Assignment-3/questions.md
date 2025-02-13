1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  'fgets()' is a good choice for this application since we want to check what the user inputs for one line commands when running the application, which is what 'fgets()' does.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  At first I didn't allocate memory for 'cmd_buff' in 'dsh_cli.c' and realized none of the commands were being registered when testing the application. But the use of 'malloc()' is needed instead of allocating a fixed-size array because allocating memory on the heap would cause less memory errors than using an array and allocating memory on the stack.

3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  It is necessary to trim leading and trailing spaces for each command before storing it because whitespaces can lead to inconsistent formatting or returning stored data that looks like a mess, and sometimes leading and trailing whitespaces are hard to see and can sometimes mess up a command on the command line.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  Example One: We should implement in our custom shell the ability to redirect outputs into a file. Example Two: Another thing we should implement in our custom shell is the ability to redirect contents of a file into an application to be ran. Example Three: Lastly we should implement in our custom shell the ability to redirect error messages into a file after running an application. Challenges that might occur when implementing them is being able to know if we are redirecting into a valid file or not and being able to use the right commands like > and < to input or output what we want.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection is used for redirecting information or data from one place to another, like a programs output into a text file or using the inputs of a textfile and redirecting it into a program. While piping is used to chain commands together to have something specific be done like listing out x amount of names that only have y amount of letters.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  It is important to keep these separate because it is beneficial to have the option to pick and choose whether we would want just the output from running a program or just the error messages, and even being able to mix and match instead of not having these separate in the shell and always printing out an error message with a correct output.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  In our custom shell, if a command fails, we should handle it by printing the output as the error message for the commands that fail while printing the output for the ones that did not fail in a way where its clear which commands failed. I think that we should also provide a way to merge them because we sometimes want to see both the command as well as what the error was in that command that was typed. We could merge them with the use of piping, by having a chain of commands do something specific that we want like having the output be inside a text file and chaining that with error messages for the x amount of outputs that failed.