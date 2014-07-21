// ml.h
//
// System-Wide Values for CallCommander.
//
//   (C) Copyright 2002-2004 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef ML_H
#define ML_H

/*
 * Current Database Version
 */
#define ML_VERSION_DATABASE 21

/*
 * Default Configuration Directory
 */
#define ML_CONFIG_DIR ".callcommander"

/*
 * Default Connection File Extension
 */
#define ML_CONNECTION_EXTENSION "ccc"

/*
 * Registry Keys
 */
#define ML_REGISTRY_PATH "/SalemRadioLabs/"
#define ML_REGISTRY_INSTALL_DIR "/InstallDir"
#define ML_REGISTRY_CONNECTION_DIR "/Connections"
#define ML_REGISTRY_NUMBERS_DIR "/Numbers"

/*
 * Default mld(8) Configuration File
 */
#define MLD_CONF_FILE "/etc/mld.conf"

//
// mld(8) PID File
//
#define MLD_PID_FILE "/var/run/mld.pid"

//
// mcidmd(8) PID File
//
#define MCIDMD_PID_FILE "/var/run/mcidmd.pid"

//
// mlmld(8) PID File
//
#define MLMLD_PID_FILE "/var/run/mlmld.pid"

//
// Maximum Studio Groups
//
#define MAX_GROUPS 8

//
// Maximum Incoming Lines
//
#define MAX_LINES 12

//
// Maximum Desktop Directors
//
#define ML_MAX_CONSOLES 8
#define MAX_DIRECTORS 8

//
// Maximum Banks
//
#define MAX_BANKS 4

//
// Maximum CallerId Sources per Host
//
#define ML_MAX_CALLERID_SOURCES 8

/*
 * Default mySQL Settings
 */
#define DEFAULT_MYSQL_HOSTNAME "localhost"
#define DEFAULT_MYSQL_DATABASE "CallCommander"
#define DEFAULT_MYSQL_USERNAME "mluser"
#define DEFAULT_MYSQL_PASSWORD "letmein"
#define DEFAULT_MYSQL_DBTYPE "QMYSQL3"

//
// CallCommander Virtual Protocol Values
//
#define ML_VIRTUAL_TCP_PORT 5299
#define ML_VIRTUAL_MAX_MSG_LENGTH 1024

//
// Default Show Code
//
#define ML_DEFAULT_SHOW_CODE "TST"
#define ML_DEFAULT_SHOW_TITLE "Test Sample Show"

//
// Default User Name
//
#define ML_DEFAULT_LOGIN_NAME "admin"
#define ML_DEFAULT_FULL_NAME "Call Commander Administrator"
#define ML_DEFAULT_DESCRIPTION_NAME "Default CallCommander User"

//
// Default Kill Device
//
#define ML_DEFAULT_KILL_DEVICE_TYPE "Producer"
#define ML_DEFAULT_KILL_DEVICE_CONSOLE -1

//
// Default Refresh Interval
// 
#define ML_DEFAULT_DETAILS_REFRESH_INTERVAL 10

//
// Maximum MessageBox Message Length
#define ML_MAX_MESSAGE_LENGTH 64

#define MAX_ARGS 10
#define MAX_LENGTH 256
#define RESTART_INTERVAL 500

//
// Send Client Keepalives Once This Many millisecs
//
#define ML_SEND_CLIENT_KEEPALIVE_INTERVAL 5000

//
// Clear Client Table Entries After Missing Keepalives For This Many secs
//
#define ML_MAX_CLIENT_KEEPALIVE_INTERVAL 15

//
// UDP Port for mcidmd(8) Reload Packets
//
#define ML_MCIDMD_RELOAD_PORT 5088

//
// UDP Port for mld(8) Reload Packets
//
#define ML_MLD_RELOAD_PORT 5089

//
// UDP Port for mlmld(8) Reload Packets
//
#define ML_MLMLD_RELOAD_PORT 5090

//
// Default Max Redialer Entries
//
#define ML_MAX_REDIAL_ENTRIES 10

//
// Redialer Number List Filename
//
#define ML_NUMBER_LIST_FILENAME "dialed_numbers"

//
// Logic Module File Extension
//
#define ML_MLM_EXTENSION "mlm"

//
// Number to indicate blocked Caller ID
//
#define ML_CALLERID_NULL_NUMBER "0000000000"

//
// Asterisk Stuff
//
#define ML_DEFAULT_AMI_TCP_PORT 5038

//
// "Continuation" nick
//
#define ML_CONTINUATION_NICK "cOnT"

#endif  // ML_H
