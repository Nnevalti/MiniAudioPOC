#include "IOStream.h"
#include <iostream>

int main()
{
	IOStream ioStream;
	// std::cout << "GETTING DEVICE INFO ..." << std::endl;
	ioStream.getDeviceInfo();

	// std::cout << "INITIALIZING DEVICE CONFIG ..." << std::endl;
	ioStream.initDeviceConfig();

	// std::cout << "INITIALIZING INPUT ..." << std::endl;
	ioStream.initInput(1);

	// std::cout << "INITIALIZING OUTPUT ..." << std::endl;
	ioStream.initOutput(2);

	// std::cout << "INITIALIZING DEVICE ..." << std::endl;
	ioStream.initDevice();

	// std::cout << "STARTING STREAM ..." << std::endl;
	ioStream.startStream();

	char input;
    std::cout << "\nRunning ... press <enter> to quit" << std::endl;
    std::cin.get(input);
	std::cout << "Stopping stream ..." << std::endl;
	ioStream.stopStream();

	return 0;
}