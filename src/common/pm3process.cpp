#include "pm3process.h"

PM3Process::PM3Process(QThread* thread, QObject* parent): QProcess(parent)
{
    moveToThread(thread);
    setProcessChannelMode(PM3Process::MergedChannels);
    isRequiringOutput = false;
    requiredOutput = new QString();
    serialListener = new QTimer(); // if using new QTimer(this), the debug output will show "Cannot create children for a parent that is in a different thread."
    serialListener->moveToThread(this->thread());// I've tried many ways to creat a QTimer instance, but all of the instances are in the main thread(UI thread), so I have to move it manually
    serialListener->setInterval(1000);
    serialListener->setTimerType(Qt::VeryCoarseTimer);
    connect(serialListener, &QTimer::timeout, this, &PM3Process::onTimeout);
    connect(this, &PM3Process::readyRead, this, &PM3Process::onReadyRead);
    portInfo = nullptr;

    qRegisterMetaType<QProcess::ProcessError>("QProcess::ProcessError");
}

void PM3Process::connectPM3(const QString& path, const QStringList args)
{
    QString initialOutput;
    QString result;
    Util::ClientType clientType = Util::CLIENTTYPE_ICEMAN;
    setRequiringOutput(true);
    QRegularExpression osPattern("^\\s*(?:os:|OS\\.+)\\s*([^\\r\\n]+)", QRegularExpression::MultilineOption);
    QRegularExpression promptPattern("pm3\\s*-->|proxmark3\\s*>", QRegularExpression::CaseInsensitiveOption);

    // stash for reconnect
    currPath = path;
    currArgs = args;

    // using "-f" option to make the client output flushed after every print.
    // single '\r' might appear. Don't use QProcess::Text there or '\r' is ignored.
    start(path, args, QProcess::Unbuffered | QProcess::ReadWrite);
    if(waitForStarted(10000))
    {
        for(int i = 0; i < 50; i++)
        {
            waitForReadyRead(200);
            initialOutput = *requiredOutput;
            if(promptPattern.match(initialOutput).hasMatch())
                break;
        }
        setRequiringOutput(false);
        result = initialOutput;

        if(result.contains("[=]") || result.contains("iceman", Qt::CaseInsensitive) || result.contains("RRG", Qt::CaseInsensitive))
        {
            clientType = Util::CLIENTTYPE_ICEMAN;
        }
        else
        {
            clientType = Util::CLIENTTYPE_OFFICIAL;
        }

        if(!osPattern.match(result).hasMatch())
        {
            setRequiringOutput(true);
            write("hw version\n");
            for(int i = 0; i < 100; i++)
            {
                waitForReadyRead(200);
                result = initialOutput + *requiredOutput;
                if(osPattern.match(result).hasMatch())
                    break;
            }
            setRequiringOutput(false);
        }

        QRegularExpressionMatch osMatch = osPattern.match(result);
        if(osMatch.hasMatch())
        {
            emit changeClientType(clientType);
            emit PM3StatedChanged(true, osMatch.captured(1).trimmed());
        }
        else
        {
            qDebug() << "unexpected output:" << (result.isEmpty() ? "(empty)" : result);
            emit HWConnectFailed();
            kill();
        }
    }

    setRequiringOutput(false);
}

void PM3Process::reconnectPM3()
{
    connectPM3(currPath, currArgs);
}

void PM3Process::setRequiringOutput(bool st)
{
    isRequiringOutput = st;
    if(isRequiringOutput)
        requiredOutput->clear();
}

bool PM3Process::waitForReadyRead(int msecs)
{
    return QProcess::waitForReadyRead(msecs);
}

void PM3Process::setSerialListener(const QString& name, bool state)
{
    if(state)
    {
        currPort = name;
        portInfo = new QSerialPortInfo(name);
        serialListener->start();
        qDebug() << serialListener->thread();
    }
    else
    {
        serialListener->stop();
        if(portInfo != nullptr)
        {
            delete portInfo;
            portInfo = nullptr;
        }
    }
}

void PM3Process::setSerialListener(bool state)
{
    setSerialListener(currPort, state);
}

void PM3Process::onTimeout() //when the proxmark3 client is unexpectedly terminated or the PM3 hardware is removed, the isBusy() will return false(only tested on Windows);
{
//    isBusy() is a deprecated function because it will block the serial port when the port is not in use.
//    However, the PM3 client is supposed to use the target serial port exclusively, so it should be fine
//    isBusy() will always return false on Raspbian, in this case, check "Keep the client active" in the Settings panel.
//
//    qDebug()<<portInfo->isBusy();
    if(!portInfo->isBusy())
    {
        killPM3();
    }
}

void PM3Process::testThread()
{
    qDebug() << "PM3:" << QThread::currentThread();
}

qint64 PM3Process::write(QString data)
{
    return QProcess::write(data.toLatin1());
}

void PM3Process::onReadyRead()
{
    QString out = readAll();
    if(isRequiringOutput)
        requiredOutput->append(out);
    if(out != "")
    {
//        qDebug() << "PM3Process::onReadyRead:" << out;
        emit newOutput(out);

    }
}

void PM3Process::setProcEnv(const QStringList* env)
{
//    qDebug() << "passed Env List" << *env;
    this->setEnvironment(*env);
    //    qDebug() << "final Env List" << processEnvironment().toStringList();
}

void PM3Process::setWorkingDir(const QString& dir)
{
    // the working directory cannot be the default, or the client will failed to load the dll
    this->setWorkingDirectory(dir);
}

void PM3Process::killPM3()
{
    kill();
    emit PM3StatedChanged(false);
    setSerialListener(false);
}
