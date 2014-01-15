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
#include <QMouseEvent>
#include <QTabletEvent>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsPathItem>

#include "tablettestview.h"

namespace {
	const QString _DEV_PUCK = "puck";
	const QString _DEV_STYLUS = "stylus";
	const QString _DEV_AIRBRUSH = "airbrush";
	const QString _DEV_4DMOUSE = "4D mouse";
	const QString _DEV_ROTSTYLUS = "rot. stylus";
	const QString _DEV_UNKNOWN = "???";

	const QString _PTR_PEN = "pen";
	const QString _PTR_CURSOR = "cursor";
	const QString _PTR_ERASER = "eraser";

	const QString &devicename(QTabletEvent::TabletDevice device)
	{
		switch(device) {
		case QTabletEvent::Puck: return _DEV_PUCK;
		case QTabletEvent::Stylus: return _DEV_STYLUS;
		case QTabletEvent::Airbrush: return _DEV_AIRBRUSH;
		case QTabletEvent::FourDMouse: return _DEV_4DMOUSE;
		case QTabletEvent::RotationStylus: return _DEV_ROTSTYLUS;
		default: return _DEV_UNKNOWN;
		}
	}

	const QString &pointername(QTabletEvent::PointerType pointer) {
		switch(pointer) {
		case QTabletEvent::Pen: return _PTR_PEN;
		case QTabletEvent::Cursor: return _PTR_CURSOR;
		case QTabletEvent::Eraser: return _PTR_ERASER;
		default: return _DEV_UNKNOWN;
		}
	}

	QPointF tabletEventPos(const QEvent *event)
	{
		const QTabletEvent *e = static_cast<const QTabletEvent*>(event);
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
		return e->pos() + QPointF(0.5, 0.5);
#else
		return e->posF();
#endif
	}
}

TabletTestView::TabletTestView(QWidget *parent) :
	QGraphicsView(parent), _pendown(false)
{
	_testscene = new QGraphicsScene(this);

	// Pointer path
	_pointerpath = new QGraphicsPathItem;
	_pointerpath->hide();
	_testscene->addItem(_pointerpath);

	// Construct pointer graphics item
	_pointeritem = new QGraphicsItemGroup;

	QPainterPath chpath;
	chpath.moveTo(-10.5, 0.5);
	chpath.lineTo(10.5, 0.5);
	chpath.moveTo(0.5, -10.5);
	chpath.lineTo(0.5, 10.5);

	new QGraphicsPathItem(chpath, _pointeritem);

	_pointertext = new QGraphicsSimpleTextItem("", _pointeritem);
	_pointertext->moveBy(20, 0);

	_testscene->addItem(_pointeritem);
	_pointeritem->moveBy(50,50);

	setScene(_testscene);

	setTracking(true);
	setIgnoreMouseTablet(true);
	setRenderHint(QPainter::Antialiasing);

}

void TabletTestView::setTracking(bool tracking)
{
	viewport()->setMouseTracking(tracking);
}

void TabletTestView::setIgnoreMouseTablet(bool ignore)
{
	_ignoremousetablet = ignore;
}

void TabletTestView::resizeEvent(QResizeEvent *event)
{
	_testscene->setSceneRect(QRect(QPoint(), event->size()));
	QGraphicsView::resizeEvent(event);
}

void TabletTestView::mousePressEvent(QMouseEvent *event)
{
	if(!_pendown || !_ignoremousetablet) {
		showMouseEvent(event);

		// Start pointer path
		QPainterPath path;
		path.moveTo(event->pos() + QPointF(0.5, 0.5));
		_pointerpath->setPen(QPen(Qt::blue));
		_pointerpath->setPath(path);
		_pointerpath->show();
	}
}

void TabletTestView::mouseMoveEvent(QMouseEvent *event)
{
	// A mouse event is generated for pen movement, so ignore it
	// if the pen is down.
	if(!_pendown || !_ignoremousetablet) {
		showMouseEvent(event);

		// Add stroke to pointerpath
		if(event->buttons()) {
			QPainterPath path = _pointerpath->path();
			path.lineTo(event->pos() + QPointF(0.5, 0.5));
			_pointerpath->setPath(path);
		}
	}
}

void TabletTestView::mouseReleaseEvent(QMouseEvent *event)
{
	showMouseEvent(event);
}

void TabletTestView::mouseDoubleClickEvent(QMouseEvent *event)
{
	showMouseEvent(event);
}

void TabletTestView::wheelEvent(QWheelEvent *event)
{
	QGraphicsView::wheelEvent(event);

	_pointeritem->setPos(mapToScene(event->pos()));

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
	_pointertext->setText(
				QString("mouse wheel at %1, %2, delta=%3, orientation=%4")
				.arg(event->x())
				.arg(event->y())
				.arg(event->delta())
				.arg(event->orientation() == Qt::Horizontal ? "horizontal" : "vertical")
	);

#else

	_pointertext->setText(
				QString("mouse wheel at %1, %2, pixeldelta=%3,%4, angledelta=%5,%6")
				.arg(event->x())
				.arg(event->y())
				.arg(event->pixelDelta().x())
				.arg(event->pixelDelta().y())
				.arg(event->angleDelta().x())
				.arg(event->angleDelta().y())
	);

#endif

	emit logEvent(_pointertext->text());
}

bool TabletTestView::viewportEvent(QEvent *event)
{
	if(event->type() == QEvent::TabletMove) {
		event->accept();
		showTabletEvent(static_cast<QTabletEvent*>(event));

		// Continue pointer path
		if(_pendown) {
			QPainterPath path = _pointerpath->path();
			path.lineTo(tabletEventPos(event));
			_pointerpath->setPath(path);
		}

	} else if(event->type() == QEvent::TabletPress) {
		event->accept();
		_pendown = true;
		showTabletEvent(static_cast<QTabletEvent*>(event));

		// Start pointer path
		QPainterPath path;
		path.moveTo(tabletEventPos(event));
		_pointerpath->setPath(path);
		_pointerpath->setPen(QPen(Qt::red));
		_pointerpath->show();

	} else if(event->type() == QEvent::TabletRelease) {
		event->accept();
		_pendown = false;
		showTabletEvent(static_cast<QTabletEvent*>(event));

	} else {
		return QGraphicsView::viewportEvent(event);
	}

	return true;
}

void TabletTestView::showMouseEvent(QMouseEvent *event)
{
	_pointeritem->setPos(mapToScene(event->pos()));

	QString buttons;
	if((event->buttons() & Qt::LeftButton))
		buttons += "Left";
	if((event->buttons() & Qt::MidButton))
		buttons += "Mid";
	if((event->buttons() & Qt::RightButton))
		buttons += "Right";

	QString action;
	if(event->type() == QEvent::MouseMove)
		action = "move";
	else if(event->type() == QEvent::MouseButtonPress)
		action = "pressed";
	else if(event->type() == QEvent::MouseButtonRelease)
		action = "released";
	else if(event->type() == QEvent::MouseButtonDblClick)
		action = "doubleclicked";

	_pointertext->setText(
				QString("mouse %1 at %2, %3, buttons=[%4]")
				.arg(action)
				.arg(event->x())
				.arg(event->y())
				.arg(buttons)
	);
	emit logEvent(_pointertext->text());
}

void TabletTestView::showTabletEvent(QTabletEvent *event)
{
	QString action;
	if(event->type() == QEvent::TabletMove)
		action = "moved";
	if(event->type() == QEvent::TabletPress)
		action = "pressed";
	else if(event->type() == QEvent::TabletRelease)
		action = "lifted";

	_pointeritem->setPos(event->pos());
	QPointF pos = tabletEventPos(event);
	_pointertext->setText(
				QString("%1 (%2) %3 at %4, %5, pressure=%6%")
				.arg(devicename(event->device()))
				.arg(pointername(event->pointerType()))
				.arg(action)
				.arg(pos.x(), 0, 'f', 1)
				.arg(pos.y(), 0, 'f', 1)
				.arg(event->pressure() * 100.0, 0, 'f', 1)
	);
	emit logEvent(_pointertext->text());
}
