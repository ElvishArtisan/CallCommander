// list_winners.cpp
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
#include <QtGui/QFileDialog>
#include <QtCore/qfile.h>
#include <QtGui/QMessageBox>
#include <QtCore/QTimer>
#include <QtGui/QResizeEvent>
#include <QtGui/QLabel>
#include <QtSql/QSqlQuery>

#include <mlconf.h>
#include <mldconfig.h>
#include <calleriddetail.h>

#include "calleriddetail.h"
#include "globals.h"
#include "list_winners.h"
#include "filter_dialog.h"
#include "list_reports.h"

//
// Icons
//
#include "../icons/checkmark.xpm"
#include "../icons/ex.xpm"

ListWinners::ListWinners(QString showcode,QWidget *parent,const char *name)
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
  setCaption(QString().sprintf("%s - Contest Winners",(const char *)title));

  //
  // Create Fonts
  //
  QFont normal_font=QFont("Helvetica",12,QFont::Normal);
  normal_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);
  QFont small_font=QFont("Helvetica",10,QFont::Normal);
  small_font.setPixelSize(10);

  //
  // Create Icons
  //
  list_checkmark_map=new QPixmap(checkmark_xpm);
  list_ex_map=new QPixmap(ex_xpm);

  //
  // Dialogs
  //
  list_edit_winner=new EditWinner(this);

  //
  // Filter Checkbox
  //
  list_filterbox_box=new QCheckBox(this);
  connect(list_filterbox_box,SIGNAL(stateChanged(int)),
	  this,SLOT(filterStateEnabledData(int)));
  list_filterbox_label=new QLabel(tr("Filter by Date/Time"),this);
  list_filterbox_label->setFont(font);
  
  //
  // Filter
  //
  list_filter_button=new QPushButton(this,"list_filter_button");
  list_filter_button->setFont(font);
  list_filter_button->setText("&Set\nFilter");
  connect(list_filter_button,SIGNAL(clicked()),this,SLOT(setFilterData()));
  list_filter_label=new QLabel(this,"list_filter_label");
  list_filter_label->setFont(normal_font);

  //
  // Pending Checkbox
  //
  list_pending_box=new QCheckBox(this);
  list_pending_box->setChecked(true);
  connect(list_pending_box,SIGNAL(stateChanged(int)),
	  this,SLOT(filterStateChangedData(int)));
  list_pending_label=new QLabel(tr("Show Pending Awards"),this);
  list_pending_label->setFont(font);
  
  //
  // Complete Checkbox
  //
  list_complete_box=new QCheckBox(this);
  connect(list_complete_box,SIGNAL(stateChanged(int)),
	  this,SLOT(filterStateChangedData(int)));
  list_complete_label=new QLabel(tr("Show Completed Awards"),this);
  list_complete_label->setFont(font);

  //
  // Directory List
  //
  list_winners_list=new WinnerListView(list_showcode,this);
  list_winners_list->setAllColumnsShowFocus(true);
  list_winners_list->setItemMargin(5);
  connect(list_winners_list,SIGNAL(doubleClicked(Q3ListViewItem *)),
	  this,SLOT(doubleClickedData(Q3ListViewItem *)));
  list_winners_list->addColumn(" ");
  list_winners_list->setColumnAlignment(0,Qt::AlignCenter);
  list_winners_list->addColumn("Origin DateTime");
  list_winners_list->setColumnAlignment(1,Qt::AlignCenter);
  list_winners_list->addColumn("Name");
  list_winners_list->setColumnAlignment(2,Qt::AlignLeft|Qt::AlignVCenter);
  list_winners_list->addColumn("City");
  list_winners_list->setColumnAlignment(3,Qt::AlignLeft|Qt::AlignVCenter);
  list_winners_list->addColumn("State");
  list_winners_list->setColumnAlignment(4,Qt::AlignCenter);
  list_winners_list->addColumn("ZIP Code");
  list_winners_list->setColumnAlignment(5,Qt::AlignCenter);
  list_winners_list->addColumn("Contact Number");
  list_winners_list->setColumnAlignment(6,Qt::AlignCenter);

  //
  //  Add Button
  //
  list_add_button=new QPushButton(this);
  list_add_button->setFont(font);
  list_add_button->setText("&Add\nWinner");
  connect(list_add_button,SIGNAL(clicked()),this,SLOT(addData()));

  //
  //  Edit Button
  //
  list_edit_button=new QPushButton(this);
  list_edit_button->setFont(font);
  list_edit_button->setText("&Update\nWinner");
  connect(list_edit_button,SIGNAL(clicked()),this,SLOT(editData()));

  //
  //  Report Button
  //
  list_report_button=new QPushButton(this);
  list_report_button->setFont(font);
  list_report_button->setText("&Reports");
  connect(list_report_button,SIGNAL(clicked()),this,SLOT(reportsData()));
  list_report_button->hide();

  //
  //  Dump Button
  //
  list_dump_button=new QPushButton(this);
  list_dump_button->setFont(font);
  list_dump_button->setText("&Dump\nData");
  connect(list_dump_button,SIGNAL(clicked()),this,SLOT(dumpData()));

  //
  //  Close Button
  //
  list_close_button=new QPushButton(this);
  list_close_button->setDefault(true);
  list_close_button->setFont(font);
  list_close_button->setText("&Close");
  connect(list_close_button,SIGNAL(clicked()),this,SLOT(closeData()));

  RefreshFilter();
}


ListWinners::~ListWinners()
{
  delete list_edit_winner;
  delete list_checkmark_map;
  delete list_ex_map;
}


QSize ListWinners::sizeHint() const
{
  return QSize(640,510);
} 


QSizePolicy ListWinners::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ListWinners::filterStateEnabledData(int state)
{
  RefreshFilter();
}


void ListWinners::filterStateChangedData(int state)
{
  RefreshList();
}


void ListWinners::setFilterData()
{
  FilterDialog *dialog=new FilterDialog(this,"dialog");
  if(dialog->exec()==0) {
    list_max_id=-1;
    RefreshFilter();
    RefreshList();
  }
  delete dialog;
}


void ListWinners::addData()
{
  QString sql;
  QSqlQuery *q;
  int last_id=-1;
  WinnerListViewItem *item=NULL;

  sql=QString().sprintf("insert into WINNERS set \
                         ORIGIN_DATETIME=now(),\
                         ORIGIN_LOGIN_NAME=\"%s\"",
			(const char *)man_loginname.toAscii());
  q=new QSqlQuery(sql);
  delete q;
  sql="select LAST_INSERT_ID() from WINNERS";
  q=new QSqlQuery(sql);
  if(q->first()) {
    last_id=q->value(0).toInt();
  }
  delete q;
  if(list_edit_winner->exec(last_id)==0) {
    item=new WinnerListViewItem(list_winners_list);
    item->setId(last_id);
    RefreshItem(item);
  }
  else {
    sql=QString().sprintf("delete from WINNERS where ID=%d",last_id);
    q=new QSqlQuery(sql);
    delete q;
  }
}


void ListWinners::editData()
{
  WinnerListViewItem *item=(WinnerListViewItem *)list_winners_list->selectedItem();
  if(item==NULL) {
    return;
  }
  if(list_edit_winner->exec(item->id())==0) {
    RefreshItem(item);
  }
}


void ListWinners::doubleClickedData(Q3ListViewItem *item)
{
  editData();
}


void ListWinners::reportsData()
{
  ListReports *dialog=new ListReports(list_showcode,this,"dialog");
  dialog->exec();
  delete dialog;
}


void ListWinners::dumpData()
{
  QString sql;
  QSqlQuery *q;

  //
  // Open Output File
  //
  QString filename=QFileDialog::getSaveFileName();
  /*
  if(QFile::exists(filename)) {
    if(QMessageBox::question(this,"File Exists",
			     QString().sprintf("The file \"%s\" already exists!\nOverwrite?",(const char *)filename),QMessageBox::Yes,QMessageBox::No)!=QMessageBox::Yes) {
      return;
    }
  }
  */
  if(filename.isEmpty()) {
    return;
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
  fprintf(f,"SHOW_CODE,NUMBER,STATUS,FIRST_NAME,LAST_NAME,GENDER,AGE,STATION,EMAIL,PHONE,ADDRESS1,ADDRESS2,CITY,STATE,ZIPCODE,ORIGIN_LOGIN_NAME,ORIGIN_DATETIME,DISPOSITION_LOGIN_NAME,DISPOSITION_DATETIME,PRIZE_DESCRIPTION,REMARKS\n");

  //
  // Generate Dump
  //
  sql=QString().sprintf("select SHOW_CODE,NUMBER,STATUS,FIRST_NAME,LAST_NAME,\
                         GENDER,AGE,EMAIL,PHONE,ADDRESS1,ADDRESS2,\
                         CITY,STATE,ZIPCODE,ORIGIN_LOGIN_NAME,ORIGIN_DATETIME,\
                         DISPOSITION_LOGIN_NAME,DISPOSITION_DATETIME,\
                         PRIZE_DESCRIPTION,REMARKS from WINNERS %s \
                         order by ORIGIN_DATETIME",
			(const char *)GenerateWhere());
  q=new QSqlQuery(sql);
  while(q->next()) {
    fprintf(f,"\"%s\",",(const char *)q->value(0).toString().simplified());
    fprintf(f,"\"%s\",",(const char *)q->value(1).toString().simplified());
    fprintf(f,"%d,",q->value(2).toInt());
    fprintf(f,"\"%s\",",(const char *)q->value(3).toString().simplified());
    fprintf(f,"\"%s\",",(const char *)q->value(4).toString().simplified());
    fprintf(f,"\"%s\",",(const char *)q->value(5).toString().simplified());
    fprintf(f,"%d,",q->value(6).toInt());
    fprintf(f,"\"%s\",",(const char *)q->value(8).toString().simplified());
    fprintf(f,"\"%s\",",(const char *)q->value(9).toString().simplified());
    fprintf(f,"\"%s\",",(const char *)q->value(10).toString().simplified());
    fprintf(f,"\"%s\",",(const char *)q->value(11).toString().simplified());
    fprintf(f,"\"%s\",",(const char *)q->value(12).toString().simplified());
    fprintf(f,"\"%s\",",(const char *)q->value(13).toString().simplified());
    fprintf(f,"\"%s\",",(const char *)q->value(14).toString().simplified());
    fprintf(f,"\"%s\",",(const char *)q->value(15).toString().simplified());
    fprintf(f,"\"%s\",",(const char *)q->value(16).toDateTime().
	    toString("yyyy-MM-dd hh:mm:ss"));
    fprintf(f,"\"%s\",",(const char *)q->value(17).toString().simplified());
    fprintf(f,"\"%s\",",(const char *)q->value(18).toDateTime().
	    toString("yyyy-MM-dd hh:mm:ss"));
    fprintf(f,"\"%s\",",(const char *)q->value(19).toString().simplified());
    fprintf(f,"\"%s\",",(const char *)q->value(20).toString().simplified());
    fprintf(f,"\n");
  }
  delete q;

  //
  // Clean Up
  //
  fclose(f);
}


void ListWinners::closeData()
{
  done(0);
}


void ListWinners::resizeEvent(QResizeEvent *e)
{
  list_filterbox_box->setGeometry(15,11,20,20);
  list_filterbox_label->setGeometry(35,10,140,20);
  list_filter_button->setGeometry(160,10,80,50);
  list_filter_label->setGeometry(250,12,size().width()-260,40);
  list_pending_box->setGeometry(15,66,20,20);
  list_pending_label->setGeometry(35,65,200,20);
  list_complete_box->setGeometry(250,66,20,20);
  list_complete_label->setGeometry(270,65,200,20);
  list_winners_list->setGeometry(10,90,size().width()-20,size().height()-160);
  list_add_button->setGeometry(10,size().height()-60,80,50);
  list_edit_button->setGeometry(100,size().height()-60,80,50);
  list_report_button->setGeometry(250,size().height()-60,80,50);
  list_dump_button->setGeometry(340,size().height()-60,80,50);
  list_close_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


void ListWinners::RefreshList()
{
  QString sql;
  QSqlQuery *q;
  WinnerListViewItem *item;

  //
  // Generate List
  //
  list_winners_list->clear();
  sql=QString().sprintf("select ID,NUMBER,DETAIL_ID,STATUS,ORIGIN_DATETIME,\
                         FIRST_NAME,LAST_NAME,CITY,STATE,ZIPCODE,NUMBER \
                         from WINNERS %s order by ORIGIN_DATETIME",
			(const char *)GenerateWhere());
  q=new QSqlQuery(sql);
  while (q->next()) {
    item=new WinnerListViewItem(list_winners_list);
    item->setId(q->value(0).toInt());
    item->setPnum(q->value(1).toString());
    item->setDetailId(q->value(2).toUInt());
    if(q->value(3).toUInt()==0) {
      item->setPixmap(0,*list_checkmark_map);
    }
    else {
      item->setPixmap(0,*list_ex_map);
    }
    item->
      setText(1,q->value(4).toDateTime().toString("MM/dd/yyyy - hh:mm:ss"));
    item->setText(2,q->value(5).toString()+" "+q->value(6).toString()); // Name
    item->setText(3,q->value(7).toString());   // City
    item->setText(4,q->value(8).toString());   // State
    item->setText(5,q->value(9).toString());   // ZIP Code
    item->setText(6,CallerIdDetail::formattedNumber(q->value(10).toString()));
  }
  delete q;
}


void ListWinners::RefreshItem(WinnerListViewItem *item)
{
  QString sql;
  QSqlQuery *q;

  sql=QString().sprintf("select NUMBER,DETAIL_ID,STATUS,ORIGIN_DATETIME,\
                         FIRST_NAME,LAST_NAME,CITY,STATE,ZIPCODE,NUMBER \
                         from WINNERS where ID=%d",
			item->id());
  q=new QSqlQuery(sql);
  if(q->first()) {
    item->setPnum(q->value(0).toString());
    item->setDetailId(q->value(1).toUInt());
    if(q->value(2).toUInt()==0) {
      item->setPixmap(0,*list_checkmark_map);
    }
    else {
      item->setPixmap(0,*list_ex_map);
    }
    item->
      setText(1,q->value(3).toDateTime().toString("MM/dd/yyyy - hh:mm:ss"));
    item->setText(2,q->value(4).toString()+" "+q->value(5).toString()); // Name
    item->setText(5,q->value(6).toString());   // City
    item->setText(6,q->value(7).toString());   // State
    item->setText(7,q->value(8).toString());   // ZIP Code
    item->setText(8,CallerIdDetail::formattedNumber(q->value(9).toString()));
  }
  delete q;
}


void ListWinners::RefreshFilter()
{
  list_filter_label->
    setText(QString().sprintf(
  "Showing awards on dates from %s to %s\nand between the times of %s and %s.",
   (const char *)start_datetime.toString("MM/dd/yyyy"),
   (const char *)end_datetime.toString("MM/dd/yyyy"),
   (const char *)start_datetime.toString("hh:mm:ss"),
   (const char *)end_datetime.toString("hh:mm:ss")));
  list_filter_button->setEnabled(list_filterbox_box->isChecked());
  list_filter_label->setEnabled(list_filterbox_box->isChecked());
  RefreshList();
}


 QString ListWinners::GenerateWhere()
 {
   QString filter="where (SHOW_CODE=\""+list_showcode+"\")";

   if(list_pending_box->isChecked()) {
     if(list_complete_box->isChecked()) {
     }
     else {
       filter+="&&(STATUS!=0)&&(STATUS!=2)";
     }
   }
   else {
     if(list_complete_box->isChecked()) {
       filter+="&&(STATUS!=1)";
     }
     else {
       filter+="&&(STATUS!=0)&&(STATUS!=1)&&(STATUS!=2)";
     }
   }
   if(list_filterbox_box->isChecked()) {
     if(!filter.isEmpty()) {
       filter+="&&";
     }
     filter+=QString().sprintf("(ORIGIN_DATETIME>=\"%s\")&&\
                                (ORIGIN_DATETIME<=\"%s\")",
			       (const char *)start_datetime.
			       toString("yyyy-MM-dd hh:mm:ss"),
			       (const char *)end_datetime.
			       toString("yyyy-MM-dd hh:mm:ss"));
   }
   return filter;
 }


int ListWinners::GetBaseYear()
{
  QString sql;
  QSqlQuery *q;
  int year;

  sql=QString().sprintf("select CALL_DATE from WINNERS_%s\
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

