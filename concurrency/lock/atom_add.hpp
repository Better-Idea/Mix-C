#ifndef xpack_lock_atom_add
#define xpack_lock_atom_add
#define xa_name     atom_add
#define xa_namex    __atomic_add_fetch
#include"concurrency/lock/private/xgen.atomop.hpp"
#endif

xexport_space(mixc::concurrency_lock_atom_add::origin)
