#ifdef xuser
    #ifndef xusing_docker_stack
        #include"docker/private/stack.hpp"
    #endif

    namespace xuser::inc{
        template<class item_t>
        struct stack : xusing_docker_stack::stack<stack<item_t>, item_t>{
            using xusing_docker_stack::stack<stack<item_t>, item_t>::stack;
        };
    }
    #undef xusing_docker_stack
#endif