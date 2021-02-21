#pragma once
#define xexport_space(...)              namespace xuser{ using namespace ::__VA_ARGS__; }
#define xexport_spacex(addition,...)    namespace xuser::addition{ using namespace ::__VA_ARGS__; }
#define xexport(...)                    namespace xuser{ using ::__VA_ARGS__; }
#define xexportx(addition,...)          namespace xuser::addition{ using ::__VA_ARGS__; }
