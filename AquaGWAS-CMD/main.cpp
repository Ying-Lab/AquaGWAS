#include <QCoreApplication>
#include "commandlineparser.h"

int main(int argc, char *argv[])
{  
    QCoreApplication app(argc, argv);

    app.setApplicationName("H_G_CMD");
    app.setApplicationVersion("0.0.1");

    CommandLineParser parser;//创建对象的同时就利用构造函数将其初始化了，里面设置了各种命令选项

    parser.process(app);
    parser.parseCommand(); //解析命令 并调用相应工具

    return 0;
}

