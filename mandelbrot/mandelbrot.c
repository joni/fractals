/*
 * mandelbrot.c - plot the Mandelbrot set as a PNG image
 *   
 * Compile with: 
 *
 * gcc -std=c99 mandelbrot.c -o mandelbrot $(pkg-config --cflags --libs glib-2.0 gtk+-2.0)
 *
 */

#include <stdlib.h>
#include <glib.h>
#include <glib-object.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

int main(int argc, char *argv[])
{
    int width = 1920, height = 1080, max = 1000;
    const int black = 0x000000, white = 0xffffff;

    GError *error = NULL;
    GdkPixbuf *image = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, width, height);
    guchar *pixels = gdk_pixbuf_get_pixels(image);
    int rowstride = gdk_pixbuf_get_rowstride(image);

    for (int row = 0; row < height; row++) {
        guchar *pixels_row = pixels;
        for (int col = 0; col < width; col++) {
            double c_re = (col - width/2)*4.0/width;
            double c_im = (row - height/2)*4.0/width;
            double x = 0, y = 0;
            int iterations = 0;
            while (x*x+y*y < 4 && iterations < max) {
                double x_new = x*x-y*y+c_re;
                y = 2*x*y+c_im;
                x = x_new;
                iterations++;
            } 

            int rgb;
            if (iterations < max) {
                rgb = white;
            } else {
                rgb = black;
            }
            *pixels_row++ = rgb >> 16;
            *pixels_row++ = rgb >> 8;
            *pixels_row++ = rgb >> 0;
        }
        pixels += rowstride;
    }

    if (!gdk_pixbuf_save(image, "mandelbrot.png", "png", &error, NULL)) {
        fprintf (stderr, "Unable to write file: %s\n", error->message);
    }

    g_object_unref(image);
    
    return 0;
}
