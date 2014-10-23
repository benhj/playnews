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

#include "HeadersWidget.h"
#include "ui_HeadersWidget.h"
#include <QListWidgetItem>
#include <QStandardItem>
#include <cassert>

HeadersWidget::HeadersWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeadersWidget)
{
    ui->setupUi(this);
    ui->headersTableWidget->setColumnWidth(0, 600);
    ui->headersTableWidget->setColumnWidth(1, 180);
    ui->headersTableWidget->setColumnWidth(2, 50);
    ui->headersTableWidget->horizontalHeader()->setStretchLastSection(true);
}



HeadersWidget::~HeadersWidget()
{
    delete ui;
}

void
HeadersWidget::removeAllItems()
{
    ui->headersTableWidget->clear();
}

void
HeadersWidget::addItem(core::Header &header, int const index)
{
    auto rowCount = ui->headersTableWidget->rowCount();
    ui->headersTableWidget->setRowCount(rowCount + 1);
    QTableWidgetItem *subjectItem = new QTableWidgetItem;
    subjectItem->setData(Qt::DisplayRole, QVariant(header.getSubject().c_str()));
    QTableWidgetItem *authorItem = new QTableWidgetItem;
    authorItem->setData(Qt::DisplayRole, QVariant(header.getAuthor().c_str()));
    QTableWidgetItem *dateItem = new QTableWidgetItem;
    dateItem->setData(Qt::DisplayRole, QVariant(header.getDate().c_str()));
    ui->headersTableWidget->setItem(index, 0, subjectItem);
    ui->headersTableWidget->setItem(index, 1, authorItem);
    ui->headersTableWidget->setItem(index, 2, dateItem);
}

void
HeadersWidget::setBoldFontForItem(int const index)
{
    auto font = ui->headersTableWidget->item(index,0)->font();
    font.setBold(true);
    ui->headersTableWidget->item(index, 0)->setFont(font);
}

void
HeadersWidget::setLightFontForItem(int const index)
{

    auto font = ui->headersTableWidget->item(index,0)->font();
    font.setBold(false);
    ui->headersTableWidget->item(index, 0)->setFont(font);
    ui->headersTableWidget->item(index, 1)->setFont(font);
    ui->headersTableWidget->item(index, 2)->setFont(font);

}

void
HeadersWidget::makeRed(int const index)
{
    ui->headersTableWidget->item(index, 0)->setForeground(Qt::darkRed);
}

void
HeadersWidget::makeGreen(int const index)
{
    ui->headersTableWidget->item(index, 0)->setForeground(Qt::darkGreen);
}


void
HeadersWidget::makeBlack(int const index)
{
    ui->headersTableWidget->item(index, 0)->setForeground(Qt::black);
}
