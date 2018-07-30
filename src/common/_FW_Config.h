#ifndef FWCONFIG_H
#define FWCONFIG_H

//===========================================================================
//========================= Fracktal Works Julia 2018 =======================
//===========================================================================

#include "_FracktalWorks.h"

#ifndef VARIANT
  #error "Variant not defined"
#endif

/**  Machine Branding  **/
#define USE_AUTOMATIC_VERSIONING

/** Machine name for variant **/
#if BV(JULIA_2018_GLCD)
	#define CUSTOM_MACHINE_NAME "Julia 2018 Basic"
#elif BV(JULIA_2018_GLCD_HB)
	#define CUSTOM_MACHINE_NAME "Julia 2018 Intermediate"
#elif BV(JULIA_2018_RPI)
	#define CUSTOM_MACHINE_NAME "Julia 2018 Advanced"
#elif BV(JULIA_2018_RPI_E)
	#define CUSTOM_MACHINE_NAME "Julia 2018 Extended"
#elif BV(JULIA_2018_PRO_SINGLE)
  #define CUSTOM_MACHINE_NAME "Julia 2018 Pro Single"
#elif BV(JULIA_2018_PRO_DUAL)
  #define CUSTOM_MACHINE_NAME "Julia 2018 Pro Dual"
#endif

#define STRING_CONFIG_H_AUTHOR  "Fracktal Works"
#define STRING_SPLASH_LINE1     "1.01"
#define STRING_SPLASH_LINE2     "www.fracktal.in"

#define SHOW_BOOTSCREEN
#define SHOW_CUSTOM_BOOTSCREEN


/**  Control board  **/
#define MOTHERBOARD BOARD_RAMPS_13_EFB
#define BAUDRATE 115200

/**  Filament  **/
#define DEFAULT_NOMINAL_FILAMENT_DIA 1.75

/**  Bed dimensions  **/
#if BV_REG()
  #define X_BED_SIZE  210
  #define Y_BED_SIZE  200
  #define Z_MAX_POS   210
#elif BV(JULIA_2018_RPI_E)
  #define X_BED_SIZE  260
  #define Y_BED_SIZE  250
  #define Z_MAX_POS   300
#elif BV_PRO()
  #define X_BED_SIZE  420
  #define Y_BED_SIZE  410
  #define Z_MAX_POS   405
#endif

/**  Kinematics  **/
#define COREXY

/**  Enstops  **/
#define USE_XMIN_PLUG
#define USE_YMAX_PLUG
#define USE_ZMAX_PLUG

/**  Movement  **/
#define DEFAULT_AXIS_STEPS_PER_UNIT   { 160,  160, 1007.874, 280 }
#define DEFAULT_MAX_FEEDRATE          { 200, 200, 20, 45 }
#define DEFAULT_MAX_ACCELERATION      { 1000, 1000, 50, 10000 }
#define DEFAULT_ACCELERATION          1000    // X, Y, Z and E acceleration for printing moves
#define DEFAULT_RETRACT_ACCELERATION  2000    // E acceleration for retracts
#define DEFAULT_TRAVEL_ACCELERATION   1000    // X, Y, Z acceleration for travel (non printing) moves
#define DEFAULT_XJERK                 5.0
#define DEFAULT_YJERK                 5.0
#define DEFAULT_ZJERK                 0.4


/**  Stepper  **/
#define INVERT_X_DIR    false
#define INVERT_Y_DIR    false
#define INVERT_Z_DIR    false
// #define INVERT_E0_DIR   true
// #define INVERT_E1_DIR   true
#if BV_PRO()
  #define INVERT_E0_DIR   false
  #define INVERT_E1_DIR   true
#else
  #define INVERT_E0_DIR   true
#endif

/**  Homing  **/
#define X_HOME_DIR         -1
#define Y_HOME_DIR          1
#define Z_HOME_DIR          1
#define HOMING_FEEDRATE_Z   (20*60)

/**  Thermistor  **/
#define TEMP_SENSOR_0 3
#if BV(JULIA_2018_GLCD)
	#define TEMP_SENSOR_BED 0
#else
	#define TEMP_SENSOR_BED 3
#endif

/**  Thermal  **/
#define TEMP_RESIDENCY_TIME     1
#define TEMP_HYSTERESIS         4
#define TEMP_BED_RESIDENCY_TIME 2

/**  PID temperature settings  **/
#define  DEFAULT_Kp 42.96
#define  DEFAULT_Ki 5.14
#define  DEFAULT_Kd 89.73

/**  Thermal Runaway  **/
#if NBV(JULIA_2018_GLCD)
	#define THERMAL_PROTECTION_BED
#endif

/**  Bed leveling  **/
#define MESH_BED_LEVELING
#define MESH_TEST_HOTEND_TEMP   210.0   // (°C) Default nozzle temperature for the G26 Mesh Validation Tool.
#if BV(JULIA_2018_RPI_E) || BV_PRO()
  #define GRID_MAX_POINTS_X 3
#else
  #define GRID_MAX_POINTS_X 2
#endif
#if BV_NPI()
  #define LCD_BED_LEVELING
#endif


/**  MKS-MINI12864 LCD  **/
#if BV_NPI()
	#define SDSUPPORT
	#define ENCODER_PULSES_PER_STEP 2
	#define ENCODER_STEPS_PER_MENU_ITEM 2
	#define SPEAKER
	#define MKS_MINI_12864
#endif


/**  Additional features  **/
#define EEPROM_SETTINGS     // Enable for M500 and M501 commands
#define NOZZLE_PARK_FEATURE
#define PRINTCOUNTER
#if BV_NPI()
	#define PRINT_RESTORE		  // print restore
#endif

//===========================================================================
//===================== End of Fracktal Works Julia 2018 ====================
//===========================================================================

#endif