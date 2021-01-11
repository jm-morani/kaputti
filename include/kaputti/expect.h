#ifndef KAPUTTI_EXPECT_H_
#define KAPUTTI_EXPECT_H_

#include <list>
#include <string>
#include <iostream>
#include <forward_list>
#include <fmt/core.h>

#include <kaputti/failure.h>


namespace kaputti
{
	namespace test
	{
		template<
			typename X,
			typename Y,
			typename std::enable_if<
				std::is_convertible<decltype(std::declval<X>() == std::declval<Y>()), bool>
			::value, int>::type = 0>
		inline bool equal(const X a, const Y b)
		{
			return (a == b);
		}

		// Auto casting to test a float, while the
		// reference litteral is defacto a double.
		template<>
		inline bool equal(const float a, const double b)
		{
			return (a == (float)b);
		}

		template<
			typename X,
			typename Y>
		bool deepequal(const X x, const Y y)
		{
			auto ix = x.begin();
			auto iy = y.begin();
			for (; ix != x.end() || iy != y.end(); ++ix, ++iy) {
				if (!equal(*ix, *iy)) {
					return false;
				};
			};
			return ix == x.end() && iy == y.end();
		}
	}

	namespace repr
	{
		template<
			typename T,  // Other
			typename std::enable_if_t<
				!std::is_pointer_v<T> &&
				!std::is_same_v<T, std::string>
			>* = nullptr
		>
		inline std::string stringify(const T valeur)
		{
			return fmt::format("{}", valeur);
		}

		template<
			typename T,  // Raw pointer, except char*
			typename std::enable_if_t<
				std::is_pointer_v<T> &&
				!std::is_same_v<T, char*>
			>* = nullptr
		>
		inline std::string stringify(const T valeur)
		{
			return fmt::format("{:#x}", (size_t)valeur);
		}

		template<
			typename T,  // std::string
			typename std::enable_if_t<
				std::is_same_v<T, std::string>
			>* = nullptr
		>
		inline std::string stringify(const T valeur)
		{
			return fmt::format("\"{}\"", valeur);
		}

		template<
			typename T,  // char*
			typename std::enable_if_t<
				std::is_same_v<T, char*>
			>* = nullptr
		>
		inline std::string stringify(const T valeur)
		{
			return fmt::format("\"{}\"", valeur);
		}

		template<typename T>
		std::string stringify_iterable(std::forward_list<T> collection)
		{
			std::string s = std::string("[ ");
			for(auto i: collection)
				s += stringify(i) + " ";
			return s + "]";
		}

		template<typename T>
		std::string stringify(std::list<T> collection)
		{
			std::string s = std::string("[ ");
			for(auto i: collection)
				s += stringify(i) + " ";
			return s + "]";
		}
	}

	namespace expect
	{
		class ExpectBase;

		class AssertionError : public Failure
		{
			public:
				AssertionError(ExpectBase *expect, const std::string why);
		};

		class ExpectBase
		{
			friend AssertionError;

			private:
				std::string source;
				std::string file;
				int line;

			protected:
				ExpectBase(const std::string source, const std::string file, int line);
		};

		template<typename T_NOTICED>
		class Expect;

		template<typename T_NOTICED>
		class Never
		{
			private:
				Expect<T_NOTICED> *ex;

			public:
				Never(Expect<T_NOTICED> *ex) : ex(ex) {};

				template<
					typename T_EXPECTED,
					typename std::enable_if<
						std::is_invocable<decltype(test::equal<T_NOTICED, T_EXPECTED>), T_NOTICED, T_EXPECTED>
					::value, int>::type = 0>
				Expect<T_NOTICED> *equal(const T_EXPECTED expected)
				{
					if (test::equal(ex->noticed, expected)) {
						ex->fail(fmt::format("to never be equal to {}", repr::stringify(expected)));
					}
					return ex;
				};
		};

		template<typename T_NOTICED>
		class Deep
		{
			private:
				Expect<T_NOTICED> *ex;

			public:
				Deep(Expect<T_NOTICED> *ex) : ex(ex) {};

				template<typename T_EXPECTED>
				Expect<T_NOTICED> *equal(const T_EXPECTED expected)
				{
					if (!test::deepequal(ex->noticed, expected)) {
						ex->fail(fmt::format("to be deep equal to {}", repr::stringify(expected)));
					}
					return ex;
				};

				template<typename T=int>  // GCC n'arrive pas à découvrir int :-(
				Expect<T_NOTICED> *equal(std::list<T> expected)
				{
					if (!test::deepequal(ex->noticed, expected)) {
						ex->fail(fmt::format("to be deep equal to {}", repr::stringify(expected)));
					}
					return ex;
				};
		};

		template<typename T_NOTICED>
		class To
		{
			private:
				Expect<T_NOTICED> *ex;

			public:
				Deep<T_NOTICED> deep;
				Never<T_NOTICED> never;

				To(Expect<T_NOTICED> *ex) : ex(ex), deep(ex), never(ex) {};

				template<
					typename T_EXPECTED,
					typename std::enable_if<
						std::is_invocable<decltype(test::equal<T_NOTICED, T_EXPECTED>), T_NOTICED, T_EXPECTED>
					::value, int>::type = 0>
				Expect<T_NOTICED> *equal(const T_EXPECTED expected)
				{
					if (!test::equal(ex->noticed, expected)) {
						ex->fail(fmt::format("to be equal to {}", repr::stringify(expected)));
					}
					return ex;
				};
		};

		template<typename T_NOTICED>
		class Expect : public ExpectBase
		{
			friend To<T_NOTICED>;
			friend Deep<T_NOTICED>;
			friend Never<T_NOTICED>;

			private:
				T_NOTICED noticed;

				void fail(const std::string should)
				{
					throw new AssertionError(this, fmt::format("expected {} but was {}", should, repr::stringify(noticed)));
				};

			public:
				To<T_NOTICED> to;

				Expect(const T_NOTICED noticed, std::string source, std::string file, int line) :
					ExpectBase(source, file, line), noticed(noticed), to(this) {};
		};

		template<typename T_NOTICED>
		Expect<T_NOTICED> *expect(const T_NOTICED noticed, const std::string source, const std::string file, int line)
		{
			return new Expect<T_NOTICED>(noticed, source, file, line);
		};
	}
};


// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4220.pdf §6.5
// __LINE__ correct for multiline macro à partir de GCC 9
#define expect(noticed) kaputti::expect::expect(noticed, #noticed,__FILE__, __LINE__)

#endif
