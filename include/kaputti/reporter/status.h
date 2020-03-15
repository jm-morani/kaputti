#ifndef KAPUTTI_REPORTER_STATUS_H_
#define KAPUTTI_REPORTER_STATUS_H_

#include <string>

#include <kaputti/event.h>


namespace kaputti
{
	namespace reporter
	{
		class Status : public EventListener
		{
			private:
				int status = 0;
				void on_test_failure(const Failure failure);
			public:
				int getStatus();
		};
	}
}

#endif
