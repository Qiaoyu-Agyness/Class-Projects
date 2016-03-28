

## Start of the Makefile
main: main.o decide.o small.o robomall.o init_mall.o initializeRobot.o treeFunctions.o eventListFunctions.o initializeShopper.o eventListShopperFunctions.o
	g++ -o main main.o decide.o small.o robomall.o init_mall.o initializeRobot.o treeFunctions.o eventListFunctions.o initializeShopper.o eventListShopperFunctions.o

main.o: main.cpp globals.h
	g++ -Wall -c main.cpp

decide.o: decide.cpp globals.h externals.h
	g++ -Wall -c decide.cpp

small.o: small.cpp globals.h externals.h
	g++ -Wall -c small.cpp

robomall.o: robomall.cpp globals.h externals.h
	g++ -Wall -c robomall.cpp

init_mall.o: init_mall.cpp globals.h externals.h
	g++ -Wall -c init_mall.cpp

eventListFunctions.o: eventListFunctions.cpp globals.h
	g++ -Wall -c eventListFunctions.cpp

initializeRobot.o: initializeRobot.cpp globals.h
	g++ -Wall -c initializeRobot.cpp

treeFunctions.o: treeFunctions.cpp globals.h
	g++ -Wall -c treeFunctions.cpp

initializeShopper.o: initializeShopper.cpp globals.h
	g++ -Wall -c initializeShopper.cpp

eventListShopperFunctions.o: eventListShopperFunctions.cpp globals.h
	g++ -Wall -c eventListShopperFunctions.cpp

clean:
	rm *.o
## end of the Makefile
