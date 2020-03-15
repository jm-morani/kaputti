#include <deque>
#include <string>
#include <iostream>

#include <kaputti/event.h>
#include <kaputti/reporter/junit.h>


namespace kaputti
{
	namespace reporter
	{
		template<class Iterable>
		std::string join(Iterable chunks, std::string sep)
		{
			std::string result;
			bool first = true;
			for(auto chunk: chunks) {
				if (!first) {
					result = result + sep;
				}
				result = result + chunk;
				first = false;
			}
			return result;
		}

		template<typename Type>
		class AttributeValue {
			public:
				Type raw; // public because I failed to fix non-template-friend warning
				AttributeValue(const Type raw): raw(raw) { };
		};
		std::ostream& operator<< (std::ostream& os, const AttributeValue<std::string>& av)
		{
			os << "\"";
			for (char const& c: av.raw) {
				if (c == '&') {
					os << "&amp;";
				} else if (c == '"') {
					os << "&quot;";
				} else {
					os << c;
				}
			}
			return os << "\"";
		}
		std::ostream& operator<< (std::ostream& os, const AttributeValue<int>& av)
		{
			return os << "\"" << av.raw << "\"";
		}


		JUnit::JUnit(const std::string filename)
		{
			out.open(filename);
			out << "<?xml version=\"1.0\"?>\n<testsuites>\n";
		}

		void JUnit::on_component_enter(const std::string name)
		{
			if (level == 0) {
				testSuite = new TestSuite(name);
			};
			level++;
			components.push_back(name);
		}

		void JUnit::on_test_start(const std::string name)
		{
			testCase = new TestCase(name, join(components, " "));
			testSuite->testcases.push_back(testCase);
			testSuite->tests++;
		}

		void JUnit::on_test_failure(const Failure failure)
		{
			testCase->failed = true;
			testCase->failure = failure;
			testSuite->failures++;
		}

		void JUnit::on_component_leave()
		{
			components.pop_back();
			level--;
			if (level == 0) {
				out << "\t<testsuite"
					<< " name=" << AttributeValue<std::string>(testSuite->name)
					<< " tests=" << AttributeValue<int>(testSuite->tests)
					<< " failures=" << AttributeValue<int>(testSuite->failures)
					<< ">\n";

				for(TestCase *testCase: testSuite->testcases) {
					out << "\t\t<testcase"
						<< " classname=" << AttributeValue<std::string>(testCase->classname)
						<< " name=" << AttributeValue<std::string>(testCase->name)
						<< " time=\"0.0\">\n";

					if (testCase->failed) {
						out << "\t\t\t<failure"
							<< " message=" << AttributeValue<std::string>(join(testCase->failure.getDetails(), " "))
							<< "></failure>\n";
					}

					out << "\t\t</testcase>\n";
					delete testCase;
				};

				out << "\t</testsuite>\n";
				delete testSuite;
			}
		}

		void JUnit::on_close()
		{
			out << "</testsuites>\n";
			out.close();
		}
	}
}
