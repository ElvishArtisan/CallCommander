/* mlm_dump_calls.c
 *
 * A sample CallCommander Logic Module
 *
 *   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * This is a sample CallCommander Logic Module.  It will cause the
 * connected phone system to hang up on all unlocked calls currently
 * on the air whenever the DUMP button is pressed (either on a control
 * surface or in CallCommander).
 *
 * To compile this module, just do:
 * 
 *   gcc -shared -o mlm_dump_calls.mlm mlm_dump_calls.c
 */

#include <stdio.h>
#include <mlm/mlm.h>


void MLMDumpStateChanged(int state)
{
  if(state) {
    MLMDrop(0);
    MLMDrop(1);
    MLMDrop(2);
    MLMDrop(3);
  }
}
