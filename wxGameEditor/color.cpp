//Color Math
//http://www.nathanm.com/photoshop-blending-math/

#include "color.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define min3(a, b, c) min(min(a, b), c)
#define max3(a, b, c) max(max(a, b), c)

long Color_HueToRgb(double Hue, double M1, double M2, double *Channel)
{
    if (Hue < 0.0)
        Hue += 1.0;
    if (Hue > 1.0)
        Hue -= 1.0;

    if ((6.0 * Hue) < 1.0)
        *Channel = (M1 + (M2 - M1) * Hue * 6.0);
    else if ((2.0 * Hue) < 1.0)
        *Channel = (M2);
    else if ((3.0 * Hue) < 2.0)
        *Channel = (M1 + (M2 - M1) * ((2.0 / 3.0) - Hue) * 6.0);
    else
        *Channel = (M1);

    return true;
}

long Color_RgbToHls(unsigned char Red, unsigned char Green, unsigned char Blue, double *Hue, double *Lumination, double *Saturation)
{
    double Delta;
    double Max, Min;
    double Redf, Greenf, Bluef;

    Redf    = (double)Red   / 255.0F;
    Greenf  = (double)Green / 255.0F;
    Bluef   = (double)Blue  / 255.0F; 

    Max     = max3(Redf, Greenf, Bluef);
    Min     = min3(Redf, Greenf, Bluef);

    *Hue        = 0;
    *Lumination = (Max + Min) / 2.0;
    *Saturation = 0;

    if (Max == Min)
        return true;

    Delta = (Max - Min);

    if (*Lumination < 0.5)
        *Saturation = Delta / (Max + Min);
    else
        *Saturation = Delta / (2.0 - Max - Min);

    if (Redf == Max)
        *Hue = (Greenf - Bluef) / Delta;
    else if (Greenf == Max)
        *Hue = (2.0 + (Bluef - Redf) / Delta);
    else
        *Hue = (4.0 + (Redf - Greenf) / Delta);

    *Hue /= 6.0;

    if (*Hue < 0.0)
        *Hue += 1.0;

    return true;
}

long Color_HlsToRgb(double Hue, double Lumination, double Saturation, unsigned char *Red, unsigned char *Green, unsigned char *Blue)
{
    double M1, M2;
    double Redf, Greenf, Bluef;

    if (Saturation == 0)
        {
        *Red    = (unsigned char)(Lumination * 255.0F);
        *Green  = *Red;
        *Blue   = *Red;
        }
    else
        {
        if (Lumination <= 0.5)
            M2 = (Lumination * (1.0 + Saturation));
        else
            M2 = (Lumination + Saturation) - (Lumination * Saturation);

        M1 = (2.0 * Lumination - M2);

        Color_HueToRgb(M1, M2, Hue + (1.0 / 3.0), &Redf);
        Color_HueToRgb(M1, M2, Hue, &Greenf);
        Color_HueToRgb(M1, M2, Hue - (1.0 / 3.0), &Bluef);

        *Red    = (unsigned char)(Redf * 255.0F);
        *Blue   = (unsigned char)(Bluef * 255.0F);
        *Green  = (unsigned char)(Greenf * 255.0F);
        }

    return true;
}