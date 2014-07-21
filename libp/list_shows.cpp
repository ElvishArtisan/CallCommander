// list_shows.cpp
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
#include "list_shows.h"
#include "list_consoles.h"
#include "telos2101_driver.h"

ListShows::ListShows(ConnectionData *conn,BusDriver *driver,
		     QWidget *parent,const char *name)
  : QDialog(parent,name,true)
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

  setCaption(tr("Available Shows"));

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
    new QLabel(tr("Select a Show to associate with this connection."),this);
  list_question_label->setGeometry(10,5,sizeHint().width()-20,20);
  list_question_label->setFont(button_font);
  list_question_label->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  list_question_label->setDisabled(true);

  //
  // Show List
  //
  list_show_view=new Q3ListView(this,"list_show_view");
  list_show_view->
    setGeometry(10,30,sizeHint().width()-20,80);
  list_show_view->setFont(font);
  list_show_view->setAllColumnsShowFocus(true);
  list_show_view->setItemMargin(5);
  connect(list_show_view,
	  SIGNAL(doubleClicked(Q3ListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(listDoubleclickedData(Q3ListViewItem *,const QPoint &,int)));
  connect(list_show_view,SIGNAL(clicked(Q3ListViewItem *)),
	  this,SLOT(listClickedData(Q3ListViewItem *)));
  list_show_view->addColumn(tr("Show Name"));
  list_show_view->setColumnAlignment(0,Qt::AlignLeft);
  list_show_view->addColumn(tr("Location"));
  list_show_view->setColumnAlignment(1,Qt::AlignLeft);
  list_show_view->addColumn(tr("Active"));
  list_show_view->setColumnAlignment(2,Qt::AlignCenter);
  list_show_view->setDisabled(true);

  QPushButton *button;

  //
  // Password
  //
  list_password_edit=new QLineEdit(this,"list_password_edit");
  list_password_edit->
    setGeometry(100,sizeHint().height()-90,sizeHint().width()-130,20);
  list_password_edit->setFont(font);
  list_password_edit->setEchoMode(QLineEdit::Password);
  list_password_edit->setDisabled(true);
  list_password_label=new QLabel(list_password_edit,tr("Password:"),
		   this,"list_password_label");
  list_password_label->setGeometry(10,sizeHint().height()-90,85,20);
  list_password_label->setFont(button_font);
  list_password_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  list_password_label->setDisabled(true);
  if(conn->subType()==1) {
    list_password_edit->hide();
    list_password_label->hide();
  }

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
	  SIGNAL(currentShowList(unsigned,std::vector <ShowData *> *)),
	  this,
	  SLOT(busShowListData(unsigned,std::vector <ShowData *> *)));
  connect(list_bus_driver,SIGNAL(currentShow(unsigned,const QString &,bool)),
	  this,SLOT(busShowData(unsigned,const QString &,bool)));
  list_bus_driver->requestShowList();
}


ListShows::~ListShows()
{
}


QSize ListShows::sizeHint() const
{
  if(list_connection->subType()==1) {
    return QSize(300,188);
  }
  return QSize(300,210);
}


QSizePolicy ListShows::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ListShows::scanDeviceData()
{
  list_bus_driver->scanDevice();
}


void ListShows::busErrorData(unsigned id,BusDriver::Error err)
{
  QMessageBox::warning(this,tr("Network Error"),BusDriver::errorText(err));
}


void ListShows::busShowListData(unsigned id,
				std::vector <ShowData *> *show_list)
{
  ConnViewItem *item=NULL;
  ShowData *show_data;

  for(unsigned i=0;i<show_list->size();i++) {
    item=new ConnViewItem(list_show_view);
    show_data=new ShowData(show_list->at(i)->name(),
			   show_list->at(i)->hostName(),
			   show_list->at(i)->isActive(),
			   show_list->at(i)->passwordRequired());
    item->setShowData(show_data);
    item->setText(0,show_list->at(i)->name());
    item->setText(1,show_list->at(i)->hostName());
    if(show_list->at(i)->isActive()) {
      item->setText(2,tr("Yes"));
    }
    else {
      item->setText(2,tr("No"));
    }
  }
  list_question_label->setEnabled(true);
  list_show_view->setEnabled(true);
}


void ListShows::busShowData(unsigned id,const QString &show,bool state)
{
  if(state) {
    if(show!=list_showname) {
      QMessageBox::warning(this,tr("Show Error"),tr("Show name mismatch!"));
      return;
    }
    list_connection->setShowName(show);
    list_connection->setShowPassword(list_showpasswd);
    list_connection->setLocation(list_showlocation);
    ListConsoles *list_consoles=
      new ListConsoles(list_connection,list_bus_driver,this);
    if(list_consoles->exec()<0) {
      delete list_consoles;
      return;
    }
    delete list_consoles;
    done(0);
  }
  else {
    QMessageBox::warning(this,tr("Show Error"),tr("The password is invalid!"));
  }
}


void ListShows::listClickedData(Q3ListViewItem *item)
{
  list_password_label->setDisabled(item==NULL);
  list_password_edit->setDisabled(item==NULL);
  list_ok_button->setDisabled(item==NULL);
  list_ok_button->setDefault(item!=NULL);
}


void ListShows::listDoubleclickedData(Q3ListViewItem *item,const QPoint &pt,
				      int col)
{
  okData();
}


void ListShows::okData()
{
  ConnViewItem *item=(ConnViewItem *)list_show_view->selectedItem();
  if(item==NULL) {
    return;
  }
  list_showname=item->text(0);
  list_showpasswd=list_password_edit->text();
  list_showlocation=item->text(1);
  list_bus_driver->connectToShow(item->text(0),list_password_edit->text(),
				 item->text(1));
}


void ListShows::cancelData()
{
  done(-1);
}


