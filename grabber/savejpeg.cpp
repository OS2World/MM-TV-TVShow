#include "base.h"
#include "gd.h"
#include "gdfontg.h"
#include "gdfonts.h"
#include <sys\stat.h>
APIRET SaveFrameAsJPEG(char * filename, char * filetext, int width, int height, int quality, BOOL iscolor, char * sharebuf)
{

FILE *outfile;
gdImagePtr im_out;
struct stat buf;
int red,white,black;
int i,j,k,l,l1;
USHORT pix;
char rpix,gpix,bpix;
char ret;
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
   for (i = 0; i < height; i++)
      {
         for (j = 0; j < width; j++)
            {
               if (iscolor)
                  {
                     k=i*width*2+j*2;
 //                       printf("width %d, height %d j %d k%d \n",width, height,j,k);
                     pix=sharebuf[k]+(sharebuf[k+1]<<8);
                     rpix=(pix&0xf800)>>8;
                     gpix=(pix&0x7e0)>>3;
                     bpix=(pix&0x1f)<<3;
                     l=gdTrueColor(rpix,gpix,bpix);
                     im_out->tpixels[i][j] = l;
                   }
                   else
                   {
                      k=i*width+j;
                      l1=sharebuf[k];
                      l=gdTrueColor(l1,l1,l1);
                      im_out->tpixels[i][j] = l;
                   }
            }
       }
   gdImageFilledRectangle(im_out, 0, 0, width, 20, black);
   gdImageString(im_out, gdFontGiant, 9, 15, (unsigned char *) filetext, white);
/*   if (!IsLicenseValid((char *)keybuffer))
      {
         for (i = 1; i < height+20; i=i+15)
         {
            for (j = 1; j < width; j=j+135)
               {
                   gdImageString(im_out, gdFontGiant, j, i, (unsigned char *)"Illegal COPY!!", red);
               }
         }
      }
*/
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
