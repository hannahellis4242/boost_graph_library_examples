targets = main
boost_dir = $(BOOST_DIR)
all : $(targets)

clean :
	rm *.o $(targets)
main : main.o
	clang++ $^ -o $@

main.o : main.cpp
	clang++ -I $(boost_dir) -c $<
