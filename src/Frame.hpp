#ifndef FRAME_H_
#define FRAME_H_

#include "Plane.hpp"
#include "Model.tpp"

#include "bgfx/bgfx.h"

namespace worldWp {

class Frame : public Model<uint16_t> {
public:
	Frame(const util::PlaneSpecs& ms);
private:
	util::PlaneSpecs ms;

	void add_frame_vertices_2d(
	  Dimension dim,
	  bx::Vec3 pos, float dim1, float dim2,
	  int start_pos );

	void add_frame(float y_start, float height);
	void add_frame_indzs(int start_indx, int vert_offset);
};

};

#endif
