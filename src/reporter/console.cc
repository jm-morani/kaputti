#include <string>
#include <iostream>

#include <kaputti/event.h>
#include <kaputti/failure.h>
#include <kaputti/reporter/console.h>


namespace kaputti
{
	namespace reporter
	{
		Console::Console()
		{
			red = "\033[31m";
			green = "\033[32m";
			faint = "\033[2m";
			title = "\033[4m";
			normal = "\033[m";
			std::cout << normal;
		}

		void Console::indenter()
		{
			for(int i=0; i<level; i++)
				std::cout << "  ";
		}

		void Console::right(unsigned int n)
		{
			std::cout << "\033[1000C\033[" << n << "D";
		}

		void Console::on_component_enter(const std::string name)
		{
			indenter();
			std::cout << title << name << normal << std::endl;
			level++;
		}

		void Console::on_component_leave()
		{
			level--;
			if (level == 0)
				std::cout << "\n";
		}

		void Console::on_test_start(const std::string name)
		{
			indenter();
			std::cout << normal << name;
		}

		void Console::on_test_success()
		{
			right(8);
			std::cout << normal << "[ " << green << "PASS" << normal << " ]" << std::endl;
		}

		void Console::on_test_failure(const Failure failure)
		{
			right(8);
			std::cout << "[ " << red << "FAIL" << normal << " ]\n";
			level++;
			for(std::string detail : failure.getDetails()) {
				indenter();
				std::cout << faint << detail << normal << "\n";
			}
			std::cout << std::flush;
			level--;
		}
	}
}
