CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter -std=c++17
CXXFLAGS=$(CFLAGS)
OBJECTS=dye.o frame.o animation.o run_waves.o waves.o media.o
BINARIES=dye frame animation run_waves

# Where our library resides. You mostly only need to change the
# RGB_LIB_DISTRIBUTION, this is where the library is checked out.
RGB_LIB_DISTRIBUTION=..
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

all : $(BINARIES)

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)

dye : dye.o media.o
frame : frame.o media.o
animation : animation.o media.o
run_waves: run_waves.o waves.o

# All the binaries that have the same name as the object file.
% : %.o $(RGB_LIBRARY)
	$(CXX) $^ -o $@ $(LDFLAGS)

%.o : %.cc
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(BINARIES)

FORCE:
.PHONY: FORCE
