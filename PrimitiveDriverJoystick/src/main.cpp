#include "PrimitiveDriverJoystick.h"
#include <iostream>

void printMenu()
{
	std::cout << "Menu:" << std::endl;
	std::cout << "t - Print System Tree" << std::endl;
	std::cout << "1 - Find Primitive Driver" << std::endl;
	std::cout << "2 - Request Primitive Driver Control" << std::endl;
	std::cout << "3 - Release Primitive Driver Control" << std::endl;
	std::cout << "4 - Start reading joystick." << std::endl;
	std::cout << "5 - Send query" << std::endl;
	std::cout << "6 - Send command" << std::endl;
	std::cout << "? - Output Menu" << std::endl;
	std::cout << "ESC - Exit Component" << std::endl;
};

int main(void)
{
	openjaus::system::Application::setTerminalMode();
	try
	{
		JAUSTools::PrimitiveDriverJoystick myDriver;
		myDriver.run();

		printMenu();

		usleep(2000000);
		//myDriver.findPrimitiveDriver();
		//myDriver.requestDriverControl();
		//usleep(2000000);
		//myDriver.sendQuery();
		//myDriver.sendCommand();
		//myDriver.startJoystickThread();

		unsigned char choice = 0;
		while(choice != 27) // ESC
		{
			choice = openjaus::system::Application::getChar();
			switch(choice)
			{
				case 't':
					std::cout << myDriver.getSystemTree()->toString();
					break;
				case '?':
					printMenu();
					break;
				case '1':
					myDriver.findPrimitiveDriver();
					break;
				case '2':
					myDriver.requestDriverControl();
					break;
				case '3':
					myDriver.releaseDriverControl();
					break;
				case '4':
					myDriver.startJoystickThread();
					break;
				case '5':
					myDriver.sendQuery();
					break;
				case '6':
					myDriver.sendCommand();
					break;
			}
		}
	}
	catch(openjaus::system::Exception& expn)
	{
		openjaus::system::Logger::log(expn);
	}

	return 0;
}
