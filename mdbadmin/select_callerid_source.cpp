// select_callerid_source.cpp
//
// Select a CallCommander CallerID Source.
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

#include <QtGui/QLabel>
#include <QtSql/QSqlQuery>
#include <QtCore/QVariant>

#include "select_callerid_source.h"

SelectCallerIdSource::SelectCallerIdSource(const QString &hostname,
					   int *host_id,
					   QWidget *parent,const char *name)
  :  QDialog(parent,name,true)
{
  QString sql;
  QSqlQuery *q;

  select_hostname=hostname;
  select_host_id=host_id;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setCaption("Call Administrator - CallerID Sources");

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);

  //
  // Source List
  //
  select_source_list=new Q3ListBox(this,"select_source_list");
  select_source_list->setGeometry(10,30,sizeHint().width()-20,
				  sizeHint().height()-100);
  connect(select_source_list,SIGNAL(doubleClicked(Q3ListBoxItem *)),
	  this,SLOT(doubleClickedData(Q3ListBoxItem *)));
  QLabel *label=new QLabel(select_source_list,tr("CallerID Sources"),
			   this,"select_source_label");
  label->setGeometry(10,10,sizeHint().width()-20,20);
  label->setFont(label_font);
  label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  //  OK Button
  //
  QPushButton *button=new QPushButton(this,"ok_button");
  button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  button->setDefault(true);
  button->setFont(label_font);
  button->setText("&OK");
  connect(button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  button=new QPushButton(this,"cancel_button");
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setFont(label_font);
  button->setText("&Cancel");
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));

  //
  // Load Data
  //
  select_source_list->insertItem(tr("[use native source]"));
  sql=QString().sprintf("select HOST_ID from CID_SOURCES\
                         where HOSTNAME=\"%s\" order by HOST_ID",
			(const char *)hostname);
  q=new QSqlQuery(sql);
  while(q->next()) {
    select_source_list->insertItem(QString().sprintf("%s:%d",
						     (const char *)hostname,
						     q->value(0).toInt()));
  }
  delete q;
}


SelectCallerIdSource::~SelectCallerIdSource()
{
}


QSize SelectCallerIdSource::sizeHint() const
{
  return QSize(300,250);
}


QSizePolicy SelectCallerIdSource::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void SelectCallerIdSource::doubleClickedData(Q3ListBoxItem *item)
{
  okData();
}


void SelectCallerIdSource::okData()
{
  if(select_source_list->currentText().isEmpty()) {
    return;
  }
  if(select_source_list->currentItem()==0) {
    *select_host_id=0;
  }
  else {
    *select_host_id=select_source_list->currentText().right(1).toInt();
  }
  done(0);
}


void SelectCallerIdSource::cancelData()
{
  done(-1);
}
