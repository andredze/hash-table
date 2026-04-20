DED_FLAGS = -D _DEBUG -ggdb3 -std=c++17 -Wall -Wextra -Weffc++ -Wsign-conversion 			 	 \
		   -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations 		   		 \
		   -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported  		   		 \
		    -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral 		 			 \
		   -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op 		   		 \
		   -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith  		 \
		   -Winit-self -Wredundant-decls -Wshadow -Wsign-promo		   		 					 \
		   -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn		   		 \
		   -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default 	 \
		   -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast 		 \
		   -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing  \
		   -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation 	 \
		   -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer 	 \
		   -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla -Wconversion

ASAN = -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,$\
	    float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,$\
	    null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,$\
	    undefined,unreachable,vla-bound,vptr

# ——————————————————————————————————————————————————————————————————————————————————————————

CXX = g++

CXXFLAGS = -march=native -O2 -g -fno-omit-frame-pointer -pie -fPIE

INCLUDES += -I include -I include/hash_table -I include/list

SOURCES = src/main.cpp	 		 				  \
		  src/input.cpp			 				  \
		  src/list/listDebug.cpp 	 			  \
		  src/list/listGraph.cpp	  	 		  \
		  src/list/listCommands.cpp 	 		  \
		  src/list/graphCommon.cpp	 			  \
		  src/hash_table/hash_table.cpp	 		  \
		  src/hash_table/hash_table_test_case.cpp \
		  src/hash_table/hash_functions.cpp

# ------------------------------------------------------------------ #

ifdef SAN
	CXXFLAGS += $(ASAN) $(DED_FLAGS)
endif

ifdef DEBUG
	CXXFLAGS += -D DEBUG $(DED_FLAGS) $(ASAN)
else
	CXXFLAGS += -DNDEBUG
endif

ifdef HIST
	CXXFLAGS += -D HIST
endif

ifdef TEST
	CXXFLAGS += -D TEST
endif

# ------------------------------------------------------------------ #

OBJS = $(SOURCES:src/%.cpp=obj/%.o)

ASM_OBJS = obj/hash_table/crc32.o

TARGET = run

# ——————————————————————————————————————————————————————————————————————————————————————————

$(shell mkdir -p obj log obj/hash_table obj/list)

all: clean build

build: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(ASM_OBJS) -o $@ $(CXXFLAGS) $(LD_FLAGS)

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -rf $(OBJS) $(TARGET)

.PHONY: all clean build

# ——————————————————————————————————————————————————————————————————————————————————————————