#include "Frame.hpp"

//eight triangles, both side, 3 verts per triangle.
const int frame_indzs_count = 8*3*2;

const float frame_width = .2;

//offsets from an xy coordinate to construct a frame.
const float frame_verts[12][2] {
	{0          , 0          },
	{frame_width, 0          },
	{frame_width, frame_width},

	{0, 0          },
	{0, frame_width},
	{-frame_width          , frame_width},

	{0, 0},
	{-frame_width          , 0},
	{0          ,           -frame_width},

	{0          , 0},
	{0          ,           -frame_width},
	{frame_width,           -frame_width}
};

const uint16_t frame_indzs[frame_indzs_count] {
	 1,  2,  4,
	 1,  4,  2,
	 1,  4,  3,
	 1,  3,  4,

	 4,  5,  7,
	 4,  7,  5,
	 4,  7,  6,
	 4,  6,  7,

	 7,  8, 10,
	 7, 10,  8,
	 7, 10,  9,
	 7,  9, 10,

	10, 11,  1,
	10,  1, 11,
	10,  1,  0,
	10,  0,  1
};

const int ibuf_sz{frame_indzs_count*6},
          vbuf_sz{12*6};

namespace worldWp {

Frame::Frame(const util::ModelSpecs& ms)
	: ms{ms},
	  verts{ new util::PosNormalColorVertex[vbuf_sz] },
	  indz{ new uint16_t[ibuf_sz] } {
	add_frame(-40.02, 90);
}


void Frame::add_frame_vertices_2d(
  Dimension dim,
  bx::Vec3 pos, float dim1_sz, float dim2_sz,
  int start_pos
) {
	int dim0{ static_cast<int>(dim) },
	    dim1{ (dim0+1)%3 },
	    dim2{ (dim1+1)%3 };

	//initialize all verices here, assign correct position later.
	for(int i{0}; i != 12; ++i)
		verts[start_pos+i] = {0,0,0, 0,1,0, 0xff111111};

	float* pos_f = ((float*)&pos);
	float corners[4][2] {
		{pos_f[dim1]        , pos_f[dim2]        },
		{pos_f[dim1]+dim1_sz, pos_f[dim2]        },
		{pos_f[dim1]+dim1_sz, pos_f[dim2]+dim2_sz},
		{pos_f[dim1]        , pos_f[dim2]+dim2_sz}
	};

	int indx{0};
	for(int i{0}; i != 4; ++i) {
		for(int j{0}; j != 3; ++j, ++indx) {
			util::PosNormalColorVertex& v{ verts[start_pos+indx] };
			v.pos[dim0] = pos_f[dim0];
			v.pos[dim1] = corners[i][0]+frame_verts[indx][0];
			v.pos[dim2] = corners[i][1]+frame_verts[indx][1];
		}
	}
}

void Frame::add_frame(float y_start, float height) {
	int vert_offset{0},
	    indx_offset{0};

	//add some offset to frame so polygons dont overlap.
	float x_length{float((ms.x_dim-1)*ms.res)+.2f},
	      z_length{float((ms.z_dim-1)*ms.res)+.2f};

	add_frame_vertices_2d(Dimension::Y,
		{ -x_length/2, y_start, -z_length/2 },
		z_length, x_length, vert_offset );
	add_frame_indzs(indx_offset, vert_offset);
	vert_offset += 12, indx_offset += frame_indzs_count;

	add_frame_vertices_2d(Dimension::Y,
		{ -x_length/2, y_start+height, -z_length/2 },
		z_length, x_length, vert_offset );
	add_frame_indzs(indx_offset, vert_offset);
	vert_offset += 12, indx_offset += frame_indzs_count;

	add_frame_vertices_2d(Dimension::Z,
		{ -x_length/2, y_start, -z_length/2 },
		x_length, height, vert_offset );
	add_frame_indzs(indx_offset, vert_offset);
	vert_offset += 12, indx_offset += frame_indzs_count;

	add_frame_vertices_2d(Dimension::Z,
		{ -x_length/2, y_start, z_length/2 },
		x_length, height, vert_offset );
	add_frame_indzs(indx_offset, vert_offset);
	vert_offset += 12, indx_offset += frame_indzs_count;

	add_frame_vertices_2d(Dimension::X,
		{ x_length/2, y_start, -z_length/2 },
		height, z_length, vert_offset );
	add_frame_indzs(indx_offset, vert_offset);
	vert_offset += 12, indx_offset += frame_indzs_count;

	add_frame_vertices_2d(Dimension::X,
		{ -x_length/2, y_start, -z_length/2 },
		height, z_length, vert_offset );
	add_frame_indzs(indx_offset, vert_offset);
	vert_offset += 12, indx_offset += frame_indzs_count;
}

void Frame::add_frame_indzs(int start_indx, int vertex_offset) {
	for(int i{0}; i != frame_indzs_count; ++i)
		indz[start_indx+i] = frame_indzs[i]+vertex_offset;
}

bgfx::IndexBufferHandle Frame::getIBufferHandle() {
	return bgfx::createIndexBuffer(bgfx::makeRef(indz,
		ibuf_sz*sizeof(uint16_t)));
}

bgfx::VertexBufferHandle Frame::getVBufferHandle() {
	return bgfx::createVertexBuffer(
		bgfx::makeRef(verts,
			vbuf_sz*sizeof(util::PosNormalColorVertex)),
			util::PosNormalColorVertex::layout);
}

};
