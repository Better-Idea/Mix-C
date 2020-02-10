#pragma once
#include"docker/array.hpp"
#include"definition/mini.hpp"

xspace(mixc)
    xspace(test)
        xtest_unit(array, 
            docker::array<i32>())

            xtest_item(group_three)
                uxx current_memory_usage = memory::allocated_bytes();
                xlocal
                    docker::array<i32> v;
                    xfor(i32 i = 1; i <= 8; i++)
                        v->push_back(i);
                    $
                    xfor(i32 i = 9; --i > 0; )
                        xassert(v->pop_back() == i, "i = %d", i) break;
                    $
                    xassert(current_memory_usage == memory::allocated_bytes());
                $
                xassert(current_memory_usage == memory::allocated_bytes());
            $
        
            xtest_item(group_two)
                uxx current_memory_usage = memory::allocated_bytes();
                xlocal docker::array<i32> v { 1 }; $
                xassert(current_memory_usage == memory::allocated_bytes());

                xlocal docker::array<i32> v { 1, 2 }; $
                xassert(current_memory_usage == memory::allocated_bytes());

                xlocal docker::array<i32> v { 1, 2, 3, }; $
                xassert(current_memory_usage == memory::allocated_bytes());

                xlocal docker::array<i32> v { 1, 2, 3, 4, }; $
                xassert(current_memory_usage == memory::allocated_bytes());

                xlocal docker::array<i32> v { 1, 2, 3, 4, 5, }; $
                xassert(current_memory_usage == memory::allocated_bytes());

                xlocal docker::array<i32> v { 1, 2, 3, 4, 5, 6, }; $
                xassert(current_memory_usage == memory::allocated_bytes());

                xlocal docker::array<i32> v { 1, 2, 3, 4, 5, 6, 7, }; $
                xassert(current_memory_usage == memory::allocated_bytes());

                xlocal docker::array<i32> v { 1, 2, 3, 4, 5, 6, 7, 8, }; $
                xassert(current_memory_usage == memory::allocated_bytes());
            $

            xtest_item(group_one)
                uxx current_memory_usage = memory::allocated_bytes();
                xlocal
                    docker::array<i32> v;
                    xassert(v->empty);
                    xassert(v->capacity == 0);
                    xassert(v->length == 0);

                    v->push_back(1);
                    xassert(v->empty == false);
                    xassert(v[0] == 1);
                    xassert(v->length == 1);
                    xassert(v->capacity == 1);
                    
                    v->push_back(2);
                    xassert(v->empty == false);
                    xassert(v[0] == 1);
                    xassert(v[1] == 2);
                    xassert(v->length == 2);
                    xassert(v->capacity == 2);

                    v->push_back(3);
                    xassert(v->empty == false);
                    xassert(v[0] == 1);
                    xassert(v[1] == 2);
                    xassert(v[2] == 3);
                    xassert(v->length == 3);
                    xassert(v->capacity == 4);

                    v->push_back(4);
                    xassert(v->empty == false);
                    xassert(v[0] == 1);
                    xassert(v[1] == 2);
                    xassert(v[2] == 3);
                    xassert(v[3] == 4);
                    xassert(v->length == 4);
                    xassert(v->capacity == 4);

                    xfor(i32 i = 5; i <= 8; i++)
                        v->push_back(i);
                        xassert(v->empty == false);
                        xassert(v->length == i, "i = %d", i);
                        xassert(v->capacity == 8, "i = %d", i);

                        xfor(i32 j = 0; j < v->length; j++)
                            xassert(v[j] == j + 1, "i = %d, j = %d", i, j) break;
                        $
                    $

                    v->first = 2;
                    v->last  = 3;
                    xassert(v->first == 2 && v[0] == 2);
                    xassert(v->last  == 3 && v[v->length - 1] == 3);
                $
                xassert(current_memory_usage == memory::allocated_bytes());
            $
        $;
    $
$