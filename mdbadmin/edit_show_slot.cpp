// edit_show_slot.cpp
//
// Edit a CallCommander Show Time Slot Entry.
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

#include <QtSql/QSqlDatabase>
#include <QtGui/QPainter>
#include <QtGui/QRadioButton>

#include <math.h>

#include "edit_filter.h"
#include "edit_show_slot.h"

EditShowSlot::EditShowSlot(MlTimeslot *slot,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  edit_slot=slot;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setCaption("Call Administrator - Edit Time Slot");

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);
  QFont day_font=QFont("Helvetica",10,QFont::Normal);
  day_font.setPixelSize(10);

  //
  // Monday Button
  //
  edit_mon_button=new QCheckBox(this,"edit_mon_button");
  edit_mon_button->setGeometry(22,10,20,20);
  edit_mon_label=new QLabel(edit_mon_button,
		   tr("Monday"),this,"edit_mon_label");
  edit_mon_label->setGeometry(42,10,115,20);
  edit_mon_label->setFont(day_font);
  edit_mon_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter|Qt::TextShowMnemonic);

  //
  // Tuesday Button
  //
  edit_tue_button=new QCheckBox(this,"edit_tue_button");
  edit_tue_button->setGeometry(99,10,20,20);
  edit_tue_label=new QLabel(edit_tue_button,
		   tr("Tuesday"),this,"edit_tue_label");
  edit_tue_label->setGeometry(119,10,115,20);
  edit_tue_label->setFont(day_font);
  edit_tue_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter|Qt::TextShowMnemonic);

  //
  // Wednesday Button
  //
  edit_wed_button=new QCheckBox(this,"edit_wed_button");
  edit_wed_button->setGeometry(181,10,20,20);
  edit_wed_label=new QLabel(edit_wed_button,
		   tr("Wednesday"),this,"edit_wed_label");
  edit_wed_label->setGeometry(201,10,115,20);
  edit_wed_label->setFont(day_font);
  edit_wed_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter|Qt::TextShowMnemonic);

  //
  // Thursday Button
  //
  edit_thu_button=new QCheckBox(this,"edit_thu_button");
  edit_thu_button->setGeometry(283,10,20,20);
  edit_thu_label=new QLabel(edit_thu_button,
		   tr("Thursday"),this,"edit_thu_label");
  edit_thu_label->setGeometry(303,10,115,20);
  edit_thu_label->setFont(day_font);
  edit_thu_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter|Qt::TextShowMnemonic);

  //
  // Friday Button
  //
  edit_fri_button=new QCheckBox(this,"edit_fri_button");
  edit_fri_button->setGeometry(370,10,20,20);
  edit_fri_label=new QLabel(edit_fri_button,
		   tr("Friday"),this,"edit_fri_label");
  edit_fri_label->setGeometry(390,10,35,20);
  edit_fri_label->setFont(day_font);
  edit_fri_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter|Qt::TextShowMnemonic);

  //
  // Saturday Button
  //
  edit_sat_button=new QCheckBox(this,"edit_sat_button");
  edit_sat_button->setGeometry(142,35,20,20);
  edit_sat_label=new QLabel(edit_sat_button,
		   tr("Saturday"),this,"edit_sat_label");
  edit_sat_label->setGeometry(162,35,60,20);
  edit_sat_label->setFont(day_font);
  edit_sat_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter|Qt::TextShowMnemonic);

  //
  // Sunday Button
  //
  edit_sun_button=new QCheckBox(this,"edit_sun_button");
  edit_sun_button->setGeometry(246,35,20,20);
  edit_sun_label=new QLabel(edit_sun_button,
		   tr("Sunday"),this,"edit_sun_label");
  edit_sun_label->setGeometry(266,35,60,20);
  edit_sun_label->setFont(day_font);
  edit_sun_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter|Qt::TextShowMnemonic);

  //
  // Start Time
  //
  edit_starttime_edit=new Q3TimeEdit(this,"edit_starttime_edit");
  edit_starttime_edit->setGeometry(112,60,80,20);
  edit_starttime_edit->
    setDisplay(Q3TimeEdit::Hours|Q3TimeEdit::Minutes|Q3TimeEdit::Seconds);
  edit_starttime_label=new QLabel(edit_starttime_edit,tr("&Starting at:"),
		   this,"edit_starttime_label");
  edit_starttime_label->setGeometry(22,60,85,20);
  edit_starttime_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_starttime_label->setFont(label_font);

  //
  // End Time
  //
  edit_endtime_edit=new Q3TimeEdit(this,"edit_endtime_edit");
  edit_endtime_edit->setGeometry(306,60,80,20);
  edit_endtime_edit->
    setDisplay(Q3TimeEdit::Hours|Q3TimeEdit::Minutes|Q3TimeEdit::Seconds);
  edit_endtime_label=new QLabel(edit_endtime_edit,tr("&Ending at:"),
		   this,"edit_endtime_label");
  edit_endtime_label->setGeometry(216,60,85,20);
  edit_endtime_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_endtime_label->setFont(label_font);

  //
  // Filter Policy
  //
  edit_filter_policy_group=new Q3ButtonGroup(tr("Filter Policy"),this);
  edit_filter_policy_group->setGeometry(22,93,300,50);
  edit_filter_policy_group->setFont(label_font);
  edit_filter_policy_group->setRadioButtonExclusive(true);
  QRadioButton *radio=new QRadioButton(tr("Default Accept"),this);
  radio->setGeometry(32,115,130,16);
  edit_filter_policy_group->insert(radio,MlTimeslot::AcceptPolicy);
  radio=new QRadioButton(tr("Default Deny"),this);
  radio->setGeometry(162,115,130,16);
  edit_filter_policy_group->insert(radio,MlTimeslot::DenyPolicy);
  
  //
  // Filter List
  //
  edit_filter_list=new Q3ListView(this);
  edit_filter_list->setGeometry(22,170,sizeHint().width()-92,84);
  edit_filter_label=new QLabel(edit_filter_list,tr("Filter List:"),
		   this,"edit_filter_label");
  edit_filter_label->setGeometry(27,154,85,16);
  edit_filter_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
  edit_filter_label->setFont(label_font);
  edit_filter_list->setItemMargin(5);
  edit_filter_list->setAllColumnsShowFocus(true);
  connect(edit_filter_list,
	  SIGNAL(doubleClicked(Q3ListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(doubleClickedData(Q3ListViewItem *,const QPoint &,int)));
  edit_filter_list->addColumn("Filter String");
  edit_filter_list->setColumnAlignment(0,Qt::AlignLeft|Qt::AlignVCenter);

  QPushButton *button=new QPushButton(tr("Add"),this);
  button->setGeometry(sizeHint().width()-60,170,50,25);
  connect(button,SIGNAL(clicked()),this,SLOT(addFilterData()));

  button=new QPushButton(tr("Edit"),this);
  button->setGeometry(sizeHint().width()-60,200,50,25);
  connect(button,SIGNAL(clicked()),this,SLOT(editFilterData()));

  button=new QPushButton(tr("Delete"),this);
  button->setGeometry(sizeHint().width()-60,230,50,25);
  connect(button,SIGNAL(clicked()),this,SLOT(deleteFilterData()));

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
  edit_starttime_edit->setTime(slot->startTime());
  edit_endtime_edit->setTime(slot->endTime());
  edit_sun_button->setChecked(slot->dowActive(7));
  edit_mon_button->setChecked(slot->dowActive(1));
  edit_tue_button->setChecked(slot->dowActive(2));
  edit_wed_button->setChecked(slot->dowActive(3));
  edit_thu_button->setChecked(slot->dowActive(4));
  edit_fri_button->setChecked(slot->dowActive(5));
  edit_sat_button->setChecked(slot->dowActive(6));
  edit_filter_policy_group->setButton((int)slot->filterPolicy());
  for(unsigned i=0;i<edit_slot->filters();i++) {
    Q3ListViewItem *item=new Q3ListViewItem(edit_filter_list);
    item->setText(0,edit_slot->filter(i));
  }
}


EditShowSlot::~EditShowSlot()
{
}


QSize EditShowSlot::sizeHint() const
{
  return QSize(450,328);
} 


QSizePolicy EditShowSlot::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditShowSlot::addFilterData()
{
  QString str;
  EditFilter *d=new EditFilter(&str,this);
  if(d->exec()==0) {
    Q3ListViewItem *item=new Q3ListViewItem(edit_filter_list);
    item->setText(0,str);
  }
  delete d;
}


void EditShowSlot::editFilterData()
{
  Q3ListViewItem *item=edit_filter_list->selectedItem();
  if(item==NULL) {
    return;
  }
  QString str=item->text(0);
  EditFilter *d=new EditFilter(&str,this);
  if(d->exec()==0) {
    item->setText(0,str);
  }
  delete d;
}


void EditShowSlot::deleteFilterData()
{
  Q3ListViewItem *item=edit_filter_list->selectedItem();
  if(item==NULL) {
    return;
  }
  delete item;
}


void EditShowSlot::doubleClickedData(Q3ListViewItem *item,const QPoint &pt,
				     int col)
{
  editFilterData();
}


void EditShowSlot::okData()
{
  edit_slot->setStartTime(edit_starttime_edit->time());
  edit_slot->setEndTime(edit_endtime_edit->time());
  edit_slot->setDowActive(7,edit_sun_button->isChecked());
  edit_slot->setDowActive(1,edit_mon_button->isChecked());
  edit_slot->setDowActive(2,edit_tue_button->isChecked());
  edit_slot->setDowActive(3,edit_wed_button->isChecked());
  edit_slot->setDowActive(4,edit_thu_button->isChecked());
  edit_slot->setDowActive(5,edit_fri_button->isChecked());
  edit_slot->setDowActive(6,edit_sat_button->isChecked());
  edit_slot->setFilterPolicy((MlTimeslot::FilterPolicy)
			     edit_filter_policy_group->selectedId());
  edit_slot->clearFilters();
  Q3ListViewItem *item=edit_filter_list->firstChild();
  while(item!=NULL) {
    edit_slot->addFilter(item->text(0));
    item=item->nextSibling();
  }
  done(0);
}


void EditShowSlot::cancelData()
{
  done(-1);
}
