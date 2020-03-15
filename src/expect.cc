#include <kaputti/expect.h>


namespace kaputti
{
	namespace expect
	{
		ExpectBase::ExpectBase(const std::string source, const std::string file, int line) :
			source(source), file(file), line(line) {};

		AssertionError::AssertionError(ExpectBase *expect, const std::string why)
		{
			const std::string what = fmt::format(
				"Expression `{}`",
				expect->source
			);

			const std::string where = fmt::format(
				"in file {} at line {}",
				expect->file,
				expect->line
			);

			details.push_front(where);
			details.push_front(why);
			details.push_front(what);
		}
	}
}
