# Guessing Game

This guessing game is a simple game where a player is either a guesser
or a setter, the guesser sends integer numbers to the setter who then
decides whether the given guess was correct or not. If the guesser was
incorrect, then the setter tells the guesser whether their guess was
too high or too low.

## Architecture
	
### Client

The clie

### Server

The server is made up of three main components:
 - UI
 - Controller
 - Server/Networking
 
Each is defined as an interface, allowing us to swap out the concrete
implementations easily for future work and testing.
 
#### UI
As the name suggests, this component is what handles the interaction
between the user and the application. 

## Protocol

The guessing game uses a very simple text based protocol similar to
that of the IRC chat protocol. Messages are made up of a command
followed by parameters if required as follows:

**COMMAND** **PARAMETER**

Each part is seperated by a space. Each message has a maximum size of
512 bytes, anything after this limit is ignored.

The commands are as follows:

| Command        | Parameter            | Description                                                                           | Sender | Responses                   |
|----------------|----------------------|---------------------------------------------------------------------------------------|--------|-----------------------------|
| **CONNECT**    | *name*               | This signals the start of a session, the *name* parameter is the player's identifier. | Client | **OK**, **BUSY**, **ERROR** |
| **DISCONNECT** | *name*               | This signals the end of the session.                                                  | Client | **OK**, **ERROR**           |
| **OK**         |                      | Acknowledgement of a successful command.                                              | Server |                             |
| **BUSY**       |                      | Signals to the client that the server has reached it's player capacity.               | Server |                             |
| **ERROR**      |                      | Signals that an error has occured.                                                    | Server |                             |
| **GUESS**      | *number*             | Client sends a guess to the server.                                                   | Client | **ANSWER**, **ERROR**       |
| **ANSWER**     | *result*             | Returns either correct, higher or lower depending on the guess.                       | Server |                             |
| **GET_SCORE**  | *name*               | Client requests the current score for player *name*.                                  | Client | **SCORE**, **ERROR**        |
| **SCORE**      | *correct*/*attempts* | Server sends the requested players current score and total attempts, separated by /.  | Server |                             |
