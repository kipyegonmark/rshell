CFLAG = -Wall -Werror -ansi -pedantic
COMPILER = g++

all: rshell ls rm mv cp

rshell:
    mkdir -p bin
    $(COMPILER) $(CFLAG) src/main.cpp -o bin/rshell