// award_data.h
//
// A Container Class for CallCommander Award Data.
//
//   (C) Copyright 2011 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef AWARD_DATA_H
#define AWARD_DATA_H

#include <vector>

#include <QtCore/QString>

#include <bus_driver.h>

class AwardData
{
 public:
  AwardData(const QString showcode);
  void processString(BusDriver::Element elem,unsigned line,const QString &str);
  void clear();
  void clear(unsigned line);

 private:
  void WriteAward(unsigned line);
  void AddLine(unsigned line);
  QString award_showcode;
  std::vector<QString> award_first_names;
  std::vector<QString> award_last_names;
  std::vector<QString> award_address1s;
  std::vector<QString> award_address2s;
  std::vector<QString> award_citys;
  std::vector<QString> award_states;
  std::vector<QString> award_zipcodes;
  std::vector<QString> award_phones;
  std::vector<QString> award_emails;
  std::vector<QString> award_genders;
  std::vector<QString> award_prize_descriptions;
  std::vector<QString> award_remarks;
  std::vector<QString> award_numbers;
  std::vector<QString> award_usernames;
  std::vector<unsigned> award_detail_ids;
  std::vector<int> award_ages;
};


#endif  // AWARD_DATA_H
