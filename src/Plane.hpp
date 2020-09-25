#ifndef MODEL_BUILDER_H_
#define MODEL_BUILDER_H_

#include "Util.hpp"

#include "FastNoise.h"
#include "bgfx/bgfx.h"

#include <functional>

namespace worldWp {

struct ModelSpecs {
	int x_dim,
	    z_dim,
	    res;
};

enum Dimension {
	X, Y, Z
};

class Plane {
public:
	Plane(
	  const ModelSpecs& ms,
	  const FastNoise& fn,
	  const worldWp::util::NoiseMods& nm );

	bgfx::VertexBufferHandle getVBufferHandle();
	bgfx::IndexBufferHandle getIBufferHandle();
	void for_each_vertex(
	  const std::function<void(util::PosNormalColorVertex&, int indx)>& fn );

	float* get_raw_noise(const FastNoise& fn);
	void add_normals();
private:
	ModelSpecs ms;
	worldWp::util::NoiseMods nm;
	worldWp::util::PosNormalColorVertex *plane_verts;
	uint32_t *plane_indz;

	void add_plane_vertices(const FastNoise& fn);

	void add_base_vertices(float y_start);
	void add_base_indizes();
};

};

#endif
