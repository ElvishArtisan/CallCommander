// list_reports.cpp
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

#include <QtGui/QPushButton>
#include <QtSql/QSqlDatabase>
#include <QtGui/QPainter>
#include <QtSql/QSqlQuery>
#include <QtGui/QPrinter>
#include <QtGui/QMessageBox>

#include <math.h>

#include <mldconfig.h>

#include "add_report.h"
#include "edit_report.h"
#include "print_report.h"
#include "list_reports.h"

ListReports::ListReports(QString showcode,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  QString title;

  setCaption("Call Manager - Reports");

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());

  list_showcode=showcode;

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);
  QFont small_font=QFont("Helvetica",10,QFont::Normal);
  small_font.setPixelSize(10);

  //
  // Directory List
  //
  list_reports_list=new Q3ListView(this,"list_reports_list");
  list_reports_list->setGeometry(10,10,
				 sizeHint().width()-20,sizeHint().height()-80);
  list_reports_list->setAllColumnsShowFocus(true);
  list_reports_list->setItemMargin(5);
  connect(list_reports_list,
	  SIGNAL(doubleClicked(Q3ListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(doubleClickedData(Q3ListViewItem *,const QPoint &,int)));
  list_reports_list->addColumn("Title");
  list_reports_list->setColumnAlignment(0,Qt::AlignCenter);
  list_reports_list->addColumn("Type");
  list_reports_list->setColumnAlignment(1,Qt::AlignCenter);
  list_reports_list->addColumn("Header 1");
  list_reports_list->setColumnAlignment(1,Qt::AlignCenter);
  list_reports_list->addColumn("Header 2");
  list_reports_list->setColumnAlignment(1,Qt::AlignCenter);

  //
  //  Add Button
  //
  QPushButton *button=new QPushButton(this,"list_add_button");
  button->setGeometry(10,sizeHint().height()-60,80,50);
  button->setFont(font);
  button->setText("&Add");
  connect(button,SIGNAL(clicked()),this,SLOT(addData()));

  //
  //  Edit Button
  //
  button=new QPushButton(this,"list_edit_button");
  button->setGeometry(100,sizeHint().height()-60,80,50);
  button->setFont(font);
  button->setText("&Edit");
  connect(button,SIGNAL(clicked()),this,SLOT(editData()));

  //
  //  Delete Button
  //
  button=new QPushButton(this,"list_delete_button");
  button->setGeometry(190,sizeHint().height()-60,80,50);
  button->setFont(font);
  button->setText("&Delete");
  connect(button,SIGNAL(clicked()),this,SLOT(deleteData()));

  //
  //  Run Button
  //
  button=new QPushButton(this,"list_run_button");
  button->setGeometry(350,sizeHint().height()-60,80,50);
  button->setFont(font);
  button->setText("&Run\nReport");
  connect(button,SIGNAL(clicked()),this,SLOT(runData()));

  //
  //  Close Button
  //
  button=new QPushButton(this,"list_close_button");
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setDefault(true);
  button->setFont(font);
  button->setText("&Close");
  connect(button,SIGNAL(clicked()),this,SLOT(closeData()));

  RefreshList();
}


ListReports::~ListReports()
{
}


QSize ListReports::sizeHint() const
{
  return QSize(600,400);
} 


QSizePolicy ListReports::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ListReports::addData()
{
  QString title;
  QString sql;
  QSqlQuery *q;

  AddReport *add=new AddReport(list_showcode,&title,this,"add");
  if(add->exec()==0) {
    sql=QString().sprintf("insert into REPORTS set SHOW_CODE=\"%s\",\
                           TITLE=\"%s\"",
			  (const char *)list_showcode,
			  (const char *)title);
    q=new QSqlQuery(sql);
    delete q;
    EditReport *edit=new EditReport(list_showcode,title,this,"edit");
    if(edit->exec()==0) {
      Q3ListViewItem *item=new Q3ListViewItem(list_reports_list);
      item->setText(0,title);
      UpdateItem(item);
      list_reports_list->setSelected(item,true);
      list_reports_list->ensureItemVisible(item);
    }
    else {
      sql=QString().sprintf("delete from REPORTS where (SHOW_CODE=\"%s\")&&\
                             (TITLE=\"%s\")",
			    (const char *)list_showcode,
			    (const char *)title);
      q=new QSqlQuery(sql);
      delete q;
    }
    delete edit;
  }
  delete add;
}


void ListReports::editData()
{
  Q3ListViewItem *item=list_reports_list->selectedItem();
  
  if(item==NULL) {
    return;
  }
  EditReport *edit=new EditReport(list_showcode,item->text(0),this,"edit");
  if(edit->exec()==0) {
    UpdateItem(item);
  }
  delete edit;
}


void ListReports::deleteData()
{
  QString sql;
  QSqlQuery *q;
  Q3ListViewItem *item=list_reports_list->selectedItem();

  if(item==NULL) {
    return;
  }
  if(QMessageBox::question(this,"Delete Report",
	 QString().sprintf("Are you sure you want to delete the %s report?",
			   (const char *)item->text(0)),
			   QMessageBox::Yes,
			   QMessageBox::No)==QMessageBox::No) {
    return;
  }
  sql=QString().sprintf("delete from REPORTS where (SHOW_CODE=\"%s\")&&\
                         TITLE=\"%s\"",
			(const char *)list_showcode,
			(const char *)item->text(0));
  q=new QSqlQuery(sql);
  delete q;
  delete item;
}


void ListReports::runData()
{
  Q3ListViewItem *item=list_reports_list->selectedItem();
  if(item==NULL) {
    return;
  }
  PrintReport *report=new PrintReport(list_showcode,item->text(0),
				      this,"report");
  report->exec();
  delete report;
}


void ListReports::doubleClickedData(Q3ListViewItem *item,const QPoint &pt,
				      int c)
{
  editData();
}


void ListReports::closeData()
{
  done(0);
}


void ListReports::RefreshList()
{
  QString sql;
  QSqlQuery *q;
  Q3ListViewItem *item;

  list_reports_list->clear();
  sql=QString().
    sprintf("select TITLE,TYPE,HEADER1,HEADER2\
             from REPORTS where SHOW_CODE=\"%s\"",
	    (const char *)list_showcode);

  q=new QSqlQuery(sql);
  while (q->next()) {
    item=new Q3ListViewItem(list_reports_list);
    item->setText(0,q->value(0).toString());
    switch(q->value(1).toInt()) {
	case 0:   // Standard Age Breakdown
	  item->setText(1,"Age & Gender Report");
	  break;

	case 1:  // Area Code Report
	  item->setText(1,"Source Number Report");
	  break;

	default:
	  item->setText(1,"Unknown");
    }
    item->setText(2,q->value(2).toString());
    item->setText(3,q->value(3).toString());
  }
  delete q;
}


void ListReports::UpdateItem(Q3ListViewItem *item)
{
  QString sql;
  QSqlQuery *q;

  sql=QString().sprintf("select TYPE,HEADER1,HEADER2\
                         from REPORTS where (SHOW_CODE=\"%s\")&&\
                         TITLE=\"%s\"",
			(const char *)list_showcode,
			(const char *)item->text(0));
  q=new QSqlQuery(sql);
  if(q->first()) {
    switch(q->value(0).toInt()) {
	case 0:  // Standard Age Breakdwon
	  item->setText(1,"Age & Gender Report");
	  break;

	case 1:  // Area Code Report
	  item->setText(1,"Source Numbers Report");
	  break;

	default:
	  item->setText(1,"Unknown");
	  break;
    }
    item->setText(2,q->value(1).toString());
    item->setText(3,q->value(2).toString());
  }
  delete q;
}
