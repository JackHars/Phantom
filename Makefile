ifeq ($(shell uname -m),aarch64)
	mkdir -p phonelibs/mapbox-gl-native-qt/aarch64/
	ln -sf $$(PWD)/phonelibs/mapbox-gl-native-qt/x86_64/libqmapboxgl.so $$(PWD)/phonelibs/mapbox-gl-native-qt/aarch64/libqmapboxgl.so
endif
