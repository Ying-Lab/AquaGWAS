#include "process.h"

Process::Process(QObject *parent) : QProcess(parent)
{
    connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(on_readProcessOutput()));
    connect(this, SIGNAL(readyReadStandardError()), this, SLOT(on_readProcessError()));
}

void Process::on_readProcessOutput()
{
    QString message = QString::fromLocal8Bit(this->readAllStandardOutput().data());
    emit outMessageReady(message);
}

void Process::on_readProcessError()
{
    QString message = QString::fromLocal8Bit(this->readAllStandardError().data());
    emit errMessageReady(message);
}

void Process::on_terminateProcess()
{
    this->kill();
}
