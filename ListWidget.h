/*
Copyright (c) 2013-2014, BenHJ

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef LIST_WIDGET_H__
#define LIST_WIDGET_H__

#include <QSKineticScroller.h>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QFrame>
#include <QPropertyAnimation>
#include <QtWidgets/QStyledItemDelegate>
#include <QtWidgets/QMenu>
#include <QMouseEvent>
#include <QtWidgets/QTapAndHoldGesture>
#include <QtWidgets/QGestureEvent>
#include <QDebug>
#include <QtWidgets/QScrollBar>

class RemoveSelectionDelegate : public QStyledItemDelegate {
public:
    RemoveSelectionDelegate(QObject *parent = 0)
        : QStyledItemDelegate(parent) {
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const {
        // Call the original paint method with the selection state cleared
        // to prevent painting the original selection background
        QStyleOptionViewItemV4 optionV4 =
            *qstyleoption_cast<const QStyleOptionViewItemV4 *>(&option);
        optionV4.state &= ~QStyle::State_Selected;
        QStyledItemDelegate::paint(painter, optionV4, index);
    }
};

class ListWidget : public QListWidget {
    Q_OBJECT
public:
    ListWidget(QWidget *parent = 0)
        : QListWidget(parent)
        , selectionFrame(this)
        , animation(&selectionFrame, "geometry")
        , kineticScroller(this){
        // Create a semi-transparent frame that doesn't interact with anything
        selectionFrame.setAttribute(Qt::WA_TransparentForMouseEvents);
        setAttribute(Qt::WA_AcceptTouchEvents);
        grabGesture(Qt::TapAndHoldGesture);
        selectionFrame.setFocusPolicy(Qt::NoFocus);

        // You can put your transparent image in that stylesheet
        selectionFrame.setStyleSheet("background: solid rgba(0, 0, 125, 25%);");
        selectionFrame.hide();
        animation.setDuration(250);
        animation.setEasingCurve(QEasingCurve::InOutBack);

        connect(this,
                SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
                SLOT(updateSelection(QListWidgetItem*)) );
        setItemDelegate(new RemoveSelectionDelegate(this));

        this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        kineticScroller.enableKineticScrollFor(this);

        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        verticalScrollBar()->hide();
        verticalScrollBar()->resize(0, 0);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        horizontalScrollBar()->hide();
        horizontalScrollBar()->resize(0, 0);

    }

private slots:
    void resizeEvent(QResizeEvent *e) {
        QListWidget::resizeEvent(e);
        updateSelection(currentItem());
    }

    void updateSelection(QListWidgetItem* current) {
        animation.stop();
        if (!current) {
            selectionFrame.hide();
            return;
        }
        if (!selectionFrame.isVisible()) {
            selectionFrame.setGeometry(visualItemRect(current));
            selectionFrame.show();
            return;
        }
        animation.setStartValue(selectionFrame.geometry());
        animation.setEndValue(visualItemRect(current));
        animation.start();
    }
private:
    QFrame selectionFrame;
    QPropertyAnimation animation;
    QsKineticScroller kineticScroller;

    bool gestureEvent(QGestureEvent *event)
     {

         if (QGesture *tapandhold = event->gesture(Qt::TapAndHoldGesture))
         {
             tapandholdTriggered(static_cast<QTapAndHoldGesture *>(tapandhold));
          }
         return true;
     }

    void  tapandholdTriggered(QTapAndHoldGesture *tapandhold)
     {
         qDebug() << "TAPANDHOLD";
     }

    /*
    bool eventFilter(QObject *obj, QEvent *event)
    {
        if(event->type() == QEvent::ContextMenu)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*> (event);
            QMenu *menu = new QMenu(this);

            menu->addAction(new QAction("Load headers",this));
            menu->addAction(new QAction("Delete group",this));
            menu->exec(mouseEvent->globalPos());

            return false;
        }
        else
            return QListWidget::eventFilter(obj, event);
    }*/

};

#endif
