#include <stdio.h>

struct Coord {
    int x;
    int y;
};

struct Coord *createStruct(int, int);

int main(int argc, const char * argv[]) {
    struct Coord *c1 = createStruct(10, 20);
    struct Coord *c2 = createStruct(30, 40);
    int x1 = c1->x, y1 = c1->y;
    int x2 = c2->x, y2 = c2->y;
    printf("First: x1=%d y1=%d x2=%d y2=%d\n", x1, y1, x2, y2);
    printf("Second: x1=%d y1=%d x2=%d y2=%d\n", c1->x, c1->y, c2->x, c2->y);
    return 0;
}

struct Coord *createStruct(int x, int y) {
    struct Coord c;
    c.x = x;
    c.y = y;
    return &c;
}
