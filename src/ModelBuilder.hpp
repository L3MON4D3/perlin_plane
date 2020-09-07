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

struct ModelSpecs {
	int x_dim,
	    y_dim,
	    res;
};

struct NoiseMods {
	float x_stretch,
	      y_stretch,
	      res_stretch;
};

class ModelBuilder {
public:
    ModelBuilder(ModelSpecs ms, FastNoise fn, NoiseMods nm);
    bgfx::VertexBufferHandle getVBufferHandle();
    bgfx::IndexBufferHandle getIBufferHandle();
private:
	ModelSpecs ms;
	NoiseMods nm;
    PosNormalColorVertex *plane_verts;
    uint32_t *plane_indz;

	void add_normals();
};

};

#endif
