#include"configure/platform.hpp"

#ifdef  xis_msvc_native

#define xa_name         atom_add
#define xa_func         _InterlockedExchangeAdd
#define xa_addition     + b
#include"concurrency/private/xgen.atomop.hpp"

#define xa_name         atom_sub
#define xa_func         _InterlockedExchangeAdd
#define xa_addition     - b
#define xa_is_sub
#include"concurrency/private/xgen.atomop.hpp"

#define xa_name         atom_and
#define xa_func         _InterlockedAnd
#define xa_addition     & b
#include"concurrency/private/xgen.atomop.hpp"

#define xa_name         atom_or
#define xa_func         _InterlockedOr
#define xa_addition     | b
#include"concurrency/private/xgen.atomop.hpp"

#define xa_name         atom_xor
#define xa_func         _InterlockedXor
#define xa_addition     ^ b
#include"concurrency/private/xgen.atomop.hpp"

#define xa_name         atom_fetch_add
#define xa_func         _InterlockedExchangeAdd
#define xa_addition
#include"concurrency/private/xgen.atomop.hpp"

#define xa_name         atom_fetch_sub
#define xa_func         _InterlockedExchangeAdd
#define xa_addition
#define xa_is_sub
#include"concurrency/private/xgen.atomop.hpp"

#define xa_name         atom_fetch_and
#define xa_func         _InterlockedAnd
#define xa_addition
#include"concurrency/private/xgen.atomop.hpp"

#define xa_name         atom_fetch_or
#define xa_func         _InterlockedOr
#define xa_addition
#include"concurrency/private/xgen.atomop.hpp"

#define xa_name         atom_fetch_xor
#define xa_func         _InterlockedXor
#define xa_addition
#include"concurrency/private/xgen.atomop.hpp"

#define xa_name         atom_swap
#define xa_func         _InterlockedExchange
#define xa_addition
#include"concurrency/private/xgen.atomop.hpp"

#endif
