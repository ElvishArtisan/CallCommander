// number_list.cpp
//
// A Container Class for CallCommander Redialler Number Entries.
//
//   (C) Copyright 2007 Fred Gleason <fredg@paravelsystems.com>
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

#include <stdlib.h>
#include <stdio.h>

#include "mlprofile.h"
#include "number_list.h"
#include "ml.h"

NumberList::NumberList()
{
  list_max_size=ML_MAX_REDIAL_ENTRIES;
#ifdef WIN32
  // Win32 Stuff Here
#else
  if(getenv("HOME")!=NULL) {
    list_filename=QString().sprintf("%s/%s/%s",getenv("HOME"),
				    ML_CONFIG_DIR,ML_NUMBER_LIST_FILENAME);
  }
#endif  // WIN32
  LoadNumbers();
}


unsigned NumberList::size() const
{
  return list_numbers.size();
}


void NumberList::setMaxSize(int size)
{
  list_max_size=size;
  TrimList();
}


QString NumberList::number(int n) const
{
  return list_numbers[n];
}


void NumberList::pushNumber(const QString &number)
{
  //
  // Add Number
  //
  std::vector<QString>::iterator it=list_numbers.begin();
  list_numbers.insert(it,1,number);

  //
  // Remove Previous Entry
  //
  for(unsigned i=1;i<list_numbers.size();i++) {
    if(number==list_numbers[i]) {
      std::vector<QString>::iterator it=list_numbers.begin()+i;
      list_numbers.erase(it,it+1);
    }
  }

  //
  // Trim List and Save
  //
  TrimList();
  SaveNumbers();
}


void NumberList::LoadNumbers()
{
  QString str;
  bool ok=false;
  MLProfile *profile=new MLProfile();
  profile->setSource(list_filename);
  for(unsigned i=0;i<list_max_size;i++) {
    str=profile->
      stringValue("RedialList",QString().sprintf("Number%d",i+1),"",&ok);
    if(ok) {
      list_numbers.push_back(str);
    }
  }
  delete profile;
}


void NumberList::SaveNumbers() const
{
  FILE *f=fopen(list_filename,"w");
  if(f==NULL) {
    return;
  }
  fprintf(f,"[RedialList]\n");
  for(unsigned i=0;i<list_numbers.size();i++) {
    fprintf(f,"Number%d=%s\n",i+1,(const char *)list_numbers[i]);
  }
  fclose(f);
}


void NumberList::TrimList()
{
  if(list_numbers.size()>list_max_size) {
    std::vector<QString>::iterator it=list_numbers.begin()+list_max_size;
    list_numbers.erase(it,it+list_numbers.size()-list_max_size);
  }
}
