#ifndef xpack_lock_atom_fetch_sub
#define xpack_lock_atom_fetch_sub
#define xa_name     atom_fetch_sub
#define xa_namex    __atomic_fetch_sub
#include"concurrency/lock/private/xgen.atomop.hpp"
#endif

xexport(mixc::concurrency_lock_atom_fetch_sub::atom_fetch_sub)
