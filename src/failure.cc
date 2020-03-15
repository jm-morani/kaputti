#include <kaputti/failure.h>


namespace kaputti
{
	Failure::Failure() {}

	Failure::Failure(const Failure &failure)
	{
		details = failure.details;
	};

	std::forward_list<std::string> Failure::getDetails() const
	{
		return details;
	}
}
