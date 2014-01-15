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

#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QDateTime>

#include <QDebug>

#include "eventlogdialog.h"

EventLogDialog::EventLogDialog(QWidget *parent) :
	QDialog(parent)
{
	setWindowTitle("QTabletTester event log");
	QVBoxLayout *layout = new QVBoxLayout(this);
	_log = new QTextEdit(this);

	layout->addWidget(_log);

	QDialogButtonBox *buttons = new QDialogButtonBox(this);
	layout->addWidget(buttons);

	QPushButton *clear = buttons->addButton("Clear", QDialogButtonBox::ResetRole);
	QPushButton *save = buttons->addButton("Save...", QDialogButtonBox::YesRole);
	QPushButton *close = buttons->addButton("Close", QDialogButtonBox::RejectRole);

	connect(clear, SIGNAL(clicked()), _log, SLOT(clear()));
	connect(save, SIGNAL(clicked()), this, SLOT(saveLog()));
	connect(close, SIGNAL(clicked()), this, SLOT(hide()));

	resize(640, 400);

	_starttimestamp = QDateTime::currentMSecsSinceEpoch();
}

void EventLogDialog::saveLog()
{
	QString filename = QFileDialog::getSaveFileName(this, "Write event log", QString(), "Text files (*.txt)");

	if(filename.isEmpty())
		return;

	if(!filename.contains('.'))
		filename.append(".txt");

	QFile logfile(filename);
	if(!logfile.open(QFile::WriteOnly | QFile::Text)) {
		QMessageBox::warning(this, "Couldn't open file", logfile.errorString());
		return;
	}

	QTextStream stream(&logfile);
	stream << _log->document()->toPlainText();

}

void EventLogDialog::logEvent(const QString &message)
{
	_log->append(QString("%1: %2").arg(QDateTime::currentMSecsSinceEpoch() - _starttimestamp).arg(message));
}
