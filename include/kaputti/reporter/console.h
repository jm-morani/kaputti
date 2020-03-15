#ifndef KAPUTTI_REPORTER_CONSOLE_H_
#define KAPUTTI_REPORTER_CONSOLE_H_

#include <string>

#include <kaputti/event.h>


namespace kaputti
{
	namespace reporter
	{
		class Console : public EventListener
		{
			private:
				std::string red;
				std::string green;
				std::string faint;
				std::string title;
				std::string normal;
				void right(unsigned int n);

				int level = 0;
				void indenter();

				void on_component_enter(const std::string name);
				void on_component_leave();
				void on_test_start(const std::string name);
				void on_test_success();
				void on_test_failure(const Failure failure);
			public:
				Console();
		};
	}
}

#endif
