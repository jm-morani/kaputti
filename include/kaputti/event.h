#ifndef KAPUTTI_EVENT_H_
#define KAPUTTI_EVENT_H_

#include <string>
#include <forward_list>
#include <kaputti/failure.h>


namespace kaputti
{
	class EventDispatcher;


	class EventListener
	{
		private:
			friend class EventDispatcher;

			virtual void on_component_enter(const std::string name);
			virtual void on_component_leave();

			virtual void on_test_start(const std::string name);
			virtual void on_test_success();
			virtual void on_test_failure(const Failure failure);

			virtual void on_close();
	};


	class EventDispatcher
	{
		private:
			std::forward_list<class EventListener*> listeners;

		public:
			void add_listener(EventListener *listener);

			void component_enter(const std::string name);
			void component_leave();

			void test_start(const std::string name);
			void test_success();
			void test_failure(const Failure failure);

			void close();
	};
};

#endif
