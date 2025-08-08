# IRC - Internet Relay Client
> A command-line IRC client written in C that allows users to connect to an IRC server, join a channel, and send messages. This program provides a simple and intuitive interface for interacting with IRC servers.

## Features
- **Improved Error Handling**: Robust error handling ensures stability and reliability.
- **Code Organization**: Modular code structure for better readability and maintainability.
- **Memory Management**: Efficient use of system resources with no memory leaks.
- **Logging**: Comprehensive logging for debugging and tracking program execution.
- **Threading**: Concurrent input/output handling using threads for enhanced performance.

## How to Use

### Compiling the Program
1. Navigate to the directory containing the `Makefile`.
2. Run the command `make` to compile the program.
3. The executable file `ircy` will be created in the `bin` directory.

### Running the Program
1. Navigate to the `bin` directory.
2. Run the command: 
   ```bash
   ./ircy -s <irc-server> -n <nick> -c <channel>
   ```
3. Replace `<irc-server>` with the hostname or IP address of the IRC server.
4. Replace `<nick>` with your desired nickname.
5. Replace `<channel>` with the channel you wish to join.

Example: `./ircy -s irc.example.com -n Disease -c channel-1`


## Using the Program

Type messages to send them to the channel.
 
Use IRC commands like `/join` to switch channels or `/quit` to exit the program.

### Available Options
- `-s <irc-server>`: Specify the IRC server to connect to.
- `-n <nick>`: Specify the nickname to use.
- `-c <channel>`: Specify the channel to join.

## Changes & Additions
- **Error Handling**: Enhanced error handling for socket and connection operations.
- **Code Organization**: Refactored the main function into separate, modular functions.
- **Code Style**: Improved readability with consistent formatting and descriptive variable names.
- **Security**: Input validation and safer string manipulation techniques.
- **Performance**: Non-blocking and asynchronous I/O for improved responsiveness.
- **Portability**: Utilized portable functions for network operations.
- **Command-line Argument Parsing**: Implemented using the getopt library.
- **IRC Protocol Implementation**: Added support for handling PING/PONG messages.
- **User Interface**: Enhanced command-line interface with additional features.

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request with your improvements.

#### License
> This project is licensed under the MIT License. 2024
