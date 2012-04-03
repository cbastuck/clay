#pragma once

#include <clay-core/base/ModuleHelper.h>
#include <clay-data-types/audio/SampleBuffer.h>
#include <clay-modules/audio/asio/ISampleProvider.h>

#include <portaudio.h>

namespace CLAY{ namespace MODULE{

class PortAudioModule : public ModuleHelper<PortAudioModule,
                                            HELPER::IntegerEncoding<'P','O','R','T'>::value,
                                            HELPER::IntegerEncoding<'A','U','I','O'>::value >,
                        public ISampleProvider
{
private:
  typedef ModuleHelper<PortAudioModule,
                       HELPER::IntegerEncoding<'P','O','R','T'>::value,
                       HELPER::IntegerEncoding<'A','U','I','O'>::value > tBase;

  typedef MODEL::SampleBufferInput   tSampleInput;

public:
  PortAudioModule(const tString& sId);

  virtual bool init(XERCES::DOMNode* pNode);
  bool init(unsigned int uSampleRate, unsigned int uBufferSize, ISampleProvider& aSampleProvider);

  void registerModuleInputs();

  virtual void deInit();
  virtual ProcessResultCode process();

  virtual CLAY::MODEL::SampleBuffer* getSampleBuffer();

  bool isPlaying() const;
  bool stopEngine();
  bool startEngine(unsigned int uFadeInSamples=0);

  static int audioCallback(const void* input,
                           void* output,
                           unsigned long frameCount,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void* userData);

private:
  PaStream*             m_pStream;
  unsigned int          m_uSampleRate;
  unsigned int          m_uBufferSize;
  ISampleProvider*      m_pSampleProvider;
  volatile unsigned int m_uFadeInStatus;
  volatile unsigned int m_uFadeInSamples;
};

} }
