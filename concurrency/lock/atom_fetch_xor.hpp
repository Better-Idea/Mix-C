#ifndef xpack_lock_atom_fetch_xor
#define xpack_lock_atom_fetch_xor
#define xa_name     atom_fetch_xor
#define xa_namex    __atomic_fetch_xor
#include"concurrency/lock/private/xgen.atomop.hpp"
#endif

xexport(mixc::concurrency_lock_atom_fetch_xor::atom_fetch_xor)
