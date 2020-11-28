#include "workdirectory.h"

WorkDirectory::WorkDirectory()
{

}

void WorkDirectory::setProjectName(QString const name)
{
    if (!name.isEmpty())
    {
        this->projectName = name;
    }
}

QString WorkDirectory::getProjectName(void)
{
    return this->projectName;
}

void WorkDirectory::setOutputDirectory(QString const dir)
{
    if (!dir.isEmpty())
    {
        this->outputDirectory = dir;
    }
}

QString WorkDirectory::getOutputDirectory(void)
{
    return this->outputDirectory;
}
