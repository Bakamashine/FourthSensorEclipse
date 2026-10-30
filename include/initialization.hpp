/*
 * initilization.hpp
 *
 *  Created on: Oct 30, 2026
 *      Author: User
 */

#ifndef INITIALIZATION_HPP_
#define INITIALIZATION_HPP_
#include <avr/wdt.h>

#include "constants/pin.hpp"
#include "sensor.hpp"
#include "settings.hpp"
#include "ui/ui_class.hpp"
#include "validate.hpp"
#include "burner.hpp"


#define ATTEMPTS 5
static constexpr int vent_delay = 1000 * 5;         // 5 sec
static constexpr int ignition_fp_delay = 1000 * 2;  // 2 sec
static constexpr int ignition_sp_delay = 1000 * 3;  // 3 sec
static constexpr int ignition_delay =
    ignition_fp_delay + ignition_sp_delay;  // 5 sec
static constexpr int pilot_delay = 1000 * 5;
static constexpr int pilot_valve_delay = 1000 * 3;      // pilot valve on time
static constexpr int pilot_valve_off_delay = 1000 * 2;  // pilot valve off time
static constexpr int burner_sensor_window = 1000 * 2;   // burner signal search
static constexpr int burner_sensor_step = burner_sensor_window / 5;

inline bool _vent_status = false;
inline bool _ign_trans = false;
inline bool _pil_val = false;
inline bool _bur_sens = false;

// successful prepare
inline bool suc_prep = false;

// for gui
inline bool loop_vent_status = false;
inline bool loop_ign_trans = false;
inline bool loop_pil_val = false;
inline bool loop_bur_sens = false;
namespace Init {

void prepare();

} // namespace Init


#endif /* INITIALIZATION_HPP_ */
