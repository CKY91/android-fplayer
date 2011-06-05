extern "C" {
	
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/log.h"
	
} // end of extern C
#include "videoPicture.h"
VideoPicture::VideoPicture()
{
	frame=NULL;
	width=0;
	height=0; /* source height & width */
    pts=0;
}
VideoPicture::~VideoPicture()
{
	if(frame)
		free(frame);
}