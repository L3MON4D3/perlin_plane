#include "PlaneBuilder.hpp"
#include "Util.hpp"

#include <cmath>
#include <iostream>
#include <ostream>

namespace worldWp {

PlaneBuilder::PlaneBuilder(
  const ModelSpecs& ms,
  const FastNoise& fn,
  const worldWp::util::NoiseMods& nm
)
	: ms{ ms },
	  nm{ nm },
	  //double space, store duplicate indizes ith different normals.
	  plane_verts{ new worldWp::util::PosNormalColorVertex[ms.x_dim*ms.z_dim*2] },
	  plane_indz{ new uint32_t[(ms.x_dim-1)*(ms.z_dim-1)*12] } {

	add_plane_vertices(fn);
	add_normals();

	//fill plane_indz.
	{
		int offset{ ms.x_dim*ms.z_dim };
		int plane_x_dim{ ms.x_dim-1 },
		    plane_z_dim{ ms.z_dim-1 };
		for(int i = 0; i != plane_x_dim; ++i)
			for(int j = 0; j != plane_z_dim; ++j) {
				int vert_start_indx {i*ms.x_dim + j};
				
				//init vertices for triangles.
				int v1{ vert_start_indx },
				    v2{ v1+1 },
				    v3{ vert_start_indx+ms.z_dim },
				    v4{ v3+1 };
				
				int tri_start_indx {(i*plane_x_dim + j) * 12};
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

void PlaneBuilder::add_normals() {
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

void PlaneBuilder::add_plane_vertices(const FastNoise& fn) {
	//fill plane_verts with values from ns_gen.
	//indx = i*j at any point in loop.
	int indx {0};
	int offset {ms.x_dim*ms.z_dim};
	for(int i {0}; i != ms.x_dim*ms.res; i+=ms.res)
		for(int j {0}; j != ms.z_dim*ms.res; j+=ms.res, ++indx)
			plane_verts[indx+offset] =
			plane_verts[indx       ] = { (float) i-ms.x_dim*ms.res/2,
			                             util::get_noise_mdfd(i, j, fn, nm),
			                             (float) j-ms.z_dim*ms.res/2, 
			                             0, 0, 0,
			                             0xff666666 };
}

float* PlaneBuilder::get_raw_noise(const FastNoise& fn) {
	float* ns {new float[ms.x_dim*ms.z_dim]};

	int indx {0};
	for(int i {0}; i != ms.x_dim*ms.res; i+=ms.res)
		for(int j {0}; j != ms.z_dim*ms.res; j+=ms.res, ++indx)
			ns[indx] = util::get_noise_mdfd(i, j, fn, nm);
	return ns;
}

bgfx::IndexBufferHandle PlaneBuilder::getIBufferHandle() {
	return bgfx::createIndexBuffer(bgfx::makeRef(plane_indz,
		(ms.x_dim-1)*(ms.z_dim-1)*12*sizeof(uint32_t)), BGFX_BUFFER_INDEX32);
}

bgfx::VertexBufferHandle PlaneBuilder::getVBufferHandle() {
	return bgfx::createVertexBuffer(
		bgfx::makeRef(plane_verts,
			ms.x_dim*ms.z_dim*2*sizeof(worldWp::util::PosNormalColorVertex)),
			worldWp::util::PosNormalColorVertex::layout);
}

void PlaneBuilder::for_each_vertex(
  const std::function<void(util::PosNormalColorVertex&, int indx)>& fn
) {
	for(int i{0}; i != ms.x_dim*ms.z_dim; ++i) {
		//apply function to both vertices (each vertex exists twice for normals).
		fn(plane_verts[i], i);
		fn(plane_verts[i+ms.x_dim*ms.z_dim], i);
	}
}

};
