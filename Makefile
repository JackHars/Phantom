all:
	@if [ "$(shell uname -m)" = "aarch64" ]; then \
		mkdir -p phonelibs/mapbox-gl-native-qt/aarch64/; \
		ln -sf $(shell pwd)/phonelibs/mapbox-gl-native-qt/x86_64/libqmapboxgl.so $(shell pwd)/phonelibs/mapbox-gl-native-qt/aarch64/libqmapboxgl.so; \
	fi

.PHONY: all
