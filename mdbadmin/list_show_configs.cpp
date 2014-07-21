// list_show_configs.cpp
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

#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtSql/QSqlDatabase>
#include <QtGui/QResizeEvent>
#include <QtSql/QSqlQuery>
#include <QtGui/QMessageBox>

#include <math.h>

#include <mldconfig.h>
#include "list_show_configs.h"
#include "edit_show.h"
#include "add_show.h"
#include "shows.h"

ListShowConfigs::ListShowConfigs(QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());

  setCaption("Call Administrator - Show Configurations");

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);
  QFont small_font=QFont("Helvetica",10,QFont::Normal);
  small_font.setPixelSize(10);

  //
  // Shows List
  //
  list_shows_list=new Q3ListView(this,"list_shows_list");
  list_shows_list->setAllColumnsShowFocus(true);
  list_shows_list->setItemMargin(5);
  connect(list_shows_list,
	  SIGNAL(doubleClicked(Q3ListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(doubleClickedData(Q3ListViewItem *,const QPoint &,int)));
  list_shows_list->addColumn("Title");
  list_shows_list->setColumnAlignment(0,Qt::AlignLeft|Qt::AlignVCenter);
  list_shows_list->addColumn("Show Code");
  list_shows_list->setColumnAlignment(1,Qt::AlignCenter);
  list_shows_list->addColumn("Default Label Source");
  list_shows_list->setColumnAlignment(2,Qt::AlignCenter);

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


ListShowConfigs::~ListShowConfigs()
{
}


QSize ListShowConfigs::sizeHint() const
{
  return QSize(400,300);
} 


QSizePolicy ListShowConfigs::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ListShowConfigs::addData()
{
  QString showcode;
  QString sql;
  QSqlQuery *q;

  AddShow *add=new AddShow(&showcode,this,"add");
  if(add->exec()==0) {
    CreateShow(showcode,"");
    EditShow *edit=new EditShow(showcode,this,"edit");
    if(edit->exec()==0) {
      Q3ListViewItem *item=new Q3ListViewItem(list_shows_list);
      item->setText(1,showcode);
      UpdateItem(item);
      list_shows_list->setSelected(item,true);
      list_shows_list->ensureItemVisible(item);
    }
    else {
      sql=QString().sprintf("delete from SHOWS where SHOW_CODE=\"%s\"",
			    (const char *)showcode);
      q=new QSqlQuery(sql);
      delete q;
    }
    delete edit;
  }
  delete add;
}


void ListShowConfigs::editData()
{
  Q3ListViewItem *item=list_shows_list->selectedItem();
  
  if(item==NULL) {
    return;
  }
  EditShow *edit=new EditShow(item->text(1),this,"edit");
  if(edit->exec()==0) {
    UpdateItem(item);
  }
  delete edit;
}


void ListShowConfigs::deleteData()
{
  Q3ListViewItem *item=list_shows_list->selectedItem();

  if(item==NULL) {
    return;
  }
  if(QMessageBox::question(this,"Delete Show",
      QString().sprintf("Are you sure you want to delete the entry for \"%s\"",
			   (const char *)item->text(0)),
			   QMessageBox::Yes,
			   QMessageBox::No)==QMessageBox::No) {
    return;
  }
  DeleteShow(item->text(1));
  delete item;
}


void ListShowConfigs::doubleClickedData(Q3ListViewItem *item,const QPoint &pt,
				      int c)
{
  editData();
}


void ListShowConfigs::closeData()
{
  done(0);
}


void ListShowConfigs::resizeEvent(QResizeEvent *e)
{
  list_shows_list->setGeometry(10,10,size().width()-20,size().height()-80);
  list_add_button->setGeometry(10,size().height()-60,80,50);
  list_edit_button->setGeometry(100,size().height()-60,80,50);
  list_delete_button->setGeometry(190,size().height()-60,80,50);
  list_close_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


void ListShowConfigs::RefreshList()
{
  QSqlQuery *q;
  Q3ListViewItem *item;

  list_shows_list->clear();
  q=new QSqlQuery("select TITLE,SHOW_CODE,DEFAULT_LABEL_ACTION from SHOWS");
  while (q->next()) {
    item=new Q3ListViewItem(list_shows_list);
    item->setText(0,q->value(0).toString());
    item->setText(1,q->value(1).toString());
    switch((MldConfig::LabelAction)q->value(2).toInt()) {
	case MldConfig::Blank:
	  item->setText(2,"Set Blank");
	  break;

	case MldConfig::UseFirstKnown:
	  item->setText(2,"Use First Call");
	  break;

	case MldConfig::UseLastKnown:
	  item->setText(2,"Use Most Recent Call");
	  break;

	case MldConfig::UseSpecified:
	  item->setText(2,"Use Specified Data");
	  break;

	default:
	  item->setText(2,"");
	  break;
    }
  }
  delete q;
}


void ListShowConfigs::UpdateItem(Q3ListViewItem *item)
{
  QSqlQuery *q=new 
    QSqlQuery(QString().sprintf("select TITLE,DEFAULT_LABEL_ACTION from SHOWS\
                                 where SHOW_CODE=\"%s\"",
				(const char *)item->text(1)));
  if(q->first()) {
    item->setText(0,q->value(0).toString());
    switch((MldConfig::LabelAction)q->value(1).toInt()) {
	case MldConfig::Blank:
	  item->setText(2," Set Blank");
	  break;

	case MldConfig::UseFirstKnown:
	  item->setText(2,"Use First Call");
	  break;

	case MldConfig::UseLastKnown:
	  item->setText(2,"Use Most Recent Call");
	  break;

	case MldConfig::UseSpecified:
	  item->setText(2,"Use Specified Data");
	  break;

	default:
	  item->setText(2,"");
	  break;
    }
  }
  delete q;
}
