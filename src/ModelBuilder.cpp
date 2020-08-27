#include "ModelBuilder.hpp"

namespace worldWp {

void PosColorVertex::init() {
    layout
        .begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float      )
        .add(bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8, true)
        .end();
}

ModelBuilder::ModelBuilder(int x_dim, int y_dim, RandomGenerator::Perlin ns_gen)
    : x_dim{x_dim},
      y_dim{y_dim},
      plane_verts{ new PosColorVertex[x_dim*y_dim] },
      plane_indz{ new PosColorVertex[(x_dim-1)*(y_dim-1)*3] },
      ns_gen{ns_gen} {}

};
