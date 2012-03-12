#include "PrimitiveDriverJoystick.h"

void printMenu()
{
	std::cout << "Menu:" << std::endl;
	std::cout << "t - Print System Tree" << std::endl;
	std::cout << "1 - Find Primitive Driver" << std::endl;
	std::cout << "2 - Request Primitive Driver Control" << std::endl;
	std::cout << "3 - Release Primitive Driver Control" << std::endl;
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

		unsigned char choice = 0;
		while(choice != 27) // ESC
		{
			choice = openjaus::system::Application::getChar();
			switch(choice)
			{
				case 't':
					LOG(myDriver.getSystemTree()->toString());
					break;
				case '?':
					printMenu();
					break;
				case '1':
					myDriver.findPrimitiveDriver();
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
