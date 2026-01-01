#ifndef NAV_HPP
#define NAV_HPP
#include <iostream>

namespace nav {
    enum arrowKeys {
        UP = 72,
        DOWN = 80,
        LEFT = 75,
        RIGHT = 77,
        UNKNOWN = -1
    };

    arrowKeys getKey()
    {
        int ch = _getch();
        if (ch == 224)
        {
            ch = _getch();
            switch (ch)
            {
                case UP:
                    return UP;
                case DOWN:
                    return DOWN;
                case RIGHT:
                    return RIGHT;
                case LEFT:
                    return LEFT;
                default:
                    return UNKNOWN;
            }
        }
        // Optionally, quit on 'q'
        if (ch == 'q' || ch == 'Q')
            exit(0);
        return UNKNOWN;
    }
}


#endif
