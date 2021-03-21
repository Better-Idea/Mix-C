#ifndef xpack_lock_atom_fetch_or
#define xpack_lock_atom_fetch_or
#define xa_name     atom_fetch_or
#define xa_namex    __atomic_fetch_or
#include"concurrency/lock/private/xgen.atomop.hpp"
#endif

xexport_space(mixc::concurrency_lock_atom_fetch_or::origin)
