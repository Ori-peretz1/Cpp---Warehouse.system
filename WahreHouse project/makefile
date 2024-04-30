all: WareHouse.o Action.o Customer.o main.o Order.o Volunteer.o 
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -o bin/WareHouse bin/Action.o bin/Customer.o bin/main.o bin/Order.o bin/Volunteer.o bin/WareHouse.o

Action.o: 
	g++ -c -g -Wall -Weffc++ -std=c++11 -Iinclude -o bin/Action.o src/Action.cpp

Customer.o: 
	g++ -c -g -Wall -Weffc++ -std=c++11 -Iinclude -o bin/Customer.o src/Customer.cpp

main.o: 
	g++ -c -g -Wall -Weffc++ -std=c++11 -Iinclude -o bin/main.o src/main.cpp

Order.o: 
	g++ -c -g -Wall -Weffc++ -std=c++11 -Iinclude -o bin/Order.o src/Order.cpp

Volunteer.o: 
	g++ -c -g -Wall -Weffc++ -std=c++11 -Iinclude -o bin/Volunteer.o src/Volunteer.cpp

WareHouse.o: 
	g++ -c -g -Wall -Weffc++ -std=c++11 -Iinclude -o bin/WareHouse.o src/WareHouse.cpp


clean:
	rm bin/*.o WareHouse
		