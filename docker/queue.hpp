#ifdef xuser
	#ifndef xusing_docker_queue
		#include"docker/private/queue.hpp"
	#endif

	namespace xuser::inc {
		template<class item_t>
		struct queue : xusing_docker_queue::queue<queue<item_t>, item_t> {
			using the_t = xusing_docker_queue::queue<queue<item_t>, item_t>;
			using the_t::the_t;
		};
	}
#endif
