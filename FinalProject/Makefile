# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99

# Source files
SRCS = main.c Address.c Appointment.c Barber.c BarberSalon.c Branch.c \
       Client.c Date.c General.c Kitchen.c KitchenWorker.c list.c \
       Product.c Schedule.c Supplier.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
EXEC = main

# Default rule to build the executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

# Rule to compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(EXEC)

