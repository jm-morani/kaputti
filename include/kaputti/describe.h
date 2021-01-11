#ifndef KAPUTTI_DESCRIBE_H_
#define KAPUTTI_DESCRIBE_H_

#include <string>
#include <functional>


namespace kaputti
{
	int *do_describe(const std::string name, std::function<void()> lambda);
	int *do_it(const std::string name, std::function<void()> lambda);
}


#define KAPUTTI_CONCAT_X(A, B, C) KAP ## A ## B ## C
#define KAPUTTI_CONCAT(A, B, C) KAPUTTI_CONCAT_X(A, B, C)
#define describe(name, behavior) int *KAPUTTI_CONCAT(__COUNTER__, D, __LINE__) = kaputti::do_describe(name, []() behavior);
#define it(name, behavior) int *KAPUTTI_CONCAT(__COUNTER__, I, __LINE__) = kaputti::do_it(name, [&]() behavior);

#endif
