#ifndef MODEL_H_
#define MODEL_H_

#include "Util.hpp"
#include "bgfx/bgfx.h"

namespace worldWp {

template<typename T>
class Model {
public:
	Model(int vert_sz, int indzs_sz)
		: vert_sz{vert_sz},
		  indzs_sz{indzs_sz},
		  verts{ new util::PosNormalColorVertex[vert_sz] },
		  indzs{ new T[indzs_sz] } { }

	bgfx::VertexBufferHandle getVBufferHandle() {
		return bgfx::createVertexBuffer(
			bgfx::makeRef(verts,
				vert_sz*sizeof(util::PosNormalColorVertex)),
				util::PosNormalColorVertex::layout);
	}

	bgfx::IndexBufferHandle getIBufferHandle() {
		return bgfx::createIndexBuffer(bgfx::makeRef(indzs,
			indzs_sz*sizeof(T)),
			//Change Buffer type acording to T.
			(std::is_same<T, uint32_t>::value ? BGFX_BUFFER_INDEX32 : BGFX_BUFFER_NONE));
	}
private:
	int vert_sz, indzs_sz;
protected:
	util::PosNormalColorVertex *verts;
	T *indzs;
};

};

#endif
