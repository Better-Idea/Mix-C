#ifndef xpack_lock_atom_and
#define xpack_lock_atom_and
#define xa_name     atom_and
#define xa_namex    __atomic_and_fetch
#include"concurrency/lock/private/xgen.atomop.hpp"
#endif

xexport_space(mixc::concurrency_lock_atom_and::origin)
