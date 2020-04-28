#ifdef xuser
	#ifndef xusing_docker_priority_queue
		#include"docker/private/priority_queue.hpp"
	#endif

	namespace xuser::inc {
		template<class item_t>
		struct priority_queue : xusing_docker_name::priority_queue<priority_queue<item_t>, item_t> {
			using the_t = xusing_docker_name::priority_queue<priority_queue<item_t>, item_t>;
			using the_t::the_t;
		};
	}
#endif
