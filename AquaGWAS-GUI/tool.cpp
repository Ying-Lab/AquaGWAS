#include "tool.h"

QStringList Tool::getParamList(void)
{
    return this->paramlist;
}

QString Tool::getParamString(void)
{
    QString ret;
    for (QString item:this->paramlist)
    {
        ret += item + " ";
    }
    return ret;
}

void Tool::setParam(QStringList param)
{
    this->paramlist = param;
}

void Tool::setParam(QString param)
{
    this->paramlist = param.split(QRegExp("\\s+"), QString::SkipEmptyParts);
}
