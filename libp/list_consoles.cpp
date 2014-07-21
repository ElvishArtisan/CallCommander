// list_consoles.cpp
//
// List CallCommander Show Configurations
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

#include <stdlib.h>

#include <QtGui/QMessageBox>
#include <QtGui/QLabel>

#include "conn_view_item.h"
#include "list_consoles.h"
#include "save_connection.h"
#include "telos2101_driver.h"

ListConsoles::ListConsoles(ConnectionData *conn,BusDriver *driver,
			   QWidget *parent,const char *name)
  : QDialog(parent)
    //  : QDialog(parent,name,true)
{
  list_connection=conn;
  list_bus_driver=driver;

  //
  // HACK: Disable the Broken Custom SuSE Dialogs
  //
#ifndef WIN32
  setenv("QT_NO_KDE_INTEGRATION","1",1);
#endif  // WIN32

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMaximumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumHeight(sizeHint().height());

  setCaption(tr("Available Consoles"));

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);
  QFont button_font=QFont("Helvetica",12,QFont::Bold);
  button_font.setPixelSize(12);
  
  //
  // Question Text
  //
  list_question_label=
    new QLabel(tr("Select a Console to associate with this connection."),this);
  list_question_label->setGeometry(10,5,sizeHint().width()-20,20);
  list_question_label->setFont(button_font);
  list_question_label->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  list_question_label->setDisabled(true);

  //
  // Show List
  //
  list_console_view=new Q3ListView(this,"list_console_view");
  list_console_view->
    setGeometry(10,30,sizeHint().width()-20,sizeHint().height()-100);
  list_console_view->setFont(font);
  list_console_view->setAllColumnsShowFocus(true);
  list_console_view->setItemMargin(5);
  connect(list_console_view,
	  SIGNAL(doubleClicked(Q3ListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(listDoubleclickedData(Q3ListViewItem *,const QPoint &,int)));
  connect(list_console_view,SIGNAL(clicked(Q3ListViewItem *)),
	  this,SLOT(listClickedData(Q3ListViewItem *)));
  list_console_view->addColumn("Console Name");
  list_console_view->setColumnAlignment(0,Qt::AlignLeft);
  list_console_view->addColumn("Current Mode");
  list_console_view->setColumnAlignment(1,Qt::AlignCenter);
  list_console_view->addColumn("Console Free");
  list_console_view->setColumnAlignment(2,Qt::AlignCenter);
  list_console_view->setDisabled(true);

  QPushButton *button;

  //
  // Ok Button
  //
  list_ok_button=new QPushButton(tr("Next"),this,"list_ok_button");
  list_ok_button->
    setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  list_ok_button->setFont(button_font);
  list_ok_button->setDisabled(true);
  connect(list_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Cancel Button
  //
  button=new QPushButton(tr("Back"),this,"cancel_button");
  button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  button->setFont(button_font);
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));

  //
  // The Protocol Bus Driver
  //
  list_scan_timer=new QTimer(this);
  connect(list_scan_timer,SIGNAL(timeout()),this,SLOT(scanDeviceData()));
  list_scan_timer->start(100);
  connect(list_bus_driver,SIGNAL(error(unsigned,BusDriver::Error)),
	  this,SLOT(busErrorData(unsigned,BusDriver::Error)));
  connect(list_bus_driver,
	  SIGNAL(currentConsoleList(unsigned,std::vector <ConsoleData *> *)),
	  this,
	  SLOT(busConsoleListData(unsigned,std::vector <ConsoleData *> *)));
  list_bus_driver->requestConsoleList();
}


ListConsoles::~ListConsoles()
{
}


QSize ListConsoles::sizeHint() const
{
  return QSize(320,210);
}


QSizePolicy ListConsoles::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ListConsoles::scanDeviceData()
{
  list_bus_driver->scanDevice();
}


void ListConsoles::busErrorData(unsigned id,BusDriver::Error err)
{
  QMessageBox::warning(this,tr("Network Error"),BusDriver::errorText(err));
}


void ListConsoles::busConsoleListData(unsigned id,
				      std::vector<ConsoleData *> *console_list)
{
  ConnViewItem *item=NULL;
  ConsoleData *console_data;

  for(unsigned i=0;i<console_list->size();i++) {
    item=new ConnViewItem(list_console_view);
    console_data=new ConsoleData(console_list->at(i)->name(),
				 console_list->at(i)->console(),
				 console_list->at(i)->mode(),
				 console_list->at(i)->isFree());
    item->setConsoleData(console_data);
    item->setText(0,console_list->at(i)->name());
    switch(console_list->at(i)->mode()) {
	case ConsoleData::Producer:
	  item->setText(1,tr("Producer"));
	  break;

	case ConsoleData::Talent:
	  item->setText(1,tr("Talent"));
	  break;

	case ConsoleData::Unavailable:
	  item->setText(1,tr("Unavailable"));
	  break;
    }
    if(console_list->at(i)->isFree()) {
      item->setText(2,tr("Yes"));
    }
    else {
      item->setText(2,tr("No"));
    }
  }
  list_question_label->setEnabled(true);
  list_console_view->setEnabled(true);
  list_scan_timer->stop();
}


void ListConsoles::listClickedData(Q3ListViewItem *item)
{
  list_ok_button->setDisabled(item==NULL);
  list_ok_button->setDefault(item!=NULL);
}


void ListConsoles::listDoubleclickedData(Q3ListViewItem *item,const QPoint &pt,
				      int col)
{
  okData();
}


void ListConsoles::okData()
{
  ConnViewItem *item=(ConnViewItem *)list_console_view->selectedItem();
  if(item==NULL) {
    return;
  }
  list_connection->setConsole(item->consoleData()->console());
  list_connection->setConsoleName(item->text(0));
  SaveConnection *save_conn=
    new SaveConnection(list_connection,this);
  if(save_conn->exec()<0) {
    delete save_conn;
    return;
  }
  delete save_conn;
  done(0);
}


void ListConsoles::cancelData()
{
  done(-1);
}


