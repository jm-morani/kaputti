#include <getopt.h>

#include <deque>
#include <string>
#include <memory>
#include <iostream>
#include <functional>

#include <kaputti/event.h>
#include <kaputti/failure.h>
#include <kaputti/reporter/junit.h>
#include <kaputti/reporter/status.h>
#include <kaputti/reporter/console.h>


namespace kaputti
{
	class Description
	{
		private:
			std::string name;
			std::function<void()> lambda;

		public:
			Description(const std::string name, std::function<void()> lambda);
			void run();
	};

	bool running = false;
	EventDispatcher *dispatcher = nullptr;
	std::deque<Description*> root_descriptions;

	Description::Description(const std::string name, std::function<void()> lambda)
	{
		this->name = name;
		this->lambda = lambda;
	}

	void Description::run()
	{
		dispatcher->component_enter(name);
		lambda();
		dispatcher->component_leave();
	}

	int *do_describe(const std::string name, std::function<void()> lambda)
	{
		if (running) {
			dispatcher->component_enter(name);
			lambda();
			dispatcher->component_leave();
		} else {
			root_descriptions.push_back(new Description(name, lambda));
		}
		return 0;
	}

	int *do_it(const std::string name, std::function<void()> lambda)
	{
		dispatcher->test_start(name);
		try {
			lambda();
			dispatcher->test_success();
		} catch (Failure *failure) {
			dispatcher->test_failure(*failure);
			delete failure;
		}
		return 0;
	}
}


using namespace kaputti;

int main(int argc, char **argv)
{
	int c;
	int option_index = 0;
	struct option long_options[] = {
		{ "report-junit", required_argument, 0, 0 },
		{ 0, 0, 0, 0 }
	};

	reporter::Status statusReporter;
	dispatcher = new EventDispatcher();
	dispatcher->add_listener(&statusReporter);
	dispatcher->add_listener(new reporter::Console());

	while (-1 != (c = getopt_long(argc, argv, "", long_options, &option_index))) {
		if (c == 0) {
			switch(option_index) {
				case 0:  // "report-junit"
					dispatcher->add_listener(new reporter::JUnit(optarg));
					continue;
			}
		}
		exit(EXIT_FAILURE);
	}

	kaputti::running = true;
	for(kaputti::Description *description : kaputti::root_descriptions) {
		description->run();
	}

	dispatcher->close();

	exit(statusReporter.getStatus());
}
