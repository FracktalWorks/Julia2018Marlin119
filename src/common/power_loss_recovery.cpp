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
 * power_loss_recovery.cpp - Resume an SD print after power-loss
 */

#include "MarlinConfig.h"

#if ENABLED(POWER_LOSS_RECOVERY)

#include "power_loss_recovery.h"

#include "cardreader.h"
#include "planner.h"
#include "printcounter.h"
#include "serial.h"
#include "temperature.h"
#include "ultralcd.h"
#include "power_panic_util.h"

// Recovery data
job_recovery_info_t job_recovery_info;
JobRecoveryPhase job_recovery_phase = JOB_RECOVERY_IDLE;
//uint8_t job_recovery_commands_count; //=0
//char job_recovery_commands[BUFSIZE + APPEND_CMD_COUNT][MAX_CMD_SIZE];
bool job_recovery_found = false;

// Extern
extern uint8_t commands_in_queue, cmd_queue_index_r;
extern char command_queue[BUFSIZE][MAX_CMD_SIZE];

// Private
// static char sd_filename[MAXPATHNAMELENGTH];

#if ENABLED(DEBUG_POWER_LOSS_RECOVERY)
  void debug_print_job_recovery(const bool recovery) {
    SERIAL_PROTOCOLPAIR("valid_head:", (int)job_recovery_info.valid_head);
    SERIAL_PROTOCOLLNPAIR(" valid_foot:", (int)job_recovery_info.valid_foot);
    if (job_recovery_info.valid_head) {
      if (job_recovery_info.valid_head == job_recovery_info.valid_foot) {
        SERIAL_PROTOCOLPGM("current_position");
        LOOP_XYZE(i) SERIAL_PROTOCOLPAIR(": ", job_recovery_info.current_position[i]);
        SERIAL_EOL();
        SERIAL_PROTOCOLLNPAIR("feedrate: ", job_recovery_info.feedrate);
        SERIAL_PROTOCOLPGM("target_temperature");
        HOTEND_LOOP() SERIAL_PROTOCOLPAIR(": ", job_recovery_info.target_temperature[e]);
        SERIAL_EOL();
        SERIAL_PROTOCOLPGM("fanSpeeds");
        for(uint8_t i = 0; i < FAN_COUNT; i++) SERIAL_PROTOCOLPAIR(": ", job_recovery_info.fanSpeeds[i]);
        SERIAL_EOL();
        #if HAS_LEVELING
          SERIAL_PROTOCOLPAIR("leveling: ", int(job_recovery_info.leveling));
          SERIAL_PROTOCOLLNPAIR(" fade: ", int(job_recovery_info.fade));
        #endif
        #if HAS_HEATED_BED
          SERIAL_PROTOCOLLNPAIR("target_temperature_bed: ", job_recovery_info.target_temperature_bed);
        #endif
        SERIAL_PROTOCOLLNPAIR("cmd_queue_index_r: ", job_recovery_info.cmd_queue_index_r);
        SERIAL_PROTOCOLLNPAIR("commands_in_queue: ", job_recovery_info.commands_in_queue);
        if (recovery)
          for (uint8_t i = 0; i < job_recovery_commands_count; i++) SERIAL_PROTOCOLLNPAIR("> ", job_recovery_commands[i]);
        else
          for (uint8_t i = 0; i < job_recovery_info.commands_in_queue; i++) SERIAL_PROTOCOLLNPAIR("> ", job_recovery_info.command_queue[i]);
        SERIAL_PROTOCOLLNPAIR("sd_filename: ", job_recovery_info.sd_filename);
        SERIAL_PROTOCOLLNPAIR("sdpos: ", job_recovery_info.sdpos);
        SERIAL_PROTOCOLLNPAIR("print_job_elapsed: ", job_recovery_info.print_job_elapsed);
      }
      else
        SERIAL_PROTOCOLLNPGM("INVALID DATA");
    }
  }
#endif // DEBUG_POWER_LOSS_RECOVERY

/**
 * Check for Print Job Recovery
 * If the file has a saved state, populate the job_recovery_commands queue
 */
void do_print_job_recovery() {
  //if (job_recovery_commands_count > 0) return;
  memset(&job_recovery_info, 0, sizeof(job_recovery_info));
  //ZERO(job_recovery_commands);
	
	// SERIAL_PROTOCOLLNPAIR("Ressurect file: ", card.getRessurectFileName());
	
  if (!card.cardOK) card.initsd();

  if (card.cardOK) {

    #if ENABLED(DEBUG_POWER_LOSS_RECOVERY)
      SERIAL_PROTOCOLLNPAIR("Init job recovery info. Size: ", (int)sizeof(job_recovery_info));
    #endif

    if (card.jobRecoverFileExists()) {
      card.openJobRecoveryFile(true);
      card.loadJobRecoveryInfo();
      card.closeJobRecoveryFile();
      card.removeJobRecoveryFile();

			job_recovery_found = true;
			
			char cmd[40], temp[16];
			
			card.openFile(card.getRessurectFileName(), false);

			sprintf_P(cmd, PSTR("M117 Restarting %s"), job_recovery_info.sd_filename);
			card.write_command(cmd);
			
			#if HAS_HEATED_BED
				// Restore the bed temperature
				sprintf_P(cmd, PSTR("M190 S%i"), job_recovery_info.target_temperature_bed);
				card.write_command(cmd);
			#endif

			// Restore all hotend temperatures
			HOTEND_LOOP() {
				sprintf_P(cmd, PSTR("M109 S%i"), job_recovery_info.target_temperature[e]);
				card.write_command(cmd);
			}

			// Restore print cooling fan speeds
			for (uint8_t i = 0; i < FAN_COUNT; i++) {
				sprintf_P(cmd, PSTR("M106 P%i S%i"), i, job_recovery_info.fanSpeeds[i]);
				card.write_command(cmd);
			}
				
			#if HAS_LEVELING
				strcpy_P(cmd, PSTR("M420 S0 Z0"));            // Leveling off before G92 or G28
				card.write_command(cmd);
			#endif
			
			strcpy_P(cmd, PSTR("G28"));
			card.write_command(cmd);
			
			#if HAS_LEVELING
				strcpy_P(cmd, PSTR("M420 S1"));               // Leveling on
				card.write_command(cmd);
			#endif
			
			//dtostrf(job_recovery_info.current_position[Z_AXIS] + 5, 1, 3, temp);
			//sprintf_P(cmd, PSTR("G1 Z%s"), temp);
			fmtSaveLine(cmd, PSTR("G1 Z%s"), ftostr33s, job_recovery_info.current_position[Z_AXIS] + 5);
			card.write_command(cmd);
			
			strcpy_P(cmd, PSTR("G92 E0"));
			card.write_command(cmd);
			
			strcpy_P(cmd, PSTR("G1 F200 E5"));
			card.write_command(cmd);
			
			strcpy_P(cmd, PSTR("G92 F200 E5"));
			card.write_command(cmd);
			
			//dtostrf(job_recovery_info.current_position[E_AXIS], 1, 3, temp);
			//sprintf_P(cmd, PSTR("G92 E%s"), temp);
			fmtSaveLine(cmd, PSTR("G92 E%s"), ftostr53s, job_recovery_info.current_position[E_AXIS]);
			card.write_command(cmd);
			
			//dtostrf(job_recovery_info.current_position[X_AXIS], 1, 3, temp);
			//sprintf_P(cmd, PSTR("G1 F1200 X%s"), temp);
			fmtSaveLine(cmd, PSTR("G1 F1200 X%s"), ftostr33s, job_recovery_info.current_position[X_AXIS]);
			card.write_command(cmd);
			
			//dtostrf(job_recovery_info.current_position[Y_AXIS], 1, 3, temp);
			//sprintf_P(cmd, PSTR("G1 Y%s"), temp);
			fmtSaveLine(cmd, PSTR("G1 Y%s"), ftostr33s, job_recovery_info.current_position[Y_AXIS]);
			card.write_command(cmd);
			
			fmtSaveLine(cmd, PSTR("G1 Z%s"), ftostr33s, job_recovery_info.current_position[Z_AXIS]);
			card.write_command(cmd);
			
			/*
			dtostrf(job_recovery_info.current_position[Z_AXIS], 1, 3, temp);
			sprintf_P(cmd, PSTR("G1 Z%s"), temp);
			*/
			
			uint8_t r = job_recovery_info.cmd_queue_index_r;
			while (job_recovery_info.commands_in_queue) {
				strcpy(cmd, job_recovery_info.command_queue[r]);
				card.write_command(cmd);
				
				job_recovery_info.commands_in_queue--;
				r = (r + 1) % BUFSIZE;
			}
			
		
			sprintf_P(cmd, PSTR("M23 %s"), job_recovery_info.sd_filename);  //opens a file for reading from the SD card
			card.write_command(cmd);
			
			sprintf_P(cmd, PSTR("M26 S%lu"), job_recovery_info.sdpos);
			card.write_command(cmd);
			
			strcpy_P(cmd, PSTR("M24"));
			card.write_command(cmd);

			/* sprintf_P(cmd, PSTR("M30 %s"), card.getRessurectFileName()); 
			card.write_command(cmd); */
			
			card.closefile();
			
				
		} else {
			if (job_recovery_info.valid_head != job_recovery_info.valid_foot)
				LCD_ALERTMESSAGEPGM("INVALID DATA");
			memset(&job_recovery_info, 0, sizeof(job_recovery_info));
		}
	}
}


/**
 * Save the current machine state to the "bin" file
 */
void save_job_recovery_info() {
  #if SAVE_INFO_INTERVAL_MS > 0
    static millis_t next_save_ms; // = 0;  // Init on reset
    millis_t ms = millis();
  #endif
  if (
    #if SAVE_INFO_INTERVAL_MS > 0
      ELAPSED(ms, next_save_ms) ||
    #endif
    #if ENABLED(SAVE_EACH_CMD_MODE)
      true
    #else
      (current_position[Z_AXIS] > 0 && current_position[Z_AXIS] > job_recovery_info.current_position[Z_AXIS])
    #endif
  ) {
    #if SAVE_INFO_INTERVAL_MS > 0
      next_save_ms = ms + SAVE_INFO_INTERVAL_MS;
    #endif

    // Head and foot will match if valid data was saved
    if (!++job_recovery_info.valid_head) ++job_recovery_info.valid_head; // non-zero in sequence
    job_recovery_info.valid_foot = job_recovery_info.valid_head;

    // Machine state
    COPY(job_recovery_info.current_position, current_position);
    job_recovery_info.feedrate = feedrate_mm_s;
    COPY(job_recovery_info.target_temperature, thermalManager.target_temperature);
    #if HAS_HEATED_BED
      job_recovery_info.target_temperature_bed = thermalManager.target_temperature_bed;
    #endif
    COPY(job_recovery_info.fanSpeeds, fanSpeeds);

    #if HAS_LEVELING
      job_recovery_info.leveling = planner.leveling_active;
      job_recovery_info.fade = (
        #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
          planner.z_fade_height
        #else
          0
        #endif
      );
    #endif

    // Commands in the queue
    job_recovery_info.cmd_queue_index_r = cmd_queue_index_r;
    job_recovery_info.commands_in_queue = commands_in_queue;
    
		COPY(job_recovery_info.command_queue, command_queue);

    // Elapsed print job time
    job_recovery_info.print_job_elapsed = print_job_timer.duration() * 1000UL;

    // SD file position
    card.getAbsFilename(job_recovery_info.sd_filename);
    job_recovery_info.sdpos = card.getIndex();

    #if ENABLED(DEBUG_POWER_LOSS_RECOVERY)
      SERIAL_PROTOCOLLNPGM("Saving job_recovery_info");
      debug_print_job_recovery(false);
    #endif

    card.openJobRecoveryFile(false);
    (void)card.saveJobRecoveryInfo();
  }
}

#endif // POWER_LOSS_RECOVERY
