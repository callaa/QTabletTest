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
#include <QApplication>
#include <QAction>
#include <QMenuBar>

#include "tablettester.h"
#include "tablettestview.h"
#include "eventlogdialog.h"

TabletTester::TabletTester(QWidget *parent)
	: QMainWindow(parent)
{
	_eventlog = new EventLogDialog(this);

	TabletTestView *testview = new TabletTestView(this);

	connect(testview, SIGNAL(logEvent(QString)), _eventlog, SLOT(logEvent(QString)));

	setCentralWidget(testview);

	QMenu *mainmenu = menuBar()->addMenu("QTabletTester");

	QAction *mousetracking = mainmenu->addAction("&Mouse tracking");
	mousetracking->setCheckable(true);
	mousetracking->setChecked(true);
	connect(mousetracking, SIGNAL(toggled(bool)), testview, SLOT(setTracking(bool)));

	QAction *ignoremouse = mainmenu->addAction("Ignore mouse during tablet use");
	ignoremouse->setCheckable(true);
	ignoremouse->setChecked(true);
	connect(ignoremouse, SIGNAL(toggled(bool)), testview, SLOT(setIgnoreMouseTablet(bool)));

	QAction *disabletablet = mainmenu->addAction("Disable tablet events");
	disabletablet->setCheckable(true);
	connect(disabletablet, SIGNAL(toggled(bool)), testview, SLOT(setDisableTablet(bool)));

	mainmenu->addSeparator();

	mainmenu->addAction("Show event &log...", _eventlog, SLOT(show()));
	mainmenu->addAction("&Save event log...", _eventlog, SLOT(saveLog()));

	mainmenu->addSeparator();
	mainmenu->addAction("&Quit", this, SLOT(close()), QKeySequence::Quit);

	QMenu *helpmenu = menuBar()->addMenu("&Help");
	helpmenu->addAction("About &Qt", qApp, SLOT(aboutQt()));

	resize(600, 600);
}

TabletTester::~TabletTester()
{
}
