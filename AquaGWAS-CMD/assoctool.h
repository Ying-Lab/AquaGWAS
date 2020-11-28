#ifndef ASSOCTOOL_H
#define ASSOCTOOL_H

#include "tool.h"
#include <QStringList>

// Just for Association.
class AssocTool : public Tool
{
public:
    AssocTool();
    QStringList getSupportedModel(void);
    QString getToolPath(void);

protected:
    QStringList model;
};

#endif // ASSOCTOOL_H
