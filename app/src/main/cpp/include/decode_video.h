#ifndef __DECODE_VIDEO_H__
#define __DECODE_VIDEO_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libavcodec/avcodec.h>

void pgm_save(unsigned char *buf, int wrap, int xsize, int ysize, char *filename);
int decode_write_frame(const char *outfilename, AVCodecContext *avctx, AVFrame *frame, int *frame_count, AVPacket *pkt, int last);
int decode(const char *filename,const char *outfilename);


#endif