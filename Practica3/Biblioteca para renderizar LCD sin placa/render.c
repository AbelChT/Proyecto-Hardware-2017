//
// Created by abel on 15/12/17.
//

#include <stdio.h>
#include "render.h"

static const int width = 320, height = 240;

int getColor(int color){
    return ((15 - color)*180)/15;
}

void render(unsigned int pantalla[320][240]) {
    FILE *fp;

    fp = fopen("./pantalla.", "w+");

    fprintf(fp, "P3\n");
    fprintf(fp, "%d %d\n", width, height);
    fprintf(fp, "255\n");

    int y;
    int x;
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            int c = getColor(pantalla[x][y]);
            fprintf(fp, "%d %d %d\n", c, c, c);
        }
    }

    fclose(fp);
}

