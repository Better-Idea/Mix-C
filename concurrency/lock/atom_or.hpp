#ifndef xpack_lock_atom_or
#define xpack_lock_atom_or
#define xa_name     atom_or
#define xa_namex    __atomic_or_fetch
#include"concurrency/lock/private/xgen.atomop.hpp"
#endif

xexport(mixc::concurrency_lock_atom_or::atom_or)
