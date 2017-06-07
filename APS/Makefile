start: main.o garage.o
	clang++ -std=c++11 -stdlib=libc++ main.o garage.o -o start

main.o: main.cpp
	clang++ -std=c++11 -stdlib=libc++ -c main.cpp

garage.o: garage.cpp garage.hpp
	clang++ -std=c++11 -stdlib=libc++ -c garage.cpp
    
clean:
	rm *.o start