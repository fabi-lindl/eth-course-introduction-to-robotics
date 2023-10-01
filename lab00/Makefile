# add entries for all the additional programs
all: sum_numbers manipulate_two_numbers

# linking of sum_numbers.o with functions.o
sum_numbers: sum_numbers.o functions.o
	gcc sum_numbers.o functions.o -o sum_numbers

# linking of manipulate_two_numbers.o with functions.o
manipulate_two_numbers: manipulate_two_numbers.o functions.o
	gcc manipulate_two_numbers.o functions.o -o manipulate_two_numbers
	
# compile sum_numbers.c	
sum_numbers.o: sum_numbers.c
	gcc -c sum_numbers.c
	
# compile functions.c
functions.o: functions.h functions.c
	gcc -c functions.c

# compile manipulate_two_numbers.c
manipulate_two_numbers.o: manipulate_two_numbers.c
	gcc -c manipulate_two_numbers.c

# remove generated files and programs
clean:
	rm functions.o sum_numbers.o sum_numbers
	rm manipulate_two_numbers.o manipulate_two_numbers
