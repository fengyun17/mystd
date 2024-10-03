cmake_minimum_required(VERSION 3.28)
project(ebstd)

set(CMAKE_CXX_STANDARD 23)

add_executable(ebstd main.cpp
        include/inline/bbst.h
        include/inline/heap.h
        include/inline/list.h
        include/offline/vector.h
        include/offline/bitmap.h
        include/offline/hash.h
        include/offline/ltree.h
        include/offline/btree.h
        include/offline/heap.h
        include/offline/deque.h
        include/inline/fibheap.h)
