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

#include "GroupLoader.h"
#include "ConnectorBuilder.h"
#include <sstream>

namespace core {

    GroupLoader::GroupLoader(ConnectionInfo const &connectionInfo,
                             QObject *callbackObject)
      : m_connectionInfo(connectionInfo)
      , m_callbackObject(callbackObject)
      , m_groups()
      , m_worker()
    {
    }

    GroupLoader::~GroupLoader()
    {
        m_worker.quit();
    }

    void
    GroupLoader::process()
    {
        this->moveToThread(&m_worker);
        QObject::connect(&m_worker, SIGNAL(started()), this, SLOT(loadGroups()));
        m_worker.start();
    }

    bool groupSorter(Group const &left, Group const &right)
    {
        return left.groupName < right.groupName;
    }

    void
    GroupLoader::loadGroups()
    {

        NNTPConnector connector;
        ConnectorBuilder::buildReferencedConnector(m_connectionInfo, connector, m_callbackObject);

        //
        // Call the server to LIST the groups
        //
        connector.pushCommand("LIST ACTIVE\r\n");
        connector.popAndIssueCommand();
        Groups().swap(m_groups);
        std::vector<int> codeChecks;
        codeChecks.push_back(215);
        codeChecks.push_back(503);
        std::string response = connector.getResponseString(1, "\n.\r\n");
        int status = connector.getStatus(response);
        if(status == 215) {

            std::istringstream iss(response);
            std::string responseLine;
            int g = 0;
            while ( getline(iss, responseLine, '\n') ) {
                if(g > 0) {
                    std::string name;
                    name = responseLine.substr(0, responseLine.find(" "));
                    if(!name.empty()) {
                        Group group;
                        group.groupName = name;
                        m_groups.push_back(group);
                    }
                }
                ++g;
            }
            std::sort(m_groups.begin(), m_groups.end(), groupSorter);
            qDebug() << "Finished reading active groups";
            emit groupsLoadFinishedSignal();
        }

        ConnectorBuilder::tearDownReferencedConnector(connector);
        QObject::disconnect(&m_worker, SIGNAL(started()), this, SLOT(loadGroups()));
    }

    Groups
    GroupLoader::getLoadedGroups()
    {
        return m_groups;
    }

}
