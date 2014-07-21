// print_report.cpp
//
// List the CallCommander Phone Number Directory.
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

#include <stdio.h>
#include <math.h>

#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtSql/QSqlDatabase>
#include <QtGui/QPainter>
#include <QtGui/QMessageBox>
#include <QtGui/QPrinter>
#include <QtSql/QSqlQuery>
#include <QtCore/QVariant>

#include <mldconfig.h>

#include "print_report.h"
#include "globals.h"

PrintReport::PrintReport(QString showcode,QString title,
			 QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  print_preview_page=0;

  setCaption(QString().sprintf("Call Manager - Report: %s",
			       (const char *)title));

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  print_showcode=showcode;
  print_title=title;

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);
  QFont small_font=QFont("Helvetica",10,QFont::Normal);
  small_font.setPixelSize(10);

  //
  // Preview Label
  //
  print_preview_label=new QLabel(this,"print_preview_label");
  print_preview_label->setAlignment(Qt::AlignLeft|Qt::AlignTop);
  print_preview_label->
    setGeometry(5,5,PREVIEW_X_SIZE,PREVIEW_Y_SIZE);
 
  //
  // Print Button
  //
  QPushButton *button=new QPushButton(this,"list_print_button");
  button->setGeometry(10,sizeHint().height()-60,80,50);
  button->setFont(font);
  button->setText("&Print");
  connect(button,SIGNAL(clicked()),this,SLOT(printData()));

  //
  // Close Button
  //
  button=new QPushButton(this,"list_close_button");
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setDefault(true);
  button->setFont(font);
  button->setText("&Close");
  connect(button,SIGNAL(clicked()),this,SLOT(closeData()));

  //
  // Load Filter and Report Values
  //
  LoadFilter(showcode,title);

  //
  // Draw the Preview
  //
  DrawPreview();
  if(print_preview.size()>print_preview_page) {
    print_preview_label->setPixmap(*print_preview[print_preview_page]);
  }
}


PrintReport::~PrintReport()
{
}


QSize PrintReport::sizeHint() const
{
  return QSize(PREVIEW_X_SIZE+10,PREVIEW_Y_SIZE+75);
} 


QSizePolicy PrintReport::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void PrintReport::printData()
{
  DrawReport();
}


void PrintReport::closeData()
{
  done(0);
}


void PrintReport::LoadFilter(QString showcode,QString title)
{
  QString sql;
  QSqlQuery *q;

  //
  // Report Values
  //
  sql=QString().sprintf("select TYPE,HEADER1,HEADER2 from REPORTS where\
                         (SHOW_CODE=\"%s\")&&(TITLE=\"%s\")",
			(const char *)showcode,
			(const char *)title);
  q=new QSqlQuery(sql);
  if(q->first()) {
    print_type=q->value(0).toInt();
    print_header[0]=q->value(1).toString();
    print_header[1]=q->value(2).toString();
  }
  delete q;

  //
  // Filter Values
  //
  sql=QString().sprintf("select INCLUDE_LINE1,INCLUDE_LINE2,INCLUDE_LINE3,\
                         INCLUDE_LINE4,INCLUDE_LINE5,INCLUDE_LINE6,\
                         INCLUDE_LINE7,INCLUDE_LINE8,INCLUDE_LINE9,\
                         INCLUDE_LINE10,INCLUDE_LINE11,INCLUDE_LINE12,\
                         START_AGE,END_AGE,CELLPHONE,CITY,STATE,ZIPCODE,\
                         STATION,GENDER from REPORTS where\
                         (SHOW_CODE=\"%s\")&&(TITLE=\"%s\")",
			(const char *)showcode,(const char *)title);
  q=new QSqlQuery(sql);
  if(q->first()) {
    //
    // Included Lines
    //
    print_filter="(";
    for(int i=0;i<MAX_LINES;i++) {
      if(q->value(i).toString()=="Y") {
	print_filter+=QString().sprintf("(LINE_NUMBER=%d)||",i+1);
      }
    }
    if(print_filter.length()>1) {
      print_filter=print_filter.left(print_filter.length()-2);
      print_filter+=")&&";
    }
    else {
      print_filter="";
    }

    //
    // Date
    //
    print_filter+=
      QString().sprintf("(CALL_DATE>=\"%s\")&&(CALL_DATE<=\"%s\")",
			(const char *)start_datetime.toString("yyyy/MM/dd"),
			(const char *)end_datetime.toString("yyyy/MM/dd"));

    //
    // Time
    //
    print_filter+=
      QString().sprintf("&&(CALL_TIME>=\"%s\")&&(CALL_TIME<=\"%s\")&&",
			(const char *)start_datetime.toString("hh:mm:ss"),
			(const char *)end_datetime.toString("hh:mm:ss"));

    //
    // Age
    //
    print_filter+=QString().sprintf("(AGE>=%d)&&(AGE<=%d)",
				    q->value(12).toInt(),
				    q->value(13).toInt());
    //
    // City
    //
    if(q->value(15).toString().length()>0) {
      print_filter+=QString().sprintf("&&(CITY=\"%s\")",
				      (const char *)q->value(15).toString());
    }

    //
    // State
    //
    if(q->value(16).toString().length()>0) {
      print_filter+=QString().sprintf("&&(STATE=\"%s\")",
				      (const char *)q->value(16).toString());
    }

    //
    // Zip Code
    //
    if(q->value(17).toString().length()>0) {
      print_filter+=QString().sprintf("&&(ZIPCODE=\"%s\")",
				      (const char *)q->value(17).toString());
    }

    //
    // Station
    //
    if(q->value(18).toString().length()>0) {
      print_filter+=QString().sprintf("&&(STATION=\"%s\")",
				      (const char *)q->value(18).toString());
    }

    //
    // Gender
    //
    if(!q->value(19).isNull()) {
      print_filter+=QString().sprintf("&&(GENDER=\"%s\")",
				      (const char *)q->value(19).toString());
    }
  }
  delete q;
}


void PrintReport::DrawPreview()
{
  switch(print_type) {
      case 0:  // Standard Age Report
	StandardAgeReport(&print_preview);
	break;

      case 1:  // Area Code Report
	AreaCodeReport(&print_preview);
	break;

      default:
	fprintf(stderr,"Unknown report type\n");
	break;
  }
}


void PrintReport::DrawReport()
{
  QPrinter *printer=new QPrinter(QPrinter::PrinterResolution);
  printer->setOrientation(QPrinter::Portrait);
  printer->setPageSize(QPrinter::Letter);
  printer->setFullPage(true);
  printer->setNumCopies(1);
  printer->setMinMax(1,print_preview.size());
  printer->setOptionEnabled(QPrinter::PrintToFile,true);
  printer->setOptionEnabled(QPrinter::PrintSelection,false);
  printer->setOptionEnabled(QPrinter::PrintPageRange,false);
  if(printer->setup(this)) {
    switch(print_type) {
	case 0:  // Standard Age Report
	  if(!StandardAgeReport(printer)) {
	    QMessageBox::warning(this,"Printer Error",
				 "The printer returned an error!");
	  }
	  break;
	  
	case 1:  // Area Code Report
	  if(!AreaCodeReport(printer)) {
	    QMessageBox::warning(this,"Printer Error",
				 "The printer returned an error!");
	  }
	  break;
	  
	default:
	    QMessageBox::warning(this,"Printer Error","Unknown report!");
	  break;
    }
  }
  delete printer;
}
