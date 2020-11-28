#ifndef USEROS_H
#define USEROS_H

#include <QString>

class UserOS
{
public:
    UserOS();
    ~UserOS();
    QString currentOS(void);

private:
    QString curOS;
};

#endif // USEROS_H
