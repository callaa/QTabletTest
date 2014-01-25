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
#ifndef TABLETTESTVIEW_H
#define TABLETTESTVIEW_H

#include <QGraphicsView>

class QGraphicsItem;

class TabletTestView : public QGraphicsView
{
	Q_OBJECT
public:
	explicit TabletTestView(QWidget *parent = 0);

public slots:
	//! Enable or disable mouse tracking
	void setTracking(bool tracking);

	//! Set whether mouse move events should be ignored when tablet pen is down
	void setIgnoreMouseTablet(bool ignore);

	//! Set whether tablet events should be ignored altogether
	void setDisableTablet(bool disable);

signals:
	void logEvent(const QString &message);

protected:
	void resizeEvent(QResizeEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	bool viewportEvent(QEvent *event);

private:
	void showMouseEvent(QMouseEvent *event);
	void showTabletEvent(QTabletEvent *event);

	QGraphicsScene *_testscene;

	QGraphicsItemGroup *_pointeritem;
	QGraphicsSimpleTextItem *_pointertext;
	QGraphicsPathItem *_pointerpath;

	bool _pendown;
	bool _ignoremousetablet;
	bool _disabletablet;

};

#endif // TABLETTESTVIEW_H
