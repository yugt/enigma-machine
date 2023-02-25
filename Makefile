gxx = /usr/bin/g++-11
src = ./src/main.cpp
std = -std=c++20
flag = -O3 -Wall -Wextra -Wpedantic -fdiagnostics-color=always
exce = ./build/enigma

main:
	$(gxx) $(src) $(std) $(flag) -o $(exce)

clean:
	rm $(exce)