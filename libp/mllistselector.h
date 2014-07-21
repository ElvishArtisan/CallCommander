//   mllistselector.h
//
//   An listselector widget with word wrap.
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: mllistselector.h,v 1.3 2010/11/22 13:11:40 cvs Exp $
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
//

#ifndef MLLISTSELECTOR_H
#define MLLISTSELECTOR_H

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <Qt3Support/q3hbox.h>
#include <QtGui/QColor>
#include <Qt3Support/q3listbox.h>
#include <QtGui/QPushButton>

/**
 * @short A list selector widget.
 * @author Fred Gleason <fredg@paravelsystems.com>
 *
 * MLListSelector is a widget that allows the user to select one or more items
 * from a list to go into another list.
 *
 **/
class MLListSelector : public Q3HBox
{
  Q_OBJECT

  public:
  /**
   * Create an MLListSelector with the following parameters:
   * @param parent The parent widget
   * @param name The name of the widget
   **/
   MLListSelector(QWidget *parent=0,const char *name=0);
   uint sourceCount() const;
   uint destCount() const;
   void sourceSetLabel(QString label);
   void destSetLabel(QString label);
   void sourceInsertItem(const QString &text,int index=-1);
   void destInsertItem(const QString &text,int index=-1);
   void sourceRemoveItem(int index);
   void destRemoveItem(int index);
   QString sourceText(int index) const;
   QString destText(int index) const;
   void sourceChangeItem(const QString &text,int index);
   void destChangeItem(const QString &text,int index);
   int sourceNumItemsVisible() const;
   int destNumItemsVisible() const;
   int sourceCurrentItem() const;
   int destCurrentItem() const;
   QString sourceCurrentText() const;
   QString destCurrentText() const;
   void sourceSetCurrentItem(int item);
   void destSetCurrentItem(int item);
   Q3ListBoxItem *sourceFindItem(const QString &text,
				 Q3ListBox::ComparisonFlags compare=Q3ListBox::BeginsWith) const;
   Q3ListBoxItem *destFindItem(const QString &text,
			       Q3ListBox::ComparisonFlags compare=Q3ListBox::BeginsWith) const;
   void clear();

  private slots:
   void addData();
   void removeData();

  private:
   void CheckButtons();
   Q3ListBox *list_source_box;
   QLabel *list_source_label;
   Q3ListBox *list_dest_box;
   QLabel *list_dest_label;
   QPushButton *list_add_button;
   QPushButton *list_remove_button;
};



#endif  // MLLISTSELECTOR_H
