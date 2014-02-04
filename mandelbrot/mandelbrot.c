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

int colormap[] = {
    0xffbf0000, 0xffe71600, 0xffcf2700, 0xfff74500, 0xffdf5400, 0xffc75d00, 0xffef8600,
    0xffd78d00, 0xffbf8f00, 0xffe7c300, 0xffcfc200, 0xffeff700, 0xffc3df00, 0xff9cc700,
    0xffa4ef00, 0xff80d700, 0xff60bf00, 0xff5ee700, 0xff41cf00, 0xff36f700, 0xff1cdf00,
    0xff06c700, 0xff00ef0f, 0xff00d722, 0xff00bf30, 0xff00e74f, 0xff00cf5b, 0xff00f783,
    0xff00df8b, 0xff00c78f, 0xff00efc2, 0xff00d7c3, 0xff00bfbf, 0xff00d1e7, 0xff00a8cf,
    0xff00b2f7, 0xff008bdf, 0xff006ac7, 0xff0069ef, 0xff004ad7, 0xff0030bf, 0xff0024e7,
    0xff000dcf, 0xff0800f7, 0xff1c00df, 0xff2c00c7, 0xff4b00ef, 0xff5700d7, 0xff6000bf,
    0xff8900e7, 0xff8e00cf, 0xffc100f7, 0xffc300df, 0xffc100c7, 0xffef00e0, 0xffd700b6,
    0xffbf008f, 0xffe70098, 0xffcf0075, 0xfff70074, 0xffdf0054, 0xffc70038, 0xffef002d,
    0xffd70014
};
int colormap_length = sizeof(colormap) / sizeof(*colormap);

int main(int argc, char *argv[])
{
    GError *error = NULL;
    int width = 1920, height = 1080, max_iter = 1000;
    //int width = 500, height = 300, max_iter = 9800;
    //int *pixels = malloc(sizeof(int)*width*height);
    //GdkPixbuf *image = gdk_pixbuf_new_from_data((guchar *) pixels, GDK_COLORSPACE_RGB, TRUE, 8, width, height, 3*width, NULL, NULL);
    GdkPixbuf *image = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, width, height);
    guchar *pixels = gdk_pixbuf_get_pixels(image);
    int rowstride = gdk_pixbuf_get_rowstride(image);

    printf("width %d height %d rowstride %d\n", 
            gdk_pixbuf_get_width(image), gdk_pixbuf_get_height(image), rowstride);

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            double c_re = 4.0*(col - width/2)/width;
            double c_im = 4.0*(row - height/2)/width;
            double x = 0, y = 0;
            double r2;
            int iter = 0;
            do {
                double x_new = x*x-y*y+c_re;
                y = 2*x*y+c_im;
                x = x_new;
                r2 = x*x+y*y;
                iter++;
            } while (r2 < 4 && iter < max_iter);

            int rgb = colormap[iter % colormap_length];
            if (iter < max_iter) {
                rgb = colormap[iter % colormap_length];
            } else {
                rgb = 0xff000000;
            }
            pixels[3*col+0] = rgb >> 16;
            pixels[3*col+1] = rgb >> 8;
            pixels[3*col+2] = rgb >> 0;
            //pixels[4*row+3] = rgb >> 24;
        }
        pixels += rowstride;
    }

    if (!gdk_pixbuf_save(image, "mandelbrot.png", "png", &error, NULL)) {
        fprintf (stderr, "Unable to write file: %s\n", error->message);
    }
    g_object_unref(image);
    //free(pixels);
    return 0;
}
