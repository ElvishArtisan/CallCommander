// edit_map.cpp
//
// Edit a CallCommander CID Map.
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

#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtSql/QSqlDatabase>
#include <QtGui/QPainter>
#include <QtSql/QSqlQuery>

#include <math.h>

#include <list_connections.h>

#include "select_callerid_source.h"
#include "edit_map.h"
#include "add_map_line.h"

EditMap::EditMap(QString showcode,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  edit_showcode=showcode;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setCaption("Call Administrator - Edit CID Line Map");

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);
  QFont day_font=QFont("Helvetica",10,QFont::Normal);
  day_font.setPixelSize(10);

  //
  // Map List
  //
  edit_map_list=new Q3ListView(this,"edit_map_list");
  edit_map_list->setAllColumnsShowFocus(true);
  edit_map_list->setGeometry(10,10,sizeHint().width()-110,
			     sizeHint().height()-80);
  connect(edit_map_list,
	  SIGNAL(doubleClicked(Q3ListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(doubleClickedData(Q3ListViewItem *,const QPoint &,int)));
  edit_map_list->addColumn("Src Line");
  edit_map_list->setColumnAlignment(0,Qt::AlignCenter);
  edit_map_list->addColumn("Target Line");
  edit_map_list->setColumnAlignment(1,Qt::AlignCenter);

  //
  //  Add Button
  //
  QPushButton *button=new QPushButton(this,"add_button");
  button->setGeometry(sizeHint().width()-90,10,80,50);
  button->setFont(label_font);
  button->setText(tr("&Add"));
  connect(button,SIGNAL(clicked()),this,SLOT(addData()));

  //
  //  Edit Button
  //
  button=new QPushButton(this,"edit_button");
  button->setGeometry(sizeHint().width()-90,80,80,50);
  button->setFont(label_font);
  button->setText(tr("&Edit"));
  connect(button,SIGNAL(clicked()),this,SLOT(editData()));

  //
  //  Delete Button
  //
  button=new QPushButton(this,"delete_button");
  button->setGeometry(sizeHint().width()-90,150,80,50);
  button->setFont(label_font);
  button->setText(tr("&Delete"));
  connect(button,SIGNAL(clicked()),this,SLOT(deleteData()));

  //
  //  Ok Button
  //
  button=new QPushButton(this,"ok_button");
  button->setGeometry(sizeHint().width()-190,sizeHint().height()-60,80,50);
  button->setFont(label_font);
  button->setText(tr("&Ok"));
  connect(button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  button=new QPushButton(this,"cancel_button");
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setFont(label_font);
  button->setText(tr("&Cancel"));
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));

  //
  // Load Data
  //
  QString sql;
  QSqlQuery *q;
  Q3ListViewItem *item=NULL;

  sql=QString().sprintf("select SOURCE_LINE,TARGET_LINE from CID_MAPS \
                         where SHOW_CODE=\"%s\"",(const char *)edit_showcode);
  q=new QSqlQuery(sql);
  while(q->next()) {
    item=new Q3ListViewItem(edit_map_list);
    item->setText(0,q->value(0).toString());
    if(q->value(1).toInt()==0) {
      item->setText(1,tr("OFF"));
    }
    else {
      item->setText(1,q->value(1).toString());
    }
  }
  delete q;
}


EditMap::~EditMap()
{
}


QSize EditMap::sizeHint() const
{
  return QSize(270,450);
} 


QSizePolicy EditMap::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditMap::addData()
{
  int src_line=1;
  int dest_line=1;

  AddMapLine *add=new AddMapLine(&src_line,&dest_line,true,this);
  if(add->exec()==0) {
    Q3ListViewItem *item=edit_map_list->firstChild();
    while(item!=NULL) {
      if(item->text(0).toInt()==src_line) {
	if(dest_line==0) {
	  item->setText(1,tr("OFF"));
	}
	else {
	  item->setText(1,QString().sprintf("%d",dest_line));
	}
	delete add;
	return;
      }
      item=item->nextSibling();
    }
    item=new Q3ListViewItem(edit_map_list);
    item->setText(0,QString().sprintf("%d",src_line));
    if(dest_line==0) {
      item->setText(1,tr("OFF"));
    }
    else {
      item->setText(1,QString().sprintf("%d",dest_line));
    }
  }
  delete add;
}


void EditMap::editData()
{
  int src_line;
  int dest_line;
  Q3ListViewItem *item=edit_map_list->selectedItem();

  if(item==NULL) {
    return;
  }
  src_line=item->text(0).toInt();
  dest_line=item->text(1).toInt();
  AddMapLine *add=new AddMapLine(&src_line,&dest_line,false,this);
  if(add->exec()==0) {
    Q3ListViewItem *item=edit_map_list->firstChild();
    while(item!=NULL) {
      if(item->text(0).toInt()==src_line) {
	if(dest_line==0) {
	  item->setText(1,tr("OFF"));
	}
	else {
	  item->setText(1,QString().sprintf("%d",dest_line));
	}
	delete add;
	return;
      }
      item=item->nextSibling();
    }
  }
  delete add;
}


void EditMap::deleteData()
{
  Q3ListViewItem *item=edit_map_list->selectedItem();

  if(item==NULL) {
    return;
  }
  delete item;
}


void EditMap::doubleClickedData(Q3ListViewItem *item,const QPoint &pt,int col)
{
  editData();
}


void EditMap::okData()
{
  QString sql;
  QSqlQuery *q;

  sql=QString().sprintf("delete from CID_MAPS where SHOW_CODE=\"%s\"",
			(const char *)edit_showcode);
  q=new QSqlQuery(sql);
  delete q;
  Q3ListViewItem *item=edit_map_list->firstChild();
  while(item!=NULL) {
    sql=QString().sprintf("insert into CID_MAPS set SHOW_CODE=\"%s\",\
                           SOURCE_LINE=%d,TARGET_LINE=%d",
			  (const char *)edit_showcode,item->text(0).toInt(),
			  item->text(1).toInt());
    q=new QSqlQuery(sql);
    delete q;
    item=item->nextSibling();
  }
  done(0);
}


void EditMap::cancelData()
{
  done(-1);
}
