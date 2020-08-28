#ifndef MODEL_BUILDER_H_
#define MODEL_BUILDER_H_
#include "Perlin.hpp"
#include "bgfx/bgfx.h"

namespace worldWp {

struct PosColorVertex {
    float x, y, z;
    uint32_t rgba;

    static void init();
    static bgfx::VertexLayout layout;
};

class ModelBuilder {
public:
    ModelBuilder(int x_dim, int y_dim, RandomGenerator::Perlin ns_gen);
    bgfx::VertexBufferHandle getVBufferHandle();
    bgfx::IndexBufferHandle getIBufferHandle();
private:
    int x_dim, y_dim;
    PosColorVertex *plane_verts;
    uint16_t *plane_indz;
};
};


#endif
