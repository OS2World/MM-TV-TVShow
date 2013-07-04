/*
** Module   :SAVEJPEG.CPP
** Abstract :saving image as jpeg file routines
**
** Copyright (C) Alex Cherkaev
**
** Log: Sat  24/04/2004 Created
**
*/

#include "base.h"
#include "gd.h"
#include "gdfontg.h"
#include "gdfonts.h"
#include <sys\stat.h>
static void yuvtorgb(int Y, int U, int V, unsigned char *rgb) {
   int r, g, b;
   static short L1[256], L2[256], L3[256], L4[256], L5[256];
   static int initialised;

   if (!initialised) {
       int i;
       initialised = 1;
       for (i = 0; i < 256; i++) {
           L1[i] = 1.164 * (i - 16);
           L2[i] = 1.596 * (i - 128);
           L3[i] = -0.813 * (i - 128);
           L4[i] = 2.018 * (i - 128);
           L5[i] = -0.391 * (i - 128);
       }
   }

   r = L1[Y] + L2[V];
   g = L1[Y] + L3[U] + L5[V];
   b = L1[Y] + L4[U];

   if (r < 0) r = 0;
   if (g < 0) g = 0;
   if (b < 0) b = 0;
   if (r > 255) r = 255;
   if (g > 255) g = 255;
   if (b > 255) b = 255;

   rgb[0] = r;
   rgb[1] = g;
   rgb[2] = b;
}
APIRET SaveFrameAsJPEG(PCHAR filename, PCHAR filetext, int width, int height, int quality, BOOL bColor, PUCHAR pFrameBuf)
{
    FILE *outfile;
	gdImagePtr im_out;
	struct stat buf;
	int red,white,black;
	long i,j,k,l,l1;
	USHORT pix;
	char rpix,gpix,bpix;
	double R1,G1,B1,R2,G2,B2,Y1,Y2,U,V;
	char ret;
    unsigned char rgb[4]={0,0,0,0};
	ret=0;
    if (NULL == (outfile=fopen(filename, "wb")))
        {
        	printf("Error open file for writing pic \n");
           	return 1;
        }
    im_out = gdImageCreateTrueColor (width, height+20);
	white = gdImageColorAllocate (im_out, 255, 255, 255);
	black = gdImageColorAllocate (im_out, 0, 0, 0);
    red = gdImageColorAllocate(im_out, 255, 0, 0);
    gdImageFilledRectangle(im_out, 0, 0, width, height+20, black);

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            k=i*width*2+j*2;
            Y1=pFrameBuf[k];
            U=pFrameBuf[k+1];
            Y2=pFrameBuf[k+2];
            V=pFrameBuf[k+3];
            yuvtorgb(Y1,U,V,rgb);
            im_out->tpixels[i][j] = (rgb[0]<<16)|(rgb[1]<<8)|rgb[2];
            yuvtorgb(Y2,U,V,rgb);
            im_out->tpixels[i][j+1]=(rgb[0]<<16)|(rgb[1]<<8)|rgb[2];
            j++;
       }
   }
   gdImageString(im_out, gdFontGiant, 9, 15, (unsigned char *) filetext, white);
/*   if (!IsLicenseValid((char *)keybuffer)) */
    {
        for (i = 40; i < height; i=i+15)
        {
//            for (j = 1; j < width; j=j+135)
            {
                gdImageString(im_out, gdFontGiant, 10, i, (unsigned char *)"Unregistered COPY", red);
            }
        }
    }
    gdImageJpeg(im_out, outfile, 75);
    if (fflush(outfile))
    {
        printf("Error flush file for writing pic \n");
        ret=1;
    }
    if (fstat(fileno(outfile),&buf)!=0)
    {
        printf("Error get status file for writing pic \n");
        ret=1;
    }
    if (buf.st_size<20000) ret=9;
    if (fclose(outfile)!=0)
    {
        printf("Error close file for writing pic \n");
        ret=1;
    }
    gdImageDestroy (im_out);
    return ret;
}
