#ifndef MODEL_BUILDER_H_
#define MODEL_BUILDER_H_

#include "Util.hpp"
#include "Model.tpp"

#include "FastNoise.h"
#include "bgfx/bgfx.h"

#include <functional>

namespace worldWp {

enum Dimension {
	X, Y, Z
};

class Plane : public Model<uint32_t> {
public:
	Plane(
	  const util::PlaneSpecs& ms,
	  const FastNoise& fn,
	  const util::NoiseMods& nm,
	  const uint32_t abgr );

	void for_each_vertex(
	  const std::function<void(util::PosNormalColorVertex&, int indx)>& fn );

	float* get_raw_noise(const FastNoise& fn);
	void add_normals();
private:
	util::PlaneSpecs ms;
	worldWp::util::NoiseMods nm;

	void add_plane_vertices(const FastNoise& fn, const uint32_t abgr);

	void add_base_vertices(float y_start, const uint32_t abgr);
	void add_base_indizes();
};

};

#endif
