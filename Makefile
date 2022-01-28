PACKAGE = bp_tree
SRCS	= node.cpp node.h main.cpp
OBJS	= $(SRCS:.c=.o)
#HEADERS = 
CC	= g++
CFLAGS	= -g -O2 -Wall

all: $(PACKAGE)

$(PACKAGE) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

.o : $(SRCS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS) *~ $(PACKAGE)
