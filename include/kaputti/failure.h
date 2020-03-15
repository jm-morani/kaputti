#ifndef KAPUTTI_FAILURE_H_
#define KAPUTTI_FAILURE_H_

#include <string>
#include <forward_list>


namespace kaputti
{
	class Failure
	{
		protected:
			std::forward_list<std::string> details;

		public:
			Failure();
			Failure(Failure const &failure);
			std::forward_list<std::string> getDetails() const;
	};
}

#endif
