
#ifndef _SCIMG_H_
#define _SCIMG_H_

typedef enum {
    FMT_RGB,
    FMT_YUV,
}IMG_FMT;

typedef enum{
    ORI_UPP_LEFT,
    ORI_BOT_LEFT,
    ORI_UPP_RIGHT,
    ORI_BOT_RIGHT,
}ORIGIN_T;

typedef struct {
    unsigned w,h;
    unsigned fmt;
    unsigned long size;
    BYTE* data;
    int depth;
    
    ORIGIN_T origin;
}scimg;

#endif
