#pragma once
#define xexport_space(...)      namespace xuser{ using namespace ::__VA_ARGS__; }
#define xexport(...)            namespace xuser{ using ::__VA_ARGS__; }
