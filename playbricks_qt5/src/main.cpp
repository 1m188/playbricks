#include "QApplication"
#include "Director.h"
#include "Config.h"

int main(int argc, char *argv[])
{
#ifdef RELEASE
    QApplication::addLibraryPath("./plugins"); // 发布版本从程序所在目录下寻找相关组件
#endif

    QApplication a(argc, argv);
    Config::getInstance()->init();                    // 先加载设置
    Director::getInstance()->setWindow(new Window()); // 创建游戏窗口
    Director::getInstance()->getWindow()->show();     // 显示窗口
    a.exec();
    Config::getInstance()->uninit(); // 保存相关设置
    return 0;
}
