#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    if (x < 0) {
        x = 0;
    }
    if (y < 0) {
        y = 0;
    }
    if (x > im.w) {
        x = im.w;
    }
    if (y > im.h) {
        y = im.h;
    }

    return im.data[c*im.h*im.w + y*im.w + x];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    im.data[c*im.h*im.w + y*im.w + x] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            for (int k = 0; k < im.c; k++) {
                copy.data[k*copy.h*copy.w + j*copy.w + i] = im.data[k*im.h*im.w + j*im.w + i];
            }
        }
    }
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    float r_i = 0.299;
    float g_i = 0.587;
    float b_i = 0.114;
    image gray = make_image(im.w, im.h, 1);
    for (int x = 0; x < gray.w; x++) {
        for (int y = 0; y < gray.h; y++) {
            float r = get_pixel(im, x, y, 0);
            float g = get_pixel(im, x, y, 1);
            float b = get_pixel(im, x, y, 2);
            gray.data[y*im.w + x] = r*r_i + g*g_i + b*b_i;
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    for (int x = 0; x < im.w; x++) {
        for (int y = 0; y < im.h; y++) {
            float p = get_pixel(im, x, y, c);
            set_pixel(im, x, y, c, p+v);
        }
    }
}

void clamp_image(image im)
{
    for (int i = 0; i < im.w * im.h * im.c; i++) {
        if (im.data[i] < 0) {
            im.data[i] = 0;
        } else if (im.data[i] > 1) {
            im.data[i] = 1;
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    /*
    //Calculate Value
    image V = make_image(im.w, im.h, 1);
    for (int x = 0; x<im.w; x++) {
        for (int y = 0; y<im.h; y++) {
            int i = y*im.w + x;

            V.data[i] = three_way_max(im.data[i], im.data[i+im.w*im.h], im.data[i+im.w*im.h*2]);
        }
    }
    //Calculate Saturation
    image S = make_image(im.w, im.h, 1);
    for (int x = 0; x<im.w; x++) {
        for (int y = 0; y<im.h; y++) {
            int i = y*im.w + x;

            float min = three_way_min(im.data[i], im.data[i+im.w*im.h], im.data[i+im.w*im.h*2]);
            float C = S.data[i] - min;

            S.data[i] = C / V.data[i];
        }
    }
    //Calculate Hue
    image H = make_image(im.w, im.h, 1);
    for (int x = 0; x<im.w; x++) {
        for (int y = 0; y<im.h; y++) {
            int i = y*im.w + x;
            float C = V.data[i] - three_way_min(im.data[i], im.data[i+im.w*im.h], im.data[i+im.w*im.h*2]);

            if (C == 0) {
                H.data[i] = 0;
            }
            //if V == R: (G-B)/C
            else if (V.data[i] == im.data[i]) {
                H.data[i] = (im.data[i+im.w*im.h] - im.data[i+im.w*im.h*2]) / C;
            }
            //if V == G: (B-R)/C + 2
            else if (V.data[i] == im.data[i+im.w*im.h]) {
                H.data[i] = (im.data[i+im.w*im.h*2] - im.data[i]) / C + 2;
            }
            //if V == B: (R-G)/C + 4
            else if (V.data[i] == im.data[i+im.w*im.h*2]) {
                H.data[i] = (im.data[i] - im.data[i+im.w*im.h]) / C + 4;
            }

            if (H.data[i] < 0) {
                H.data[i] = H.data[i] / 6 + 1;
            } else {
                H.data[i] = H.data[i] / 6;
            }
        }
    }
    //compile Hue, Saturation, Value
    for (int x = 0; x<im.w; x++) {
        for (int y = 0; y<im.h; y++) {
            int i = y*im.w + x;
            im.data[i] = H.data[i];
            im.data[i+im.w*im.h] = S.data[i];
            im.data[i+im.w*im.h*2] = V.data[i];
        }
    }*/
}

void hsv_to_rgb(image im)
{
    /*
    image im_rgb = make_image(im.w, im.h, im.c);

    for (int x = 0; x<im.w; x++) {
        for (int y = 0; y<im.h; y++) {
            int c_0 = im.w*y + x;
            int c_1 = im.w*im.h*1 + im.w*y + x;
            int c_2 = im.w*im.h*2 + im.w*y + x;

            //Calculate C = V*S
            float C = im.data[c_2] * im.data[c_1];
            //Calculate m = V-C
            float m = im.data[c_2] - C;
            //Calculate X = C * (1-|H * 6 % 2 - 1|)
            float X = C * (1 - abs((int)(im.data[c_0] * 6) % 2 - 1));

            if (0 <= im.data[c_0]*6 && im.data[c_0]*6 < 1){
                im_rgb.data[c_0] = C+m;
                im_rgb.data[c_1] = X+m;
                im_rgb.data[c_2] = m; 
            }
            else if (1 <= im.data[c_0]*6 && im.data[c_0]*6 < 2){
                im_rgb.data[c_0] = X+m;
                im_rgb.data[c_1] = C+m;
                im_rgb.data[c_2] = m; 
            }
            else if (2 <= im.data[c_0]*6 && im.data[c_0]*6 < 3){
                im_rgb.data[c_0] = m;
                im_rgb.data[c_1] = C+m;
                im_rgb.data[c_2] = X+m; 
            }
            else if (3 <= im.data[c_0]*6 && im.data[c_0]*6 < 4){
                im_rgb.data[c_0] = m;
                im_rgb.data[c_1] = X+m;
                im_rgb.data[c_2] = C+m; 
            }
            else if (4 <= im.data[c_0]*6 && im.data[c_0]*6 < 5){
                im_rgb.data[c_0] = X+m;
                im_rgb.data[c_1] = m;
                im_rgb.data[c_2] = C+m; 
            }
            else if (5 <= im.data[c_0]*6 && im.data[c_0]*6 < 6){
                im_rgb.data[c_0] = C+m;
                im_rgb.data[c_1] = m;
                im_rgb.data[c_2] = X+m; 
            }
        }
    }
    im = im_rgb;*/
}
