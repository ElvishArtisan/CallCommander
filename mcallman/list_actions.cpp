// list_actions.cpp
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

#include <QtSql/QSqlDatabase>
#include <QtGui/QResizeEvent>
#include <QtSql/QSqlQuery>
#include <QtGui/QMessageBox>

#include <math.h>

#include <mldconfig.h>

#include "calleriddetail.h"
#include "list_actions.h"
#include "edit_action.h"
#include "add_action.h"

ListActions::ListActions(QString showcode,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  QString sql;
  QSqlQuery *q;
  QString title;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());

  list_showcode=showcode;

  sql=QString().sprintf("select TITLE from SHOWS where SHOW_CODE=\"%s\"",
			(const char *)list_showcode);
  q=new QSqlQuery(sql);
  if(q->first()) {
    title=q->value(0).toString();
  }
  delete q;
  setCaption(QString().sprintf("%s - Call Actions",(const char *)title));

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);
  QFont small_font=QFont("Helvetica",10,QFont::Normal);
  small_font.setPixelSize(10);

  //
  // Filter Edit
  //
  list_filter_edit=new QLineEdit(this,"list_filter_edit");
  connect(list_filter_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(filterChangedData(const QString &)));
  list_filter_label=new QLabel(list_filter_edit,"Filter:",
			       this,"list_filter_label");
  list_filter_label->setFont(font);
  list_filter_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  list_clear_button=new QPushButton(this,"list_clear_button");
  list_clear_button->setFont(font);
  list_clear_button->setText("Clear");
  connect(list_clear_button,SIGNAL(clicked()),
	  this,SLOT(filterClearedData()));

  //
  // Directory List
  //
  list_actions_list=new Q3ListView(this,"list_actions_list");
  list_actions_list->setAllColumnsShowFocus(true);
  list_actions_list->setItemMargin(5);
  connect(list_actions_list,
	  SIGNAL(doubleClicked(Q3ListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(doubleClickedData(Q3ListViewItem *,const QPoint &,int)));
  list_actions_list->addColumn("Number");
  list_actions_list->setColumnAlignment(0,Qt::AlignCenter);
  list_actions_list->addColumn("Calls");
  list_actions_list->setColumnAlignment(1,Qt::AlignCenter);
  list_actions_list->addColumn("Route Action");
  list_actions_list->setColumnAlignment(1,Qt::AlignCenter);
  list_actions_list->addColumn("Label Action");
  list_actions_list->setColumnAlignment(2,Qt::AlignCenter);
  list_actions_list->addColumn("Name");
  list_actions_list->setColumnAlignment(3,Qt::AlignLeft|Qt::AlignVCenter);
  list_actions_list->addColumn("Age");
  list_actions_list->setColumnAlignment(4,Qt::AlignCenter);
  list_actions_list->addColumn("City");
  list_actions_list->setColumnAlignment(5,Qt::AlignLeft|Qt::AlignVCenter);
  list_actions_list->addColumn("State");
  list_actions_list->setColumnAlignment(6,Qt::AlignCenter);
  list_actions_list->addColumn("ZIP Code");
  list_actions_list->setColumnAlignment(7,Qt::AlignCenter);
  list_actions_list->addColumn("Gender");
  list_actions_list->setColumnAlignment(8,Qt::AlignCenter);
  list_actions_list->addColumn("Cellphone");
  list_actions_list->setColumnAlignment(9,Qt::AlignCenter);

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


ListActions::~ListActions()
{
}


QSize ListActions::sizeHint() const
{
  return QSize(640,480);
} 


QSizePolicy ListActions::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ListActions::addData()
{
  QString num;
  QString sql;
  QSqlQuery *q;

  AddAction *add=new AddAction(&num,list_showcode,this,"add");
  if(add->exec()==0) {
    sql=QString().sprintf("insert into ACTIONS_%s set NUMBER=\"%s\"",
			  (const char *)list_showcode,
			  (const char *)num);
    q=new QSqlQuery(sql);
    delete q;
    EditAction *edit=new EditAction(num,list_showcode,-1,this);
    if(edit->exec()==0) {
      Q3ListViewItem *item=new Q3ListViewItem(list_actions_list);
      item->setText(0,num);
      UpdateItem(item);
      list_actions_list->setSelected(item,true);
      list_actions_list->ensureItemVisible(item);
    }
    else {
      sql=QString().sprintf("delete from ACTIONS_%s where NUMBER=\"%s\"",
			    (const char *)list_showcode,
			    (const char *)num);
      q=new QSqlQuery(sql);
      delete q;
    }
    delete edit;
  }
  delete add;
}


void ListActions::editData()
{
  Q3ListViewItem *item=list_actions_list->selectedItem();
  
  if(item==NULL) {
    return;
  }
  EditAction *edit=
    new EditAction(CallerIdDetail::simplifiedNumber(item->text(0)),
		   list_showcode,-1,this);
  if(edit->exec()==0) {
    UpdateItem(item);
  }
  delete edit;
}


void ListActions::deleteData()
{
  QString sql;
  QSqlQuery *q;
  Q3ListViewItem *item=list_actions_list->selectedItem();

  if(item==NULL) {
    return;
  }
  QString pnum=CallerIdDetail::simplifiedNumber(item->text(0));
  if(QMessageBox::question(this,"Delete Number",
	 QString().sprintf("This will remove ALL detail records referencing this number!\nAre you sure you want to delete the entry for %s?",
			   (const char *)item->text(0)),
			   QMessageBox::Yes,
			   QMessageBox::No)==QMessageBox::No) {
    return;
  }
  sql=QString().sprintf("delete from DETAILS_%s where NUMBER=\"%s\"",
			(const char *)list_showcode,
			(const char *)pnum);
  q=new QSqlQuery(sql);
  delete q;
  sql=QString().sprintf("delete from ACTIONS_%s where NUMBER=\"%s\"",
			(const char *)list_showcode,
			(const char *)pnum);
  q=new QSqlQuery(sql);
  delete q;
  delete item;
}


void ListActions::doubleClickedData(Q3ListViewItem *item,const QPoint &pt,
				      int c)
{
  editData();
}


void ListActions::filterChangedData(const QString &str)
{
  RefreshList();
}


void ListActions::filterClearedData()
{
  list_filter_edit->clear();
  filterChangedData("");
}


void ListActions::closeData()
{
  done(0);
}


void ListActions::resizeEvent(QResizeEvent *e)
{
  list_filter_label->setGeometry(10,10,50,20);
  list_filter_edit->setGeometry(65,10,size().width()-140,20);
  list_clear_button->setGeometry(size().width()-65,10,50,20);
  list_actions_list->setGeometry(10,40,size().width()-20,size().height()-110);
  list_add_button->setGeometry(10,size().height()-60,80,50);
  list_edit_button->setGeometry(100,size().height()-60,80,50);
  list_delete_button->setGeometry(190,size().height()-60,80,50);
  list_close_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


void ListActions::RefreshList()
{
  QString sql;
  QSqlQuery *q;
  Q3ListViewItem *item;

  list_actions_list->clear();
  sql=QString().sprintf("select NUMBER,ROUTE_ACTION,LABEL_ACTION,\
                         NAME,AGE,CITY,STATE,ZIPCODE,GENDER,CELLPHONE,\
                         CALL_COUNT from ACTIONS_%s",
			(const char *)list_showcode);
  if(!list_filter_edit->text().isEmpty()) {
    sql+=QString().sprintf(" where %s",
		  (const char *)ActionSearchText(list_filter_edit->text()));
  }

  q=new QSqlQuery(sql);
  while (q->next()) {
    item=new Q3ListViewItem(list_actions_list);
    item->setText(0,CallerIdDetail::formattedNumber(q->value(0).toString()));
    item->setText(1,q->value(10).toString());
    switch((MldConfig::RouteAction)q->value(1).toInt()) {
	case MldConfig::Accept:
	  item->setText(2,"Accept");
	  break;

	case MldConfig::Warn:
	  item->setText(2,"Warn");
	  break;

	case MldConfig::Drop:
	  item->setText(2,"Drop");
	  break;
    }
    switch((MldConfig::LabelAction)q->value(2).toInt()) {
	case MldConfig::Blank:
	  item->setText(3,"Set Blank");
	  break;

	case MldConfig::UseFirstKnown:
	  item->setText(3,"Use Data from First Call");
	  break;

	case MldConfig::UseLastKnown:
	  item->setText(3,"Use Data from Most Recent Call");
	  break;

	case MldConfig::UseSpecified:
	  item->setText(3,"Use Specified Data");
	  break;
    }
    item->setText(4,q->value(3).toString());
    if(q->value(4).toInt()>0) {
      item->setText(5,q->value(4).toString());
    }
    item->setText(6,q->value(5).toString());
    item->setText(7,q->value(6).toString());
    item->setText(8,q->value(7).toString());
    item->setText(9,q->value(8).toString());
    item->setText(10,q->value(9).toString());
  }
  delete q;
}


void ListActions::UpdateItem(Q3ListViewItem *item)
{
  QString sql;
  QSqlQuery *q;

  sql=QString().sprintf("select ROUTE_ACTION,LABEL_ACTION,\
                         NAME,AGE,CITY,STATE,ZIPCODE,GENDER,CELLPHONE,\
                         CALL_COUNT from ACTIONS_%s where NUMBER=\"%s\"",
			(const char *)list_showcode,
			(const char *)CallerIdDetail::
			simplifiedNumber(item->text(0)));
  q=new QSqlQuery(sql);
  if(q->first()) {
    item->setText(1,q->value(9).toString());
    switch((MldConfig::RouteAction)q->value(0).toInt()) {
	case MldConfig::Accept:
	  item->setText(2,"Accept");
	  break;

	case MldConfig::Warn:
	  item->setText(2,"Warn");
	  break;

	case MldConfig::Drop:
	  item->setText(2,"Drop");
	  break;
	default:
	  item->setText(2,"");
    }
    switch((MldConfig::LabelAction)q->value(1).toInt()) {
	case MldConfig::Blank:
	  item->setText(3,"Set Blank");
	  break;

	case MldConfig::UseFirstKnown:
	  item->setText(3,"Use Data from First Call");
	  break;

	case MldConfig::UseLastKnown:
	  item->setText(3,"Use Data from Most Recent Call");
	  break;

	case MldConfig::UseSpecified:
	  item->setText(3,"Use Specified Data");
	  break;
    }
    item->setText(4,q->value(2).toString());
    if(q->value(3).toInt()>0) {
      item->setText(5,q->value(3).toString());
    }
    else {
      item->setText(5,"");
    }
    item->setText(6,q->value(4).toString());
    item->setText(7,q->value(5).toString());
    item->setText(8,q->value(6).toString());
    item->setText(9,q->value(7).toString());
    item->setText(10,q->value(8).toString());
  }
  delete q;
}


QString ListActions::ActionSearchText(QString filter)
{
  QString search=QString().sprintf(" ((NUMBER like \"%%%s%%\")||\
                                      (NAME like \"%%%s%%\")||\
                                      (CITY like \"%%%s%%\")||\
                                      (STATE like \"%%%s%%\")||\
                                      (ZIPCODE like \"%%%s%%\"))",
			   (const char *)filter,
			   (const char *)filter,
			   (const char *)filter,
			   (const char *)filter,
			   (const char *)filter);
  return search;
}
