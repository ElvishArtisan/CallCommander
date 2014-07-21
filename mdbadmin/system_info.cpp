// system_info.cpp
//
// Display System Information for Rivendell
//
//   (C) Copyright 2002-2003 Fred Gleason <fredg@paravelsystems.com>
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
#include <Qt3Support/q3textedit.h>
#include <QtGui/QLabel>

#include <mllabel.h>
#include <mllicense.h>

#include "system_info.h"

SystemInfo::SystemInfo(QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMaximumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumHeight(sizeHint().height());

  setCaption("Call Administrator - System Information");

  //
  // Create Fonts
  //
  QFont title_font=QFont("Helvetica",24,QFont::DemiBold);
  title_font.setPixelSize(22);
  QFont slogan_font=QFont("Helvetica",14,QFont::Normal);
  slogan_font.setPixelSize(14);
  QFont button_font=QFont("Helvetica",12,QFont::Bold);
  button_font.setPixelSize(12);
  QFont bold_font=QFont("Helvetica",10,QFont::Bold);
  bold_font.setPixelSize(10);
  QFont font=QFont("Helvetica",10,QFont::Normal);
  font.setPixelSize(10);

  //
  // Title
  //
  QLabel *label=new QLabel("CallCommander",this,"title_label");
  label->setGeometry(10,10,sizeHint().width()-20,36);
  label->setFont(title_font);

  //
  // Slogan
  //
  label=new QLabel("The Civilized Call Screener",this,"title_label");
  label->setGeometry(10,38,sizeHint().width()-20,18);
  label->setFont(slogan_font);

  //
  // Version
  //
  label=new QLabel(QString().sprintf("Version %s",VERSION),this,"title_label");
  label->setGeometry(10,58,200,14);
  label->setFont(font);

  //
  // Signature
  //
  label=new QLabel(QString().sprintf("Copyright 2002-2011 Fred Gleason <%s>","support@paravelsystems.com"),this,"title_label");
//  label=new QLabel(QString().sprintf("Copyright 2002-2011 Fred Gleason <%s>",PACKAGE_BUGREPORT),this,"title_label");
  label->setGeometry(10,73,sizeHint().width()-20,14);
  label->setFont(font);

  //
  // Disclaimer
  //
  label=new MLLabel(this,"title_label");
  label->setGeometry(10,90,sizeHint().width()-20,60);
  label->setFont(font);
  label->setText("This program is free software, and comes with ABSOLUTELY NO WARRANTY,\nnot even the implied warranties of MERCHANTIBILITY or FITNESS FOR A\nPARTICULAR PURPOSE.  Touch the \"View License\" button for details.");

  //
  // License Button
  //
  QPushButton *button=new QPushButton(this,"license_button");
  button->setGeometry(10,sizeHint().height()-60,80,50);
  button->setFont(button_font);
  button->setText("View\n&License");
  connect(button,SIGNAL(clicked()),this,SLOT(viewLicenseData()));

  //
  //  Close Button
  //
  button=new QPushButton(this,"close_button");
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,
			    80,50);
  button->setFont(button_font);
  button->setText("&Close");
  button->setDefault(true);
  connect(button,SIGNAL(clicked()),this,SLOT(closeData()));
}


QSize SystemInfo::sizeHint() const
{
  return QSize(460,220);
} 


QSizePolicy SystemInfo::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void SystemInfo::viewLicenseData()
{
  MLLicense *lic=new MLLicense(this,"license_dialog");
  lic->exec(MLLicense::GplV2);
  delete lic;
}


void SystemInfo::closeData()
{
  done(0);
}
