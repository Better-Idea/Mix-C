#ifndef xpack_lock_atom_fetch_and
#define xpack_lock_atom_fetch_and
#define xa_name     atom_fetch_and
#define xa_namex    __atomic_fetch_and
#include"concurrency/lock/private/xgen.atomop.hpp"
#endif

xexport_space(mixc::concurrency_lock_atom_fetch_and::origin)
