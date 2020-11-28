#include "assoctool.h"

AssocTool::AssocTool()
{

}

/**
 * @brief AssocTool::getSupportedModel
 * @return Supported model of current tool.
 */
QStringList AssocTool::getSupportedModel(void)
{
    return this->model;
}

