// globals.h
//
// Global Variables for the CallCommander Logic Module Host
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as
//   published by the Free Software Foundation; either version 2 of
//   the License, or (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>

#include <Qt3Support/q3socketdevice.h>
#include <QtCore/QTimer>

#include <mlttydevice.h>

#include <bus_driver.h>
#include <mldconfig.h>
#include <vguest.h>
#include <mlm.h>

//
// Functions
//
void SigHandler(int signum);
int GetVGuestIndex(int handle);

//
// Variables
//
extern MldConfig *mlm_config;
extern BusDriver *global_bus_driver;
extern bool global_shutdown_flag;
extern Q3SocketDevice *global_send_socket;
extern std::vector<Q3SocketDevice *> global_receive_sockets;
extern std::vector<MLTTYDevice *> global_tty_devices;
extern std::vector<VGuest *> global_vguests;
extern QTimer *global_callback_timers[MLM_MAX_TIMERS];
extern QTimer *global_receive_timer;
extern char global_module_args[256];
extern QString global_show;
extern int global_show_state;
extern int global_console;
extern int global_console_mode;
extern int global_lstate[MAX_LINES];
extern int global_bank[MAX_LINES];
extern int global_line_console[MAX_LINES];
extern int global_watchdog_state;
extern int global_dump_state;
extern int global_busy_all_state;
extern int global_record_state;
extern int global_screen_state;
extern int global_mute_state;
extern int global_conference_state;
extern int global_vip_state;
extern int global_delay_offset;
extern bool global_init_complete;
extern bool global_hard_shutdown;
extern void (*mlm_startup_sym)(const char *);
extern void (*mlm_shutdown_sym)();
extern void (*mlm_metadata_update_requested_sym)();
extern void (*mlm_watchdog_state_changed_sym)(int);
extern void (*mlm_show_changed_sym)(const char *,int);
extern void (*mlm_console_changed_sym)(int,int);
extern void (*mlm_line_state_changed_sym)(int,int,int,int);
extern void (*mlm_dump_state_changed_sym)(int);
extern void (*mlm_record_state_changed_sym)(int);
extern void (*mlm_busy_all_state_changed_sym)(int);
extern void (*mlm_screen_state_changed_sym)(int);
extern void (*mlm_mute_state_changed_sym)(int);
extern void (*mlm_conference_state_changed_sym)(int);
extern void (*mlm_vip_state_changed_sym)(int);
extern void (*mlm_delay_offset_changed_sym)(int);
extern void (*mlm_received_string_sym)(int,int,const char *);
extern void (*mlm_caller_id_received_sym)(int,const char *);
extern void (*mlm_chat_message_received_sym)(const char *,const char *);
extern void (*mlm_caller_element_received_sym)(int,int,const char *);
extern void (*mlm_udp_received_sym)(int,const char *,int);
extern void (*mlm_serial_received_sym)(int,const char *,int);
extern void (*mlm_timer_expired_sym)(int);
extern void (*mlm_vguest_pulse_received_sym)(int,int,int,int,int,int);
extern void (*mlm_vguest_turn_on_received_sym)(int,int,int,int,int);
extern void (*mlm_vguest_turn_off_received_sym)(int,int,int,int,int);
extern void (*mlm_vguest_set_input_assign_received_sym)(int,int,int,int,int,int);
extern void (*mlm_vguest_set_input_mode_received_sym)(int,int,int,int,int);
extern void (*mlm_vguest_set_fader_level_received_sym)(int,int,int,int,int);

#endif  // GLOBALS_H
