// list_callerid_sources.cpp
//
// List CallCommander CallerID Sources.
//
//   (C) Copyright 2002-2006 Fred Gleason <fredg@paravelsystems.com>
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

#include <math.h>

#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtSql/QSqlDatabase>
#include <QtGui/QMessageBox>
#include <QtGui/QResizeEvent>
#include <QtSql/QSqlQuery>

#include <mlttydevice.h>
#include <mldconfig.h>

#include "list_callerid_sources.h"
#include "add_callerid_source.h"
#include "edit_callerid_source.h"
#include "globals.h"

ListCallerIdSources::ListCallerIdSources(QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());

  setCaption("Call Administrator - CallerID Sources");

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);
  QFont small_font=QFont("Helvetica",10,QFont::Normal);
  small_font.setPixelSize(10);

  //
  // Users List
  //
  list_sources_list=new Q3ListView(this,"list_sources_list");
  list_sources_list->setAllColumnsShowFocus(true);
  list_sources_list->setItemMargin(5);
  connect(list_sources_list,
	  SIGNAL(doubleClicked(Q3ListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(doubleClickedData(Q3ListViewItem *,const QPoint &,int)));
  list_sources_list->addColumn("Host");
  list_sources_list->setColumnAlignment(0,Qt::AlignLeft|Qt::AlignVCenter);
  list_sources_list->addColumn("Source #");
  list_sources_list->setColumnAlignment(1,Qt::AlignCenter);
  list_sources_list->addColumn("Serial Port");
  list_sources_list->setColumnAlignment(2,Qt::AlignLeft|Qt::AlignVCenter);
  list_sources_list->addColumn("Baud Rate");
  list_sources_list->setColumnAlignment(3,Qt::AlignCenter);
  list_sources_list->addColumn("Data Bits");
  list_sources_list->setColumnAlignment(4,Qt::AlignCenter);
  list_sources_list->addColumn("Stop Bits");
  list_sources_list->setColumnAlignment(5,Qt::AlignCenter);
  list_sources_list->addColumn("Parity");
  list_sources_list->setColumnAlignment(6,Qt::AlignCenter);

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
  //  Close Button
  //
  list_close_button=new QPushButton(this,"list_close_button");
  list_close_button->setDefault(true);
  list_close_button->setFont(font);
  list_close_button->setText("&Close");
  connect(list_close_button,SIGNAL(clicked()),this,SLOT(closeData()));

  RefreshList();
}


ListCallerIdSources::~ListCallerIdSources()
{
}


QSize ListCallerIdSources::sizeHint() const
{
  return QSize(400,300);
} 


QSizePolicy ListCallerIdSources::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ListCallerIdSources::addData()
{
  QString hostname;
  int host_id;
  QString sql;
  QSqlQuery *q;

  AddCallerIdSource *add=new AddCallerIdSource(&hostname,&host_id,this,"add");
  if(add->exec()<0) {
    delete add;
    return;
  }
  sql=QString().sprintf("insert into CID_SOURCES set HOSTNAME=\"%s\",\
                         HOST_ID=%d",
			  (const char *)hostname,host_id);
  q=new QSqlQuery(sql);
  delete q;
  EditCallerIdSource *edit=
    new EditCallerIdSource(hostname,host_id,this,"edit");
  if(edit->exec()<0) {
    sql=QString().sprintf("delete from CID_SOURCES \
                           where (HOSTNAME=\"%s\")&&(HOST_ID=%d)",
			  (const char *)hostname,host_id);
    q=new QSqlQuery(sql);
    delete q;
    delete edit;
    delete add;
    return;
  }
  Q3ListViewItem *item=new Q3ListViewItem(list_sources_list);
  item->setText(0,hostname);
  item->setText(1,QString().sprintf("%d",host_id));
  UpdateItem(item);
  list_sources_list->setSelected(item,true);
  list_sources_list->ensureItemVisible(item);
  delete edit;
  delete add;
}


void ListCallerIdSources::editData()
{
  Q3ListViewItem *item=list_sources_list->selectedItem();
    if(item==NULL) {
    return;
  }
  EditCallerIdSource *edit=
    new EditCallerIdSource(item->text(0),item->text(1).toInt(),this,"edit");
  if(edit->exec()==0) {
    UpdateItem(item);
  }
  delete edit;
}


void ListCallerIdSources::deleteData()
{
  QString sql;
  QSqlQuery *q;
  Q3ListViewItem *item=list_sources_list->selectedItem();
  if(item==NULL) {
    return;
  }
  if(QMessageBox::question(this,"Delete CallerIdSource",
	      tr("Are you sure you want to delete this CallerId source?"),
			   QMessageBox::Yes,
			   QMessageBox::No)==QMessageBox::No) {
    return;
  }
  sql=QString().sprintf("delete from CID_SOURCES \
                         where (HOSTNAME=\"%s\")&&(HOST_ID=%d)",
			(const char *)item->text(0),item->text(1).toInt());
  q=new QSqlQuery(sql);
  delete q;
  delete item;
}


void ListCallerIdSources::doubleClickedData(Q3ListViewItem *item,
					    const QPoint &pt,int c)
{
  editData();
}


void ListCallerIdSources::closeData()
{
  done(0);
}


void ListCallerIdSources::resizeEvent(QResizeEvent *e)
{
  list_sources_list->setGeometry(10,10,size().width()-20,size().height()-80);
  list_add_button->setGeometry(10,size().height()-60,80,50);
  list_edit_button->setGeometry(100,size().height()-60,80,50);
  list_delete_button->setGeometry(190,size().height()-60,80,50);
  list_close_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


void ListCallerIdSources::RefreshList()
{
  QSqlQuery *q;
  Q3ListViewItem *item;

  list_sources_list->clear();
  q=new QSqlQuery("select HOSTNAME,HOST_ID,TTY_PORT,BAUD_RATE,DATA_BITS,\
                   STOP_BITS,PARITY from CID_SOURCES\
                   order by HOSTNAME,HOST_ID");
  while (q->next()) {
    item=new Q3ListViewItem(list_sources_list);
    item->setText(0,q->value(0).toString());
    item->setText(1,q->value(1).toString());
    item->setText(2,q->value(2).toString());
    item->setText(3,q->value(3).toString());
    item->setText(4,q->value(4).toString());
    item->setText(5,q->value(5).toString());
    switch((MLTTYDevice::Parity)q->value(6).toInt()) {
	case MLTTYDevice::None:
	  item->setText(6,tr("None"));
	  break;

	case MLTTYDevice::Even:
	  item->setText(6,tr("Even"));
	  break;

	case MLTTYDevice::Odd:
	  item->setText(6,tr("Odd"));
	  break;
    }
  }
  delete q;
}


void ListCallerIdSources::UpdateItem(Q3ListViewItem *item)
{
  QSqlQuery *q=new 
    QSqlQuery(QString().sprintf("select TTY_PORT,BAUD_RATE,DATA_BITS,\
                                 STOP_BITS,PARITY from CID_SOURCES\
                                 where (HOSTNAME=\"%s\")&&(HOST_ID=%d)",
				(const char *)item->text(0),
				item->text(1).toInt()));
  if(q->first()) {
    item->setText(2,q->value(0).toString());
    item->setText(3,q->value(1).toString());
    item->setText(4,q->value(2).toString());
    item->setText(5,q->value(3).toString());
    switch((MLTTYDevice::Parity)q->value(4).toInt()) {
	case MLTTYDevice::None:
	  item->setText(6,tr("None"));
	  break;

	case MLTTYDevice::Even:
	  item->setText(6,tr("Even"));
	  break;

	case MLTTYDevice::Odd:
	  item->setText(6,tr("Odd"));
	  break;
    }
  }
  delete q;
}
