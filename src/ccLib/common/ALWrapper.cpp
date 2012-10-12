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

void ALWrapper::SetAudioCtx(ALenum channels, ALuint rate, ALenum format)
{
    m_audChannels = channels;
    m_audRate = rate;
    m_audFormat = format;
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

    alGenSources(1, &m_audSource);
	assert(alGetError() == AL_NO_ERROR && "Could not create sources");

    alGenBuffers(AUDIO_BUFFER_NUMBER, m_audBuffers);
	assert(alGetError() == AL_NO_ERROR && "Could not create buffers");

	alSource3i(m_audSource, AL_POSITION, 0, 0, -1);
	alSourcei(m_audSource, AL_SOURCE_RELATIVE, AL_TRUE);
	alSourcei(m_audSource, AL_ROLLOFF_FACTOR, 0);
	assert(alGetError() == AL_NO_ERROR && "Could not set source parameters");

	alSourceRewind(m_audSource);
	alSourcei(m_audSource, AL_BUFFER, 0);
	assert(alGetError() == AL_NO_ERROR && "Could not rewind buffers");
}

void ALWrapper::InitAudioFrame(AudioFrame* pAudioFrame, int index)
{
    alBufferData(m_audBuffers[index],
                 m_audFormat,
                 pAudioFrame->GetFrameData(),
                 pAudioFrame->GetFrameSize(),
                 m_audRate);
    alSourceQueueBuffers(m_audSource, 1, &m_audBuffers[index]);
}

void ALWrapper::Play()
{
    alSourcePlay(m_audSource);
}

} /* namespace cc */
