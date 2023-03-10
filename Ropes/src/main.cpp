#include "Application.h"
#include "globals.h"

#include <SFML/Graphics.hpp>
int main()
{
    Application app(SCREEN_WIDTH, SCREEN_HEIGHT);
    app.run();
    return 0;
}