cmake_minimum_required(VERSION 3.8)
project(ctcp)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        ctcp.c
        ctcp.h
        ctcp_linked_list.c
        ctcp_linked_list.h
        ctcp_sys.h
        ctcp_sys_internal.c
        ctcp_sys_internal.h
        ctcp_utils.c
        ctcp_utils.h
        test.c
        tester-student.py)

add_executable(ctcp ${SOURCE_FILES})