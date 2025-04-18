## mvt-bench-fixtures

Sample vector tiles at z14. 210 of 'em. Designed to be used to benchmark decoders.

Contains `mapbox.mapbox-terrain-v2` and `mapbox.mapbox-streets-v7`.

Can be loaded like:

```c++
        for (std::size_t x=4680;x<=4693;++x) {
            for (std::size_t y=6260;y<=6274;++y) {
                std::string path = "fixtures/14-" + std::to_string(x) + "-" + std::to_string(y) + ".mvt";
                // open `path`....
            }
        }
```