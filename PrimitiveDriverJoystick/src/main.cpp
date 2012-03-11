#include "PrimitiveDriverJoystick.h"

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
			}
		}
	}
	catch(openjaus::system::Exception& expn)
	{
		openjaus::system::Logger::log(expn);
	}

	return 0;
}
