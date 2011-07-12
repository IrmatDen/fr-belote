#include "BeloteApplication.h"
#include "MainWindow.h"

int main(int argc, char **argv)
{
    BeloteApplication app(argc, argv);

    MainWindow mw;
    mw.show();

    return app.exec();
}
