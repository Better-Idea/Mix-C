#pragma once
#define xindex_rollback(length,index,...)   if (index < 0) index += length __VA_ARGS__
