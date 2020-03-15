#ifndef KAPUTTI_REPORTER_JUNIT_H_
#define KAPUTTI_REPORTER_JUNIT_H_

#include <deque>
#include <string>
#include <fstream>

#include <kaputti/event.h>
#include <kaputti/failure.h>


namespace kaputti
{
	namespace reporter
	{
		struct TestCase
		{
			std::string classname;
			std::string name;
			std::string system_err;
			bool failed = false;
			Failure failure;
			TestCase(std::string name, std::string classname) :
				name(name), classname(classname) {};
		};

		struct TestSuite
		{
			std::string name;
			int tests = 0;
			int errors = 0;
			int failures = 0;
			std::deque<TestCase*> testcases;
			TestSuite(std::string name) : name(name) {};
		};

		class JUnit : public EventListener
		{
			private:
				std::ofstream out;
				int level = 0;
				TestCase *testCase;
				TestSuite *testSuite;
				std::deque<std::string> components;

				void on_component_enter(const std::string name);
				void on_component_leave();
				void on_test_start(const std::string name);
				void on_test_failure(const Failure failure);
				void on_close();

			public:
				JUnit(const std::string filename);
		};
	}
}

#endif
