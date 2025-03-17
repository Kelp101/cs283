1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

The remote client determines when a command's output is fully received from the server by looking through end of file or line signals. Techniques we can use to handle partial reads or ensure complete message transmission is the recv() function to read the incoming data on sockets.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

A networked shell protocol defines and detects the beginning and end of a command sent over a TCP connection by being able to check where the end of the command usually is by using delimiters to detect either a new line or a null terminator to detect the end of the line of command. Challenges that arise if this is not handled correctly is that some commands might not be read properly or multiple commands might become one or not execute at all.

3. Describe the general differences between stateful and stateless protocols.

Stateful protocols store data within the client and server based interactions while stateless protocols are solely independent and does not store any data or information during client requests to the server.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

Even though UDP is "unreliable", we would use UDP for time-sensitive situations, knowing that UDP is individually packaged and delivered from the sender to the receiver on time.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

The socket interface/abstraction is provided by the operating system to enable applications to use network communications since it is the endpoint for sending and receiving data across network communications.
