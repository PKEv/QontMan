#include "mainwindow.h"
#include <QApplication>
#include <QMutexLocker>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QDateTime>

#include "RunGuard/RunGuard.cpp"


static QAtomicPointer<FILE> logFileFile = nullptr;
static QList<QByteArray>* logBuffer = new QList<QByteArray>();   //Store log messages until log file opened
QMutex* logBufferMutex = new QMutex();


void logMessageHandler(QtMsgType type, const QMessageLogContext& ctxt, const QString& msg)
{
    // Silence qWarning spam due to bug in QTextBrowser (trying to open a file for base64 images)
    if (ctxt.function == QString("virtual bool QFSFileEngine::open(QIODevice::OpenMode)")
            && msg == QString("QFSFileEngine::open: No file name specified"))
        return;

    QString LogMsg = QString("[%1] %2:%3 : ")
                .arg(QTime::currentTime().toString("HH:mm:ss.zzz")).arg(ctxt.file).arg(ctxt.line);
    switch (type)
    {
        case QtDebugMsg:
            LogMsg += "Debug";
            break;
        case QtWarningMsg:
            LogMsg += "Warning";
            break;
        case QtCriticalMsg:
            LogMsg += "Critical";
            break;
        case QtFatalMsg:
            LogMsg += "Fatal";
            break;
        default:
            break;
    }

    LogMsg += ": " + msg + "\n";
    QByteArray LogMsgBytes = LogMsg.toUtf8();
    fwrite(LogMsgBytes.constData(), 1, LogMsgBytes.size(), stderr);


    FILE * logFilePtr = logFileFile.load(); // atomically load the file pointer
    if (!logFilePtr)
    {
        logBufferMutex->lock();
        if(logBuffer)
        {
            logBuffer->append(LogMsgBytes);
        }
        logBufferMutex->unlock();
    }
    else
    {
        logBufferMutex->lock();
        if(logBuffer)
        {
            // empty logBuffer to file
            foreach(QByteArray msg, *logBuffer)
            {
                fwrite(msg.constData(), 1, msg.size(), logFilePtr);
            }

            delete logBuffer;   // no longer needed
            logBuffer = nullptr;
        }
        logBufferMutex->unlock();

        fwrite(LogMsgBytes.constData(), 1, LogMsgBytes.size(), logFilePtr);
        fflush(logFilePtr);
    }
}



int main(int argc, char *argv[])
{
    #ifdef LOG_TO_FILE
    qInstallMessageHandler(logMessageHandler);
    #endif

    RunGuard guard( "some_random_key" );
    if ( !guard.tryToRun() )
            return 0;

    Q_INIT_RESOURCE(res);
    QApplication a(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);
    MainWindow w;

    #ifdef LOG_TO_FILE
    QString logFileDir = QCoreApplication::applicationDirPath() + QDir::separator();
    QString logfile = logFileDir + "LOG.log";
    FILE * mainLogFilePtr = fopen(logfile.toLocal8Bit().constData(), "a");
    if(!mainLogFilePtr)
        qCritical() << "Couldn't open logfile" << logfile;

    logFileFile.store(mainLogFilePtr);   // atomically set the logFile
    #endif

    qDebug() << "Test begin";

    w.show();

    int errorcode = a.exec();

    #ifdef LOG_TO_FILE
    logFileFile.store(nullptr);   // atomically disable logging to file
    fclose(mainLogFilePtr);
    #endif

    qDebug() << "Clean exit with status" << errorcode;


    return errorcode;
}
