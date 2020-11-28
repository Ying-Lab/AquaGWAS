#ifndef PROCESS_H
#define PROCESS_H

#include <QDebug>
#include <QProcess>

class Process : public QProcess
{
    Q_OBJECT
public:
    explicit Process(QObject *parent = nullptr);
    QString getMessage();

public slots:
    void on_readProcessOutput();
    void on_readProcessError();
    void on_terminateProcess();

signals:
    void outMessageReady(const QString);
    void errMessageReady(const QString);
};

#endif // PROCESS_H
