all: CImg.h match.cpp
	g++ match.cpp -Wextra -o match -lX11 -lpthread -I. -Isiftpp -O3 siftpp/sift.cpp

clean:
	rm match
