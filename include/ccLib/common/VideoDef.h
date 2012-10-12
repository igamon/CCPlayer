#ifndef VIDEODEF_H
#define VIDEODEF_H

namespace CCPlayer
{

#define VIDEO_OUTPUT_WIDTH  1024
#define VIDEO_OUTPUT_HEIGHT 1024

class VideoFrame
{
public:
	VideoFrame(const char* pFrameData,
               int frameSize,
               double showTime)
	{
	    m_frameSize = frameSize;

	    assert(m_frameSize != 0);

	    m_pFrameData = new char[m_frameSize];
	    memset(m_pFrameData, 0, m_frameSize);
	    memcpy(m_pFrameData, pFrameData, m_frameSize);

	    m_showTime = showTime;
	}

	VideoFrame(const VideoFrame& videoFrame)
	{
	    m_frameSize = videoFrame.m_frameSize;

	    assert(m_frameSize != 0);

	    m_pFrameData = new char[m_frameSize];
	    memset(m_pFrameData, 0, m_frameSize);
	    memcpy(m_pFrameData, videoFrame.m_pFrameData, m_frameSize);

	    m_showTime = videoFrame.m_showTime;
	}
    const VideoFrame& operator=(const VideoFrame &videoFrame)
	{
        if(m_pFrameData != NULL)
        {
            delete m_pFrameData;
            m_pFrameData = NULL;
        }

	    m_frameSize = videoFrame.m_frameSize;

	    assert(m_frameSize != 0);

	    m_pFrameData = new char[m_frameSize];
	    memset(m_pFrameData, 0, m_frameSize);
	    memcpy(m_pFrameData, videoFrame.m_pFrameData, m_frameSize);

	    m_showTime = videoFrame.m_showTime;

	    return *this;
	}

	virtual ~VideoFrame()
	{
	    if(m_pFrameData != NULL)
        {
            delete m_pFrameData;
            m_pFrameData = NULL;
        }
	}

private:
	char *m_pFrameData;
	int m_frameSize;

private:
	double m_showTime;
};

}

#endif