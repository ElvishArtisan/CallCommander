// list_modules.cpp
//
// List CallCommander Logic Modules.
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
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

#include <QtGui/QMessageBox>
#include <QtGui/QLabel>
#include <QtSql/QSqlDatabase>
#include <QtGui/QResizeEvent>
#include <QtSql/QSqlQuery>

#include <mldconfig.h>
#include <ml_reload_packet.h>

#include "list_modules.h"
#include "edit_module.h"
#include "globals.h"

ListModules::ListModules(QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());

  setCaption("Call Administrator - Logic Modules");

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);
  QFont small_font=QFont("Helvetica",10,QFont::Normal);
  small_font.setPixelSize(10);

  //
  // Systems List
  //
  list_modules_list=new Q3ListView(this,"list_modules_list");
  list_modules_list->setItemMargin(5);
  list_modules_list->setAllColumnsShowFocus(true);
  connect(list_modules_list,
	  SIGNAL(doubleClicked(Q3ListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(doubleClickedData(Q3ListViewItem *,const QPoint &,int)));
  list_modules_list->addColumn("Description");
  list_modules_list->setColumnAlignment(0,Qt::AlignLeft|Qt::AlignVCenter);
  list_modules_list->addColumn("Hostname");
  list_modules_list->setColumnAlignment(1,Qt::AlignLeft|Qt::AlignVCenter);
  list_modules_list->addColumn("Connection");
  list_modules_list->setColumnAlignment(2,Qt::AlignLeft|Qt::AlignVCenter);
  list_modules_list->addColumn("Module Path");
  list_modules_list->setColumnAlignment(3,Qt::AlignLeft|Qt::AlignVCenter);
  list_modules_list->addColumn("Module Args");
  list_modules_list->setColumnAlignment(4,Qt::AlignLeft|Qt::AlignVCenter);
  list_modules_list->addColumn("Log Path");
  list_modules_list->setColumnAlignment(5,Qt::AlignLeft|Qt::AlignVCenter);

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


ListModules::~ListModules()
{
}


QSize ListModules::sizeHint() const
{
  return QSize(600,400);
} 


QSizePolicy ListModules::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ListModules::addData()
{
  QString showname;
  QString sql;
  QSqlQuery *q;
  int id=-1;

  sql="insert into LOGIC_MODULES set DESCRIPTION=\"New Module Configuration\"";
  q=new QSqlQuery(sql);
  delete q;
  sql="select ID from LOGIC_MODULES order by ID desc";
  q=new QSqlQuery(sql);
  if(q->first()) {
    id=q->value(0).toInt();
  }
  delete q;
  if(id<0) {
    return;
  }
  EditModule *edit=new EditModule(id,this,"edit");
  if(edit->exec()==0) {
    MlListViewItem *item=new MlListViewItem(list_modules_list);
    item->setId(id);
    UpdateItem(item);
    list_modules_list->ensureItemVisible(item);
  }
  else {
    sql=QString().sprintf("delete from LOGIC_MODULES where ID=%d",id);
    q=new QSqlQuery(sql);
    delete q;
  }
  delete edit;
}


void ListModules::editData()
{
  MlListViewItem *item=(MlListViewItem *)list_modules_list->selectedItem();
    if(item==NULL) {
    return;
  }
  EditModule *edit=new EditModule(item->id(),this,"edit");
  if(edit->exec()==0) {
    UpdateItem(item);
  }
  delete edit;
}


void ListModules::deleteData()
{
  QString sql;
  QSqlQuery *q;
  MlListViewItem *item=(MlListViewItem *)list_modules_list->selectedItem();
  if(item==NULL) {
    return;
  }
  if(QMessageBox::question(this,"Delete Module Config",
      tr("Are you sure you want to delete this\nLogic Module configuration?"),
			   QMessageBox::Yes,
			   QMessageBox::No)==QMessageBox::No) {
    return;
  }
  QString hostname;
  sql=QString().sprintf("select HOSTNAME from LOGIC_MODULES where ID=%d",
			item->id());
  q=new QSqlQuery(sql);
  if(q->first()) {
    hostname=q->value(0).toString();
  }
  delete q;
  sql=QString().sprintf("delete from LOGIC_MODULES where ID=%d",item->id());
  q=new QSqlQuery(sql);
  delete q;
  delete item;
  if(!hostname.isEmpty()) {
    QHostAddress addr=MlResolveIpAddress(hostname);
    if(!addr.isNull()) {
      MlSendReloadPacket(admin_config,addr,ML_MLMLD_RELOAD_PORT,
			 admin_reload_socket);
    }
  }
}


void ListModules::doubleClickedData(Q3ListViewItem *item,
					    const QPoint &pt,int c)
{
  editData();
}


void ListModules::closeData()
{
  done(0);
}


void ListModules::resizeEvent(QResizeEvent *e)
{
  list_modules_list->setGeometry(10,10,size().width()-20,size().height()-80);
  list_add_button->setGeometry(10,size().height()-60,80,50);
  list_edit_button->setGeometry(100,size().height()-60,80,50);
  list_delete_button->setGeometry(190,size().height()-60,80,50);
  list_close_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


void ListModules::RefreshList()
{
  QSqlQuery *q;
  MlListViewItem *item;

  list_modules_list->clear();
  q=new QSqlQuery("select ID,DESCRIPTION,HOSTNAME,CONNECTION_NAME,MODULE_PATH,\
                   MODULE_ARGS,LOG_PATH from LOGIC_MODULES");
  while (q->next()) {
    item=new MlListViewItem(list_modules_list);
    item->setId(q->value(0).toInt());
    item->setText(0,q->value(1).toString());
    item->setText(1,q->value(2).toString());
    item->setText(2,q->value(3).toString());
    item->setText(3,q->value(4).toString());
    item->setText(4,q->value(5).toString());
    item->setText(5,q->value(6).toString());
  }
  delete q;
}


void ListModules::UpdateItem(MlListViewItem *item)
{
  QSqlQuery *q=new 
    QSqlQuery(QString().sprintf("select DESCRIPTION,HOSTNAME,CONNECTION_NAME,\
                                 MODULE_PATH,MODULE_ARGS,LOG_PATH \
                                 from LOGIC_MODULES where ID=%d",
				item->id()));
  if(q->first()) {
    item->setText(0,q->value(0).toString());
    item->setText(1,q->value(1).toString());
    item->setText(2,q->value(2).toString());
    item->setText(3,q->value(3).toString());
    item->setText(4,q->value(4).toString());
    item->setText(5,q->value(5).toString());
  }
  delete q;
}
