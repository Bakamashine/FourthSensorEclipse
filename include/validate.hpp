#pragma once
#include "sensor.hpp"

namespace Val {
inline int errorCode = 0;

void pipeline(Sensors &);
int pipelineWithoutHalt(Sensors &);
void haltSystem();
namespace check {
	void error();
}
inline bool systemHalted = false;
}  // namespace Val
