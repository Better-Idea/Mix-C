#pragma once
#define xexport_space(...)      namespace xuser::inc{ using namespace ::__VA_ARGS__; }
#define xexport_space_adv(...)  namespace xuser::adv{ using namespace ::__VA_ARGS__; }
#define xexport(...)            namespace xuser::inc{ using ::__VA_ARGS__; }
#define xexport_adv(...)        namespace xuser::adv{ using ::__VA_ARGS__; }
