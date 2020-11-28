#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <QObject>
#include <QCommandLineParser>
#include <QCommandLineOption>

class CommandLineParser : public QCommandLineParser
{
    Q_OBJECT
public:
    CommandLineParser();
};

#endif // COMMANDLINEPARSER_H
