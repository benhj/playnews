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

#pragma once

#include "ConnectionInfo.h"
#include "NNTPConnector.h"
#include <QObject>
#include <QScopedPointer>
#include <vector>
#include <QThread>

namespace core {

    /**
     * @brief The Group struct stores data related to a group
     */
    struct Group
    {
        std::string groupName;
        int upperArticle;
        int lowerArticle;
    };

    typedef std::vector<Group> Groups;

    class GroupLoader;
    typedef QScopedPointer<GroupLoader> GroupLoaderPtr;

    class GroupLoader : public QObject
    {
        Q_OBJECT
      public:
        GroupLoader(ConnectionInfo const &connectionInfo, QObject *callbackObject);

        ~GroupLoader();

        /**
         * @brief getLoadedGroups get vector of groups
         * @return vector of groups
         */
        Groups getLoadedGroups();

        void process();

      public slots:

        void loadGroups();

      signals:
        void groupsLoadFinishedSignal();

      private:
        ConnectionInfo m_connectionInfo;
        QObject *m_callbackObject;
        Groups m_groups;
        QThread m_worker;     
    };

}
