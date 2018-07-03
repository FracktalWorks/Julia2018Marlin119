/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * power_loss_recovery.h - Resume an SD print after power-loss
 */

#ifndef _POWER_LOSS_RECOVERY_H_
#define _POWER_LOSS_RECOVERY_H_

#include "cardreader.h"
#include "types.h"
#include "MarlinConfig.h"

//#define DEBUG_POWER_LOSS_RECOVERY

// #define RECOVERY_CODE_FILE "RESR.GCODE"

typedef struct {
  uint8_t valid_head;

  // Machine state
  float current_position[NUM_AXIS], feedrate;
  int16_t target_temperature[HOTENDS],
          fanSpeeds[FAN_COUNT];

  #if HAS_HEATED_BED
    int16_t target_temperature_bed;
  #endif

  #if HAS_LEVELING
    bool leveling;
    float fade;
  #endif

  // Command queue
  uint8_t cmd_queue_index_r, commands_in_queue;
  char command_queue[BUFSIZE][MAX_CMD_SIZE];

  // SD File position
  uint32_t sdpos;

  // Job elapsed time
  millis_t print_job_elapsed;

  uint8_t valid_foot;
	
	char sd_filename[MAXPATHNAMELENGTH];
} job_recovery_info_t;

extern job_recovery_info_t job_recovery_info;

/*
enum JobRecoveryPhase : unsigned char {
  JOB_RECOVERY_IDLE,
  JOB_RECOVERY_MAYBE,
  JOB_RECOVERY_YES
};
extern JobRecoveryPhase job_recovery_phase;
*/

enum RestorationPhase : unsigned char {
  IDLE,
  BIN_FOUND,
  FILE_MADE,
  LCD_MAYBE,
	START
};
extern RestorationPhase restoration_phase;


//#define APPEND_CMD_COUNT HOTENDS + FAN_COUNT + 15

/* FRACKTAL WORKS: START */
//extern char job_recovery_commands[BUFSIZE + APPEND_CMD_COUNT][MAX_CMD_SIZE];
//extern uint8_t job_recovery_commands_count;
//extern bool job_recovery_found;
/* FRACKTAL WORKS: END */


void do_print_job_recovery();
void save_job_recovery_info();

#endif // _POWER_LOSS_RECOVERY_H_
