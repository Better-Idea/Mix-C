#ifndef xpack_lock_atom_xor
#define xpack_lock_atom_xor
#define xa_name     atom_xor
#define xa_namex    __atomic_xor_fetch
#include"concurrency/lock/private/xgen.atomop.hpp"
#endif

xexport(mixc::concurrency_lock_atom_xor::atom_xor)
