// mlami.h
//
//   A connector for the Asterisk Manager Interface
//
//   (C) Copyright 2008 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: mlami.h,v 1.2 2010/11/22 13:11:40 cvs Exp $
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU Library General Public License 
//   version 2 as published by the Free Software Foundation.
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

#ifndef MLAMI_H
#define MLAMI_H

#include <vector>

#include <QtCore/QObject>
#include <Qt3Support/q3socket.h>

class MLAmi : public QObject
{
 Q_OBJECT
 public:
  MLAmi(unsigned id,QObject *parent=0,const char *name=0);
  void connectToHost(const QString &hostname,const QString &username,
		     const QString &passwd,Q_UINT16 port);
  unsigned sendAction(const QString &action,std::vector<QString> *labels,
		      std::vector<QString> *values);

 signals:
  void connected(unsigned id,bool state);
  void actionReceived(unsigned id,unsigned action_id,
		      std::vector<QString> *labels,
		      std::vector<QString> *values);
  void eventReceived(unsigned id,std::vector<QString> *labels,
		     std::vector<QString> *values);

 private slots:
  void connectedData();
  void readyReadData();
  void errorData(int err);

 private:
  void SendString(QString str);
  void DispatchLine(const QString &label,const QString &value);
  void Authenticate(const QString &label,const QString &value);
  Q3Socket *ami_socket;
  int ami_id;
  unsigned ami_next_action_id;
  unsigned ami_action_id;
  QString ami_username;
  QString ami_password;
  Q_UINT16 ami_port;
  QString ami_label;
  QString ami_value;
  int ami_istate;
  bool ami_authenticated;
  std::vector<QString> ami_labels;
  std::vector<QString> ami_values;
  bool ami_processing_event;
};


#endif  // MLAMI_H
