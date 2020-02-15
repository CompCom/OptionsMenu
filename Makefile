TARGET_NAME = options_deluxe
CXX = g++
STRIP = strip
CXXFLAGS = -std=c++11 -Os `sdl2-config --cflags`
LDLIBS =  `sdl2-config --libs` -lpng
LDFLAGS = 
SOURCES = src/main.cpp src/command.cpp src/framework/sdl_context.cpp src/framework/texture.cpp src/framework/controller.cpp src/framework/powerwatch.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET_NAME).hmod $(TARGET_NAME)_b_down.hmod

$(TARGET_NAME)_b_down.hmod: $(TARGET_NAME).hmod
	cp button.cfg mod/etc/options_menu/
	cd mod/; tar -czvf "../$(TARGET_NAME)_b_down.hmod" *

$(TARGET_NAME).hmod: mod/etc/options_menu/options mod/etc/options_menu/optiond mod/etc/options_menu/mod_uninstall/mod_uninstall
	$(CROSS_PREFIX)$(STRIP) mod/etc/options_menu/options
	$(CROSS_PREFIX)$(STRIP) mod/etc/options_menu/optiond
	$(CROSS_PREFIX)$(STRIP) mod/etc/options_menu/mod_uninstall/mod_uninstall
	rm -f mod/etc/options_menu/button.cfg
	cd mod/; tar -czvf "../$(TARGET_NAME).hmod" *

mod/etc/options_menu/options: $(OBJECTS)
	$(CROSS_PREFIX)$(CXX) $(OBJECTS) $(LDLIBS) $(LDFLAGS) -o mod/etc/options_menu/options

mod/etc/options_menu/optiond: src/daemon.o src/framework/controller.o
	$(CROSS_PREFIX)$(CXX) $(LDFLAGS) src/daemon.o src/framework/controller.o -o mod/etc/options_menu/optiond

mod/etc/options_menu/mod_uninstall/mod_uninstall: src/mod_uninstall.o
	$(CROSS_PREFIX)$(CXX) src/mod_uninstall.o src/framework/*.o $(LDLIBS) $(LDFLAGS) -o mod/etc/options_menu/mod_uninstall/mod_uninstall

%.o: %.cpp
	$(CROSS_PREFIX)$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	find -name "*.o" -type f -delete
	rm -f mod/etc/options_menu/options mod/etc/options_menu/optiond mod/etc/options_menu/mod_uninstall/mod_uninstall mod/etc/options_menu/button.cfg $(TARGET_NAME).hmod $(TARGET_NAME)_b_down.hmod

.PHONY: clean
