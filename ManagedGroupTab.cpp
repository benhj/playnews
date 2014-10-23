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

#include "ManagedGroupTab.h"
#include "ui_MainWidget.h"
#include "HeadersWidget.h"
#include "ui_HeadersWidget.h"
#include "PostWidget.h"
#include "ui_PostWidget.h"
#include "MessageReader.h"
#include "ui_MessageReader.h"
#include "PostMaster.h"
#include "SearchDialog.h"
#include "StatusMessageDisplayer.h"
#include "PictureViewer.h"
#include "ui_PictureViewer.h"
#include "MessageReader.h"
#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QUrl>
#include <QImageReader>
#include <sstream>
#include <fstream>

ManagedGroupTab::ManagedGroupTab(QObject *parent,
                                 QString const &groupName,
                                 MainWidget &w,
                                 ConnectionPtr &connection,
                                 ConnectionInfo const& connectionInfo,
                                 QThread &worker,
                                 StatusMessageDisplayer &statusMessageDisplayer,
                                 Headers &headers) :
                                    QObject(parent),
                                    m_groupName(groupName),
                                    m_w(w),
                                    m_connection(connection),
                                    m_connectionInfo(connectionInfo),
                                    m_worker(worker),
                                    m_statusMessageDisplayer(statusMessageDisplayer),
                                    m_articleLoaderThread(NULL),
                                    m_parentWidget((QTabWidget*)parent),
                                    m_headers(headers)
{
    this->addGroupTab();
}


ManagedGroupTab::~ManagedGroupTab()
{
    //
    // (1) Remove tab from tab widget
    //
    int tabIndex = m_w.ui->tabWidget->indexOf(m_selectedGroupTab);
    m_w.ui->tabWidget->removeTab(tabIndex);

    //
    // (2) Delete grid layout
    //
    delete m_gridLayout;

    //
    // (4) Delete tab
    //
    delete m_selectedGroupTab;

    //
    // (5) Clear headers vector
    //
    Headers().swap(m_headers);
}

void
ManagedGroupTab::addGroupTab()
{
    //
    // Create a new tab with selected group name and a ListWidget
    // for displaying the actual headers. Note
    // I'm sure there's an easier way to set name of
    // tab; method here ripped from exported designer code
    //
    m_selectedGroupTab = new QWidget();
    m_selectedGroupTab->setAttribute(Qt::WA_DeleteOnClose);
    m_gridLayout = new QGridLayout(m_selectedGroupTab);
    m_gridLayout->setContentsMargins(0, 0, 0, 0);
    m_gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    m_headersWidget = new HeadersWidget(m_selectedGroupTab);

    //
    // When a given article is selected, read it
    //
    QObject::connect(m_headersWidget->ui->readButton, SIGNAL(clicked()), this,
                     SLOT(readArticleSlot()));

    //
    // For posting a new article to the group
    //
//    QObject::connect(m_headersWidget->ui->postButton, SIGNAL(clicked()), this,
//                     SLOT(postSlot()));

    QObject::connect(m_headersWidget->ui->searchButton, SIGNAL(clicked()), this,
                     SLOT(searchSlot()));

    QObject::connect(m_headersWidget->ui->showAllButton, SIGNAL(clicked()), this,
                     SLOT(showAllSlot()));

//    QObject::connect(m_headersWidget->ui->bulkDLButton, SIGNAL(clicked()), this,
//                     SLOT(bulkDownloadSlot()));


    m_headersWidget->setObjectName(QString::fromUtf8("headersWidget"));
    m_headersWidget->setStyleSheet(QString::fromUtf8("border: none"));
    m_gridLayout->addWidget(m_headersWidget, 1, 0, 1, 1);
    m_w.ui->tabWidget->addTab(m_selectedGroupTab, QString());

    m_w.ui->tabWidget->setTabText(m_w.ui->tabWidget->indexOf(m_selectedGroupTab),
                                  QApplication::translate("MainWidget",
                                                          m_groupName.toStdString().c_str(),
                                                          0));

    //
    // Add headers to tab
    //
    Headers::reverse_iterator headerIterator;
    for(headerIterator = m_headers.rbegin() ;
        headerIterator != m_headers.rend() ;
        ++headerIterator) {
            m_headerIndices.push_back(headerIterator->index);
            m_headersWidget->addItem(*headerIterator, headerIterator-m_headers.rbegin());
            headerIterator->row = headerIterator - m_headers.rbegin();
            m_headersWidget->setBoldFontForItem(headerIterator - m_headers.rbegin());
            if(headerIterator->isComposite) {
                if(headerIterator->downloadPath.isEmpty()) {
                    m_headersWidget->makeRed(headerIterator - m_headers.rbegin());
                } else {
                    m_headersWidget->makeGreen(headerIterator - m_headers.rbegin());
                }
            }
    }
}

void
ManagedGroupTab::readArticleSlot()
{
    int currentRow = m_headersWidget->ui->headersTableWidget->currentRow();
    m_headers[m_headerIndices[currentRow]].hasBeenSelected = true;
    m_headersWidget->setLightFontForItem(currentRow);

    int index = m_headerIndices[currentRow];
    m_selectedArticleIndex = index;    // so can save path in header when saved if binary

    //
    // See if a cached copy of the file already exists. If so then we can just open it
    // and return rather than re-downloading it. If not, it can be downloaded
    //
    if(!m_headers[index].downloadPath.isEmpty()) {
        emit loadingMediaSignal("Loading media from cache...");
        openBinary(m_headers[index], false);
        return;
    }

    //
    // Deal with composite headers which consist of multiple article IDs
    // differently than simple single-article headers
    //
    if(!m_headers[index].isComposite) {
        //
        // what is the header index as it exists in the vector of headers?
        //
        m_articleCode = m_headers[m_headerIndices[currentRow]].messageId;
        if(m_articleCode > -1) {
            qDebug() << "no, not composite..";
            QObject::connect(m_connection.data(),
                             SIGNAL(articleDataReadSignal(ArticleData&)),
                             this,
                             SLOT(displayArticleSlot(ArticleData&)));

            m_subject = m_headers[m_headerIndices[currentRow]].subject;
            m_author = m_headers[m_headerIndices[currentRow]].author;
            m_date = m_headers[m_headerIndices[currentRow]].date;
            m_connection->selectAndRead(m_groupName, m_articleCode);
        }
    } else {

        bool compositeReadMode = true;
        m_binaryGrabberPtr.reset(new BinaryGrabber(m_connection,
                                                   m_groupName,
                                                   m_headers[index],
                                                   m_worker,
                                                   compositeReadMode));


        QObject::connect(m_binaryGrabberPtr.data(), SIGNAL(binaryHasBeenReadSignal(Header, bool)),
                         this, SLOT(openBinary(Header, bool)));

        QObject::connect(m_binaryGrabberPtr.data(), SIGNAL(resetProgressBarSignal()),
                         this, SLOT(resetProgressBar()));

        QObject::connect(m_binaryGrabberPtr.data(), SIGNAL(setProgressBarMaximum(int)),
                         this, SLOT(setProgressBarMaximum(int)));

        QObject::connect(m_binaryGrabberPtr.data(), SIGNAL(partDecodedSignal()),
                         this, SLOT(partDecodedSlot()));

        qDebug() << "yes composite";
        m_binaryGrabberPtr->readMultiPartBinary();

    }
}

void
ManagedGroupTab::updateProgressBar(int const val)
{
    m_headersWidget->ui->progressBar->setValue(val);
}

void
ManagedGroupTab::setProgressBarMaximum(int const val)
{
    m_headersWidget->ui->progressBar->setMaximum(val);
}

void
ManagedGroupTab::resetProgressBar()
{
    m_headersWidget->ui->progressBar->reset();
}

void
ManagedGroupTab::updateBytesDisplay(double const val)
{
    QString temp = QString::number(val, 'f', 2);
    m_headersWidget->ui->bytesDisplay->display(temp);
}

void
ManagedGroupTab::displayArticleSlot(ArticleData &articleData)
{
    qDebug() << "displayArticleSlot";
    if(articleData.binary == 0) {
        QString article = articleData.data;
        MessageReader *m_mr = new MessageReader;
        m_mr->setAttribute(Qt::WA_DeleteOnClose);

        //mr->ui->messageEdit->insertHtml(article);
        m_mr->ui->messageEdit->clear();
        m_mr->setSubject(m_subject.c_str());
        m_mr->setAuthor(m_author.c_str());
        m_mr->setDate(m_date.c_str());
        m_mr->setArticle(article);
        m_mr->ui->messageEdit->moveCursor (QTextCursor::Start) ;
        m_mr->ui->messageEdit->ensureCursorVisible();
        m_mr->resize(1024,600);
        m_mr->show();
        m_statusMessageDisplayer.raise();
    } else {
        qDebug() << "displayArticleSlot binary";
        //
        // File contains y-encoded binary data so handle this differently
        // (e.g. save... / setup a file queue etc.)
        //
        bool compositeReadMode = false;
        m_binaryGrabberPtr.reset(new BinaryGrabber(m_connection,
                                                   m_groupName,
                                                   m_headers[m_selectedArticleIndex],
                                                   m_worker,
                                                   compositeReadMode));
        QObject::connect(m_binaryGrabberPtr.data(), SIGNAL(binaryHasBeenReadSignal(Header, bool)),
                         this, SLOT(openBinary(Header, bool)));
        m_binaryGrabberPtr->handleBinaryData();

    }
    QObject::disconnect(m_connection.data(),
                     SIGNAL(articleDataReadSignal(ArticleData&)),
                     this,
                     SLOT(displayArticleSlot(ArticleData&)));
}

//bool copyFile_(const QString& sourceFile, const QString& destinationDir)
//{
//    QFileInfo fileInfo(sourceFile);
//    QString destinationFile = destinationDir + QDir::separator() + fileInfo.fileName();
//    bool result = QFile::copy(sourceFile, destinationFile);
//    return result;
//}

void ManagedGroupTab::openBinary(Header header, bool const autoSave)
{

    QString fileName = header.downloadPath;
    QFileInfo fileInfo(fileName);
    std::string filename = fileInfo.fileName().toStdString();

    qDebug() << "in ManagedGroupTab openBinary..";

    if(!QString(QImageReader::imageFormat(fileName)).isEmpty() && !autoSave) {
        QImage image(fileName);
        image = image.scaledToWidth(1024, Qt::FastTransformation);
        PictureViewer *pictureViewer = new PictureViewer(fileName);
        QObject::connect(pictureViewer, SIGNAL(pictureSavedSignal(QString)), this, SLOT(pictureSavedSlot(QString)));
        pictureViewer->setAttribute(Qt::WA_DeleteOnClose);
        QTextCursor cursor = pictureViewer->ui->pictureEdit->textCursor();
        QTextDocument *document = pictureViewer->ui->pictureEdit->document();
        document->addResource(QTextDocument::ImageResource, QUrl("image"), image);
        cursor.insertImage("image");
        m_parentWidget->addTab(pictureViewer, QString(filename.c_str()));
        m_statusMessageDisplayer.raise();
    } /* else { // some other binary file, just save to disk
        QString savePath = "/accounts/1000/shared/downloads";
        qDebug() <<"a2";
        (void)copyFile_(fileName, savePath);
        qDebug() <<"a3";
        emit pictureSavedSignal("Media saved to downloads folder...");
        qDebug() <<"a4";
    }*/
    this->setProgressBarMaximum(1);

    if(!autoSave) {
        QObject::disconnect(m_binaryGrabberPtr.data(), SIGNAL(binaryHasBeenReadSignal(Header, bool)),
                            this, SLOT(openBinary(Header, bool)));
        QObject::disconnect(m_binaryGrabberPtr.data(), SIGNAL(resetProgressBarSignal()),
                         this, SLOT(resetProgressBar()));
        QObject::disconnect(m_binaryGrabberPtr.data(), SIGNAL(setProgressBarMaximum(int)),
                         this, SLOT(setProgressBarMaximum(int)));
        QObject::disconnect(m_binaryGrabberPtr.data(), SIGNAL(partDecodedSignal()),
                         this, SLOT(partDecodedSlot()));
    }

    int row = header.row;
    m_headersWidget->makeGreen(row);

    qDebug() <<"a5";
}

//void
//ManagedGroupTab::postSlot()
//{
//    PostMaster *pm = new PostMaster(m_connectionInfo, m_groupName, this);
//    pm->displayPostWidget();
//}

//void
//ManagedGroupTab::replySlot(QString const &articleData)
//{

//    PostMaster *pm = new PostMaster(m_connectionInfo, m_groupName, this);
//    std::stringstream ss(articleData.toStdString());
//    std::string articleLine;
//    std::stringstream processed;
//    processed << "\n\n";
//    while(std::getline(ss, articleLine)) {
//        processed << ">" << articleLine << "\n";
//    }

//    pm->displayPostWidget(QString(processed.str().c_str()));

//}

void
ManagedGroupTab::searchSlot()
{
    QWidget widget;
    SearchDialog sd(&widget);
    std::vector<int>().swap(m_headerIndices);
    if(sd.exec() == QDialog::Accepted) {
        QString returned = sd.getText();
        m_headersWidget->removeAllItems();
        int row = 0;
        for(Headers::reverse_iterator it = m_headers.rbegin(); it != m_headers.rend(); ++it) {
            if(std::string(returned.toStdString()).empty()) {
                m_headerIndices.push_back(it->index);
                m_headersWidget->addItem(*it, row);
                if(it->hasBeenSelected) {
                    m_headersWidget->setLightFontForItem(row);
                } else {
                    m_headersWidget->setBoldFontForItem(row);
                }

                if(it->isComposite) {
                    if(it->downloadPath.isEmpty()) {
                        m_headersWidget->makeRed(row);
                    } else {
                        m_headersWidget->makeGreen(row);
                    }
                }
                it->row = row;
                row++;

            } else if(std::string(it->subject).find(returned.toStdString()) != std::string::npos) {
                m_headerIndices.push_back(it->index);
                m_headersWidget->addItem(*it, row);
                if(it->hasBeenSelected) {
                    m_headersWidget->setLightFontForItem(row);
                } else {
                    m_headersWidget->setBoldFontForItem(row);
                }

                if(it->isComposite) {
                    if(it->downloadPath.isEmpty()) {
                        m_headersWidget->makeRed(row);
                    } else {
                        m_headersWidget->makeGreen(row);
                    }
                }
                it->row = row;
                row++;
            }
        }
    }
}

void
ManagedGroupTab::showAllSlot()
{
    emit resetHeadersList("Showing all headers...");
    m_headersWidget->removeAllItems();
    std::vector<int>().swap(m_headerIndices);
    int row = 0;
    for(Headers::reverse_iterator it = m_headers.rbegin(); it != m_headers.rend(); ++it) {
        m_headerIndices.push_back(it->index);
        m_headersWidget->addItem(*it, row);
        if(it->hasBeenSelected) {
            m_headersWidget->setLightFontForItem(row);
        } else {
            m_headersWidget->setBoldFontForItem(row);
        }

        if(it->isComposite) {
            if(it->downloadPath.isEmpty()) {
                m_headersWidget->makeRed(row);
            } else {
                m_headersWidget->makeGreen(row);
            }
        }
        it->row = row;
        row++;
    }
}

void
ManagedGroupTab::partDecodedSlot()
{
    int oldValue = m_headersWidget->ui->progressBar->value();
    m_headersWidget->ui->progressBar->setValue(oldValue + 1);
}

void
ManagedGroupTab::pictureSavedSlot(QString message)
{
    emit pictureSavedSignal(message);
}

//void
//ManagedGroupTab::bulkDownloadSlot()
//{
//    QWidget widget;
//    SearchDialog sd(&widget);
//    sd.setLabelText("Bulk DL mask: ");

//    if(sd.exec() == QDialog::Accepted) {
//        m_bulkDownloaderPtr.reset(new BulkDownloader(m_connection, m_groupName, m_worker));
//        QObject::connect(m_bulkDownloaderPtr.data(), SIGNAL(openBinarySignal(Header,bool)), this,
//                         SLOT(openBinary(Header,bool)));
//        std::string mask = sd.getText().toStdString();
//        Headers::iterator it;
//        for(it = m_headers.begin(); it != m_headers.end(); ++it) {
//            std::string subjectLine = it->subject;
//            if(subjectLine.find(mask) != std::string::npos) {
//                m_bulkDownloaderPtr->addHeader(&(*it));
//                qDebug() << "added header with subjectline "<<(it->subject).c_str();
//            }
//        }
//        m_bulkDownloaderPtr->bulkDownload();
//    }
//}
