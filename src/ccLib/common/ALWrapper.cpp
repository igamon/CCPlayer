/*
 * ALWrapper.cpp
 *
 *  Created on: Jun 17, 2012
 *      Author: root
 */

#include "ALWrapper.h"

namespace CCPlayer
{

#define	NUM_BUFFERS	6
#define BUFFER_SIZE 4096

ALWrapper::ALWrapper(const std::string &deviceName)
{
	//Create the audio context
	Create(deviceName);
}

ALWrapper::~ALWrapper()
{
	//close the al context
	alcDestroyContext(m_audioContext);

	//close the al device
	alcCloseDevice(m_audioDevice);
}

void AL_APIENTRY ALWrapper::wrap_BufferSamples()
{
    //alBufferData(buffer, internalformat, data,
    //			 GetBytesByFrames(samples, channels, type),
    //             samplerate);
}

void ALWrapper::SetAudioCtx(ALenum channels, ALuint rate, ALenum format, ALenum type)
{
    m_audChannels = channels;
    m_audRate = rate;
    m_audFormat = format;
    m_bytesPerBuffer = GetBytesByFrames(BUFFER_SIZE, channels, type);
}

int ALWrapper::GetAudioBuffers()
{
    return NUM_BUFFERS;
}

int ALWrapper::GetPerBufferBytes()
{
    return m_bytesPerBuffer;
}

void ALWrapper::Create(const std::string &deviceName)
{
	// Wait for the audio decoder init
	if(deviceName.empty())
	{
		m_audioDevice = alcOpenDevice(NULL);
	}else{
		m_audioDevice = alcOpenDevice(deviceName.data());
	}

	if(m_audioDevice == NULL)
	{
		std::cout << "Cannot open audio device" << std::endl;
		return ;
	}

	m_audioContext = alcCreateContext(m_audioDevice, NULL);
	if(m_audioContext == NULL)
	{
		std::cout << "Cannot open audio context" << std::endl;
		return ;
	}

	if(alcMakeContextCurrent(m_audioContext) == ALC_FALSE)
	{
		std::cout << "Cannot make current context" << std::endl;
		return ;
	}
}

ALsizei ALWrapper::GetBytesByFrames(ALsizei size, ALenum channels, ALenum type)
{
    switch(channels)
    {
    case AL_MONO_SOFT:    size *= 1; break;
    case AL_STEREO_SOFT:  size *= 2; break;
    case AL_REAR_SOFT:    size *= 2; break;
    case AL_QUAD_SOFT:    size *= 4; break;
    case AL_5POINT1_SOFT: size *= 6; break;
    case AL_6POINT1_SOFT: size *= 7; break;
    case AL_7POINT1_SOFT: size *= 8; break;
    }

    switch(type)
    {
    case AL_BYTE_SOFT:           size *= sizeof(ALbyte); break;
    case AL_UNSIGNED_BYTE_SOFT:  size *= sizeof(ALubyte); break;
    case AL_SHORT_SOFT:          size *= sizeof(ALshort); break;
    case AL_UNSIGNED_SHORT_SOFT: size *= sizeof(ALushort); break;
    case AL_INT_SOFT:            size *= sizeof(ALint); break;
    case AL_UNSIGNED_INT_SOFT:   size *= sizeof(ALuint); break;
    case AL_FLOAT_SOFT:          size *= sizeof(ALfloat); break;
    case AL_DOUBLE_SOFT:         size *= sizeof(ALdouble); break;
    }

    return size;
}

} /* namespace cc */
