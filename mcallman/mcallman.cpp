// mcallman.cpp
//
// The Call Manager Utility for CallCommander.
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


#include <stdlib.h>
#ifndef WIN32
#include <unistd.h>
#endif  // WIN32
#include <sys/types.h>
#include <QtGui/QApplication>
#include <QtGui/QPainter>
#include <QtSql/QSqlDatabase>
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <Qt3Support/q3filedialog.h>
#include <QtGui/QCloseEvent>
#include <QtGui/QPixmap>
#include <QtSql/QSqlQuery>

#include <ml.h>
#include <mldconfig.h>
#include <login.h>
#include <escapestring.h>

#include "globals.h"
#include "select_operation.h"
#include "mcallman.h"

//
// Global Classes
//
MldConfig *man_config;
QString man_loginname;
bool exiting=false;
QDateTime start_datetime(QDate::currentDate(),QTime());
QDateTime end_datetime(QDateTime(start_datetime.date(),QTime(23,59,59)));
int base_year=2005;

//
// Icons
//
#include "../icons/callcommander-22x22.xpm"


MainWidget::MainWidget(QWidget *parent,const char *name)
  :QWidget(parent,name)
{
  QString sql;
  QSqlQuery *q;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMaximumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumHeight(sizeHint().height());

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);
  QFont default_font("Helvetica",12,QFont::Normal);
  default_font.setPixelSize(12);
  QFont title_font=QFont("Helvetica",16,QFont::Bold);
  title_font.setPixelSize(16);
  qApp->setFont(default_font);

  //
  // Create And Set Icon
  //
  man_callcommander_map=new QPixmap(callcommander_xpm);
  setIcon(*man_callcommander_map);

  //
  // Load Configs
  //
  man_config=new MldConfig();
  man_config->load();

  //
  // Open Database Connection
  //
  if(!man_config->mysqlHostname().isEmpty()) {
    man_database=QSqlDatabase::addDatabase(man_config->mysqlDbtype());
    /*
    if(!man_database) {
      QMessageBox::warning(this,"Database Error",
			   "Unable to connect to the database server!");
      exit(1);
    }
    */
    man_database.setDatabaseName(man_config->mysqlDbname());
    man_database.setUserName(man_config->mysqlUsername());
    man_database.setPassword(man_config->mysqlPassword());
    man_database.setHostName(man_config->mysqlHostname());
    if(!man_database.open()) {
      man_database.removeDatabase(man_config->mysqlDbname());
      QMessageBox::warning(this,"Database Error",
			   "Unable to log into to the database server!");
      exit(1);
    }
  }

  //
  // Log In
  //
  QString password;
  Login *login=new Login(&man_loginname,&password,false,this);
  if(login->exec()!=0) {
    exit(0);
  }
  sql=QString().sprintf("select LOGIN_NAME from USERS where\
                         LOGIN_NAME=\"%s\" && PASSWORD=\"%s\"",
			(const char *)EscapeString(man_loginname),
			(const char *)EscapeString(password));
  q=new QSqlQuery(sql);
  if(q->size()<=0) {
    QMessageBox::warning(this,"Login Failed","Invalid Login!");
    exiting=true;
  }
  delete q;
  setCaption(QString().sprintf("Call Manager - User: %s",
			       (const char *)man_loginname));

  //
  // Action Label
  //
  QLabel *label=new QLabel("Select a show:",this,"main_title_label");
  label->setGeometry(10,5,sizeHint().width()-20,20);
  label->setFont(title_font);
  label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // Shows List
  //
  man_shows_list=new Q3ListView(this,"man_shows_list");
  man_shows_list->setGeometry(10,32,
			      sizeHint().width()-20,sizeHint().height()-100);
  man_shows_list->setAllColumnsShowFocus(true);
  connect(man_shows_list,
	  SIGNAL(doubleClicked(Q3ListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(doubleClickedData(Q3ListViewItem *,const QPoint &,int)));
  man_shows_list->addColumn("Title");
  man_shows_list->setColumnAlignment(0,Qt::AlignLeft|Qt::AlignVCenter);
  man_shows_list->addColumn("Show Code");
  man_shows_list->setColumnAlignment(1,Qt::AlignCenter);

  //
  // Open Button
  //
  QPushButton *button=new QPushButton(this,"open_button");
  button->setGeometry(sizeHint().width()/2-90,sizeHint().height()-60,80,50);
  button->setFont(font);
  button->setText("&Open");
  connect(button,SIGNAL(clicked()),this,SLOT(openData()));

  //
  // Close Button
  //
  button=new QPushButton(this,"close_button");
  button->setGeometry(sizeHint().width()/2+10,sizeHint().height()-60,80,50);
  button->setFont(font);
  button->setText("&Close");
  connect(button,SIGNAL(clicked()),this,SLOT(closeData()));

  RefreshList();
}


MainWidget::~MainWidget()
{
}


QSize MainWidget::sizeHint() const
{
  return QSize(280,270);
}


QSizePolicy MainWidget::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void MainWidget::doubleClickedData(Q3ListViewItem *item,const QPoint &pt,int c)
{
  openData();
}


void MainWidget::openData()
{
  Q3ListViewItem *item=man_shows_list->selectedItem();

  if(item==NULL) {
    return;
  }
  SelectOperation *sel=
    new SelectOperation(item->text(1),item->text(0),this,"edit");
  sel->exec();
  delete sel;
}


void MainWidget::closeData()
{
  exit(0);
}


void MainWidget::closeEvent(QCloseEvent *e)
{
  closeData();
}


void MainWidget::RefreshList()
{
  QString sql;
  QSqlQuery *q;
  Q3ListViewItem *item;

  sql=QString().sprintf("select SHOW_CODE from USER_PERMS \
                         where LOGIN_NAME=\"%s\"",
			(const char *)man_loginname);
  q=new QSqlQuery(sql);
  sql="select TITLE,SHOW_CODE from SHOWS where ";
  while(q->next()) {
    sql+=QString().sprintf("(SHOW_CODE=\"%s\")||",
			   (const char *)q->value(0).toString());
  }
  sql=sql.left(sql.length()-2);
  delete q;
  q=new QSqlQuery(sql);
  man_shows_list->clear();
  while (q->next()) {
    item=new Q3ListViewItem(man_shows_list);
    item->setText(0,q->value(0).toString());
    item->setText(1,q->value(1).toString());
  }
  delete q;
}


int main(int argc,char *argv[])
{
  QApplication a(argc,argv);
  
  MainWidget *w=new MainWidget(NULL,"main");
  if(exiting) {
      exit(0);
  }
  a.setMainWidget(w);
  w->setGeometry(w->geometry().x(),w->geometry().y(),
		 w->sizeHint().width(),w->sizeHint().height());
  w->show();
  return a.exec();
}


