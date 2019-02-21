
#pragma comment ( lib, "avcodec.lib" )
#pragma comment ( lib, "avdevice.lib")
#pragma comment ( lib, "avfilter.lib")
#pragma comment ( lib, "avformat.lib")
#pragma comment ( lib, "avutil.lib")
#pragma comment ( lib, "postproc.lib")
#pragma comment ( lib, "swresample.lib")
#pragma comment ( lib, "swscale.lib")

#pragma comment ( lib, "SDL2.lib")
#pragma comment ( lib, "SDL2main.lib")
#pragma comment ( lib, "SDL2test.lib")
extern "C" //FFmpeg가 C라이브러리이기 때문에 이부분이 필요하다.

{
#include"libavcodec/avcodec.h"
#include "libavcodec/avcodec.h"
#include "libavdevice/avdevice.h"
#include "libavfilter/avfilter.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libpostproc/postprocess.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
#include "libavutil/mathematics.h"

#include "libavutil/opt.h"
#include "libavutil/imgutils.h"


#include "libavutil/avassert.h"
#include "libavutil/channel_layout.h"


#include "libavutil/timestamp.h"



}

#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



#define STREAM_DURATION   10.0
#define STREAM_FRAME_RATE 25 /* 25 images/s */
#define STREAM_PIX_FMT    AV_PIX_FMT_YUV420P /* default pix_fmt */
#define SCALE_FLAGS SWS_BICUBIC
// a wrapper around a single output AVStream
int main() {
	return 0;
}