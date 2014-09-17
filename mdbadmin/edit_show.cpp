// edit_show.cpp
//
// Edit a CallCommander Show Configuration Entry.
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
#include <QtGui/QPainter>
#include <QtGui/QMessageBox>
#include <QtSql/QSqlQuery>

#include <mlconf.h>
#include <math.h>

#include <ml_reload_packet.h>

#include "globals.h"
#include "list_connections.h"
#include "select_callerid_source.h"
#include "edit_show.h"
#include "edit_show_slot.h"
#include "edit_map.h"

EditShow::EditShow(QString showcode,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  edit_showcode=showcode;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setCaption("Call Administrator - Edit Show");

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
  // Showcode
  //
  edit_showcode_edit=new QLineEdit(this,"edit_showcode_edit");
  edit_showcode_edit->setGeometry(130,10,40,20);
  edit_showcode_edit->setFont(font);
  edit_showcode_edit->setReadOnly(true);
  QLabel *label=
    new QLabel(edit_showcode_edit,tr("Showcode:"),this,"edit_showcode_label");
  label->setGeometry(10,10,115,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Title
  //
  edit_title_edit=new QLineEdit(this,"edit_title_edit");
  edit_title_edit->setGeometry(130,32,sizeHint().width()-140,20);
  edit_title_edit->setFont(font);
  edit_title_edit->setMaxLength(64);
  label=new QLabel(edit_title_edit,tr("Title:"),this,"edit_title_label");
  label->setGeometry(10,32,115,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Default Label Action
  //
  edit_defaultlabel_box=new QComboBox(this,"edit_defaultlabel_box");
  edit_defaultlabel_box->setGeometry(130,54,sizeHint().width()-140,20);
  edit_defaultlabel_box->setFont(font);
  edit_defaultlabel_box->insertItem(tr("Set Blank"));
  edit_defaultlabel_box->insertItem(tr("Use Data from First Call"));
  edit_defaultlabel_box->insertItem(tr("Use Data from Most Recent Call"));
  label=new QLabel(edit_defaultlabel_box,tr("Default Label:"),
		   this,"edit_defaultlabel_label");
  label->setGeometry(10,54,115,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Is Active Checkbox
  //
  edit_isactive_box=new QCheckBox(this,"edit_isactive_box");
  edit_isactive_box->setGeometry(10,89,16,15);
  connect(edit_isactive_box,SIGNAL(toggled(bool)),
	  this,SLOT(activeToggledData(bool)));
  label=new QLabel(edit_isactive_box,tr("Activate Caller ID Tracking"),
		   this,"edit_isactive_label");
  label->setGeometry(32,86,sizeHint().width()-40,20);
  label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Hostname
  //
  edit_hostname_edit=new QLineEdit(this,"edit_hostname_edit");
  edit_hostname_edit->setGeometry(130,115,sizeHint().width()-140,20);
  edit_hostname_edit->setFont(font);
  edit_hostname_edit->setMaxLength(255);
  edit_hostname_label=new QLabel(edit_hostname_edit,tr("Monitor Hostname:"),
		   this,"edit_hostname_label");
  edit_hostname_label->setGeometry(10,115,115,20);
  edit_hostname_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_hostname_label->setFont(label_font);

  //
  // Connection
  //
  edit_connection_edit=new QLineEdit(this,"edit_connection_edit");
  edit_connection_edit->setGeometry(130,149,sizeHint().width()-220,20);
  edit_connection_edit->setFont(font);
  edit_connection_edit->setMaxLength(255);
  connect(edit_connection_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(connectionTextChangedData(const QString &)));
  edit_connection_label=new QLabel(edit_connection_edit,tr("Connection:"),
		   this,"edit_connection_label");
  edit_connection_label->setGeometry(10,149,115,20);
  edit_connection_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_connection_label->setFont(label_font);
  edit_connection_button=new QPushButton(this,"connection_button");
  edit_connection_button->setGeometry(sizeHint().width()-80,143,70,30);
  edit_connection_button->setFont(label_font);
  edit_connection_button->setText("&Select");
  connect(edit_connection_button,SIGNAL(clicked()),
	  this,SLOT(selectConnectionData()));

  //
  // Kill Calls Checkbox
  //
  edit_killcalls_box=new QCheckBox(this,"edit_killcalls_box");
  edit_killcalls_box->setGeometry(135,173,15,15);
  edit_killcalls_label=
    new QLabel(edit_killcalls_box,tr("Use this connection to block calls"),
	       this,"edit_killcalls_label");
  edit_killcalls_label->setGeometry(150,170,sizeHint().width()-220,20);
  edit_killcalls_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
  edit_killcalls_label->setFont(label_font);

  //
  // Caller ID Source
  //
  edit_callerid_edit=new QLineEdit(this,"edit_callerid_edit");
  edit_callerid_edit->setGeometry(130,202,sizeHint().width()-220,20);
  edit_callerid_edit->setFont(font);
  edit_callerid_edit->setReadOnly(true);
  edit_callerid_label=new QLabel(edit_callerid_edit,tr("CallerID Source:"),
		   this,"edit_callerid_label");
  edit_callerid_label->setGeometry(10,202,115,20);
  edit_callerid_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_callerid_label->setFont(label_font);
  edit_callerid_button=new QPushButton(this,"tty_button");
  edit_callerid_button->setGeometry(sizeHint().width()-80,197,70,30);
  edit_callerid_button->setFont(label_font);
  edit_callerid_button->setText("S&elect");
  connect(edit_callerid_button,SIGNAL(clicked()),
	  this,SLOT(selectSourceData()));
  edit_cidmap_button=new QPushButton(this,"tty_button");
  edit_cidmap_button->setGeometry(sizeHint().width()-200,225,130,30);
  edit_cidmap_button->setFont(label_font);
  edit_cidmap_button->setText("CID Line Map");
  connect(edit_cidmap_button,SIGNAL(clicked()),
	  this,SLOT(selectCidmapData()));

  //
  // Local Area Code
  //
  edit_areacode_edit=new QLineEdit(this,"edit_areacode_edit");
  edit_areacode_edit->setGeometry(130,231,50,20);
  edit_areacode_edit->setFont(font);
  edit_areacode_edit->setMaxLength(10);
  edit_areacode_label=new QLabel(edit_areacode_edit,tr("Local Area C&ode:"),
		   this,"edit_areacode_label");
  edit_areacode_label->setGeometry(10,231,115,20);
  edit_areacode_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_areacode_label->setFont(label_font);

  //
  // Active Days Label
  //
  edit_active_label=
    new QLabel(tr("Active Time Slots"),this,"active_days_label");
  edit_active_label->setGeometry(20,263,100,19);
  edit_active_label->setFont(label_font);
  edit_active_label->setAlignment(Qt::AlignHCenter|Qt::TextShowMnemonic);


  //
  // Time Slot List
  //
  edit_showslots_list=new Q3ListView(this,"edit_showslots_list");
  edit_showslots_list->
    setGeometry(10,280,sizeHint().width()-20,sizeHint().height()-395);
  edit_showslots_list->setAllColumnsShowFocus(true);
  edit_showslots_list->setItemMargin(5);
  connect(edit_showslots_list,
	  SIGNAL(doubleClicked(Q3ListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(doubleClickedData(Q3ListViewItem *,const QPoint &,int)));
  edit_showslots_list->addColumn(tr("Start Time"));
  edit_showslots_list->setColumnAlignment(0,Qt::AlignCenter);
  edit_showslots_list->addColumn(tr("End Time"));
  edit_showslots_list->setColumnAlignment(1,Qt::AlignCenter);
  edit_showslots_list->addColumn(tr("Su"));
  edit_showslots_list->setColumnAlignment(2,Qt::AlignCenter);
  edit_showslots_list->addColumn(tr("Mn"));
  edit_showslots_list->setColumnAlignment(3,Qt::AlignCenter);
  edit_showslots_list->addColumn(tr("Tu"));
  edit_showslots_list->setColumnAlignment(4,Qt::AlignCenter);
  edit_showslots_list->addColumn(tr("We"));
  edit_showslots_list->setColumnAlignment(5,Qt::AlignCenter);
  edit_showslots_list->addColumn(tr("Th"));
  edit_showslots_list->setColumnAlignment(6,Qt::AlignCenter);
  edit_showslots_list->addColumn(tr("Fr"));
  edit_showslots_list->setColumnAlignment(7,Qt::AlignCenter);
  edit_showslots_list->addColumn(tr("Sa"));
  edit_showslots_list->setColumnAlignment(8,Qt::AlignCenter);
  edit_showslots_list->addColumn(tr("Policy"));
  edit_showslots_list->setColumnAlignment(9,Qt::AlignCenter);

  edit_addslot_button=new QPushButton(this,"edit_addslot_button");
  edit_addslot_button->setGeometry(20,sizeHint().height()-110,60,30);
  edit_addslot_button->setFont(label_font);
  edit_addslot_button->setText(tr("Add"));
  connect(edit_addslot_button,SIGNAL(clicked()),this,SLOT(addSlotData()));

  edit_editslot_button=new QPushButton(this,"edit_editslot_button");
  edit_editslot_button->setGeometry(80,sizeHint().height()-110,60,30);
  edit_editslot_button->setFont(label_font);
  edit_editslot_button->setText(tr("Edit"));
  connect(edit_editslot_button,SIGNAL(clicked()),this,SLOT(editSlotData()));

  edit_deleteslot_button=new QPushButton(this,"edit_deleteslot_button");
  edit_deleteslot_button->setGeometry(140,sizeHint().height()-110,60,30);
  edit_deleteslot_button->setFont(label_font);
  edit_deleteslot_button->setText(tr("Delete"));
  connect(edit_deleteslot_button,SIGNAL(clicked()),
	  this,SLOT(deleteSlotData()));


  //
  //  OK Button
  //
  QPushButton *button=new QPushButton(this,"ok_button");
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
  QString sql;
  QSqlQuery *q;
  sql=QString().sprintf("select TITLE,DEFAULT_LABEL_ACTION,IS_ACTIVE,\
                         CID_HOSTNAME,CID_CONNECTION,CID_SOURCE_ID,\
                         START_TIME,END_TIME,\
                         SUN,MON,TUE,WED,THU,FRI,SAT,LOCAL_AREA_CODE,\
                         KILL_CALLS from SHOWS where SHOW_CODE=\"%s\"",
			(const char *)edit_showcode);
  q=new QSqlQuery(sql);
  if(q->first()) {
    edit_showcode_edit->setText(edit_showcode);
    edit_title_edit->setText(q->value(0).toString());
    edit_defaultlabel_box->setCurrentItem(q->value(1).toInt());
    edit_isactive_box->setChecked(MLBool(q->value(2).toString()));
    edit_hostname_edit->setText(q->value(3).toString());
    edit_connection_edit->setText(q->value(4).toString());
    edit_callerid_hostid=q->value(5).toInt();
    edit_areacode_edit->setText(q->value(15).toString());
    edit_killcalls_box->setChecked(MLBool(q->value(16).toString()));
  }
  delete q;
  RefreshList();
  DisplayCallerIdSource();
  activeToggledData(edit_isactive_box->isChecked());
}


EditShow::~EditShow()
{
}


QSize EditShow::sizeHint() const
{
  return QSize(450,500);
} 


QSizePolicy EditShow::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditShow::activeToggledData(bool on)
{
  edit_hostname_label->setEnabled(on);
  edit_hostname_edit->setEnabled(on);
  edit_connection_label->setEnabled(on);
  edit_connection_edit->setEnabled(on);
  edit_connection_button->setEnabled(on);
  edit_callerid_label->setEnabled(on);
  edit_callerid_edit->setEnabled(on);
  edit_callerid_button->setEnabled(on);
  edit_areacode_label->setEnabled(on);
  edit_areacode_edit->setEnabled(on);
  edit_active_label->setEnabled(on);
  connectionTextChangedData(edit_connection_edit->text());
}


void EditShow::connectionTextChangedData(const QString &text)
{
  edit_killcalls_label->
    setDisabled(text.isEmpty()||(!edit_isactive_box->isChecked()));
  edit_killcalls_box->
    setDisabled(text.isEmpty()||(!edit_isactive_box->isChecked()));
}


void EditShow::selectConnectionData()
{
  ConnectionData *conn=new ConnectionData(ConnectionData::StorageDatabase);
  conn->setName(edit_connection_edit->text());
  conn->setOfferStartConnection(false);
  ListConnections *list_conn=new ListConnections(conn,tr("OK"),NULL,this);
  if(list_conn->exec()<0) {
    delete list_conn;
    return;
  }
  delete list_conn;
  edit_connection_edit->setText(conn->name());
  delete conn;
}


void EditShow::selectSourceData()
{
  SelectCallerIdSource *select=
    new SelectCallerIdSource(edit_hostname_edit->text(),
			     &edit_callerid_hostid,this);
  if(select->exec()<0) {
    delete select;
    return;
  }
  DisplayCallerIdSource();
  delete select;
}


void EditShow::selectCidmapData()
{
  EditMap *editmap=new EditMap(edit_showcode,this);
  editmap->exec();
  delete editmap;
}


void EditShow::doubleClickedData(Q3ListViewItem *item,const QPoint &pt,int col)
{
  editSlotData();
}


void EditShow::addSlotData()
{
  QString sql;
  QSqlQuery *q;
  int slot_id=-1;

  sql=QString().sprintf("insert into SHOW_SLOTS set SHOW_CODE=\"%s\"",
			(const char *)edit_showcode);
  q=new QSqlQuery(sql);
  delete q;
  sql="select LAST_INSERT_ID() from SHOW_SLOTS";
  q=new QSqlQuery(sql);
  if(q->first()) {
    slot_id=q->value(0).toInt();
  }
  delete q;
  if(slot_id<0) {
    QMessageBox::warning(this,tr("MDbAdmin - Database Error"),
			 tr("Unable to create new SHOW_SLOTS entry!"));
    return;
  }

  MlTimeslot slot(slot_id);
  EditShowSlot *edit_slot=new EditShowSlot(&slot,this);
  if(edit_slot->exec()<0) {
    sql=QString().sprintf("delete from SHOW_SLOTS where ID=%d",slot_id);
    q=new QSqlQuery(sql);
    delete q;
    delete edit_slot;
    return;
  }
  MlListViewItem *item=new MlListViewItem(edit_showslots_list);
  item->setId(slot_id);
  RefreshItem(item,&slot);
  edit_showslots_list->ensureItemVisible(item);
  delete edit_slot;
}


void EditShow::editSlotData()
{
  MlListViewItem *item=(MlListViewItem *)edit_showslots_list->selectedItem();
  if(item==NULL) {
    return;
  }
  MlTimeslot slot(item->id());
  slot.setStartTime(item->text(0));
  slot.setEndTime(item->text(1));
  slot.setDowActive(7,!item->text(2).isEmpty());
  slot.setDowActive(1,!item->text(3).isEmpty());
  slot.setDowActive(2,!item->text(4).isEmpty());
  slot.setDowActive(3,!item->text(5).isEmpty());
  slot.setDowActive(4,!item->text(6).isEmpty());
  slot.setDowActive(5,!item->text(7).isEmpty());
  slot.setDowActive(6,!item->text(8).isEmpty());
  if(item->text(9)==tr("Deny")) {
    slot.setFilterPolicy(MlTimeslot::DenyPolicy);
  }
  else {
    slot.setFilterPolicy(MlTimeslot::AcceptPolicy);
  }
  for(int i=0;i<item->filterList()->size();i++) {
    slot.addFilter((*item->filterList())[i]);
  }
  EditShowSlot *edit_slot=new EditShowSlot(&slot,this);
  if(edit_slot->exec()<0) {
    delete edit_slot;
    return;
  }
  RefreshItem(item,&slot);
  delete edit_slot;
}


void EditShow::deleteSlotData()
{
  MlListViewItem *item=(MlListViewItem *)edit_showslots_list->selectedItem();
  if(item==NULL) {
    return;
  }
  edit_deleted_slots.push_back(item->id());
  delete item;
}


void EditShow::okData()
{
  QSqlQuery *q;
  QString sql;
  QHostAddress addr=MlResolveIpAddress(edit_hostname_edit->text());

  //
  // Check for valid Monitor Hostname
  //
  if(!edit_hostname_edit->text().isEmpty()) {
    if(addr.isNull()) {
      if(QMessageBox::question(this,tr("Address Error"),
			       tr("The Monitor Hostname is invalid.\nDo you still want to save?"),QMessageBox::Yes,QMessageBox::No)==QMessageBox::No) {
	return;
      }
    }
  }

  //
  // Update Show Record
  //
  sql=QString().sprintf("update SHOWS set TITLE=\"%s\",\
                         DEFAULT_LABEL_ACTION=%d,\
                         IS_ACTIVE=\"%s\",\
                         CID_HOSTNAME=\"%s\",\
                         CID_CONNECTION=\"%s\",\
                         CID_SOURCE_ID=%d,\
                         LOCAL_AREA_CODE=\"%s\",\
                         KILL_CALLS=\"%s\" \
                         where SHOW_CODE=\"%s\"",
			(const char *)edit_title_edit->text(),
			edit_defaultlabel_box->currentItem(),
			(const char *)MLYesNo(edit_isactive_box->isChecked()),
			(const char *)edit_hostname_edit->text(),
			(const char *)edit_connection_edit->text(),
			edit_callerid_hostid,
			(const char *)edit_areacode_edit->text(),
			(const char *)MLYesNo(edit_killcalls_box->isChecked()),
			(const char *)edit_showcode);
  q=new QSqlQuery(sql);
  delete q;

  //
  // Remove Deleted Slots
  //
  for(unsigned i=0;i<edit_deleted_slots.size();i++) {
    sql=QString().sprintf("delete from CID_FILTERS where SLOT_ID=%d",
			  edit_deleted_slots[i]);
    q=new QSqlQuery(sql);
    delete q;
    sql=QString().sprintf("delete from SHOW_SLOTS where ID=%d",
			  edit_deleted_slots[i]);
    q=new QSqlQuery(sql);
    delete q;
  }

  //
  // Update Remaining Slots
  //
  MlListViewItem *item=(MlListViewItem *)edit_showslots_list->firstChild();
  while(item!=NULL) {
    MlTimeslot::FilterPolicy policy;
    if(item->text(9)==tr("Deny")) {
      policy=MlTimeslot::DenyPolicy;
    }
    else {
      policy=MlTimeslot::AcceptPolicy;
    }
    sql=QString().sprintf("update SHOW_SLOTS set \
                           SHOW_CODE=\"%s\",\
                           START_TIME=\"%s\",\
                           END_TIME=\"%s\",\
                           FILTER_POLICY=%d,\
                           SUN=\"%s\",\
                           MON=\"%s\",\
                           TUE=\"%s\",\
                           WED=\"%s\",\
                           THU=\"%s\",\
                           FRI=\"%s\",\
                           SAT=\"%s\" \
                           where ID=%d",
			  (const char *)edit_showcode,
			  (const char *)item->text(0),
			  (const char *)item->text(1),
			  policy,
			  (const char *)MLYesNo(!item->text(2).isEmpty()),
			  (const char *)MLYesNo(!item->text(3).isEmpty()),
			  (const char *)MLYesNo(!item->text(4).isEmpty()),
			  (const char *)MLYesNo(!item->text(5).isEmpty()),
			  (const char *)MLYesNo(!item->text(6).isEmpty()),
			  (const char *)MLYesNo(!item->text(7).isEmpty()),
			  (const char *)MLYesNo(!item->text(8).isEmpty()),
			  item->id());
    q=new QSqlQuery(sql);
    delete q;

    sql=QString().sprintf("delete from CID_FILTERS where SLOT_ID=%d",
			  item->id());
    q=new QSqlQuery(sql);
    delete q;
    for(int i=0;i<item->filterList()->size();i++) {
      sql=QString().sprintf("insert into CID_FILTERS set \
                             SLOT_ID=%d,\
                             FILTER=\"%s\"",
			    item->id(),
			    (const char *)(*item->filterList())[i]);
      q=new QSqlQuery(sql);
      delete q;
    }

    item=(MlListViewItem *)item->nextSibling();
  }
  if(!addr.isNull()) {
    MlSendReloadPacket(admin_config,addr,ML_MCIDMD_RELOAD_PORT,
		       admin_reload_socket);
  }

  done(0);
}


void EditShow::cancelData()
{
  done(-1);
}


void EditShow::DisplayCallerIdSource()
{
  if(edit_callerid_hostid==0) {
    edit_callerid_edit->setText(tr("[use native source]"));
    edit_cidmap_button->setDisabled(true);
  }
  else {
    edit_callerid_edit->
      setText(QString().sprintf("%s:%d",
				(const char *)edit_hostname_edit->text(),
				edit_callerid_hostid));
    edit_cidmap_button->setEnabled(true);
  }
}


void EditShow::RefreshList() 
{
  QString sql;
  QSqlQuery *q;
  QSqlQuery *q1;

  edit_showslots_list->clear();
  MlListViewItem *item=NULL;
  sql=QString().sprintf("select START_TIME,END_TIME,\
                         SUN,MON,TUE,WED,THU,FRI,SAT,FILTER_POLICY,ID \
                         from SHOW_SLOTS where SHOW_CODE=\"%s\"",
			(const char *)edit_showcode);
  q=new QSqlQuery(sql);
  while(q->next()) {
    item=new MlListViewItem(edit_showslots_list);
    item->setId(q->value(10).toInt());
    item->setText(0,q->value(0).toTime().toString("hh:mm:ss"));
    item->setText(1,q->value(1).toTime().toString("hh:mm:ss"));
    if(q->value(2).toString()=="Y") {
      item->setText(2,tr("Su"));
    }
    if(q->value(3).toString()=="Y") {
      item->setText(3,tr("Mo"));
    }
    if(q->value(4).toString()=="Y") {
      item->setText(4,tr("Tu"));
    }
    if(q->value(5).toString()=="Y") {
      item->setText(5,tr("We"));
    }
    if(q->value(6).toString()=="Y") {
      item->setText(6,tr("Th"));
    }
    if(q->value(7).toString()=="Y") {
      item->setText(7,tr("Fr"));
    }
    if(q->value(8).toString()=="Y") {
      item->setText(8,tr("Sa"));
    }
    switch((MlTimeslot::FilterPolicy)q->value(9).toInt()) {
    case MlTimeslot::AcceptPolicy:
      item->setText(9,tr("Accept"));
      break;

    case MlTimeslot::DenyPolicy:
      item->setText(9,tr("Deny"));
      break;
    }
    sql=QString().sprintf("select FILTER from CID_FILTERS where SLOT_ID=%d",
			  item->id());
    q1=new QSqlQuery(sql);
    while(q1->next()) {
      item->filterList()->push_back(q1->value(0).toString());
    }
    delete q1;
  }
  delete q;
}


void EditShow::RefreshItem(MlListViewItem *item,MlTimeslot *slot)
{
  item->setText(0,slot->startTime().toString("hh:mm:ss"));
  item->setText(1,slot->endTime().toString("hh:mm:ss"));
  if(slot->dowActive(7)) {
    item->setText(2,tr("Su"));
  }
  else {
    item->setText(2,"");
  }
  if(slot->dowActive(1)) {
    item->setText(3,tr("Mo"));
  }
  else {
    item->setText(3,"");
  }
  if(slot->dowActive(2)) {
    item->setText(4,tr("Tu"));
  }
  else {
    item->setText(4,"");
  }
  if(slot->dowActive(3)) {
    item->setText(5,tr("We"));
  }
  else {
    item->setText(5,"");
  }
  if(slot->dowActive(4)) {
    item->setText(6,tr("Th"));
  }
  else {
    item->setText(6,"");
  }
  if(slot->dowActive(5)) {
    item->setText(7,tr("Fr"));
  }
  else {
    item->setText(7,"");
  }
  if(slot->dowActive(6)) {
    item->setText(8,tr("Sa"));
  }
  else {
    item->setText(8,"");
  }
  switch(slot->filterPolicy()) {
  case MlTimeslot::AcceptPolicy:
    item->setText(9,tr("Accept"));
    break;

  case MlTimeslot::DenyPolicy:
    item->setText(9,tr("Deny"));
    break;
  }
  item->filterList()->clear();
  for(unsigned i=0;i<slot->filters();i++) {
    item->filterList()->push_back(slot->filter(i));
  }
}
