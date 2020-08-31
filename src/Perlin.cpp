#include "Perlin.hpp"
#include <cmath>
#include <iostream>

namespace RandomGenerator {
const int Perlin::p[];

Perlin::Perlin(double) {
    //Don't stretch by default.
    Perlin(seed, 1.0, 1.0, 1.0);
}

Perlin::Perlin(double seed, double xS, double yS, double resS)
    : seed{seed},
      xStretch{xS}, yStretch{yS}, resStretch{resS} {}

double Perlin::grad(int hash, double x, double y, double z) {
    int h {hash & 15};
    double u {h < 8 ? x : y},
           v {h < 4 ? v : h == 12 || h == 14 ? x : z};
    return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}

/**
 * Get noise at position (x,y).
 * @param x X-Coord.
 * @param y Y-Coord.
 *
 * @return noise, double.
 */
double Perlin::noise_no_stretch(double x, double y) const {
    //cut 3d-noise at "seed".
    double z = seed;
    
    int xU {(int)std::floor(x) & 255},
        yU {(int)std::floor(y) & 255},
        zU {(int)std::floor(z) & 255};

    x -= std::floor(x);
    y -= std::floor(y);
    z -= std::floor(z);

    double u {fade(x)},
           v {fade(y)},
           w {fade(z)};

    int aC {p[xU  ]+yU}, aaC {p[aC]+zU}, abC {p[aC+1]+zU},
        bC {p[xU+1]+yU}, baC {p[bC]+zU}, bbC {p[bC+1]+zU};

    return lerp(w, lerp(v, lerp(u, grad(p[aaC  ], x  , y  , z  ),
                                   grad(p[baC  ], x-1, y  , z  )),
                           lerp(u, grad(p[abC  ], x  , y-1, z  ),
                                   grad(p[bbC  ], x-1, y-1, z  ))),
                   lerp(v, lerp(u, grad(p[aaC+1], x  , y  , z-1),
                                   grad(p[baC+1], x-1, y  , z-1)),
                           lerp(u, grad(p[abC+1], x  , y-1, z-1),
                                   grad(p[bbC+1], x-1, y-1, z-1))));
}

};
