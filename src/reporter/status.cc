#include <string>
#include <iostream>

#include <kaputti/event.h>
#include <kaputti/failure.h>
#include <kaputti/reporter/status.h>


namespace kaputti
{
	namespace reporter
	{
		void Status::on_test_failure(const Failure failure)
		{
			status = 1;
		}

		int Status::getStatus()
		{
			return status;
		}
	}
}
