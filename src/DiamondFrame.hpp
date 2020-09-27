#ifndef DIAMOND_FRAME_H_
#define DIAMOND_FRAME_H_

#include "Model.tpp"
#include "Util.hpp"

namespace worldWp {

class DiamondFrame : public Model<uint16_t> {
public:
	DiamondFrame(const util::PlaneSpecs& ms);
private:
	void add_indzs();
	void add_verts(bx::Vec3 dims, bx::Vec3 center);
};

};

#endif
