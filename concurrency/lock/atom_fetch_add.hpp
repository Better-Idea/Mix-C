#ifndef xpack_lock_atom_fetch_add
#define xpack_lock_atom_fetch_add
#define xa_name     atom_fetch_add
#define xa_namex    __atomic_fetch_add
#include"concurrency/lock/private/xgen.atomop.hpp"
#endif

xexport_space(mixc::concurrency_lock_atom_fetch_add::origin)
