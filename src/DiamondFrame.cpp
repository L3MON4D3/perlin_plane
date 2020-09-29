#include "DiamondFrame.hpp"
#include <cmath>

const int frame_vert_sz{6}, frame_indzs_sz{13};

const float frame_vertices[frame_vert_sz][3] {
	{ 0, 1, 0},
	{-1, 0,-1},
	{ 1, 0,-1},
	{ 1, 0, 1},
	{-1, 0, 1},
	{ 0,-1, 0}
};

const uint16_t frame_linestrip[frame_indzs_sz] {
	0,2,3,5,2,1,5,4,1,0,4,3,0
};

namespace worldWp {

DiamondFrame::DiamondFrame(const util::PlaneSpecs& ms)
	: Model(frame_vert_sz, frame_indzs_sz, BGFX_STATE_PT_LINESTRIP) {
	
	float sidelen_x{(ms.x_dim-1)*ms.res/2.0f},
	      sidelen_z{(ms.z_dim-1)*ms.res/2.0f};
	add_verts( {
		sidelen_x,
		float(std::sqrt(std::pow(sidelen_x, 2)+std::pow(sidelen_z, 2))),
		sidelen_z }, {0,0,0} );
	add_indzs();
}

void DiamondFrame::add_indzs() {
	for(int i{0}; i != frame_indzs_sz; ++i)
		indzs[i] = frame_linestrip[i];
}

void DiamondFrame::add_verts(bx::Vec3 dims, bx::Vec3 center) {
	for(int i{0}; i != frame_vert_sz; ++i)
		verts[i] = {
			center.x + dims.x*frame_vertices[i][0],
			center.y + dims.y*frame_vertices[i][1],
			center.z + dims.z*frame_vertices[i][2],
			0,1,0, 0xffffffff
		};
}

};
