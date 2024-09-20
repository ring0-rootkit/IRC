# IRC-Client
*  a command-line IRC client written in C. It allows users to connect to an IRC server, join a channel, and send messages to the channel. The program provides a simple and intuitive interface for users to interact with the IRC server.

# How to use

#### Compiling the Program:
* To compile the program, navigate to the directory containing the Makefile and run the command make. This will create an executable file called ircy in the bin directory.

#### Running the Program:
* To run the program, navigate to the bin directory and run the command ./ircy -s <irc-server> -n <nick> -c <channel>. Replace <irc-server> with the hostname or IP address of the IRC server, <nick> with the desired nickname, and <channel> with the desired channel.

**Example**: `./ircy -s irc.example.com -n <username> -c <your-channel>`

#### Using the Program:
* Once the program is running, you can type messages to send to the channel. The program will automatically send the messages to the channel. You can also use IRC commands, such as /join to join a different channel, or /quit to quit the program.

#### Available Options:

* `-s <irc-server>`: Specify the IRC server to connect to.
* `-n <nick>`: Specify the nickname to use.
* `-c <channel>`: Specify the channel to join.

# Changes & Additions
* 1. Error Handling: Added error handling for socket and connect calls.
* 2. Code Organization: Broke down the main function into separate functions, such as parse_args and startup.
* 3. Code Style: Improved code readability by using consistent indentation and spacing, and more descriptive variable names.
* 4. Security: Added input validation for user-provided input, and used safer alternatives for string manipulation.
* 5. Performance: Used non-blocking I/O and asynchronous I/O to improve performance and responsiveness.
* 6. Portability: Used more portable alternatives for inet_pton and inet_ntop functions.
* 7. Command-line Argument Parsing: Added command-line argument parsing using getopt library.
* 8. IRC Protocol Implementation: Implemented more IRC protocol features, such as handling PING and PONG messages.
* 9. User Interface: Added a more user-friendly interface, such as a command-line interface with more features.
