# cxx_complier
# main
# extern_api

cxx_flag 	= -I.

ifeq ($(cxx_complier), clang)
	cxx_flag  	+= -std=c++20 
	extern_api   = extern.api_clang
endif

ifeq ($(cxx_complier), g++)
	cxx_flag	+= -std=c++2a -fconcepts 
	extern_api   = extern.api_g++
endif

ifdef debug
	cxx_flag 	+= -g
else
	cxx_flag 	+= -O2
endif

ifdef m32
	cxx_flag 	+= -m32
	extern_api_o = $(extern_api).m32
else
	extern_api_o = $(extern_api)
endif

all:make_api make_main
	
make_api:
	$(cxx_complier) $(cxx_flag) $(extern_api).cpp -c -o $(extern_api_o).o

make_main:
	$(cxx_complier) $(cxx_flag) $(extern_api_o).o $(main) -o ../main.exe
