// mlhpiinformation.cpp
//
// A Container Class for AudioScience HPI Adapter Info
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: mlhpiinformation.cpp,v 1.4 2012/06/26 21:30:02 cvs Exp $
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
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

#include <mlhpiinformation.h>


MLHPIInformation::MLHPIInformation()
{
  clear();
}


unsigned MLHPIInformation::serialNumber() const
{
  return serial_number;
}


void MLHPIInformation::setSerialNumber(unsigned num)
{
  serial_number=num;
}


unsigned MLHPIInformation::hpiMajorVersion() const
{
  return (hpi_version>>16)&0xffff;
}


unsigned MLHPIInformation::hpiMinorVersion() const
{
  return (hpi_version>>8)&0xff;
}


unsigned MLHPIInformation::hpiPointVersion() const
{
  return hpi_version&0xff;
}


uint32_t MLHPIInformation::hpiVersion() const
{
  return hpi_version;
}


void MLHPIInformation::setHpiVersion(uint32_t ver)
{
  hpi_version=ver;
}


unsigned MLHPIInformation::dspMajorVersion() const
{
  return dsp_major_version;
}


void MLHPIInformation::setDspMajorVersion(unsigned ver)
{
  dsp_major_version=ver;
}


unsigned MLHPIInformation::dspMinorVersion() const
{
  return dsp_minor_version;
}


void MLHPIInformation::setDspMinorVersion(unsigned ver)
{
  dsp_minor_version=ver;
}


char MLHPIInformation::pcbVersion() const
{
  return pcb_version;
}


void MLHPIInformation::setPcbVersion(char ver)
{
  pcb_version=ver;
}


unsigned MLHPIInformation::assemblyVersion() const
{
  return assembly_version;
}


void MLHPIInformation::setAssemblyVersion(unsigned ver)
{
  assembly_version=ver;
}


void MLHPIInformation::clear()
{
  serial_number=0;
  hpi_version=0;
  dsp_major_version=0;
  dsp_minor_version=0;
  pcb_version='0';
  assembly_version=0;
}
