void drawSnake(std::vector<Coord> &snake) {
    int minx = std::numeric_limits<int>::max();
    int miny = std::numeric_limits<int>::max();
    int maxx = std::numeric_limits<int>::min();
    int maxy = std::numeric_limits<int>::min();
    for (auto &coord : snake) {
        if (coord.x < minx)
            minx = coord.x;
        if (coord.y < miny)
            miny = coord.y;
        if (coord.x > maxx)
            maxx = coord.x;
        if (coord.y > maxy)
            maxy = coord.y;
    }
    if (miny > -5)
        miny = -5;
    if (maxx < 5)
        maxx = 5;
    for (int y = miny; y <= maxy; ++y) {
        for (int x = minx; x <= maxx; ++x) {
            char ch = '.';
            Coord cur(x, y);
            for (int i = 0; i < snake.size(); ++i) {
                if (snake[i] == cur) {
                    ch = i == 0 ? 'H' : i == snake.size() - 1 ? 'T' : '0' + i;
                    break;
                }
            }
            std::cout << ch;
        }
        std::cout << "\n";
    }
}


