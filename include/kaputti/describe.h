#ifndef KAPUTTI_DESCRIBE_H_
#define KAPUTTI_DESCRIBE_H_

#include <string>
#include <functional>


namespace kaputti
{
	int *do_describe(const std::string name, std::function<void()> lambda);
	int *do_it(const std::string name, std::function<void()> lambda);
}


#define KAPUTTI_CONCAT_X(A, B) A ## B
#define KAPUTTI_CONCAT(A, B) KAPUTTI_CONCAT_X(A, B)
#define describe(name, behavior) static int *KAPUTTI_CONCAT(KAPD, __COUNTER__) = kaputti::do_describe(name, []() behavior);
#define it(name, behavior) static int *KAPUTTI_CONCAT(KAPI, __COUNTER__) = kaputti::do_it(name, [&]() behavior);

#endif
