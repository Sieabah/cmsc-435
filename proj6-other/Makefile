CPPFLAGS = -ansi -Wall -g

seamcarving: *.h *.cpp
	g++ stub.cpp slVector.cpp -lg -o seamcarving

clean:
	rm -f *.o *.out

test:
	./seamcarving house.jpg out.jpg 312 384

test2:
	./seamcarving house.jpg out.jpg 500 384

test3:
	./seamcarving yo_couch.jpg out.jpg 375 200

test4:
	./seamcarving guy.jpg out.jpg 100 333
