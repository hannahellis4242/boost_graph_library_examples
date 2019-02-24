targets = main
boost_dir = $(BOOST_DIR)
all : $(targets)

clean :
	rm *.o *.dot *.svg *.png $(targets)
main : main.o
	clang++ $^ -o $@

main.o : main.cpp
	clang++ -I $(boost_dir) -c $<
