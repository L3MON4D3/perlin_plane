#ifndef MODEL_BUILDER_H_
#define MODEL_BUILDER_H_

#include "FastNoise.h"
#include "bgfx/bgfx.h"

namespace worldWp {

struct PosNormalColorVertex {
    float pos[3];
	float normal[3];
    uint32_t rgba;

    static void init();
    static bgfx::VertexLayout layout;
};

class ModelBuilder {
public:
    ModelBuilder(int x_dim, int y_dim, FastNoise fn, int vert_dist);
    bgfx::VertexBufferHandle getVBufferHandle();
    bgfx::IndexBufferHandle getIBufferHandle();
private:
    int x_dim, y_dim;
    PosNormalColorVertex *plane_verts;
    uint32_t *plane_indz;
};
};


#endif
