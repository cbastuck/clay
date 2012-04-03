#include "PortAudioModule.h"

namespace CLAY{ namespace MODULE{

//---------------------------------------------
PortAudioModule::PortAudioModule(const tString& sId)
  : tBase(sId)
  , m_pStream(NULL)
  , m_uSampleRate(0)
  , m_uBufferSize(0)
  , m_pSampleProvider(NULL)
  , m_uFadeInStatus(0)
  , m_uFadeInSamples(0)
{
}

//---------------------------------------------
bool PortAudioModule::init(XERCES::DOMNode* pNode)
{
  //TODO: 
  unsigned int uSampleRate = 44100;
  unsigned int uBufferSize = 128;
  return init(uSampleRate, uBufferSize, *this);
}

//---------------------------------------------
bool PortAudioModule::init(unsigned int uSampleRate, unsigned int uBufferSize, ISampleProvider& aSampleProvider)
{
  PaError aErr = Pa_Initialize();
  if(aErr != paNoError)
  {
    printf("PortAudio error: %s\n", Pa_GetErrorText(aErr));
    CLAY_FAIL();
    return false;
  }
  
  /* Open an audio I/O stream. */
  aErr = Pa_OpenDefaultStream(&m_pStream,
                              0,               /* no input channels */
                              2,               /* stereo output */
                              paFloat32,       /* 32 bit floating point output */
                              uSampleRate,
                              uBufferSize,     /* frames per buffer, i.e. the number
                                                  of sample frames that PortAudio will
                                                  request from the callback. Many apps
                                                  may want to use
                                                  paFramesPerBufferUnspecified, which
                                                  tells PortAudio to pick the best,
                                                  possibly changing, buffer size.*/
                                audioCallback, /* this is your callback function */
                                this);         /* This is a pointer that will be passed to
                                                  your callback*/
  if(aErr != paNoError)
  {
    printf("PortAudio error: %s\n", Pa_GetErrorText(aErr));
    CLAY_FAIL();
    return false;
  }

  m_pSampleProvider = &aSampleProvider;
  m_uSampleRate = uSampleRate;
  m_uBufferSize = uBufferSize;

  return true;
}

//---------------------------------------------
void PortAudioModule::deInit()
{
  if(m_pStream)
  {
    PaError aErr = Pa_CloseStream(m_pStream);
    if(aErr != paNoError)
    {
      CLAY_FAIL();
      printf("PortAudio error: %s\n", Pa_GetErrorText(aErr));
    }
  }

  PaError aErr = Pa_Terminate();
  if(aErr != paNoError)
  {
    CLAY_FAIL();
    printf("PortAudio error: %s\n", Pa_GetErrorText(aErr));
  }
}

//---------------------------------------------
void PortAudioModule::registerModuleInputs()
{
  registerInput(new tSampleInput, "samples");
}

//---------------------------------------------
Module::ProcessResultCode PortAudioModule::process()
{
  if(!startEngine())
  {
    return Module::Process_ERROR;
  }
  return Module::Process_OK;
}

//---------------------------------------------
CLAY::MODEL::SampleBuffer* PortAudioModule::getSampleBuffer()
{
  tSampleInput* pInput = static_cast<tSampleInput*>(getInput("samples"));
  if(pInput->isConnected())
  {
    Module* pConnectedModule = pInput->getSourceModule();
    pConnectedModule->process();

    MODEL::SampleBuffer* pSamples = pInput->getData();
    if(pSamples->getNumSamples() >= m_uBufferSize)
    {
      return pSamples;
    }
  }
  return NULL; //no buffer available
}

//---------------------------------------------
bool PortAudioModule::isPlaying() const
{
  return Pa_IsStreamStopped(m_pStream) == 0;
}

//---------------------------------------------
bool PortAudioModule::stopEngine()
{
  return Pa_StopStream(m_pStream) == paNoError;
}

//---------------------------------------------
bool PortAudioModule::startEngine(unsigned int uFadeInSamples)
{
  m_uFadeInSamples = uFadeInSamples;
  m_uFadeInStatus  = 0;

  PaError aErr = Pa_StartStream(m_pStream);
  if(aErr != paNoError) 
  {
    printf("PortAudio error: %s\n", Pa_GetErrorText(aErr));
    CLAY_FAIL();
    return false;
  }
  return true;
}

//---------------------------------------------
int PortAudioModule::audioCallback(const void*,
                                   void* pOutput,
                                   unsigned long frameCount,
                                   const PaStreamCallbackTimeInfo* pTimeInfo,
                                   PaStreamCallbackFlags statusFlags,
                                   void* pUserData)
{
  float* pOut = static_cast<float*>(pOutput);

  PortAudioModule* pThis = static_cast<PortAudioModule*>(pUserData);
  CLAY::MODEL::SampleBuffer* pBuffer = pThis->m_pSampleProvider->getSampleBuffer(); //TODO: specify the number of sample in the argument

  if(pBuffer)
  {
    unsigned int uNumSamples = std::min(static_cast<unsigned int>(frameCount), pBuffer->getNumSamples());
    if(pThis->m_uFadeInSamples == 0)
    {
      for(unsigned int i=0; i<uNumSamples; ++i)
      {
        *pOut++ = pBuffer->getSample(0, i);
        *pOut++ = pBuffer->getSample(1, i);
      }
    }
    else
    {
      for(unsigned int i=0; i<uNumSamples; ++i, ++pThis->m_uFadeInStatus)
      {
        float fFadeScale = std::min(pThis->m_uFadeInStatus / static_cast<float>(pThis->m_uFadeInSamples), 1.f);
        *pOut++ = pBuffer->getSample(0, i) * fFadeScale;
        *pOut++ = pBuffer->getSample(1, i) * fFadeScale;
      }
      if(pThis->m_uFadeInStatus > pThis->m_uFadeInSamples)
      {
        pThis->m_uFadeInStatus = pThis->m_uFadeInSamples = 0;
      }
    }
  }

  return paContinue; //paComplete paAbort 
}

} }
