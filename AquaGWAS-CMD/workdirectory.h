#ifndef WORKDIRECTORY_H
#define WORKDIRECTORY_H

#include <QString>

class WorkDirectory
{
public:
    WorkDirectory();
    WorkDirectory(QString const name, QString const dir);

    void setProjectName(QString const name);
    QString getProjectName(void);
    void setOutputDirectory(QString const dir);
    QString getOutputDirectory(void);

private:
    QString projectName;
    QString outputDirectory;
};

#endif // WORKDIRECTORY_H
