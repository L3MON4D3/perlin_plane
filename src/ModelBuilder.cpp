#include "ModelBuilder.hpp"
#include "bgfx/bgfx.h"
#include <iostream>

namespace worldWp {

void PosColorVertex::init() {
    layout
        .begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8, true)
        .end();
}

ModelBuilder::ModelBuilder(int x_dim, int y_dim, RandomGenerator::Perlin ns_gen)
    : x_dim{x_dim},
      y_dim{y_dim},
      plane_verts{new PosColorVertex[x_dim*y_dim] },
      plane_indz{ new uint16_t[(x_dim-1)*(y_dim-1)*12] } {

	//fill plane_verts with values from ns_gen.
    //indx = i*j;
    int indx = 0;
    for(int i = 0; i != x_dim; ++i)
        for(int j = 0; j != y_dim; ++j, ++indx)
            plane_verts[indx] = {(float) i,
                                 (float) j, 
                                 (float) ns_gen.noise_at(i, j), 
                                 0xffffffff};

    //fill plane_indz.
    {
        int plane_x_dim = x_dim-1,
            plane_y_dim = y_dim-1;
		for(int i = 0; i != plane_x_dim; ++i)
			for(int j = 0; j != plane_y_dim; ++j) {
                int vert_start_indx = (i*y_dim + j);

                //init vertices for triangles.
                int v1 = vert_start_indx,
                    v2 = v1+1,
                    v3 = vert_start_indx+x_dim,
                    v4 = v3+1;

                int tri_start_indx = (i*plane_x_dim + j) * 12;
                //first Triangle of "square".
                plane_indz[tri_start_indx   ] = v1;
                plane_indz[tri_start_indx+ 1] = v3;
                plane_indz[tri_start_indx+ 2] = v2;

                plane_indz[tri_start_indx+ 3] = v1;
                plane_indz[tri_start_indx+ 4] = v2;
                plane_indz[tri_start_indx+ 5] = v3;

                //second Triangle of "square".
                plane_indz[tri_start_indx+ 6] = v2;
                plane_indz[tri_start_indx+ 7] = v3;
                plane_indz[tri_start_indx+ 8] = v4;

                plane_indz[tri_start_indx+ 9] = v2;
                plane_indz[tri_start_indx+10] = v4;
                plane_indz[tri_start_indx+11] = v3;
            }
    }
}

bgfx::IndexBufferHandle ModelBuilder::getIBufferHandle() {
    return bgfx::createIndexBuffer(bgfx::makeRef(plane_indz,
        (x_dim-1)*(y_dim-1)*12*sizeof(uint16_t)));
}

bgfx::VertexBufferHandle ModelBuilder::getVBufferHandle() {
    return bgfx::createVertexBuffer(
        bgfx::makeRef(plane_verts,
             x_dim*y_dim*sizeof(PosColorVertex)),
             PosColorVertex::layout);
}
};
