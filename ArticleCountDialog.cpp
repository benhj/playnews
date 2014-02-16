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

#include "ArticleCountDialog.h"
#include "ui_articlecountdialog.h"
#include <QPainter>

ArticleCountDialog::ArticleCountDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ArticleCountDialog)
{
    ui->setupUi(this);

    setAttribute( Qt::WA_TranslucentBackground, true );

    int playbookWidthDiv2 = 1024 / 2;
    int playbookHeightDiv2 = 300;
    QSize windowSize;
    windowSize = this->size();
    int width = windowSize.width();
    int height = windowSize.height();
    int x = (playbookWidthDiv2 - (width/2));
    int y = (playbookHeightDiv2 - (height / 2));
    this->move( x, y-150 );

}

ArticleCountDialog::~ArticleCountDialog()
{
    delete ui;
}

void ArticleCountDialog::paintEvent(QPaintEvent *e)
{
    QPainter painter;
    painter.begin(this);
    QLinearGradient gradient(0, 0, width(), height());
    gradient.setColorAt(0, QColor(200,200,200,240));
    gradient.setColorAt(1, QColor(40,40,40,200));
    QBrush brush( gradient);
    brush.setStyle(Qt::LinearGradientPattern);
    painter.setBrush( brush );
    painter.setPen( Qt::NoPen );
    painter.drawRoundedRect(0,0,this->width(), this->height(), 10, 10);
    painter.end();
}