// list_clients.cpp
//
// A Client List Dialog for CallCommander
//
//   (C) Copyright 2002-2008 Fred Gleason <fredg@paravelsystems.com>
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
#include <QtCore/QTimer>

#include "client_listviewitem.h"
#include "list_clients.h"

//
// Icons
//
#include "../icons/client.xpm"
#include "../icons/client_segment.xpm"
#include "../icons/logic.xpm"
#include "../icons/monitor.xpm"
#include "../icons/recorder.xpm"

ListClients::ListClients(BusDriver *driver,QWidget *parent,const char *name)
  : QDialog(parent,name,false)
{
  list_driver=driver;
  list_width=550;

  setCaption(tr("Call Screener - Currently Online"));

  //
  // Create Icons
  //
  list_client_map=new QPixmap(client_xpm);
  list_client_segment_map=new QPixmap(client_segment_xpm);
  list_logic_map=new QPixmap(logic_xpm);
  list_monitor_map=new QPixmap(monitor_xpm);
  list_recorder_map=new QPixmap(recorder_xpm);

  //
  // Client List
  //
  list_client_list=new ClientListView(this,"list_client_list");
  list_client_list->setAllColumnsShowFocus(true);
  list_client_list->setItemMargin(5);

  list_client_list->addColumn("");
  list_client_list->setColumnAlignment(0,Qt::AlignCenter);

  list_client_list->addColumn(tr("User Name"));
  list_client_list->setColumnAlignment(1,Qt::AlignCenter);

  list_client_list->addColumn(tr("Online Since"));
  list_client_list->setColumnAlignment(2,Qt::AlignCenter);

  list_client_list->addColumn(tr("Coming From"));
  list_client_list->setColumnAlignment(3,Qt::AlignCenter);

  list_client_list->addColumn(tr("Console"));
  list_client_list->setColumnAlignment(4,Qt::AlignCenter);

  list_client_list->addColumn(tr("Mode"));
  list_client_list->setColumnAlignment(5,Qt::AlignCenter);

  list_client_list->addColumn(tr("Client Type"));
  list_client_list->setColumnAlignment(6,Qt::AlignCenter);

  list_client_list->addColumn(tr("Version"));
  list_client_list->setColumnAlignment(7,Qt::AlignCenter);

  connect(driver,
	  SIGNAL(clientListChanged(unsigned,std::vector <ClientData *> *)),
	  this,
	  SLOT(clientListChangedData(unsigned,std::vector <ClientData *> *)));

  QTimer *timer=new QTimer(this,"population_timer");
  connect(timer,SIGNAL(timeout()),this,SLOT(populateListData()));
  timer->start(5000,true);
}


QSize ListClients::sizeHint() const
{
  return QSize(list_width,330);
} 


QSizePolicy ListClients::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ListClients::clientListChangedData(unsigned id,
					std::vector <ClientData *> *list)
{
  list_client_list->clear();
  ClientListViewItem *item=NULL;
  for(unsigned i=0;i<list->size();i++) {
    item=new ClientListViewItem(list_client_list);
    item->setLine(i);
    switch(list->at(i)->clientType()) {
      case ClientData::TypeUser:
	if(list->at(i)->segmentSource()) {
	  item->setPixmap(0,*list_client_segment_map);
	}
	else {
	  item->setPixmap(0,*list_client_map);
	}
	break;

      case ClientData::TypeCidMonitor:
	item->setPixmap(0,*list_monitor_map);
	break;

      case ClientData::TypeAudioRecorder:
	item->setPixmap(0,*list_recorder_map);
	break;

      case ClientData::TypeLogicModule:
	item->setPixmap(0,*list_logic_map);
	break;

      case ClientData::TypeUnknown:
	break;
    }
    item->setText(1,list->at(i)->userName());
    item->setText(2,list->at(i)->loginDateTime().
		  toString("hh:mm:ss - MM/dd/yyyy"));
    item->setText(3,QString().sprintf("%s:%d",(const char *)list->at(i)->
				      ipAddress().toString(),
				      list->at(i)->tcpPort()));
    item->setText(4,list->at(i)->consoleName());
    item->setText(5,ConsoleData::modeText(list->at(i)->consoleMode()));
    item->setText(6,ClientData::clientText(list->at(i)->clientType()));
    if(list->at(i)->version()>0) {
      item->setText(7,list->at(i)->versionString());
    }
    else {
      item->setText(7,tr("Unknown"));
    }
  }
}


void ListClients::populateListData()
{
  clientListChangedData(0,list_driver->clientList());
}


void ListClients::resizeEvent(QResizeEvent *e)
{
  list_client_list->setGeometry(0,0,e->size().width(),e->size().height());
}


void ListClients::showEvent(QShowEvent *e)
{
  list_width=4;
  for(int i=0;i<list_client_list->columns();i++) {
    list_width+=list_client_list->columnWidth(i);
  }
  setGeometry(100,100,list_width,size().height());
  list_client_list->setGeometry(0,0,list_width,size().height());
}
