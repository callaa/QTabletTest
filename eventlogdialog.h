/*

   Qt tablet event tester

   Copyright (C) 2014 Calle Laakkonen

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
#ifndef EVENTLOGDIALOG_H
#define EVENTLOGDIALOG_H

#include <QDialog>

class QTextEdit;

class EventLogDialog : public QDialog
{
	Q_OBJECT
public:
	explicit EventLogDialog(QWidget *parent = 0);

public slots:
	void logEvent(const QString &message);
	void saveLog();

private:
	qint64 _starttimestamp;
	QTextEdit *_log;
};

#endif // EVENTLOGDIALOG_H
