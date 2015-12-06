CPPFLAGS = -ansi -Wall -g

seamcarving: *.h *.cpp
	g++ stub.cpp slVector.cpp -lg -o seamcarving

clean:
	rm -f *.o *.out

test:
	./seamcarving house.jpg out.jpg 312 384

