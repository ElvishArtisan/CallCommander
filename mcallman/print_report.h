// print_report.h
//
// Print a CallCommander Report.
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

#ifndef PRINT_REPORT_H
#define PRINT_REPORT_H

#include <vector>

#include <QtGui/QDialog>
#include <Qt3Support/q3listview.h>
#include <QtGui/QLineEdit>
#include <QtGui/QLabel>
#include <QtGui/QPixmap>

//#define PREVIEW_X_SIZE 502
//#define PREVIEW_Y_SIZE 650
#define PREVIEW_X_SIZE 612
#define PREVIEW_Y_SIZE 792
#define WEB_URL "http://www.salemradiolabs.com/callcommander/"

class PrintReport : public QDialog
{
 Q_OBJECT
 public:
  PrintReport(QString showcode,QString title,
	      QWidget *parent=0,const char *name=0);
  ~PrintReport();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void printData();
  void closeData();

 private:
  void LoadFilter(QString showcode,QString title);
  void DrawPreview();
  void DrawReport();
  QString print_showcode;
  QString print_title;
  int print_type;
  QString print_header[2];
  QLabel *print_preview_label;
  unsigned print_preview_page;
  QString print_filter;
  std::vector<QPixmap *> print_preview;
  
  //
  // Standard Age Report
  //
  bool StandardAgeReport(std::vector<QPixmap *> *pages);
  bool StandardAgeReport(QPrinter *ptr);
  void paintStandardAgeReport(QPainter *p);
  void SARWriteLine(QPainter *p,int start_age,int end_age,int width,int height,
		    int ypos,int total);
  void SARGetCallers(int callers[3],int start_age,int end_age);


  //
  // Area Code Report
  //
  bool AreaCodeReport(std::vector<QPixmap *> *pages);
  bool AreaCodeReport(QPrinter *ptr);
  bool paintAreaCodeReport(QPainter *p);
};


#endif  // PRINT_REPORT_H
