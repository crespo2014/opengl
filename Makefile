


build_dir := build
src_dirs := .
srcs := 

targets := opengl_app

common_cpp := --std=c++11 -Wall
common_ld := -lpthread -lGL -lGLU -lglut
common_cpp += -finput-charset=ascii -Wall -Wextra -pedantic -Wmissing-declarations -Wpointer-arith -Wwrite-strings -Wformat=2 -Wlogical-op -Wcast-align -Wcast-qual -Wundef -Wmissing-include-dirs -Wfloat-equal -Wconversion
common_cpp += -I../cpp-lib -I/usr/X11R6/include/

opengl_app_src_dir := .

serializer_mtest_src_dir := mtest
#serializer_mtest_src := cpp-lib/posix/File.cpp cpp-lib/posix/thread.cpp
serializer_mtest_cpp := -I../cpputest/include
serializer_mtest_cpp += -include ../cpputest/include/CppUTest/MemoryLeakDetectorNewMacros.h
serializer_mtest_cpp += -include ../cpputest/include/CppUTest/MemoryLeakDetectorMallocMacros.h
serializer_mtest_ld := -Wl,-Bstatic -lCppUTest -Wl,-Bstatic -lCppUTestExt -Wl,-Bdynamic -lgcc_s  -L../cpputest/lib

include ../common/Makefile