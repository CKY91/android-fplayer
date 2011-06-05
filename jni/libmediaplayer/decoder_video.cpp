#include <android/log.h>
#include "decoder_video.h"

#define TAG "FFMpegVideoDecoder"

static uint64_t global_video_pkt_pts = AV_NOPTS_VALUE;

DecoderVideo::DecoderVideo(AVStream* stream) : IDecoder(stream)
{
	 mStream->codec->get_buffer = getBuffer;
	 mStream->codec->release_buffer = releaseBuffer;
}

DecoderVideo::~DecoderVideo()
{
	if(mFrame)
		av_free(mFrame);
}

bool DecoderVideo::prepare()
{
	mFrame = avcodec_alloc_frame();
	if (mFrame == NULL) {
		return false;
	}
	return true;
}

double DecoderVideo::synchronize(AVFrame *src_frame, double pts) {

	double frame_delay;

	if (pts != 0) {
		/* if we have pts, set video clock to it */
		mVideoClock = pts;
	} else {
		/* if we aren't given a pts, set it to the clock */
		pts = mVideoClock;
	}
	/* update the video clock */
	frame_delay = av_q2d(mStream->codec->time_base);
	/* if we are repeating a frame, adjust clock accordingly */
	frame_delay += src_frame->repeat_pict * (frame_delay * 0.001);
	//frame_delay += src_frame->repeat_pict * (frame_delay );
	mVideoClock += frame_delay;
	return pts;
}

bool DecoderVideo::process(AVPacket *packet)
{
    int	completed;
    //int pts = 0;
    double pts;
    int returnValue=0;
    
    global_video_pkt_pts = packet->pts;
	// Decode video frame
	returnValue=avcodec_decode_video(mStream->codec,
						 mFrame,
						 &completed,
						 packet->data, 
						 packet->size);

	if(returnValue<0)
	{
		 __android_log_print(ANDROID_LOG_ERROR, TAG, "process avcodec_decode_video error ");
	}
	
	if (packet->dts == AV_NOPTS_VALUE && mFrame->opaque&& *(uint64_t*) mFrame->opaque != AV_NOPTS_VALUE)
	{
		pts = *(uint64_t *) mFrame->opaque;
	} else if (packet->dts != AV_NOPTS_VALUE)
	{
		pts = packet->dts;
	} else
	{
		pts = 0;
	}
	pts *= av_q2d(mStream->time_base);
	
	 __android_log_print(ANDROID_LOG_ERROR, TAG, "pts :%f",pts);
	if (completed) 
	{
		
		pts = synchronize(mFrame, pts);

		onDecode(mFrame, pts);

		return true;
	}
	// __android_log_print(ANDROID_LOG_ERROR, TAG, "process return false");
	return false;
}

bool DecoderVideo::decode(void* ptr)
{
	AVPacket        pPacket;
	

	
    while(mRunning)
    {
        if(mQueue->get(&pPacket, true) < 0)
        {
        
            mRunning = false;
            return false;
        }
        /**
        if(!process(&pPacket))
        {
            __android_log_print(ANDROID_LOG_ERROR, TAG, "process(&pPacket)");
            mRunning = false;
            return false;
        }
        **/
        process(&pPacket);
      
        // Free the packet that was allocated by av_read_frame
        av_free_packet(&pPacket);
    }
	
    //__android_log_print(ANDROID_LOG_INFO, TAG, "decoding video ended");
	
    // Free the RGB image
    av_free(mFrame);

    return true;
}

/* These are called whenever we allocate a frame
 * buffer. We use this to store the global_pts in
 * a frame at the time it is allocated.
 */
int DecoderVideo::getBuffer(struct AVCodecContext *c, AVFrame *pic) {
	int ret = avcodec_default_get_buffer(c, pic);
	uint64_t *pts = (uint64_t *)av_malloc(sizeof(uint64_t));
	*pts = global_video_pkt_pts;
	pic->opaque = pts;
	return ret;
}
void DecoderVideo::releaseBuffer(struct AVCodecContext *c, AVFrame *pic) {
	if (pic)
		av_freep(&pic->opaque);
	avcodec_default_release_buffer(c, pic);
}
