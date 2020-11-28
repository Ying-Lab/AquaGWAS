#ifndef TOOL_H
#define TOOL_H

#include <QStringList>

class Tool
{
public:
    Tool() {}
    virtual ~Tool() {}

    QStringList getParamList(void);    // Every param can't have any space.
    QString getParamString(void);
    void setParam(QStringList param);
    void setParam(QString param);
    QString getToolPath(void);

protected:
//    const QString toolpath;
    QStringList paramlist;
};

#endif // TOOL_H
