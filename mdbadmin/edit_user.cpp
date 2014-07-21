// edit_show.cpp
//
// Edit a CallCommander User.
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
#include <QtGui/QLabel>
#include <QtSql/QSqlDatabase>
#include <QtGui/QMessageBox>
#include <QtSql/QSqlQuery>

#include <math.h>

#include <escapestring.h>
#include <change_password.h>

#include "globals.h"
#include "edit_user.h"

EditUser::EditUser(QString loginname,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  edit_loginname=loginname;
  edit_password_changed=false;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setCaption("Call Administrator - Edit User");

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);

  //
  // Login Name
  //
  edit_loginname_edit=new QLineEdit(this,"edit_loginname_edit");
  edit_loginname_edit->setGeometry(200,10,80,20);
  edit_loginname_edit->setFont(font);
  edit_loginname_edit->setReadOnly(true);
  QLabel *label=
    new QLabel(edit_loginname_edit,"Login Name:",this,"edit_loginname_label");
  label->setGeometry(10,10,185,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Full Name
  //
  edit_fullname_edit=new QLineEdit(this,"edit_fullname_edit");
  edit_fullname_edit->setGeometry(200,32,sizeHint().width()-210,20);
  edit_fullname_edit->setFont(font);
  edit_fullname_edit->setMaxLength(64);
  label=new QLabel(edit_fullname_edit,"Full Name:",this,"edit_fullname_label");
  label->setGeometry(10,32,185,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Description
  //
  edit_description_edit=new QLineEdit(this,"edit_description_edit");
  edit_description_edit->setGeometry(200,54,sizeHint().width()-210,20);
  edit_description_edit->setFont(font);
  edit_description_edit->setMaxLength(64);
  label=new QLabel(edit_description_edit,"Description:",
		   this,"edit_description_label");
  label->setGeometry(10,54,185,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Phone Number
  //
  edit_phone_edit=new QLineEdit(this,"edit_phone_edit");
  edit_phone_edit->setGeometry(200,76,120,20);
  edit_phone_edit->setFont(font);
  edit_phone_edit->setMaxLength(14);
  label=new QLabel(edit_phone_edit,"Phone Number:",this,"edit_phone_label");
  label->setGeometry(10,76,185,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Screening Priviledge
  //
  edit_screen_box=new QComboBox(this,"edit_screen_box");
  edit_screen_box->setGeometry(260,98,60,20);
  edit_screen_box->setFont(font);
  edit_screen_box->insertItem("Yes");
  edit_screen_box->insertItem("No");
  label=new QLabel(edit_screen_box,"User Can Screen Calls (virtual only):",
		   this,"edit_screen_label");
  label->setGeometry(10,98,245,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Admin Priviledge
  //
  edit_admin_box=new QComboBox(this,"edit_admin_box");
  edit_admin_box->setGeometry(260,120,60,20);
  edit_admin_box->setFont(font);
  edit_admin_box->insertItem("Yes");
  edit_admin_box->insertItem("No");
  label=new QLabel(edit_admin_box,"User Has Admin Rights:",
		   this,"edit_admin_label");
  label->setGeometry(10,120,245,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);
  connect(edit_admin_box,SIGNAL(activated(int)),
	  this,SLOT(adminActivatedData(int)));

  //
  // Change Password Button
  //
  QPushButton *button=new QPushButton(this,"passwd_button");
  button->setGeometry(340,76,90,44);
  button->setFont(label_font);
  button->setText("Change\n&Password");
  connect(button,SIGNAL(clicked()),this,SLOT(changePasswordData()));

  //
  // Show Selector
  //
  edit_show_sel=new MLListSelector(this,"edit_show_sel");
  edit_show_sel->setGeometry(50,152,edit_show_sel->sizeHint().width(),
			     edit_show_sel->sizeHint().height());
  edit_show_sel->sourceSetLabel("Available Shows");
  edit_show_sel->destSetLabel("Allowed Shows");

  //
  //  OK Button
  //
  button=new QPushButton(this,"ok_button");
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
  QString sql=QString().sprintf("select FULL_NAME,DESCRIPTION,PHONE_NUMBER,\
                                 ADMIN_PRIV,SCREENER_PRIV from USERS where \
                                 LOGIN_NAME=\"%s\"",\
				(const char *)edit_loginname);
  QSqlQuery *q=new QSqlQuery(sql);
  if(q->first()) {
    edit_loginname_edit->setText(edit_loginname);
    edit_fullname_edit->setText(q->value(0).toString());
    edit_description_edit->setText(q->value(1).toString());
    edit_phone_edit->setText(q->value(2).toString());
    if(q->value(3).toString().lower()=="y") {
      edit_admin_box->setCurrentItem(0);
    }
    else {
      edit_admin_box->setCurrentItem(1);
    }
    if(q->value(4).toString().lower()=="y") {
      edit_screen_box->setCurrentItem(0);
    }
    else {
      edit_screen_box->setCurrentItem(1);
    }
  }
  delete q;

  sql=QString().sprintf("select SHOW_CODE from USER_PERMS \
                         where LOGIN_NAME=\"%s\"",
			(const char *)edit_loginname);
  q=new QSqlQuery(sql);
  while(q->next()) {
    edit_show_sel->destInsertItem(q->value(0).toString());
  }
  delete q;

  sql=QString().sprintf("select SHOW_CODE from SHOWS");
  q=new QSqlQuery(sql);
  while(q->next()) {
    if(edit_show_sel->destFindItem(q->value(0).toString())==0) {
      edit_show_sel->sourceInsertItem(q->value(0).toString());
    }
  }
  delete q;
}


EditUser::~EditUser()
{
}


QSize EditUser::sizeHint() const
{
  return QSize(450,344);
} 


QSizePolicy EditUser::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditUser::changePasswordData()
{
  ChangePassword *change=new ChangePassword(&edit_password,this,"change");
  if(change->exec()==0) {
    edit_password_changed=true;
  }
  delete change;
}


void EditUser::adminActivatedData(int item)
{
  if((item==1)&&(admin_loginname==edit_loginname)) {
    QMessageBox::information(this,"Current User",
	"You cannot revoke administrative\nprivileges on the current user!");
    edit_admin_box->setCurrentItem(0);
    return;
  }
}


void EditUser::okData()
{
  QString admin_priv;
  QString screen_priv;

  //
  // Update User Record
  //
  if(edit_admin_box->currentItem()==0) {
    admin_priv="Y";
  }
  else {
    admin_priv="N";
  }
  if(edit_screen_box->currentItem()==0) {
    screen_priv="Y";
  }
  else {
    screen_priv="N";
  }
  QString sql=QString().sprintf("update USERS set\
                                 FULL_NAME=\"%s\",\
                                 DESCRIPTION=\"%s\",\
                                 PHONE_NUMBER=\"%s\",\
                                 ADMIN_PRIV=\"%s\",\
                                 SCREENER_PRIV=\"%s\"\
                                 where LOGIN_NAME=\"%s\"",
				(const char *)edit_fullname_edit->text(),
				(const char *)edit_description_edit->text(),
				(const char *)edit_phone_edit->text(),
				(const char *)admin_priv,
				(const char *)screen_priv,
				(const char *)edit_loginname);
  QSqlQuery *q=new QSqlQuery(sql);
  delete q;

  //
  // Update Password
  //
  if(edit_password_changed) {
    sql=QString("update USERS set PASSWORD=\"")+
      EscapeString(edit_password)+"\" "+
      "where LOGIN_NAME=\""+EscapeString(edit_loginname)+"\"";
    QSqlQuery *q=new QSqlQuery(sql);
    delete q;
  }

  //
  // Add New Shows
  //
  for(unsigned i=0;i<edit_show_sel->destCount();i++) {
    sql=QString().sprintf("select SHOW_CODE from USER_PERMS \
                           where LOGIN_NAME=\"%s\" && SHOW_CODE=\"%s\"",
			  (const char *)edit_loginname,
			  (const char *)edit_show_sel->destText(i));
    q=new QSqlQuery(sql);
    if(q->size()==0) {
      delete q;
      sql=QString().
	sprintf("insert into USER_PERMS (LOGIN_NAME,SHOW_CODE) \
                 values (\"%s\",\"%s\")",
		(const char *)edit_loginname,
		(const char *)edit_show_sel->destText(i));
      q=new QSqlQuery(sql);
    }
    delete q;
  }

  //
  // Delete Old Shows
  //
  sql=QString().sprintf("delete from USER_PERMS where LOGIN_NAME=\"%s\"",
			(const char *)edit_loginname);
  for(unsigned i=0;i<edit_show_sel->destCount();i++) {
    sql+=QString().sprintf(" && SHOW_CODE<>\"%s\"",
			   (const char *)edit_show_sel->destText(i));
  }
  q=new QSqlQuery(sql);
  delete q;

  done(0);
}


void EditUser::cancelData()
{
  done(-1);
}
