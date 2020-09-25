#ifndef FRAME_H_
#define FRAME_H_

#include "Util.hpp"
#include "Plane.hpp"

#include "bgfx/bgfx.h"

namespace worldWp {

class Frame {
public:
	Frame(const ModelSpecs& ms);

	bgfx::VertexBufferHandle getVBufferHandle();
	bgfx::IndexBufferHandle getIBufferHandle();
private:
	ModelSpecs ms;
	worldWp::util::PosNormalColorVertex *verts;
	uint16_t *indz;

	void add_frame_vertices_2d(
	  Dimension dim,
	  bx::Vec3 pos, float dim1, float dim2,
	  int start_pos );

	void add_frame(float y_start, float height);
	void add_frame_indzs(int start_indx, int vert_offset);
};

};

#endif
