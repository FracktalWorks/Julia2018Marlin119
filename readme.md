# Julia 2018 Marlin

3 September 2018 - [Marlin](https://github.com/MarlinFirmware/Marlin/tree/7b594ee4a2feba8872d86efff16f414d93dc01c7) 1.1.9

Commit - 7b594ee

[G-code](http://marlinfw.org/meta/gcode/)



## Build Variants

| Variant                  | Variant Code               | Short Code | Build Code |
|--------------------------|----------------------------|------------|-----------:|
| Julia Basic              | JULIA_2018_GLCD            | J18GX      | 0          |
| Julia Intermediate       | JULIA_2018_GLCD_HB         | J18GB      | 1          |
| Julia Advanced           | JULIA_2018_RPI             | J18RX      | 2          |
| Julia Extended           | JULIA_2018_RPI_E           | J18RE      | 3          |
| Julia Pro Single         | JULIA_2018_PRO_SINGLE      | J18PS      | 4          |
| Julia Pro Dual           | JULIA_2018_PRO_DUAL        | J18PD      | 5          |
| Julia Pro Single ABL     | JULIA_2018_PRO_SINGLE_A    | J18PT      | 6          |
| Julia Pro Dual ABL (_This is being used for building volterra_)       | JULIA_2018_PRO_DUAL_A      | J18PE      | 7          | 

## Migration Process

1. Clone Marlin to `src/Marlin` .
2. Copy overriding files to `src/common`.
3. Modify overriding files.
4. Commit source to git.

## Build Environment Initialization

1. Install [Node.js](https://nodejs.org/en/download/).
2. Change to project root in terminal.
3. Execute `npm i` to install *Node.js* package dependencies.

## Code Compilation

1. Generate intermediates using build script in **Git Bash for Windows**.    
    `./scripts/build <variant_code>`.
1. Export hex files using Arduino IDE.


## Directory Structure

- `src/` - Source root
  - `Marlin/` - [Marlin](https://github.com/MarlinFirmware/Marlin)
  - `common/` - Files common to all variants
- `scripts/` - Build automation rellated scripts
- `staging/` - Marlin + common + variant files
- `output/` - Hex files
- ~~`build.ps1` - Build automation PowerShell script~~ **redundant**
- `watcher.js` - Build automation Node.js script

## Below are changes done on RnD Julia Pro Dual

HEX link- https://github.com/FracktalWorks/Julia2018Marlin119/blob/Volterra/src/Marlin/Marlin.ino.mega.hex

# _Version.h

    Upon Julia Pro build, Changes made are below
    MACHINE_NAME "Volterra Dual"
    BUILD_UNIX_DATETIME "1600795440"
    STRING_DISTRIBUTION_DATE "2020-09-22 17:24"
    DETAILED_BUILD_VERSION "VD20_200922_1724"
    SHORT_BUILD_VERSION "VD20"

# src/Marlin/_FW_Config.h

      #define X_BED_SIZE  395
      #define Y_BED_SIZE  400
      #define Z_MAX_POS   420
      #define INVERT_X_DIR    false
      #define INVERT_Y_DIR    false
      #define EXTRUDERS 4//2
      #define TEMP_SENSOR_0       20
      #define TEMP_SENSOR_1     20
      #define TEMP_SENSOR_2     3
      #define TEMP_SENSOR_3     3
      
 # src/Marlin/Configuration.h
    //#define TEMP_SENSOR_2 0
    //#define TEMP_SENSOR_3 0
    //#define TEMP_SENSOR_4 0
    //#define TEMP_SENSOR_CHAMBER 0
    #define HEATER_0_MAXTEMP 350
    #define HEATER_1_MAXTEMP 350
    #define HEATER_2_MAXTEMP 120  //chamber heater or build volume heater
    #define HEATER_3_MAXTEMP 60  //Filament heater
    //#define HEATER_4_MAXTEMP 275
  
# src/Marlin/pins_MKS_GEN_13.h
    //#if HOTENDS > 2 || E_STEPPERS > 2
    //#error "MKS GEN 1.3/1.4 supports up to 2 hotends / E-steppers. Comment out this line to continue."
    //#endif
# Configuration_adv.h  
    #define EXTRUDER_RUNOUT_PREVENT
    #if ENABLED(EXTRUDER_RUNOUT_PREVENT)
     #define EXTRUDER_RUNOUT_MINTEMP 190
     #define EXTRUDER_RUNOUT_SECONDS 60
     #define EXTRUDER_RUNOUT_SPEED 1500  // mm/m
     #define EXTRUDER_RUNOUT_EXTRUDE 5   // mm
    #endif
# src/Marlin/pins_RAMPS.h
    #ifndef Z_MIN_PROBE_PIN
    #define Z_MIN_PROBE_PIN  40 //57  // 32
    #endif
    
    #define E2_STEP_PIN        36
    #define E2_DIR_PIN         34
    #define E2_ENABLE_PIN      30
    #ifndef E2_CS_PIN
    #define E2_CS_PIN        44
    #endif

    #define E3_STEP_PIN        36
    #define E3_DIR_PIN         34
    #define E3_ENABLE_PIN      30
    #ifndef E3_CS_PIN
    #define E3_CS_PIN        44
    #endif
    
    
    #define TEMP_0_PIN         12   // Analog Input
    #define TEMP_1_PIN         11   // Analog Input
    #define TEMP_2_PIN         13   // Analog Input
    #define TEMP_3_PIN         15   // Analog Input
    
    #define HEATER_1_PIN   MOSFET_D_PIN
    #define HEATER_2_PIN   44
    #define HEATER_3_PIN   42
    #endif
    
# src/Marlin/SanityCheck.h
    //#elif EXTRUDERS != 2
    //#error "SWITCHING_NOZZLE requires exactly 2 EXTRUDERS."
    //#elif TEMP_SENSOR_2 != 0
    //#error "TEMP_SENSOR_2 shouldn't be set with only 2 HOTENDS."
     //#elif TEMP_SENSOR_3 != 0
     //#error "TEMP_SENSOR_3 shouldn't be set with only 2 HOTENDS."
