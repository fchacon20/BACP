OBJS = main.o functions.o Course.o Period.o
CC = g++
CFLAGS = -c
LFLAGS = -std=c++11

MMAS : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o MMAS

main.o : Course.cpp Course.h Period.cpp Period.h functions.h main.cpp
	$(CC) $(CFLAGS) -std=c++11 main.cpp

functions.o : Course.cpp Course.h Period.cpp Period.h functions.h functions.cpp
	$(CC) $(CFLAGS) functions.cpp

Course.o : Course.h Course.cpp
	$(CC) $(CFLAGS) Course.cpp

Period.o : Course.h Period.h Period.cpp
	$(CC) $(CFLAGS) Period.cpp

clean:
	\rm main.o MMAS Period.o Course.o functions.o
