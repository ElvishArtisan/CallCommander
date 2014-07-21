// list_details.cpp
//
// List CallCommander Call Details.
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

#include <stdio.h>
#include <math.h>

#include <QtGui/QPushButton>
#include <QtSql/QSqlDatabase>
#include <Qt3Support/q3filedialog.h>
#include <QtCore/qfile.h>
#include <QtGui/QMessageBox>
#include <QtCore/QTimer>
#include <QtGui/QResizeEvent>
#include <QtGui/QLabel>
#include <QtSql/QSqlQuery>

#include <mlconf.h>
#include <mldconfig.h>
#include <ml_listviewitem.h>

#include "calleriddetail.h"
#include "globals.h"
#include "list_details.h"
#include "filter_dialog.h"
#include "list_reports.h"

ListDetails::ListDetails(QString showcode,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  QString sql;
  QSqlQuery *q;
  QString title;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());

  list_showcode=showcode;
  list_max_id=-1;

  base_year=GetBaseYear();

  sql=QString().sprintf("select TITLE from SHOWS where SHOW_CODE=\"%s\"",
			(const char *)list_showcode);
  q=new QSqlQuery(sql);
  if(q->first()) {
    title=q->value(0).toString();
  }
  delete q;
  setCaption(QString().sprintf("%s - Call Details",(const char *)title));

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);
  QFont small_font=QFont("Helvetica",10,QFont::Normal);
  small_font.setPixelSize(10);

  //
  // Filter
  //
  list_filter_button=new QPushButton(this,"list_filter_button");
  list_filter_button->setFont(font);
  list_filter_button->setText("&Set\nFilter");
  connect(list_filter_button,SIGNAL(clicked()),this,SLOT(setFilterData()));
  list_filter_label=new QLabel(this,"list_filter_label");
  list_filter_label->setFont(font);
  RefreshFilter();

  //
  // Directory List
  //
  list_details_list=new DetailListView(showcode,this,"list_details_list");
  list_details_list->setAllColumnsShowFocus(true);
  list_details_list->setItemMargin(5);
  list_details_list->addColumn("DateTime");
  list_details_list->setColumnAlignment(0,Qt::AlignCenter);
  list_details_list->addColumn("Action");
  list_details_list->setColumnAlignment(1,Qt::AlignCenter);
  list_details_list->addColumn("Name");
  list_details_list->setColumnAlignment(2,Qt::AlignLeft|Qt::AlignVCenter);
  list_details_list->addColumn("Age");
  list_details_list->setColumnAlignment(3,Qt::AlignCenter);
  list_details_list->addColumn("City");
  list_details_list->setColumnAlignment(4,Qt::AlignLeft|Qt::AlignVCenter);
  list_details_list->addColumn("State");
  list_details_list->setColumnAlignment(5,Qt::AlignCenter);
  list_details_list->addColumn("ZIP Code");
  list_details_list->setColumnAlignment(6,Qt::AlignCenter);
  list_details_list->addColumn("Gender");
  list_details_list->setColumnAlignment(7,Qt::AlignCenter);
  list_details_list->addColumn("Station");
  list_details_list->setColumnAlignment(8,Qt::AlignCenter);
  list_details_list->addColumn("Quality");
  list_details_list->setColumnAlignment(9,Qt::AlignCenter);
  list_details_list->addColumn("CellPhone");
  list_details_list->setColumnAlignment(10,Qt::AlignCenter);
  list_details_list->addColumn("RingTime");
  list_details_list->setColumnAlignment(11,Qt::AlignCenter);
  list_details_list->addColumn("HoldTime");
  list_details_list->setColumnAlignment(12,Qt::AlignCenter);
  list_details_list->addColumn("AirTime");
  list_details_list->setColumnAlignment(13,Qt::AlignCenter);
  list_details_list->addColumn("Line");
  list_details_list->setColumnAlignment(14,Qt::AlignCenter);
  list_details_list->addColumn("ID Number");
  list_details_list->setColumnAlignment(15,Qt::AlignCenter);
  list_details_list->addColumn("ID Name");
  list_details_list->setColumnAlignment(16,Qt::AlignCenter);

  //
  //  Report Button
  //
  list_report_button=new QPushButton(this,"list_report_button");
  list_report_button->setFont(font);
  list_report_button->setText("&Reports");
  connect(list_report_button,SIGNAL(clicked()),
	  this,SLOT(reportsData()));

  //
  //  Dump Button
  //
  list_dump_button=new QPushButton(this,"list_dump_button");
  list_dump_button->setFont(font);
  list_dump_button->setText("&Dump\nData");
  connect(list_dump_button,SIGNAL(clicked()),
	  this,SLOT(dumpData()));

  //
  //  Close Button
  //
  list_close_button=new QPushButton(this,"list_close_button");
  list_close_button->setDefault(true);
  list_close_button->setFont(font);
  list_close_button->setText("&Close");
  connect(list_close_button,SIGNAL(clicked()),this,SLOT(closeData()));

  RefreshList();

  //
  // Refresh Timer
  //
  if(man_config->detailsRefreshInterval()>0) {
    QTimer *timer=new QTimer(this,"refresh_timer");
    connect(timer,SIGNAL(timeout()),this,SLOT(detailsRefreshData()));
    timer->start(1000*man_config->detailsRefreshInterval());
  }
}


ListDetails::~ListDetails()
{
}


QSize ListDetails::sizeHint() const
{
  return QSize(640,480);
} 


QSizePolicy ListDetails::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ListDetails::detailsRefreshData()
{
  RefreshList();
}


void ListDetails::setFilterData()
{
  FilterDialog *dialog=new FilterDialog(this,"dialog");
  if(dialog->exec()==0) {
    list_max_id=-1;
    RefreshFilter();
    RefreshList();
  }
  delete dialog;
}


void ListDetails::reportsData()
{
  ListReports *dialog=new ListReports(list_showcode,this,"dialog");
  dialog->exec();
  delete dialog;
}


void ListDetails::dumpData()
{
  QString sql;
  QSqlQuery *q;

  //
  // Open Output File
  //
  QString filename=Q3FileDialog::getOpenFileName();
  if(QFile::exists(filename)) {
    if(QMessageBox::question(this,"File Exists",
			     QString().sprintf("The file \"%s\" already exists!\nOverwrite?",(const char *)filename),QMessageBox::Yes,QMessageBox::No)==QMessageBox::No) {
      return;
    }
  }
  FILE *f=fopen(filename,"w");
  if(f==NULL) {
    QMessageBox::warning(this,"File Error",
			 QString().sprintf("Unable to open \"%s\".",
					   (const char *)filename));
    return;
  }

  //
  // Write Header
  //
  fprintf(f,"CALL_DATE|CALL_TIME|ROUTE_ACTION|NAME|AGE|CITY|STATE|ZIPCODE|GENDER|STATION|QUALITY|CELLPHONE|RING_TIMER|HOLD_TIMER|AIR_TIMER|LINE_NUMBER|NUMBER|ID_NAME\n");

  //
  // Generate Dump
  //
  sql=QString().sprintf("select CALL_DATE,CALL_TIME,ROUTE_ACTION,NAME,AGE,\
                         CITY,STATE,ZIPCODE,GENDER,STATION,QUALITY,CELLPHONE,\
                         RING_TIMER,HOLD_TIMER,AIR_TIMER,\
                         LINE_NUMBER,NUMBER,ID_NAME\
                         from DETAILS_%s where \
                         (CALL_DATE>=\"%s\")&&(CALL_DATE<=\"%s\")&&\
                         (CALL_TIME>=\"%s\")&&(CALL_TIME<=\"%s\") order by ID",
			(const char *)list_showcode,
			(const char *)start_datetime.date().
			toString("yyyy-MM-dd"),
			(const char *)end_datetime.date().
			toString("yyyy-MM-dd"),
			(const char *)start_datetime.time().
			toString("hh:mm:ss"),
			(const char *)end_datetime.time().
			toString("hh:mm:ss"));
  q=new QSqlQuery(sql);
  while(q->next()) {
    fprintf(f,"%s|",(const char *)q->value(0).toDate().toString("MM-dd-yyyy"));
    fprintf(f,"%s|",(const char *)q->value(1).toTime().toString("hh:mm:ss"));
    fprintf(f,"%s|",(const char *)q->value(2).toString());
    fprintf(f,"%s|",(const char *)q->value(3).toString());
    fprintf(f,"%s|",(const char *)q->value(4).toString());
    fprintf(f,"%s|",(const char *)q->value(5).toString());
    fprintf(f,"%s|",(const char *)q->value(6).toString());
    fprintf(f,"%s|",(const char *)q->value(7).toString());
    fprintf(f,"%s|",(const char *)q->value(8).toString());
    fprintf(f,"%s|",(const char *)q->value(9).toString());
    fprintf(f,"%s|",(const char *)q->value(10).toString());
    fprintf(f,"%s|",(const char *)q->value(11).toString());
    fprintf(f,"%d|",q->value(12).toInt()/1000);
    fprintf(f,"%d|",q->value(13).toInt()/1000);
    fprintf(f,"%d|",q->value(14).toInt()/1000);
    fprintf(f,"%d|",q->value(15).toInt());
    fprintf(f,"%s|",(const char *)q->value(16).toString());
    fprintf(f,"%s|",(const char *)q->value(17).toString());
    fprintf(f,"\n");
  }
  delete q;

  //
  // Clean Up
  //
  fclose(f);
}


void ListDetails::closeData()
{
  done(0);
}


void ListDetails::resizeEvent(QResizeEvent *e)
{
  list_filter_label->setGeometry(20,10,400,40);
  list_filter_button->setGeometry(430,5,80,50);
  list_details_list->setGeometry(10,60,size().width()-20,size().height()-130);
  list_report_button->setGeometry(10,size().height()-60,80,50);
  list_dump_button->setGeometry(100,size().height()-60,80,50);
  list_close_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


void ListDetails::RefreshList()
{
  QString sql;
  QSqlQuery *q;
  MlListViewItem *item;

  sql=QString().sprintf("select CALL_DATE,CALL_TIME,ROUTE_ACTION,NAME,AGE,\
                         CITY,STATE,ZIPCODE,GENDER,STATION,QUALITY,CELLPHONE,\
                         RING_TIMER,HOLD_TIMER,AIR_TIMER,\
                         LINE_NUMBER,NUMBER,ID_NAME,ID\
                         from DETAILS_%s where (ID>%d) &&\
                         (CALL_DATE>=\"%s\")&&(CALL_DATE<=\"%s\")&&\
                         (CALL_TIME>=\"%s\")&&(CALL_TIME<=\"%s\") order by ID",
			(const char *)list_showcode,
			list_max_id,
			(const char *)start_datetime.date().
			toString("yyyy-MM-dd"),
			(const char *)end_datetime.date().
			toString("yyyy-MM-dd"),
			(const char *)start_datetime.time().
			toString("hh:mm:ss"),
			(const char *)end_datetime.time().
			toString("hh:mm:ss"));
  if(list_max_id<0) {
    list_details_list->clear();
  }
  q=new QSqlQuery(sql);
  while (q->next()) {
    item=new MlListViewItem(list_details_list);
    item->setId(q->value(18).toInt());
    item->
      setText(0,QString().sprintf("%s - %s",
				  (const char *)q->value(0).toDate().
				  toString("MM/dd/yyyy"),
				  (const char *)q->value(1).toTime().
				  toString("hh:mm:ss")));
    switch((MldConfig::RouteAction)q->value(2).toInt()) {
	case MldConfig::Accept:
	  item->setText(1,"Accepted");
	  break;

	case MldConfig::Warn:
	  item->setText(1,"Warned");
	  break;

	case MldConfig::Drop:
	  item->setText(1,"Dropped");
	  break;
    }
    item->setText(2,q->value(3).toString());   // Name
    if(q->value(4).toInt()>0) {
      item->setText(3,q->value(4).toString());   // Age
    }
    item->setText(4,q->value(5).toString());   // City
    item->setText(5,q->value(6).toString());   // State
    item->setText(6,q->value(7).toString());   // ZIP Code
    item->setText(7,q->value(8).toString());   // Gender
    item->setText(8,q->value(9).toString());   // Station
    if(q->value(10).toInt()>0) {
      item->setText(9,q->value(10).toString());   // Quality
    }
    item->setText(10,q->value(11).toString());   // Cellphone
    item->
      setText(11,MLGetTimeLength(q->value(12).toInt(),true,false));  // Ring
    item->
      setText(12,MLGetTimeLength(q->value(13).toInt(),true,false));  // Hold
    item->
      setText(13,MLGetTimeLength(q->value(14).toInt(),true,false));  // Air
    item->setText(14,q->value(15).toString()); // Line
    item->setText(15,CallerIdDetail::formattedNumber(q->value(16).toString()));
    item->setText(16,q->value(17).toString()); // Caller ID Name
    list_max_id=q->value(18).toUInt();
  }
  delete q;
}


void ListDetails::RefreshFilter()
{
  list_filter_label->
    setText(QString().sprintf(
   "Showing calls on dates from %s to %s\nand between the times of %s and %s.",
   (const char *)start_datetime.toString("MM/dd/yyyy"),
   (const char *)end_datetime.toString("MM/dd/yyyy"),
   (const char *)start_datetime.toString("hh:mm:ss"),
   (const char *)end_datetime.toString("hh:mm:ss")));
}


int ListDetails::GetBaseYear()
{
  QString sql;
  QSqlQuery *q;
  int year;

  sql=QString().sprintf("select CALL_DATE from DETAILS_%s\
                         where ID>%d order by CALL_DATE",
			(const char *)list_showcode,
			list_max_id);
  q=new QSqlQuery(sql);
  if(q->first()) {
    year=q->value(0).toDate().year();
  }
  else {
    year=QDate::currentDate().year();
  }
  delete q;

  return year;
}
