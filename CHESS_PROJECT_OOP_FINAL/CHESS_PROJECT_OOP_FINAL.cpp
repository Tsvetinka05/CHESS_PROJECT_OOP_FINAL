#include <iostream>
#include <windows.h>
#include "App.h"

int main() {
    SetConsoleOutputCP(65001);

    App app;
    app.run();

    return 0;
}

