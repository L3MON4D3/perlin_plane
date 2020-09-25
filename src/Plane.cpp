#include "Plane.hpp"
#include "Util.hpp"
#include "bx/math.h"

#include <cmath>
#include <iostream>
#include <ostream>

int ibuf_indzs[2],
    vbuf_indzs[2];

namespace worldWp {

std::ostream& operator<<(std::ostream& out, util::PosNormalColorVertex& v) {
	return out << "{" << v.pos[0] << ", " << v.pos[1] << ", " << v.pos[2] << "}";
}

Plane::Plane(
  const ModelSpecs& ms,
  const FastNoise& fn,
  const worldWp::util::NoiseMods& nm
)
	: ms{ ms },
	  nm{ nm } {

	vbuf_indzs[0] =                 ms.x_dim*ms.z_dim*2,
	vbuf_indzs[1] = vbuf_indzs[0] + (ms.x_dim-1 + ms.z_dim-1)*2 + 4,

	ibuf_indzs[0] =                 (ms.x_dim-1)*(ms.z_dim-1)*2*2*3,
	ibuf_indzs[1] = ibuf_indzs[0] + ((ms.x_dim-1)+(ms.z_dim-1))*2*2*3 + 6;

	//double space, store duplicate indizes ith different normals.
	plane_verts = new worldWp::util::PosNormalColorVertex[ibuf_indzs[1]];
	plane_indz = new uint32_t[ibuf_indzs[1]];

	add_plane_vertices(fn);
	add_normals();
	add_base_vertices(-40);
	add_base_indizes();

	//fill plane_indz.
	int offset{ ms.x_dim*ms.z_dim };
	{
		int plane_x_dim{ ms.x_dim-1 },
		    plane_z_dim{ ms.z_dim-1 };
		for(int i = 0; i != plane_x_dim; ++i)
			for(int j = 0; j != plane_z_dim; ++j) {
				int vert_start_indx {i*ms.z_dim + j};
				
				//init vertices for triangles.
				int v1{ vert_start_indx },
				    v2{ v1+1 },
				    v3{ vert_start_indx+ms.z_dim },
				    v4{ v3+1 };
				
				int tri_start_indx {(i*plane_z_dim + j) * 12};
				//first Triangle of "square".
				plane_indz[tri_start_indx   ] = v3;
				plane_indz[tri_start_indx+ 1] = v2;
				plane_indz[tri_start_indx+ 2] = v1;
				
				plane_indz[tri_start_indx+ 3] = v2;
				plane_indz[tri_start_indx+ 4] = v3;
				plane_indz[tri_start_indx+ 5] = v1;
				
				//second Triangle of "square".
				plane_indz[tri_start_indx+ 6] = v3+offset;
				plane_indz[tri_start_indx+ 7] = v4+offset;
				plane_indz[tri_start_indx+ 8] = v2+offset;
				
				plane_indz[tri_start_indx+ 9] = v4+offset;
				plane_indz[tri_start_indx+10] = v3+offset;
				plane_indz[tri_start_indx+11] = v2+offset;
			}
	}
}

void Plane::add_normals() {
	for(int i {0}; i != (ms.x_dim-1)*ms.z_dim; ++i) {
		worldWp::util::add_normal(&plane_verts[i],
			//pass pos of each Vertex
			(float*) &plane_verts[i+1],
			(float*) &plane_verts[i+ms.z_dim]);

		//add normals to "downward-pointing" triangle.
		int i_offset = ms.x_dim*ms.z_dim+i+1;
		worldWp::util::add_normal(&plane_verts[i_offset],
			(float*) &plane_verts[i_offset+ms.z_dim],
			(float*) &plane_verts[i_offset+ms.z_dim-1]);
	}
}

void Plane::add_plane_vertices(const FastNoise& fn) {
	//fill plane_verts with values from fn.
	//indx = i*j at any point in loop.
	int indx {0};
	int offset {ms.x_dim*ms.z_dim};
	for(int i {0}; i != ms.x_dim*ms.res; i+=ms.res)
		for(int j {0}; j != ms.z_dim*ms.res; j+=ms.res, ++indx)
			plane_verts[indx+offset] =
			plane_verts[indx       ] = { float(i-(ms.x_dim-1)*ms.res/2.0),
			                             util::get_noise_mdfd(i, j, fn, nm),
			                             float(j-(ms.z_dim-1)*ms.res/2.0), 
			                             0, 0, 0,
			                             0xff666666 };
}
void Plane::add_base_vertices(float y_start) {
	/* Example Vertex Layout: (add start_vert)
	 * 6 5 4
	 * 7   3
	 * 0 1 2
	 */
	int start_vert{vbuf_indzs[0]};
	for(int i{start_vert}; i != start_vert + (ms.x_dim-1+ms.z_dim-1)*2; ++i)
		plane_verts[i] = {0,y_start,0, 0,1,1, 0xff666666};
	
	const int dirs[2] {0,  2},
	          dir_size[2] {ms.x_dim-1, ms.z_dim-1},
	          sign[2] {1, -1};
	const float corners[4][2] {
				  {-(ms.x_dim-1)*ms.res/2.0f, -(ms.z_dim-1)*ms.res/2.0f},
				  { (ms.x_dim-1)*ms.res/2.0f, -(ms.z_dim-1)*ms.res/2.0f},
				  { (ms.x_dim-1)*ms.res/2.0f,  (ms.z_dim-1)*ms.res/2.0f},
				  {-(ms.x_dim-1)*ms.res/2.0f,  (ms.z_dim-1)*ms.res/2.0f}
			  };

	int indx{start_vert};

	for(int s{0}; s != 2; ++s)
		for(int d{0}; d != 2; ++d)
			for(int i{0}; i != dir_size[d]*ms.res; i+=ms.res, ++indx) {
				util::PosNormalColorVertex& v{ plane_verts[indx] };
				const float *corner{ corners[s*2+d] };

				v.pos[dirs[d]] = corner[d] + sign[s]*i;
				//d=1 -> 0, d=0 -> 1
				v.pos[dirs[d^1]] = corner[d^1];
			}

	//add vertex at 0 (see above) with different color for underside of base.
	plane_verts[indx] = {corners[0][0], y_start, corners[0][1],
	                     0,1,1,
	                     0xff555555};
	plane_verts[++indx] = {corners[1][0], y_start, corners[1][1],
	                     0,1,1,
	                     0xff555555};
	plane_verts[++indx] = {corners[2][0], y_start, corners[2][1],
	                     0,1,1,
	                     0xff555555};
	plane_verts[++indx] = {corners[3][0], y_start, corners[3][1],
	                     0,1,1,
	                     0xff555555};
}

void Plane::add_base_indizes() {
	int start_indx{ibuf_indzs[0]},
	    base_start_vert{vbuf_indzs[0]};

	int indx{start_indx};
	for(int i{0}; i != ms.x_dim-1; ++i, indx+=6) {
		plane_indz[indx+1] = i*ms.z_dim;
		plane_indz[indx+2] = base_start_vert+i;
		plane_indz[indx+0] = base_start_vert+i+1;

		plane_indz[indx+4] = i*ms.z_dim;
		plane_indz[indx+5] = base_start_vert+i+1;
		plane_indz[indx+3] = (i+1)*ms.z_dim;
	}

	int plane_vert_start{ (ms.x_dim-1)*ms.z_dim };
	base_start_vert+=ms.x_dim-1;
	for(int i{0}; i != ms.z_dim-1; ++i, indx+=6) {
		plane_indz[indx+1] = plane_vert_start+i;
		plane_indz[indx+2] = base_start_vert+i;
		plane_indz[indx+0] = base_start_vert+i+1;

		plane_indz[indx+4] = plane_vert_start+i;
		plane_indz[indx+5] = base_start_vert+i+1;
		plane_indz[indx+3] = plane_vert_start+i+1;
	}

	plane_vert_start += ms.z_dim-1;
	base_start_vert += ms.z_dim-1;
	for(int i{0}; i != ms.x_dim-1; ++i, indx+=6) {
		plane_indz[indx+1] = plane_vert_start-i*ms.z_dim;
		plane_indz[indx+2] = base_start_vert+i;
		plane_indz[indx+0] = base_start_vert+i+1;

		plane_indz[indx+4] = plane_vert_start-i*ms.z_dim;
		plane_indz[indx+5] = base_start_vert+i+1;
		plane_indz[indx+3] = plane_vert_start-(i+1)*ms.z_dim;
	}

	plane_vert_start = ms.z_dim-1;
	base_start_vert += ms.x_dim-1;
	for(int i{0}; i != ms.z_dim-2; ++i, indx+=6) {
		plane_indz[indx+1] = plane_vert_start-i;
		plane_indz[indx+2] = base_start_vert+i;
		plane_indz[indx+0] = base_start_vert+(i+1);

		plane_indz[indx+4] = plane_vert_start-i;
		plane_indz[indx+5] = base_start_vert+(i+1);
		plane_indz[indx+3] = plane_vert_start-(i+1);
	}

	//add last two triangles by hand, stupid in loop:
	plane_indz[indx+1] = 1;
	plane_indz[indx+2] = base_start_vert + ms.z_dim-2;
	plane_indz[indx+0] = vbuf_indzs[0];

	plane_indz[indx+4] = 1;
	plane_indz[indx+5] = vbuf_indzs[0];
	plane_indz[indx+3] = 0;
	indx+=6;

	//add rectangle on bottom of base.
	//index of first base-rectangle-vertex.
	base_start_vert = vbuf_indzs[1]-4;
	plane_indz[indx  ] = base_start_vert;
	plane_indz[indx+1] = base_start_vert+2;
	plane_indz[indx+2] = base_start_vert+1;

	plane_indz[indx+3] = base_start_vert;
	plane_indz[indx+4] = base_start_vert+3;
	plane_indz[indx+5] = base_start_vert+2;
}

float* Plane::get_raw_noise(const FastNoise& fn) {
	float* ns {new float[ms.x_dim*ms.z_dim]};

	int indx {0};
	for(int i {0}; i != ms.x_dim*ms.res; i+=ms.res)
		for(int j {0}; j != ms.z_dim*ms.res; j+=ms.res, ++indx)
			ns[indx] = util::get_noise_mdfd(i, j, fn, nm);
	return ns;
}

bgfx::IndexBufferHandle Plane::getIBufferHandle() {
	return bgfx::createIndexBuffer(bgfx::makeRef(plane_indz,
		ibuf_indzs[1]*sizeof(uint32_t)), BGFX_BUFFER_INDEX32);
}

bgfx::VertexBufferHandle Plane::getVBufferHandle() {
	return bgfx::createVertexBuffer(
		bgfx::makeRef(plane_verts,
			vbuf_indzs[1]*sizeof(util::PosNormalColorVertex)),
			util::PosNormalColorVertex::layout);
}

void Plane::for_each_vertex(
  const std::function<void(util::PosNormalColorVertex&, int indx)>& fn
) {
	for(int i{0}; i != ms.x_dim*ms.z_dim; ++i) {
		//apply function to both vertices (each vertex exists twice for normals).
		fn(plane_verts[i], i);
		fn(plane_verts[i+ms.x_dim*ms.z_dim], i);
	}
}

};
