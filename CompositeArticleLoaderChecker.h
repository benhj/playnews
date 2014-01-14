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

#ifndef COMPOSITEARTICLELOADER_H
#define COMPOSITEARTICLELOADER_H

#include "ArticleReader.h"
#include "NNTPConnector.h"
#include "ConnectorBuilder.h"
#include <QObject>
#include <QScopedPointer>
#include <QMutex>
#include <vector>

class CompositeArticleLoaderChecker;

typedef QScopedPointer<CompositeArticleLoaderChecker> CompositeCheckerPtr;
typedef std::vector<SharedArticleReader> ArticleReaders;

class CompositeArticleLoaderChecker : public QObject
{
    Q_OBJECT
public:
    CompositeArticleLoaderChecker(std::vector<int> codes,
                                  ConnectionInfo &connectionInfo,
                                  QString const &groupName,
                                  QObject* callBackObject) :
        m_codes(codes), m_connectionInfo(connectionInfo),
        m_groupName(groupName), m_callback(callBackObject),
        m_partsGrabbed(0) {it = m_codes.begin();}

    void readArticle(int const articleId)
    {
        int isBinary = 1;
        SharedArticleReader arp(new ArticleReader(m_connectionInfo, m_groupName, articleId, isBinary, m_callback));
        m_arps.push_back(arp);
        QObject::connect(arp.data(), SIGNAL(articleDataReadSignal(ArticleData&)),
                         this, SLOT(partDownloaded(ArticleData&)), Qt::QueuedConnection);

        arp->process();
        //arp->readArticle();
    }

    void doLoad()
    {
        for(it = m_codes.begin() ; it != m_codes.end(); ++it) {
            readArticle(*it);
        }
    }

public slots:
    void partDownloaded(ArticleData&)
    {
        m_mutex.lock();
        qDebug() << "part downloaded!";
        ++m_partsGrabbed;

        if(m_partsGrabbed == m_codes.size()) {
            qDebug() << "yes finished comp";
            emit compositeFinishedSignal();
        } else {
            //++it;
            //doLoad();
        }
        m_mutex.unlock();
        //
    }

signals:
    void compositeFinishedSignal();

private:
    std::vector<int> m_codes;
    ArticleReaders m_arps;
    int m_partsGrabbed;
    ConnectionInfo m_connectionInfo;
    QString m_groupName;
    QMutex m_mutex;
    QObject *m_callback;
    std::vector<int>::iterator it;
};

#endif // COMPOSITEARTICLELOADER_H
