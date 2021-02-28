#ifndef xpack_lock_atom_sub
#define xpack_lock_atom_sub
#define xa_name     atom_sub
#define xa_namex    __atomic_sub_fetch
#include"concurrency/lock/private/xgen.atomop.hpp"
#endif

xexport(mixc::concurrency_lock_atom_sub::atom_sub)
