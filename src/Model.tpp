#ifndef MODEL_H_
#define MODEL_H_

#include "bgfx/bgfx.h"
#include "Util.hpp"

namespace worldWp {

template<typename T>
class Model {
public:
	Model(int vert_sz, int indzs_sz, uint64_t indzs_state)
		: vert_sz{vert_sz},
		  indzs_sz{indzs_sz},
		  indzs_state{indzs_state},
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

	uint64_t get_indzs_state() {
		return indzs_state;
	}
private:
	int vert_sz, indzs_sz;
	uint64_t indzs_state;
protected:
	util::PosNormalColorVertex *verts;
	T *indzs;
};

};

#endif
