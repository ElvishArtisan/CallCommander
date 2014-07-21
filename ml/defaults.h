// defaults.h
//
// A Qt-based application for controlling broadcast telephone interfaces. 
//
//   (C) Copyright 2002-2003 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef DEFAULTS_H
#define DEFAULTS_H

/*
 * Default Values
 */
#define DEFAULT_CONFIG_FILE "/etc/ml.conf"

//
// The [Global] Section
//
#define DEFAULT_CONSOLE 8
#define DEFAULT_MODE "Talent"
#define DEFAULT_ANIMATE_ICONS true
#define DEFAULT_ASK_HOST true
#define DEFAULT_CAPTION "Call Commander"
#define DEFAULT_WINDOW_X 0
#define DEFAULT_WINDOW_Y 0
#define DEFAULT_SIZE_X 1000
#define DEFAULT_SIZE_Y 740
#define DEFAULT_GROUP 0
#define DEFAULT_ON_TOP false

//
// The [Debug] Section
//
#define DEFAULT_DISABLE_WATCHDOG false
#define DEFAULT_DISABLE_STATUS_SYNC false
#define DEFAULT_DISABLE_CALLER_SYNC false
#define DEFAULT_DISABLE_MESSAGE_SYNC false
#define DEFAULT_DISABLE_AUTO_DETECT true

//
// The [Logging] Section
//
#define DEFAULT_ENABLE_LOGGING true
#define DEFAULT_DEFAULT_LOG_TYPE MainWidget::Disk
#define DEFAULT_LOG_TYPE "Disk"
#define DEFAULT_SHOW_CODE ""
#define DEFAULT_LOG_FILENAME "ml%M%Y.log"
#define DEFAULT_LOG_FORMAT "%m/%d/%Y,%k:%M:%S,\"%L\",\"%n\",\"%C\",\"%A\",\"%c\",\"%t\",\"%z\",\"%w\",\"%g\",\"%p\",\"%e\",\"%r\",\"%a\",\"%h\""
#define DEFAULT_LOG_SQL_SERVER "localhost"
#define DEFAULT_LOG_SQL_DATABASE "Commander"
#define DEFAULT_LOG_SQL_USERNAME "ccuser"
#define DEFAULT_LOG_SQL_PASSWORD "letmein"

//
// The [Colors] Section
//
#define DEFAULT_IDLE_COLOR "lightGray"
#define DEFAULT_INUSE_COLOR "darkGray"
#define DEFAULT_ONAIR_COLOR "red"
#define DEFAULT_OFFHOOK_COLOR "yellow"
#define DEFAULT_ONHOLD_COLOR "green"
#define DEFAULT_SCREENEDHOLD_COLOR "green"
#define DEFAULT_READYNEXT_COLOR "green"
#define DEFAULT_RINGING_COLOR "blue"
#define DEFAULT_BUSY_COLOR "yellow"
#define DEFAULT_CALLER_COLOR "white"
#define DEFAULT_MESSAGE_COLOR "white"
#define DEFAULT_DUMP_COLOR "red"
#define DEFAULT_RECORD_COLOR "gray"
#define DEFAULT_NEXT_COLOR "gray"
#define DEFAULT_KEYPAD_COLOR "gray"
#define DEFAULT_SCREEN_COLOR "red"
#define DEFAULT_MUTE_COLOR "red"
#define DEFAULT_CONF_COLOR "red"
#define DEFAULT_VIP_COLOR "red"

//
// The [MessageBox] Section
//
#define DEFAULT_SHOW_MESSAGE_BOX true
#define DEFAULT_MESSAGE_X 175
#define DEFAULT_MESSAGE_Y 525
#define DEFAULT_MESSAGE_SIZE_X 300
#define DEFAULT_MESSAGE_SIZE_Y 165
#define DEFAULT_MESSAGE_NICKNAME ""
#define DEFAULT_MESSAGE_SCROLLBACK 100
#define DEFAULT_MESSAGE_HANG_TIME 5000
#define DEFAULT_MESSAGE_FONT_FACE "Helvetica"
#define DEFAULT_MESSAGE_FONT_SIZE 16
#define DEFAULT_MESSAGE_FONT_WEIGHT "Normal"

//
// The [Clock] Section
//
#define DEFAULT_SHOW_CLOCK true
#define DEFAULT_CLOCK_X 815
#define DEFAULT_CLOCK_Y 525
#define DEFAULT_TIME_OFFSET 0
#define DEFAULT_SHOW_HOUR true
#define DEFAULT_MILITARY_FORMAT true
#define DEFAULT_CLOCK_ORIENTATION "Vertical"

//
// The [BusyButton] Section
//
#define DEFAULT_SHOW_BUSYBUTTON true
#define DEFAULT_BUSY_X 5
#define DEFAULT_BUSY_Y 525
#define DEFAULT_BUSY_KEY "Alt+B"

//
// The [DumpButton] Section
//
#define DEFAULT_SHOW_DUMPBUTTON true
#define DEFAULT_DUMP_X 5
#define DEFAULT_DUMP_Y 610
#define DEFAULT_DUMP_KEY "Alt+D"

//
// The [RecordButton] Section
//
#define DEFAULT_SHOW_RECORDBUTTON true
#define DEFAULT_RECORD_X 90
#define DEFAULT_RECORD_Y 525
#define DEFAULT_RECORD_KEY "Alt+R"

//
// The [NextButton] Section
//
#define DEFAULT_SHOW_NEXTBUTTON true
#define DEFAULT_NEXT_X 713
#define DEFAULT_NEXT_Y 525
#define DEFAULT_NEXT_BANK 0
#define DEFAULT_NEXT_KEY "Alt+N"

//
// The [Keypad] Section
//
#define DEFAULT_SHOW_KEYPADBUTTON true
#define DEFAULT_KEYPAD_X 90
#define DEFAULT_KEYPAD_Y 610
#define DEFAULT_KEYPAD_KEY "Alt+K"

//
// The [FlashButton] Section
//
#define DEFAULT_SHOW_FLASHBUTTON false
#define DEFAULT_FLASH_X 0
#define DEFAULT_FLASH_Y 0
#define DEFAULT_FLASH_KEY "Alt+F"

//
// The [ScreenButton] Section
//
#define DEFAULT_SHOW_SCREENBUTTON false
#define DEFAULT_SCREEN_X 0
#define DEFAULT_SCREEN_Y 0
#define DEFAULT_SCREEN_KEY "Alt+S"

//
// The [MuteButton] Section
//
#define DEFAULT_SHOW_MUTEBUTTON false
#define DEFAULT_MUTE_X 0
#define DEFAULT_MUTE_Y 0
#define DEFAULT_MUTE_KEY "Alt+M"

//
// The [ConfButton] Section
//
#define DEFAULT_SHOW_CONFBUTTON false
#define DEFAULT_CONF_X 0
#define DEFAULT_CONF_Y 0
#define DEFAULT_CONF_KEY "Alt+C"

//
// The [VipButton] Section
//
#define DEFAULT_SHOW_VIPBUTTON false
#define DEFAULT_VIP_X 0
#define DEFAULT_VIP_Y 0
#define DEFAULT_VIP_KEY "Alt+V"

//
// The [MainBank] Section
//
#define DEFAULT_SHOW_MAIN_HOLDBUTTON true
#define DEFAULT_MAIN_HOLD_X 526
#define DEFAULT_MAIN_HOLD_Y 525
#define DEFAULT_SHOW_MAIN_DROPBUTTON true
#define DEFAULT_MAIN_DROP_X 526
#define DEFAULT_MAIN_DROP_Y 610
#define DEFAULT_MAIN_DROP_KEY "End"
#define DEFAULT_MAIN_HOLD_KEY "Home"

//
// The [ConfBank] Section
//
#define DEFAULT_SHOW_CONF_HOLDBUTTON true
#define DEFAULT_CONF_HOLD_X 611
#define DEFAULT_CONF_HOLD_Y 525
#define DEFAULT_SHOW_CONF_DROPBUTTON true
#define DEFAULT_CONF_DROP_X 611
#define DEFAULT_CONF_DROP_Y 610
#define DEFAULT_CONF_DROP_KEY "Shift+End"
#define DEFAULT_CONF_HOLD_KEY "Shift+Home"

//
// The [CallerBox] Section
//
#define DEFAULT_CALLER_SIZE_X 300
#define DEFAULT_CALLER_SIZE_Y 80

#define DEFAULT_CALLER_NAME true
#define DEFAULT_CALLER_NAME_X 5 
#define DEFAULT_CALLER_NAME_Y 5
#define DEFAULT_CALLER_NAME_SIZE_X 190
#define DEFAULT_CALLER_NAME_SIZE_Y 19
#define DEFAULT_CALLER_NAME_FONT_FACE "Helvetica"
#define DEFAULT_CALLER_NAME_FONT_SIZE 18
#define DEFAULT_CALLER_NAME_FONT_WEIGHT "Bold"

#define DEFAULT_CALLER_AGE false
#define DEFAULT_CALLER_AGE_X 0 
#define DEFAULT_CALLER_AGE_Y 0
#define DEFAULT_CALLER_AGE_SIZE_X 0
#define DEFAULT_CALLER_AGE_SIZE_Y 0
#define DEFAULT_CALLER_AGE_FONT_FACE "Helvetica"
#define DEFAULT_CALLER_AGE_FONT_SIZE 12
#define DEFAULT_CALLER_AGE_FONT_WEIGHT "Normal"

#define DEFAULT_CALLER_CITYSTATE true
#define DEFAULT_CALLER_CITYSTATE_X 5 
#define DEFAULT_CALLER_CITYSTATE_Y 22
#define DEFAULT_CALLER_CITYSTATE_SIZE_X 190
#define DEFAULT_CALLER_CITYSTATE_SIZE_Y 19
#define DEFAULT_CALLER_CITYSTATE_FONT_FACE "Helvetica"
#define DEFAULT_CALLER_CITYSTATE_FONT_SIZE 12
#define DEFAULT_CALLER_CITYSTATE_FONT_WEIGHT "Normal"

#define DEFAULT_CALLER_ZIP false
#define DEFAULT_CALLER_ZIP_X 0
#define DEFAULT_CALLER_ZIP_Y 0
#define DEFAULT_CALLER_ZIP_SIZE_X 0
#define DEFAULT_CALLER_ZIP_SIZE_Y 0
#define DEFAULT_CALLER_ZIP_FONT_FACE "Helvetica"
#define DEFAULT_CALLER_ZIP_FONT_SIZE 12
#define DEFAULT_CALLER_ZIP_FONT_WEIGHT "Normal"

#define DEFAULT_CALLER_PHONE false
#define DEFAULT_CALLER_PHONE_X 0 
#define DEFAULT_CALLER_PHONE_Y 0
#define DEFAULT_CALLER_PHONE_SIZE_X 0
#define DEFAULT_CALLER_PHONE_SIZE_Y 0
#define DEFAULT_CALLER_PHONE_FONT_FACE "Helvetica"
#define DEFAULT_CALLER_PHONE_FONT_SIZE 12
#define DEFAULT_CALLER_PHONE_FONT_WEIGHT "Normal"

#define DEFAULT_CALLER_STATION true
#define DEFAULT_CALLER_STATION_X 200 
#define DEFAULT_CALLER_STATION_Y 22
#define DEFAULT_CALLER_STATION_SIZE_X 60
#define DEFAULT_CALLER_STATION_SIZE_Y 19
#define DEFAULT_CALLER_STATION_FONT_FACE "Helvetica"
#define DEFAULT_CALLER_STATION_FONT_SIZE 12
#define DEFAULT_CALLER_STATION_FONT_WEIGHT "Normal"

#define DEFAULT_CALLER_GENDER true
#define DEFAULT_CALLER_GENDER_X 265
#define DEFAULT_CALLER_GENDER_Y 22
#define DEFAULT_CALLER_GENDER_SIZE_X 20
#define DEFAULT_CALLER_GENDER_SIZE_Y 19
#define DEFAULT_CALLER_GENDER_FONT_FACE "Helvetica"
#define DEFAULT_CALLER_GENDER_FONT_SIZE 12
#define DEFAULT_CALLER_GENDER_FONT_WEIGHT "Normal"

#define DEFAULT_CALLER_COMMENT true
#define DEFAULT_CALLER_COMMENT_X 5 
#define DEFAULT_CALLER_COMMENT_Y 45
#define DEFAULT_CALLER_COMMENT_SIZE_X 290
#define DEFAULT_CALLER_COMMENT_SIZE_Y 33
#define DEFAULT_CALLER_COMMENT_FONT_FACE "Helvetica"
#define DEFAULT_CALLER_COMMENT_FONT_SIZE 12
#define DEFAULT_CALLER_COMMENT_FONT_WEIGHT "Normal"

#define DEFAULT_CALLER_TIMER true
#define DEFAULT_CALLER_TIMER_X 240 
#define DEFAULT_CALLER_TIMER_Y 19
#define DEFAULT_CALLER_TIMER_FONT_FACE "Helvetica"
#define DEFAULT_CALLER_TIMER_FONT_SIZE 16
#define DEFAULT_CALLER_TIMER_FONT_WEIGHT "Normal"

#define DEFAULT_CALLER_CELL true
#define DEFAULT_CALLER_CELL_X 200
#define DEFAULT_CALLER_CELL_Y 1

#define DEFAULT_CALLER_QUALITY true
#define DEFAULT_CALLER_QUALITY_X 170
#define DEFAULT_CALLER_QUALITY_Y 1

//
// The [LineN] Sections
//
// Are defined in the 'LoadConfig()' method in 'config.cpp'.
//

#define DEFAULT_WATCHDOG_INTERVAL 10



#endif
