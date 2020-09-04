#include "ModelBuilder.hpp"
#include "Util.hpp"

#include "bgfx/bgfx.h"
#include "bx/math.h"

#include <cmath>
#include <iostream>
#include <ostream>

namespace worldWp {

void PosNormalColorVertex::init() {
    layout
        .begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8, true)
        .end();
}

ModelBuilder::ModelBuilder(const int x_dim, const int y_dim,
	FastNoise fn, int vert_dist)

    : x_dim{x_dim},
      y_dim{y_dim},
	  //double space, store duplicate indizes ith different normals.
      plane_verts{ new PosNormalColorVertex[x_dim*y_dim*2] },
      plane_indz{ new uint32_t[(x_dim-1)*(y_dim-1)*12] } {

	//fill plane_verts with values from ns_gen.
    //indx = i*j;
    int indx = 0;
	int offset = x_dim*y_dim;
    for(int i = 0; i != x_dim*vert_dist; i+=vert_dist)
        for(int j = 0; j != y_dim*vert_dist; j+=vert_dist, ++indx)
            plane_verts[indx+offset] =
			plane_verts[indx       ] = {(float) i,
                                 (float) j, 
                                 (float) 60*fn.GetNoise(i*2, j*2),
								 0, 0, 0,
								 0xffaaaaaa};

	for(int i = 0; i != (x_dim-1)*y_dim; ++i) {
	    worldWp::util::add_normal(&plane_verts[i],
		//pass pos of each Vertex
		(float*) &plane_verts[i+1],
		(float*) &plane_verts[i+y_dim]);

		//add normals to "downward-pointing" triangle.
		int i_offset = offset+i+1;
	    worldWp::util::add_normal(&plane_verts[i_offset],
		(float*) &plane_verts[i_offset+y_dim],
		(float*) &plane_verts[i_offset+y_dim-1]);
	}
	
	
    //fill plane_indz.
    {
        int plane_x_dim = x_dim-1,
            plane_y_dim = y_dim-1;
		for(int i = 0; i != plane_x_dim; ++i)
			for(int j = 0; j != plane_y_dim; ++j) {
                int vert_start_indx {i*x_dim + j};

                //init vertices for triangles.
                int v1 = vert_start_indx,
                    v2 = v1+1,
                    v3 = vert_start_indx+y_dim,
                    v4 = v3+1;

                int tri_start_indx = (i*plane_x_dim + j) * 12;
                //first Triangle of "square".
                plane_indz[tri_start_indx   ] = v1;
                plane_indz[tri_start_indx+ 1] = v3;
                plane_indz[tri_start_indx+ 2] = v2;

                plane_indz[tri_start_indx+ 3] = v1+offset;
                plane_indz[tri_start_indx+ 4] = v2+offset;
                plane_indz[tri_start_indx+ 5] = v3+offset;

                //second Triangle of "square".
                plane_indz[tri_start_indx+ 6] = v2;
                plane_indz[tri_start_indx+ 7] = v3;
                plane_indz[tri_start_indx+ 8] = v4;

                plane_indz[tri_start_indx+ 9] = v2+offset;
                plane_indz[tri_start_indx+10] = v4+offset;
                plane_indz[tri_start_indx+11] = v3+offset;
            }
    }
}

bgfx::IndexBufferHandle ModelBuilder::getIBufferHandle() {
    return bgfx::createIndexBuffer(bgfx::makeRef(plane_indz,
        (x_dim-1)*(y_dim-1)*12*sizeof(uint32_t)), BGFX_BUFFER_INDEX32);
}

bgfx::VertexBufferHandle ModelBuilder::getVBufferHandle() {
    return bgfx::createVertexBuffer(
        bgfx::makeRef(plane_verts,
             x_dim*y_dim*2*sizeof(PosNormalColorVertex)),
             PosNormalColorVertex::layout);
}

};
