#define ChannelBlend_Normal(B,L)      ((unsigned char)(B))
#define ChannelBlend_Lighten(B,L)     ((unsigned char)((L > B) ? L:B))
#define ChannelBlend_Darken(B,L)      ((unsigned char)((L > B) ? B:L))
#define ChannelBlend_Multiply(B,L)    ((unsigned char)((B * L) / 255))
#define ChannelBlend_Average(B,L)     ((unsigned char)((B + L) / 2))
#define ChannelBlend_Add(B,L)         ((unsigned char)(min(255, (B + L))))
#define ChannelBlend_Subtract(B,L)    ((unsigned char)((B + L < 255) ? 0:(B + L - 255)))
#define ChannelBlend_Difference(B,L)  ((unsigned char)(abs(B - L)))
#define ChannelBlend_Negation(B,L)    ((unsigned char)(255 - abs(255 - B - L)))
#define ChannelBlend_Screen(B,L)      ((unsigned char)(255 - (((255 - B) * (255 - L)) >> 8)))
#define ChannelBlend_Exclusion(B,L)   ((unsigned char)(B + L - 2 * B * L / 255))
#define ChannelBlend_Overlay(B,L)     ((unsigned char)((L < 128) ? (2 * B * L / 255):(255 - 2 * (255 - B) * (255 - L) / 255)))
#define ChannelBlend_SoftLight(B,L)   ((unsigned char)((L < 128) ? (2 * ((B >> 1) + 64)) * (L / 255): \
                                      (255 - (2 * (255 - ((B >> 1) + 64)) * (255 - L) / 255))))
#define ChannelBlend_HardLight(B,L)   (ChannelBlend_Overlay(L,B))
#define ChannelBlend_ColorDodge(B,L)  ((unsigned char)((B == 255) ? B:min(255, ((L << 8 ) / (255 - B)))))
#define ChannelBlend_ColorBurn(B,L)   ((unsigned char)((B == 0) ? B:max(0, (255 - ((255 - L) << 8 ) / B))))
#define ChannelBlend_LinearDodge(B,L) (ChannelBlend_Add(B,L))
#define ChannelBlend_LinearBurn(B,L)  (ChannelBlend_Subtract(B,L))
#define ChannelBlend_LinearLight(B,L) ((unsigned char)(B < 128)? ChannelBlend_LinearBurn((2*B),L):ChannelBlend_LinearDodge((2*(B - 128)),L))
#define ChannelBlend_VividLight(B,L)  ((unsigned char)(B < 128)? ChannelBlend_ColorBurn((2*B),L):ChannelBlend_ColorDodge((2*(B - 128)),L))
#define ChannelBlend_PinLight(B,L)    ((unsigned char)(B < 128)? ChannelBlend_Darken((2 * B),L):ChannelBlend_Lighten((2 *(B - 128)),L))
#define ChannelBlend_HardMix(B,L)     ((unsigned char)((ChannelBlend_VividLight(B,L) < 128) ? 0:255))
#define ChannelBlend_Reflect(B,L)     ((unsigned char)((L == 255) ? L:min(255, (B * B / (255 - L)))))
#define ChannelBlend_Glow(B,L)        (ChannelBlend_Reflect(L,B))
#define ChannelBlend_Phoenix(B,L)     ((unsigned char)(min(B,L) - max(B,L) + 255))
#define ChannelBlend_Alpha(B,L,O)     ((unsigned char)(O * B + (1 - O) * L))
#define ChannelBlend_AlphaF(B,L,F,O)  (ChannelBlend_Alpha(F(B,L),B,O))

#define ColorBlend_Buffer(T,A,B,M)      (T)[0] = ChannelBlend_##M((A)[0], (B)[0]), \
                                        (T)[1] = ChannelBlend_##M((A)[1], (B)[1]), \
                                        (T)[2] = ChannelBlend_##M((A)[2], (B)[2])
#define ColorBlend_Normal(T,A,B)        (ColorBlend_Buffer(T,A,B,Normal))
#define ColorBlend_Lighten(T,A,B)       (ColorBlend_Buffer(T,A,B,Lighten))
#define ColorBlend_Darken(T,A,B)        (ColorBlend_Buffer(T,A,B,Darken))
#define ColorBlend_Multiply(T,A,B)      (ColorBlend_Buffer(T,A,B,Multiply))
#define ColorBlend_Average(T,A,B)       (ColorBlend_Buffer(T,A,B,Average))
#define ColorBlend_Add(T,A,B)           (ColorBlend_Buffer(T,A,B,Add))
#define ColorBlend_Subtract(T,A,B)      (ColorBlend_Buffer(T,A,B,Subtract))
#define ColorBlend_Difference(T,A,B)    (ColorBlend_Buffer(T,A,B,Difference))
#define ColorBlend_Negation(T,A,B)      (ColorBlend_Buffer(T,A,B,Negation))
#define ColorBlend_Screen(T,A,B)        (ColorBlend_Buffer(T,A,B,Screen))
#define ColorBlend_Exclusion(T,A,B)     (ColorBlend_Buffer(T,A,B,Exclusion))
#define ColorBlend_Overlay(T,A,B)       (ColorBlend_Buffer(T,A,B,Overlay))
#define ColorBlend_SoftLight(T,A,B)     (ColorBlend_Buffer(T,A,B,SoftLight))
#define ColorBlend_HardLight(T,A,B)     (ColorBlend_Buffer(T,A,B,HardLight))
#define ColorBlend_ColorDodge(T,A,B)    (ColorBlend_Buffer(T,A,B,ColorDodge))
#define ColorBlend_ColorBurn(T,A,B)     (ColorBlend_Buffer(T,A,B,ColorBurn))
#define ColorBlend_LinearDodge(T,A,B)   (ColorBlend_Buffer(T,A,B,LinearDodge))
#define ColorBlend_LinearBurn(T,A,B)    (ColorBlend_Buffer(T,A,B,LinearBurn))
#define ColorBlend_LinearLight(T,A,B)   (ColorBlend_Buffer(T,A,B,LinearLight))
#define ColorBlend_VividLight(T,A,B)    (ColorBlend_Buffer(T,A,B,VividLight))
#define ColorBlend_PinLight(T,A,B)      (ColorBlend_Buffer(T,A,B,PinLight))
#define ColorBlend_HardMix(T,A,B)       (ColorBlend_Buffer(T,A,B,HardMix))
#define ColorBlend_Reflect(T,A,B)       (ColorBlend_Buffer(T,A,B,Reflect))
#define ColorBlend_Glow(T,A,B)          (ColorBlend_Buffer(T,A,B,Glow))
#define ColorBlend_Phoenix(T,A,B)       (ColorBlend_Buffer(T,A,B,Phoenix))
#define ColorBlend_Hue(T,A,B)            ColorBlend_Hls(T,A,B,HueB,LuminationA,SaturationA)
#define ColorBlend_Saturation(T,A,B)     ColorBlend_Hls(T,A,B,HueA,LuminationA,SaturationB)
#define ColorBlend_Color(T,A,B)          ColorBlend_Hls(T,A,B,HueB,LuminationA,SaturationB)
#define ColorBlend_Luminosity(T,A,B)     ColorBlend_Hls(T,A,B,HueA,LuminationB,SaturationA)

#define ColorBlend_Hls(T,A,B,O1,O2,O3) {                                        \
    double HueA, LuminationA, SaturationA;                                     \
    double HueB, LuminationB, SaturationB;                                     \
    Color_RgbToHls((A)[0],(A)[1],(A)[2], &HueA, &LuminationA, &SaturationA);    \
    Color_RgbToHls((B)[0],(B)[1],(B)[2], &HueB, &LuminationB, &SaturationB);    \
    Color_HlsToRgb(O1,O2,O3,&(T)[0],&(T)[1],&(T)[2]);                           \
    }

long Color_HueToRgb(double Hue, double M1, double M2, double *Channel);
long Color_RgbToHls(unsigned char Red, unsigned char Green, unsigned char Blue, double *Hue, double *Lumination, double *Saturation);
long Color_HlsToRgb(double Hue, double Lumination, double Saturation, unsigned char *Red, unsigned char *Green, unsigned char *Blue);
