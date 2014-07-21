// area_code_report.cpp
//
// Paint a page of an Area Code Report.
//
//   (C) Copyright 2002-2005 Fred Gleason <fredg@paravelsystems.com>
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

#include <QtGui/QPainter>
#include <QtGui/QPrinter>
#include <QtSql/QSqlDatabase>
#include <QtGui/QMessageBox>
#include <QtGui/QPixmap>
#include <QtSql/QSqlQuery>
#include <QtCore/QVariant>

#include "print_report.h"
#include "globals.h"
#include "peg_counter.h"

bool PrintReport::AreaCodeReport(vector<QPixmap *> *pages)
{
  pages->push_back(new QPixmap(PREVIEW_X_SIZE,PREVIEW_Y_SIZE));
  QPainter *p=new QPainter();
  if(!p->begin(pages->back())) {
    delete p;
    return false;
  }
  while(paintAreaCodeReport(p)) {
    p->end();
    pages->push_back(new QPixmap(PREVIEW_X_SIZE,PREVIEW_Y_SIZE));
    p->begin(pages->back());
  }
  p->end();
  delete p;
  return true;
}


bool PrintReport::AreaCodeReport(QPrinter *ptr)
{
  QPainter *p=new QPainter();
  if(!p->begin(ptr)) {
    delete p;
    return false;
  }
  while(paintAreaCodeReport(p)) {
    ptr->newPage();
  }
  p->end();
  delete p;
  return true;
}


bool PrintReport::paintAreaCodeReport(QPainter *p)
{
  QString str;
  QString sql;
  QSqlQuery *q;
  PegCounter areacodes;
  PegCounter numbers;
  bool ok;
  unsigned nullnumbers=0;

  p->fillRect(0,0,p->window().width(),p->window().height(),Qt::white);
  //
  // Display Header
  //
  p->setPen(Qt::black);
  p->setBrush(Qt::black);
  p->setFont(QFont("helvetica",16,QFont::Bold));
  p->drawText((p->window().width()-p->fontMetrics().width(print_header[0]))/2,
	      (int)(p->window().height()*.1263),print_header[0]);
  p->setFont(QFont("helvetica",14,QFont::Normal));
  p->drawText((p->window().width()-p->fontMetrics().width(print_header[1]))/2,
	      (int)(p->window().height()*.1515),print_header[1]);
  str=QString().sprintf("Calls from %s to %s",
			(const char *)start_datetime.toString("MM/dd/yyyy"),
			(const char *)end_datetime.toString("MM/dd/yyyy"));
  p->setFont(QFont("helvetica",12,QFont::Normal));
  p->drawText((p->window().width()-p->fontMetrics().width(str))/2,
	      (int)(p->window().height()*.1894),str);

  //
  // Generate the Peg Counts
  //
  sql=QString().sprintf("select NUMBER from DETAILS_%s\
                         where %s order by NUMBER",
			(const char *)print_showcode,
			(const char *)print_filter);
  q=new QSqlQuery(sql);
  while(q->next()) {
    str=q->value(0).toString();
    str.left(3).toInt(&ok);
    if(ok) {
      numbers.addItem(str);
      areacodes.addItem(str.left(3));
    }
    else {
      nullnumbers++;
    }
  }

  //
  // Print the Summary
  //
  p->drawText(p->window().width()/8,(int)(p->window().height()*.2305),
	      QString().sprintf("Total Calls"));
  str=QString().sprintf("%d",q->size());
  p->drawText(p->window().width()*7/8-p->fontMetrics().width(str),
	      (int)(p->window().height()*.2525),str);

  p->drawText(p->window().width()/8,(int)(p->window().height()*.2525),
	      QString().sprintf("Total Area Codes"));
  str=QString().sprintf("%d",areacodes.size());
  p->drawText(p->window().width()*7/8-p->fontMetrics().width(str),
	      (int)(p->window().height()*.2745),str);

  p->drawText(p->window().width()/8,(int)(p->window().height()*.2745),
	      QString().sprintf("Total Unique Numbers"));
  str=QString().sprintf("%d",numbers.size());
  p->drawText(p->window().width()*7/8-p->fontMetrics().width(str),
	      (int)(p->window().height()*.2965),str);

  p->drawText(p->window().width()/8,(int)(p->window().height()*.2965),
	      QString().sprintf("Total Blocked Number Calls"));
  str=QString().sprintf("%u",nullnumbers);
  p->drawText(p->window().width()*7/8-p->fontMetrics().width(str),
	      (int)(p->window().height()*.3185),str);

  p->drawText(p->window().width()/8,(int)(p->window().height()*.3295),
	      QString().sprintf("Area Codes:"));
  str=QString().sprintf("%u",nullnumbers);
  p->drawText(p->window().width()*7/8-p->fontMetrics().width(str),
	      (int)(p->window().height()*.3185),str);

  delete q;

  //
  // Print the Counts
  //
  p->setFont(QFont("helvetica",10,QFont::Normal));
  for(int i=0;i<areacodes.size();i+=4) {
    p->drawText(p->window().width()*3/16,
		(int)(p->window().height()*.3645)+i*p->window().height()/180,
		QString().sprintf("%s [%d]",
				  (const char *)areacodes.itemName(i),
				  areacodes.itemCount(i)));

    if((i+1)<areacodes.size()) {
      p->drawText(p->window().width()*6/16,
		  (int)(p->window().height()*.3645)+i*p->window().height()/180,
		  QString().sprintf("%s [%d]",
				    (const char *)areacodes.itemName(i+1),
				    areacodes.itemCount(i+1)));
    }

    if((i+2)<areacodes.size()) {
      p->drawText(p->window().width()*9/16,
		  (int)(p->window().height()*.3645)+i*p->window().height()/180,
		  QString().sprintf("%s [%d]",
				    (const char *)areacodes.itemName(i+2),
				    areacodes.itemCount(i+2)));
    }

    if((i+3)<areacodes.size()) {
      p->drawText(p->window().width()*12/16,
		  (int)(p->window().height()*.3645)+i*p->window().height()/180,
		  QString().sprintf("%s [%d]",
				    (const char *)areacodes.itemName(i+3),
				    areacodes.itemCount(i+3)));
    }
  }

  //
  // Print the Footer
  //
  p->setFont(QFont("helvetica",6,QFont::Light));
  p->drawText((int)(p->window().width()/8.5),(int)(p->window().height()*.9090),
	      "Generated by CallCommander");
  p->drawText(p->window().width()-p->fontMetrics().
	      width(WEB_URL)-(int)(p->window().width()/8.5),
	      (int)(p->window().height()*.9090),WEB_URL);

  return false;
}
