#include "useros.h"

UserOS::UserOS()
{
    this->curOS = QSysInfo::kernelType();
}

QString UserOS::currentOS(void)
{
    return this->curOS;
}
