extern "C" {
	
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/log.h"
	
} // end of extern C
class  VideoPicture
{
public:
 	  VideoPicture();
      ~VideoPicture();
	  AVFrame *frame;
	  int width, height; /* source height & width */
	  double pts;
};