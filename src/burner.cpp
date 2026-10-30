#include "burner.hpp"


namespace Burner {



bool checkUTempWithHyst(Sensors &sns) {
	if (Val::errorCode >0) return false;
	bool burn_st = getStatus();
	float currentTemp = getIAverageTemp(sns);
	if (!burn_st) {
		if (currentTemp <= uTemp - hyst) return true;
	}
	else {
		if (currentTemp > uTemp)  return false;
		return true;
	}
	return burn_st;
}

bool getStatus() {
	return static_cast<bool>(digitalRead(BURNER_LED_PIN));
}
bool checkUTemp(Sensors &sns) {
  if (Val::errorCode > 0) return false;
  return getIAverageTemp(sns) <= uTemp - hyst;
}

//void standard_runBurn() {
//  if (!getReadySensors(Sens::all)) return;
//  Squib::runB();
//}

void manageBurner(Sensors &sns) {
  bool res = suc_prep &&
		  Squib::getSquibStatus() &&
		  Val::pipelineWithoutHalt(sns) == 0 &&
		  checkUTempWithHyst(sns) &&
		  isSensorsReady(sns)
		  ;
    digitalWrite(BURNER_LED_PIN, res);
}


}  // namespace Burner
