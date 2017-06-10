CC = gcc

# put your server object files here
OBJS1 = asm.o

# put your client object files here
OBJS2 = sim.o 

# The executabel filename of the server app
EXEC1 = asm

# The executabel filename of the client app
EXEC2 = sim


C_COMP_FLAG = -Wall -g  -DNDEBUG 

#C_COMP_FLAG = -std=c99 -Wall -Wextra \
#-Werror -pedantic-errors -DNDEBUG

all: $(EXEC1) $(EXEC2)

asm.o:  asm.h 
		$(CC) $(C_COMP_FLAG)  -c $*.c

sim.o:	sim.h   
		$(CC) $(C_COMP_FLAG)  -c $*.c

$(EXEC1): $(OBJS1)
		$(CC) $(OBJS1) -o $@
		
$(EXEC2): $(OBJS2)
		$(CC) $(OBJS2) -o $@

# a rule for building a simple c souorce file


clean:
		rm -f $(OBJS1) $(OBJS2) $(EXEC1) $(EXEC2)

