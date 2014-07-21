// award_data.cpp
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

#include <QtSql/QSqlQuery>

#include <escapestring.h>

#include "award_data.h"

AwardData::AwardData(const QString showcode)
{
  award_showcode=showcode;
  clear();
}


void AwardData::processString(BusDriver::Element elem,unsigned line,
			      const QString &str)
{
  AddLine(line);
  switch(elem) {
  case BusDriver::FirstNameAwardElement:
    award_first_names[line]=str;
    break;

  case BusDriver::LastNameAwardElement:
    award_last_names[line]=str;
    break;

  case BusDriver::Addr1AwardElement:
    award_address1s[line]=str;
    break;

  case BusDriver::Addr2AwardElement:
    award_address2s[line]=str;
    break;

  case BusDriver::CityAwardElement:
    award_citys[line]=str;
    break;

  case BusDriver::StateAwardElement:
    award_states[line]=str;
    break;

  case BusDriver::ZipAwardElement:
    award_zipcodes[line]=str;
    break;

  case BusDriver::PhoneAwardElement:
    award_phones[line]=str;
    break;

  case BusDriver::EmailAwardElement:
    award_emails[line]=str;
    break;

  case BusDriver::GenderAwardElement:
    award_genders[line]=str;
    break;

  case BusDriver::AgeAwardElement:
    award_ages[line]=str.toInt();
    break;

  case BusDriver::PrizeAwardElement:
    award_prize_descriptions[line]=str;
    break;

  case BusDriver::RemarksAwardElement:
    award_remarks[line]=str;
    break;

  case BusDriver::NumberAwardElement:
    award_numbers[line]=str;
    break;

  case BusDriver::UsernameAwardElement:
    award_usernames[line]=str;
    break;

  case BusDriver::DetailIdAwardElement:
    award_detail_ids[line]=str.toUInt();
    break;

  case BusDriver::CommitAwardElement:
    WriteAward(line);
    clear(line);
    break;

  default:
    break;
  }
}


void AwardData::clear()
{
  award_first_names.clear();
  award_last_names.clear();
  award_address1s.clear();
  award_address2s.clear();
  award_citys.clear();
  award_states.clear();
  award_zipcodes.clear();
  award_phones.clear();
  award_emails.clear();
  award_genders.clear();
  award_prize_descriptions.clear();
  award_remarks.clear();
  award_numbers.clear();
  award_usernames.clear();
  award_detail_ids.clear();
  award_ages.clear();
}


void AwardData::clear(unsigned line)
{
  AddLine(line);
  if(line<award_first_names.size()) {
    award_first_names[line]="";
  }
  if(line<award_last_names.size()) {
    award_last_names[line]="";
  }
  if(line<award_address1s.size()) {
    award_address1s[line]="";
  }
  if(line<award_address2s.size()) {
    award_address2s[line]="";
  }
  if(line<award_citys.size()) {
    award_citys[line]="";
  }
  if(line<award_states.size()) {
    award_states[line]="";
  }
  if(line<award_zipcodes.size()) {
    award_zipcodes[line]="";
  }
  if(line<award_phones.size()) {
    award_phones[line]="";
  }
  if(line<award_emails.size()) {
    award_emails[line]="";
  }
  if(line<award_genders.size()) {
    award_genders[line]="";
  }
  if(line<award_prize_descriptions.size()) {
    award_prize_descriptions[line]="";
  }
  if(line<award_remarks.size()) {
    award_remarks[line]="";
  }
  if(line<award_numbers.size()) {
    award_numbers[line]="";
  }
  if(line<award_usernames.size()) {
    award_usernames[line]="";
  }
  if(line<award_detail_ids.size()) {
    award_detail_ids[line]=0;
  }
  if(line<award_ages.size()) {
    award_ages[line]=0;
  }
}


 void AwardData::WriteAward(unsigned line)
{
  QString sql;
  QSqlQuery *q;

  AddLine(line);
  /*
  printf("showcode: %s\n",(const char *)award_showcode);
  printf("detail_id: %u\n",award_detail_ids[line]);
  printf("number: %s\n",(const char *)award_numbers[line]);
  printf("first_name: %s\n",(const char *)award_first_names[line]);
  printf("last_name: %s\n",(const char *)award_last_names[line]);
  printf("gender: %s\n",(const char *)award_genders[line]);
  printf("emails: %s\n",(const char *)award_emails[line]);
  printf("phone: %s\n",(const char *)award_phones[line]);
  printf("address1: %s\n",(const char *)award_address1s[line]);
  printf("address2: %s\n",(const char *)award_address2s[line]);
  printf("city: %s\n",(const char *)award_citys[line]);
  printf("state: %s\n",(const char *)award_states[line]);
  printf("zipcode: %s\n",(const char *)award_zipcodes[line]);
  printf("prize_description: %s\n",(const char *)award_prize_descriptions[line]);
  printf("remark: %s\n",(const char *)award_remarks[line]);
  printf("username: %s\n",(const char *)award_usernames[line]);
  */
  sql=QString().sprintf("insert into WINNERS set \
                         SHOW_CODE=\"%s\",\
                         DETAIL_ID=%u,\
                         NUMBER=\"%s\",\
                         FIRST_NAME=\"%s\",\
                         LAST_NAME=\"%s\",\
                         GENDER=\"%s\",\
                         AGE=%d,\
                         EMAIL=\"%s\",\
                         PHONE=\"%s\",\
                         ADDRESS1=\"%s\",\
                         ADDRESS2=\"%s\",\
                         CITY=\"%s\",\
                         STATE=\"%s\",\
                         ZIPCODE=\"%s\",\
                         PRIZE_DESCRIPTION=\"%s\",\
                         REMARKS=\"%s\",\
                         ORIGIN_LOGIN_NAME=\"%s\",\
                         ORIGIN_DATETIME=now()",
			(const char *)award_showcode,
                        award_detail_ids[line],
			(const char *)EscapeString(award_numbers[line])
			.toAscii(),
			(const char *)EscapeString(award_first_names[line])
			.toAscii(),
			(const char *)EscapeString(award_last_names[line])
			.toAscii(),
			(const char *)EscapeString(award_genders[line])
			.toAscii(),
			award_ages[line],
			(const char *)EscapeString(award_emails[line])
			.toAscii(),
			(const char *)EscapeString(award_phones[line])
			.toAscii(),
			(const char *)EscapeString(award_address1s[line])
			.toAscii(),
			(const char *)EscapeString(award_address2s[line])
			.toAscii(),
			(const char *)EscapeString(award_citys[line])
			.toAscii(),
			(const char *)EscapeString(award_states[line])
			.toAscii(),
			(const char *)EscapeString(award_zipcodes[line])
			.toAscii(),
			(const char *)
			EscapeString(award_prize_descriptions[line])
			.toAscii(),
			(const char *)EscapeString(award_remarks[line])
			.toAscii(),
			(const char *)EscapeString(award_usernames[line])
			.toAscii());
  q=new QSqlQuery(sql);
  delete q;
}


void AwardData::AddLine(unsigned line)
{
  for(unsigned i=award_first_names.size();i<=line;i++) {
    award_first_names.push_back(QString(""));
    award_last_names.push_back(QString(""));
    award_address1s.push_back(QString(""));
    award_address2s.push_back(QString(""));
    award_citys.push_back(QString(""));
    award_states.push_back(QString(""));
    award_zipcodes.push_back(QString(""));
    award_phones.push_back(QString(""));
    award_emails.push_back(QString(""));
    award_genders.push_back(QString(""));
    award_prize_descriptions.push_back(QString(""));
    award_remarks.push_back(QString(""));
    award_numbers.push_back(QString(""));
    award_detail_ids.push_back(0);
    award_ages.push_back(0);
    award_usernames.push_back(QString(""));
  }
}
