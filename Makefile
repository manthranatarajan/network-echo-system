CC = gcc
CFLAGS = -Wall -g

ECHO_S = echo_s
ECHO_C = echo_c
LOG_S = log_s

# source files
SRC_ECHO_S = echo_s.c
SRC_ECHO_C = echo_c.c
SRC_LOG_S = log_s.c

.PHONY: build run_echo_s run_echo_c run_log_s clean

build: $(ECHO_S) $(ECHO_C) $(LOG_S)

$(ECHO_S): $(SRC_ECHO_S)
	$(CC) $(CFLAGS) -o $(ECHO_S) $(SRC_ECHO_S)

$(ECHO_C): $(SRC_ECHO_C)
	$(CC) $(CFLAGS) -o $(ECHO_C) $(SRC_ECHO_C)

$(LOG_S): $(SRC_LOG_S)
	$(CC) $(CFLAGS) -o $(LOG_S) $(SRC_LOG_S)

run_echo_s: $(ECHO_S)
	./$(ECHO_S) 12345

run_echo_c: $(ECHO_C)
	./$(ECHO_C) 127.0.0.1 12345

run_log_s: $(LOG_S)
	./$(LOG_S)

clean:
	rm -f $(ECHO_S) $(ECHO_C) $(LOG_S)

