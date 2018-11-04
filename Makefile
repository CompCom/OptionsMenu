TARGET_NAME = options_deluxe
CXX = g++
STRIP = strip
CXXFLAGS = -std=c++11 -Os `sdl2-config --cflags`
LDLIBS =  `sdl2-config --libs` -lpng
LDFLAGS = 
SOURCES = src/main.cpp src/command.cpp src/framework/sdl_context.cpp src/framework/texture.cpp src/framework/controller.cpp src/framework/powerwatch.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET_NAME).hmod
$(TARGET_NAME).hmod: binaries
	$(CROSS_PREFIX)$(STRIP) mod/etc/options_menu/options
	$(CROSS_PREFIX)$(STRIP) mod/etc/options_menu/optiond
	cd mod/; tar -czvf "../$(TARGET_NAME).hmod" *

binaries: $(OBJECTS) src/daemon.o
	$(CROSS_PREFIX)$(CXX) $(OBJECTS) $(LDLIBS) $(LDFLAGS) -o mod/etc/options_menu/options
	$(CROSS_PREFIX)$(CXX) $(LDFLAGS) src/daemon.o src/framework/controller.o -o mod/etc/options_menu/optiond

%.o: %.cpp
	$(CROSS_PREFIX)$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	find -name "*.o" -type f -delete
	rm -f mod/etc/options_menu/options mod/etc/options_menu/optiond $(TARGET_NAME).hmod

.PHONY: clean
