QMAKE_LFLAGS += '-Wl,-rpath,\'./app/native/lib\''

QMAKE_CFLAGS_X86_64 += -mmacosx-version-min=10.7
QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64

QT       += network
QT       += gui
QT       += core

INCLUDEPATH += /usr/local/boost_1_53_0/

SOURCES += main.cpp MainWidget.cpp \
    MainApplication.cpp \
    LoginDialog.cpp \
    ManagedNNTPConnection.cpp \
    NNTPConnector.cpp \
    CustomTextEdit.cpp \
    QSKineticScroller.cpp \
    ManagedGroupTab.cpp \
    MessageReader.cpp \
    GroupWidget.cpp \
    HeadersWidget.cpp \
    ArticleCountDialog.cpp \
    PostWidget.cpp \
    ConnectionThread.cpp \
    LoginThread.cpp \
    PostMaster.cpp \
    YencDecoder.cpp \
    SearchDialog.cpp \
    StatusMessageDisplayer.cpp \
    HeaderExtractor.cpp \
    GroupLoader.cpp \
    PictureViewer.cpp \
    PrefsWidget.cpp \
    ArticleReader.cpp \
    BulkDownloader.cpp \
    BinaryGrabber.cpp \
    HeadersExtractorStructure.cpp \
    ArticleSelectAndReadStructure.cpp \
    CompositeSelectAndReadStructure.cpp
HEADERS += MainWidget.h \
    MainApplication.h \
    LoginDialog.h \
    ManagedNNTPConnection.h \
    NNTPConnector.h \
    CustomTextEdit.h \
    ListWidget.h \
    QSKineticScroller.h \
    ManagedGroupTab.h \
    MessageReader.h \
    GroupWidget.h \
    HeadersWidget.h \
    ArticleCountDialog.h \
    PostWidget.h \
    ConnectionThread.h \
    LoginThread.h \
    PostMaster.h \
    YencDecoder.h \
    SearchDialog.h \
    StatusMessageDisplayer.h \
    CompositeMessagePart.h \
    HeaderExtractor.h \
    GroupLoader.h \
    PictureViewer.h \
    PrefsWidget.h \
    ArticleReader.h \
    BulkDownloader.h \
    ArticleHeader.h \
    BinaryGrabber.h \
    HeadersExtractorStructure.h \
    ArticleSelectAndReadStructure.h \
    CompositeSelectAndReadStructure.h \
    TableWidget.h \
    CompositeArticleLoaderChecker.h \
    ConnectorBuilder.h \
    ConnectionInfo.h
    #SocketReader.h \
    #StreamWriter.h
FORMS   += mainwidget.ui \
    messagereader.ui \
    groupwidget.ui \
    headerswidget.ui \
    articlecountdialog.ui \
    postwidget.ui \
    searchdialog.ui \
    statusmessagedisplayer.ui \
    pictureviewer.ui \
    prefswidget.ui

OTHER_FILES += \
    bar-descriptor.xml

RESOURCES += \
    iconResources.qrc


