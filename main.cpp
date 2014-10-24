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

#include "MainApplication.h"
#include "YencDecoder.h"
#include <fstream>
#include <iostream>
#include <string>
#include <QApplication>
#include <QDir>
#include <QSplashScreen>

/**
 * @brief doConfigFileDirCheck checks if configuration file path
 * exists and creates is if not.
 *
 * The path will store all the config files for the app and
 * cached group lists and prefs files etc.
 */
void doConfigFileDirCheck()
{

    // Note, on the playbook, the home path is mapped
    // to /accounts/1000/appdata/namespace.application/data
    auto homeDir = QDir::homePath();
    QString pbnews("/pbnews");
    auto fullPath = homeDir + pbnews;
    {
        QDir dir(fullPath);
        if(!dir.exists()) {
            dir.mkpath(fullPath);
        }
    }

    auto configPath = homeDir + pbnews + "/config";
    {
        QDir dir(configPath);
        if(!dir.exists()) {
            dir.mkpath(configPath);
        }
    }

    auto cachePath = homeDir + pbnews + "/cache";
    {
        QDir cacheDir(cachePath);
        if(!cacheDir.exists()) {
            cacheDir.mkpath(cachePath);
        }
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication::addLibraryPath("app/native/lib");
    QApplication a(argc, argv);
    // Set default BB10 style. You can also use bb10dark style
    a.setStyle(QLatin1String("bb10dark"));

    QPixmap pixmap("splash.jpg");
    QSplashScreen splash(pixmap);
    splash.show();
    a.processEvents();

    doConfigFileDirCheck();

    auto mainApp = new MainApplication(new QObject);
    splash.finish(&mainApp->m_mainWidget);
    return a.exec();
}

