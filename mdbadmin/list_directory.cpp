// list_directory.cpp
//
// List the CallCommander Phone Number Directory.
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

#include <stdlib.h>
#include <vector>

#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtSql/QSqlDatabase>
#include <Qt3Support/q3filedialog.h>
#include <QtGui/QApplication>
#include <QtGui/QResizeEvent>
#include <QtSql/QSqlQuery>
#include <QtGui/QMessageBox>

#include <math.h>

#include "list_directory.h"
#include "edit_directory.h"
#include "add_directory.h"
#include "purge_directory.h"
#include "name_sub.h"

using namespace std;

ListDirectory::ListDirectory(QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  list_parent=parent;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());

  setCaption("Call Administrator - Phone Number Directory");

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);
  QFont small_font=QFont("Helvetica",10,QFont::Normal);
  small_font.setPixelSize(10);

  //
  // Progress Dialog
  //
  list_progress_bar=new Q3ProgressDialog(this,"list_progress_bar",false,
				 Qt::WStyle_Customize|Qt::WStyle_DialogBorder);
  list_progress_bar->setTotalSteps(10);
  list_progress_bar->setCaption("");
  list_progress_bar->setCancelButton(NULL);

  //
  // Directory List
  //
  list_directory_list=new Q3ListView(this,"list_directory_list");
  list_directory_list->setAllColumnsShowFocus(true);
  connect(list_directory_list,
	  SIGNAL(doubleClicked(Q3ListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(doubleClickedData(Q3ListViewItem *,const QPoint &,int)));
  list_directory_list->addColumn("Number");
  list_directory_list->setColumnAlignment(0,Qt::AlignCenter);
  list_directory_list->addColumn("City");
  list_directory_list->setColumnAlignment(1,Qt::AlignLeft|Qt::AlignVCenter);
  list_directory_list->addColumn("State");
  list_directory_list->setColumnAlignment(2,Qt::AlignLeft|Qt::AlignVCenter);
  list_directory_list->addColumn("Cellphone");
  list_directory_list->setColumnAlignment(3,Qt::AlignCenter);
  list_directory_list->addColumn("Custom");
  list_directory_list->setColumnAlignment(4,Qt::AlignCenter);

  //
  //  Add Button
  //
  list_add_button=new QPushButton(this,"list_add_button");
  list_add_button->setFont(font);
  list_add_button->setText("&Add");
  connect(list_add_button,SIGNAL(clicked()),this,SLOT(addData()));

  //
  //  Edit Button
  //
  list_edit_button=new QPushButton(this,"list_edit_button");
  list_edit_button->setFont(font);
  list_edit_button->setText("&Edit");
  connect(list_edit_button,SIGNAL(clicked()),this,SLOT(editData()));

  //
  //  Delete Button
  //
  list_delete_button=new QPushButton(this,"list_delete_button");
  list_delete_button->setFont(font);
  list_delete_button->setText("&Delete");
  connect(list_delete_button,SIGNAL(clicked()),this,SLOT(deleteData()));

  //
  //  Import Button
  //
  list_import_button=new QPushButton(this,"list_import_button");
  list_import_button->setFont(font);
  list_import_button->setText("&Import");
  connect(list_import_button,SIGNAL(clicked()),this,SLOT(importData()));

  //
  //  Purge Button
  //
  list_purge_button=new QPushButton(this,"list_purge_button");
  list_purge_button->setFont(font);
  list_purge_button->setText("&Purge");
  connect(list_purge_button,SIGNAL(clicked()),this,SLOT(purgeData()));

  //
  //  Close Button
  //
  list_close_button=new QPushButton(this,"list_close_button");
  list_close_button->setDefault(true);
  list_close_button->setFont(font);
  list_close_button->setText("&Close");
  connect(list_close_button,SIGNAL(clicked()),this,SLOT(closeData()));

  RefreshList();
}


ListDirectory::~ListDirectory()
{
}


QSize ListDirectory::sizeHint() const
{
  return QSize(600,480);
} 


QSizePolicy ListDirectory::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ListDirectory::addData()
{
  QString num;
  QString sql;
  QSqlQuery *q;

  AddDirectory *add=new AddDirectory(&num,this,"add");
  if(add->exec()==0) {
    sql=QString().sprintf("insert into DIRECTORY set NUMBER=\"%s\"",
			  (const char *)num);
    q=new QSqlQuery(sql);
    delete q;
    EditDirectory *edit=new EditDirectory(num,this,"edit");
    if(edit->exec()==0) {
      Q3ListViewItem *item=new Q3ListViewItem(list_directory_list);
      item->setText(0,num);
      UpdateItem(item);
      list_directory_list->setSelected(item,true);
      list_directory_list->ensureItemVisible(item);
    }
    else {
      sql=QString().sprintf("delete from DIRECTORY where NUMBER=\"%s\"",
			    (const char *)num);
      q=new QSqlQuery(sql);
      delete q;
    }
    delete edit;
  }
  delete add;
}


void ListDirectory::editData()
{
  Q3ListViewItem *item=list_directory_list->selectedItem();
  
  if(item==NULL) {
    return;
  }
  EditDirectory *edit=new EditDirectory(item->text(0),this,"edit");
  if(edit->exec()==0) {
    UpdateItem(item);
  }
  delete edit;
}


void ListDirectory::deleteData()
{
  QString sql;
  QSqlQuery *q;
  Q3ListViewItem *item=list_directory_list->selectedItem();

  if(item==NULL) {
    return;
  }
  if(QMessageBox::question(this,"Delete Number",
	 QString().sprintf("Are you sure you want to delete the entry for %s",
			   (const char *)item->text(0)),
			   QMessageBox::Yes,
			   QMessageBox::No)==QMessageBox::No) {
    return;
  }
  sql=QString().sprintf("delete from DIRECTORY where NUMBER=\"%s\"",
			(const char *)item->text(0));
  q=new QSqlQuery(sql);
  delete q;
  delete item;
}


void ListDirectory::importData()
{
  QString startdir("/");
  if(getenv("HOME")!=NULL) {
    startdir=getenv("HOME");
  }
    
  QString filename=Q3FileDialog::getOpenFileName(startdir,
						"NANPA Data Files (*.txt)",
						this,
						"filename","Open NANPA Data");
  if(filename.isEmpty()) {
    return;
  }
  if(ImportNanpaData(filename)) {
    RefreshList();
  }
}


void ListDirectory::purgeData()
{
  PurgeDirectory *purge=new PurgeDirectory(this,"purge");
  if(purge->exec()==0) {
    RefreshList();
  }
  delete purge;
}


void ListDirectory::doubleClickedData(Q3ListViewItem *item,const QPoint &pt,
				      int c)
{
  editData();
}


void ListDirectory::closeData()
{
  done(0);
}


void ListDirectory::resizeEvent(QResizeEvent *e)
{
  list_directory_list->setGeometry(10,10,size().width()-20,size().height()-80);
  list_add_button->setGeometry(10,size().height()-60,80,50);
  list_edit_button->setGeometry(100,size().height()-60,80,50);
  list_delete_button->setGeometry(190,size().height()-60,80,50);
  list_import_button->setGeometry(310,size().height()-60,80,50);
  list_purge_button->setGeometry(400,size().height()-60,80,50);
  list_close_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


void ListDirectory::RefreshList()
{
  QSqlQuery *q;
  Q3ListViewItem *item;
  int line_step;
  int step=0;
  int lines=0;

  list_directory_list->clear();
  q=new QSqlQuery("select NUMBER,CITY,STATE,CELLPHONE,CUSTOM from DIRECTORY\
                   order by NUMBER desc");
  list_progress_bar->setLabelText("Sorting Records");
  line_step=q->size()/10;
  while (q->next()) {
    item=new Q3ListViewItem(list_directory_list);
    item->setText(0,q->value(0).toString());
    item->setText(1,q->value(1).toString());
    item->setText(2,q->value(2).toString());
    if(q->value(3).toString()=="Y") {
      item->setText(3,"Yes");
    }
    else {
      item->setText(3,"No");
    }
    if(q->value(4).toString()=="Y") {
      item->setText(4,"Yes");
    }
    else {
      item->setText(4,"No");
    }
    if(lines++>line_step) {
      list_progress_bar->setProgress(++step);
      qApp->processEvents();
      lines=0;
    }
  }
  delete q;
  list_progress_bar->setProgress(10);
}


void ListDirectory::UpdateItem(Q3ListViewItem *item)
{
  QSqlQuery *q=new 
    QSqlQuery(QString().sprintf("select CITY,STATE,CELLPHONE,CUSTOM \
                                 from DIRECTORY where NUMBER=\"%s\"",
				(const char *)item->text(0)));
  if(q->first()) {
    item->setText(1,q->value(0).toString());
    item->setText(2,q->value(1).toString());
    if(q->value(2).toString()=="Y") {
      item->setText(3,"Yes");
    }
    else {
      item->setText(3,"No");
    }
    if(q->value(3).toString()=="Y") {
      item->setText(4,"Yes");
    }
    else {
      item->setText(4,"No");
    }
  }
  delete q;
}


bool ListDirectory::ImportNanpaData(QString filename)
{
  QString sql;
  QSqlQuery *q;
  char buf[1024];
  QString str;
  int npa_pos;
  int nxx_pos;
  int rate_pos;
  int state_pos;
  int company_pos;
  char npa_str[4];
  char nxx_str[4];
  char rate_str[13];
  char state_str[3];
  char company_str[61];
  QString city;
  int n;
  int lines=0;
  int line_step;
  int step=0;
  vector<QString> wireless_names;
  NameSub name_sub;
  QString cellflag;
  QString raw_city;

  //
  // Load Wireless Names
  //
  sql="select NAME from WIRELESS_NAMES";
  q=new QSqlQuery(sql);
  while(q->next()) {
    wireless_names.push_back(q->value(0).toString());    
  }
  delete q;

  //
  // Load City Names
  //
  sql="select NAME,FULL_NAME from CITY_NAMES";
  q=new QSqlQuery(sql);
  while(q->next()) {
    name_sub.addValue(q->value(0).toString(),q->value(1).toString());
  }
  delete q;

  //
  // Open the file
  //
  FILE *file=fopen((const char *)filename,"r");
  if(file==NULL) {
    QMessageBox::warning(this,"Import Failed",
			 QString().sprintf("Unable to open file %s!",
					   (const char *)filename));
    return false;
  }

  //
  // Count Lines
  //
  while(fgets(buf,1024,file)!=NULL) {
    lines++;
  }
  fseek(file,0,SEEK_SET);
  line_step=lines/10;

  //
  // Determine the format
  //
  if(fgets(buf,1024,file)==NULL) {
    QMessageBox::warning(this,"Import Failed","Data file contains no data!");
    return false;
  }
  str=QString(buf);
  if((npa_pos=str.find("NPA")-2)<0) {
    QMessageBox::warning(this,"Import Failed","Invalid file format!");
  }
  if((nxx_pos=str.find("NXX")-2)<0) {
    QMessageBox::warning(this,"Import Failed","Invalid file format!");
  }
  if((rate_pos=str.find("RateCenter")-2)<0) {
    QMessageBox::warning(this,"Import Failed","Invalid file format!");
  }
  if((state_pos=str.find("State"))<0) {
    QMessageBox::warning(this,"Import Failed","Invalid file format!");
  }
  if((company_pos=str.find("Company")-1)<0) {
    QMessageBox::warning(this,"Import Failed","Invalid file format!");
  }

  //
  // Import Data
  //
  list_progress_bar->setLabelText("Importing Records");
  list_progress_bar->setProgress(step);
  lines=0;
  while(fgets(buf,1024,file)!=NULL) {
    n=strlen(buf);

    //
    // Area Code
    //
    if(n<(npa_pos+3)) {
      QMessageBox::warning(this,"Import Failed","Unexpected end of data!");
    }
    buf[npa_pos+3]=0;
    sscanf(buf+npa_pos,"%s",npa_str);

    //
    // Exchange
    //
    if(n<(nxx_pos+3)) {
      QMessageBox::warning(this,"Import Failed","Unexpected end of data!");
      return false;
    }
    buf[nxx_pos+3]=0;
    sscanf(buf+nxx_pos,"%s",nxx_str);

    //
    // State
    //
    if(n<(state_pos+2)) {
      QMessageBox::warning(this,"Import Failed","Unexpected end of data!");
      return false;
    }
    buf[state_pos+2]=0;
    sscanf(buf+state_pos,"%s",state_str);

    //
    // City
    //
    if(n<(rate_pos+12)) {
      QMessageBox::warning(this,"Import Failed","Unexpected end of data!");
      return false;
    }
    for(int i=0;i<12;i++) {
      rate_str[i]=buf[i+rate_pos];
    }
    rate_str[12]=0;
    for(int i=11;i>=0;i--) {
      if(rate_str[i]==' ') {
	rate_str[i]=0;
      }
      else {
	i=-1;
      }
    }
    raw_city=QString(rate_str).stripWhiteSpace();
    city=raw_city;
    name_sub.value(raw_city,&city);

    //
    // Company
    //
    if(n<(company_pos+59)) {
      QMessageBox::warning(this,"Import Failed","Unexpected end of data!");
      return false;
    }
    for(int i=0;i<59;i++) {
      company_str[i]=buf[i+company_pos];
    }
    company_str[59]=0;
    for(int i=58;i>=0;i--) {
      if(company_str[i]==' ') {
	company_str[i]=0;
      }
      else {
	company_str[i]=0;
	i=-1;
      }
    }

    //
    // Set Cellphone Flag
    //
    cellflag="N";
    str=company_str;
    for(unsigned i=0;i<wireless_names.size();i++) {
      if(str.contains(wireless_names[i],false)>0) {
	cellflag="Y";
      }
    }

    //
    // Write DB Record
    //
    sql=QString().sprintf("select CUSTOM from DIRECTORY where NUMBER=\"%s%s\"",
			  npa_str,
			  nxx_str);
    q=new QSqlQuery(sql);
    if(q->first()) {
      if(q->value(0).toString()=="N") {
	delete q;
	sql=QString().sprintf("update DIRECTORY set CITY=\"%s\",STATE=\"%s\",\
                               CELLPHONE=\"%s\" where NUMBER=\"%s%s\"",
			      (const char *)city,
			      state_str,
			      (const char *)cellflag,
			      npa_str,
			      nxx_str);
	q=new QSqlQuery(sql);
      }
      delete q;
    }
    else {
      delete q;
      sql=QString().sprintf("insert into DIRECTORY set NUMBER=\"%s%s\",\
                             CITY=\"%s\",STATE=\"%s\",CELLPHONE=\"%s\"",
			    npa_str,
			    nxx_str,
			    (const char *)city,
			    state_str,
			    (const char *)cellflag);
      q=new QSqlQuery(sql);
      delete q;
    }
    if(lines++>line_step) {
      list_progress_bar->setProgress(++step);
      qApp->processEvents();
      lines=0;
    }
  }

  //
  // The Washington DC Hack
  //
  sql="update DIRECTORY set STATE=\"DC\" where (CITY=\"WASHINGTON\")\
       &&((STATE=\"VA\")||(STATE=\"MD\"))&&(CUSTOM=\"N\")";
  q=new QSqlQuery(sql);
  delete q;

  //
  // Clean Up
  //
  list_progress_bar->setProgress(10);
  fclose(file);
  return true;
}

