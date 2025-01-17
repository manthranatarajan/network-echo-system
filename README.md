# Network Programming

## Overview
Built a network echo server (`echo_s`), an echo client (`echo_c`), and a log server (`log_s`) using C/C++. The goal is to explore networking and multiprocessing concepts by implementing TCP/UDP communication, handling multiple clients, and logging messages.

## Problem Statement
### Key Components
1. **Echo Server (`echo_s`)**:
    - Listens on a specified port and echoes back any message received.
    - Supports both TCP and UDP connections on the same port.
    - Handles multiple clients simultaneously using multiprocessing (e.g., `fork`, `select`).

2. **Echo Client (`echo_c`)**:
    - Sends messages to a server and receives the echoed messages back.

3. **Log Server (`log_s`)**:
    - Receives logs from `echo_s` via UDP and writes them to a log file `echo.log`.

### Features
1. `echo_s` Command-line Usage:
   ```
   $> echo_s <port>
   ```

2. Testing Scenarios:
   - Use `nc` (netcat) as a client with `echo_s` as the server.
   - Use `echo_c` as a client against `nc` as the server.
   - Use `echo_c` as a client against `echo_s` as the server.

   Example commands for `nc`:
   - Start a UDP echo server:
     ```
     nc -e /bin/cat -k -u -l <port>
     ```
   - Start a UDP client:
     ```
     nc -u 127.0.0.1 <port>
     ```

3. Logging Requirements:
   - `echo_s` sends the following information to `log_s` over UDP (port `9999`):
     - Client's IP address.
     - Timestamp (date and time) of the message.
     - The message received and echoed.
   - `log_s` writes logs to `echo.log` in the following format:
     ```
     <timestamp> "<message>" was received from <IP address>
     ```

### Example Log Entries
```
2017-04-07 13:21:33 "Hi, this is me" was received from 10.12.32.15
2017-04-07 13:21:34 "I am just testing" was received from 11.3.2.115
2017-04-07 13:21:35 "OK, I am done" was received from 25.120.42.7
```

## Testing Instructions
1. Compile the programs:
   ```bash
   gcc -o echo_s echo_s.c
   gcc -o echo_c echo_c.c
   gcc -o log_s log_s.c
   ```

2. Start the log server:
   ```bash
   ./log_s
   ```

3. Start the echo server:
   ```bash
   ./echo_s <port>
   ```

4. Test with `nc` and `echo_c`:
   - Use `nc` as a client to send messages to `echo_s`.
   - Use `echo_c` to send messages to `echo_s` and verify responses.
   - Use `echo_c` to interact with `nc` as a server.

5. Verify logs in `echo.log` to ensure proper functionality.

## Notes
- Ensure both `echo_s` and `log_s` handle multiple processes without blocking.
- For further details on `nc`, refer to its manual pages:
  ```bash
  man nc
  ```

---
