#ifndef ALWRAPPER_H
#define ALWRAPPER_H

#include "Common.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

namespace CCPlayer
{

class ALWrapper {
public:
	ALWrapper(const std::string &deviceName = "");
	virtual ~ALWrapper();

public:
    void SetAudioCtx(ALenum channels, ALuint rate, ALenum format, ALenum type);
    int GetAudioBuffers();
    int GetPerBufferBytes();

private:
	void AL_APIENTRY wrap_BufferSamples();

private:
	void Create(const std::string &deviceName);
	ALsizei GetBytesByFrames(ALsizei size, ALenum channels, ALenum type);

private:
    ALsizei m_bytesPerBuffer;
	ALenum m_audChannels;
	ALenum m_audFormat;
	ALuint m_audRate;

private:
	ALCdevice *m_audioDevice;
	ALCcontext *m_audioContext;
};

} /* namespace cc */
#endif /* ALWRAPPER_HPP_ */
