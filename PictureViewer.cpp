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

#include "PictureViewer.h"
#include "ui_PictureViewer.h"
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QScrollBar>

PictureViewer::PictureViewer(QString const &path, QWidget *parent)
  : m_path(path)
  , QWidget(parent)
  , ui(new Ui::PictureViewer)
{
    ui->setupUi(this);
    this->kineticScroller.enableKineticScrollFor(ui->pictureEdit);
    ui->pictureEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->pictureEdit->verticalScrollBar()->hide();
    ui->pictureEdit->verticalScrollBar()->resize(0, 0);
    ui->pictureEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->pictureEdit->horizontalScrollBar()->hide();
    ui->pictureEdit->horizontalScrollBar()->resize(0, 0);
    QObject::connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(save()));
}

PictureViewer::~PictureViewer()
{
    QObject::disconnect(ui->saveButton, SIGNAL(clicked()), this, SLOT(save()));
    delete ui;
}

bool copyFile(const QString& sourceFile, const QString& destinationDir)
{
    QFileInfo fileInfo(sourceFile);
    QString destinationFile = destinationDir + QDir::separator() + fileInfo.fileName();
    bool result = QFile::copy(sourceFile, destinationFile);
    return result;
}

void
PictureViewer::save()
{
    // save in shared/downloads
    QString savePath = "/accounts/1000/shared/downloads";
    (void)copyFile(m_path, savePath);
    emit pictureSavedSignal("Media saved to downloads folder...");
}
