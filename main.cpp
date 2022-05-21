bool windows = false;
#ifdef _WIN32
    windows = true;
#endif
#ifdef _WIN64
    windows = true;
#endif

#include <iostream>
#include "Shell.h"

int main() {
    Shell cli(windows);
    cli.launch();

    return 0;
}
