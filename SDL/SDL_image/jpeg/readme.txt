
JpegFile (+ BMP + GIF)



I've been looking for a long time for a JPEG package that will allow me to do
this :

     BYTE * RGBbuffer = ReadJPEGFile(filename,&width,&height);

and this:

     SaveJPG(RGBbuf, width, height...);

I never found one that didn't cost many hundreds of dollars, so I wrote my own. 



                                               What is it?

My class is called JpegFile.cpp. It is based on a Win32 compilation of the IJG
V.6a JPEG source. It does two main things : it reads a JPEG file into an RGB
buffer, and it saves an RGB buffer as an RGB or grayscale JPEG file. There are
some utility routines in it to do things like RGB -> BGR, vertical flip,
DWORD-align, etc., to allow easy conversion to DIB/BMP formats. It also has
code to read 1, 4, 8 and 24 bit BMP files and GIF 87a and 89a files, plus
code to write 1, 4, 8 and 24-bit BMPs and 256 color GIFs.

There are no provisions for progressive reading, progress bars, partial
reads/saves, extended error handling, etc.. I tried to keep the IJG source as
intact as possible, to ease debugging - I assume the IJG code works.perfectly
and that anything going wrong is my fault. I don't know if this is true or not.

But, my code has worked in every application I've tried it in. This has only
run (to my knowledge) on Win 95, VC++ 4.1. 

I've decided to make this class, as well as my compilation of the IJG library
available to the public. This is entirely free - no royalties, no time
bombs, no annoyance screens. All I ask is that you mention me in your software,
especially if it's commercial software - and, if possible, add a
link to my page . I just want some acknowledgment... :)

Because it is free, however, there are no guarantees - use at your own risk!
If you want software that comes with someone you can point your finger at when
it fails, spend the money on a commercial package from a company with a
legal department. I'm just sharing.



                The Package

    JPEGLIB2
                This is the VC++ 4.0 project (JPEGLIB2.MDP) for my compilation
                of the IJG source for Win95. Most of the IJG source is included
                here - everything but the example programs. This is what does
                the actual work. It is absolutely necessary that this library
                is compiled and linked to the application using the JpegFile
                class. Fortunately, it should be a matter of unZipping the
                project into it own directory and aiming VC++ 4.x at it. 

                It is possible to use this library (or the source for the
                library) on its own, if you know the IJG codebase. I really
                don't. I know that it works for my JpegFile class, though.

                I have changed only two lines of the original source. I
                commented out the exit(..) in jerror.c and replaced the
                fprintf(stderr,..) with a MessageBox(...). That's right, it
                launches a message box on warnings. It's certainly possible
                to remove this line if you absolutely can not have message
                boxes popping up. It is also possible for you to write your own
                error handler.

                The biggest benefit of using my JPEG package is that I've
                managed to get the IJG codebase into a VC++ 4.x (Win32)
                project. The other part of my package is really a thin wrapper.

    MFCAPP
                This is a small sample application using a class called
                JpegFile. JpegFile is a wrapper around the JPEGLIB2 library. If
                you know what you're doing, you can certainly ignore my class.
                If you want simple functionality for reading and writing JPEG
                files, JpegFile does it very well. If nothing else, this class
                is a great starting place for those who wish to
                figure out more of the IJG codebase. I use it for all of my
                JPEG needs. 

                The application provides the following functions :

                    Read a BMP (1, 4, 8 or 24-bit) or GIF or JPG file,
                    display the result. 
                    Save a BMP (1, 4, 8 or 24 bit) or GIF (256 color) or JPG
                    (RGB or grayscale) file. 
                    Examine the dimensions of a JPG or GIF file. 

                It should be very simple to compile and run this app, as well.
                If you want to use the JpegFile class in another app, you need
                to :

                     Add JpegFile.cpp to your project. 
                     #include "JpegFile.h" 
                     Link with JPEGLIB2.LIB. 
                     Tell the linker using Build/Settings/Link/Input to ignore
                        the LIBC and/or LIBCD files. There are some conflicts
                        with this library. I've never been able to figure out
                        what they are. But, I've never found ignoring these
                        libraries to be a problem. If someone knows what's up
                        here, tell me. 

                The BMP code is fairly simple to extract to different apps. I
                do it all the time. 

                The GIF code is completely independant of the MFCAPP app. It
                should be no problem to use this code in other apps. It is C++
                now, but is really C that was forced into C++ for the sake of
                this app - it should be no problem to turn it into plain
                Windows C. Likewise, the BMP code should be easy to move from
                this app to any other.


          Disclaimer Disclaimer Disclaimer Disclaimer Disclaimer 

I make no guarantees about its suitablilty for use. I find it very useful for
my applications, but cannot anticipate all places this code could wind up. If
you have problems with this software, you can ask me about it. If it's anything
to do with anything that happens beneath JpegFile.cpp, I won't be able to help,
probably. But, since I have left the IJG source essentially intact (but for two
lines in jerror.c) anyone familiar with the IJG code will be better at helping
you that I am.

