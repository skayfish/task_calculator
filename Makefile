all:
	rm -rf ./build && mkdir build && cd build && g++ -std=c++17 -g ../main.cpp ../tester_calculator.cpp ../expression.cpp -o calculator && cd ..

clean:
	rm -rf ./build