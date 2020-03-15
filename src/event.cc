#include <kaputti/event.h>


#define KAPUTTI_EVENT(event, args_decl, args_call) \
	void EventListener::on_ ## event(args_decl) {}; \
	void EventDispatcher::event(args_decl) { \
		for (auto listener : listeners) \
			listener->on_ ## event(args_call); \
	}


namespace kaputti
{
	void EventDispatcher::add_listener(EventListener *listener)
	{
		listeners.push_front(listener);
	};

	KAPUTTI_EVENT(component_enter, const std::string name, name);
	KAPUTTI_EVENT(component_leave, , );

	KAPUTTI_EVENT(test_start, const std::string name, name);
	KAPUTTI_EVENT(test_success, , );
	KAPUTTI_EVENT(test_failure, const Failure failure, failure);

	KAPUTTI_EVENT(close, , );
}
