/**************************************************************************\
*
*  This file is part of the C++ Modular Development Framework.
*  Copyright (C) 2009 by Christoph Bastuck.  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU General Public License
*  ("GPL") version 2 as published by the Free Software Foundation.
*  See the file LICENSE.GPL at the root directory of this source
*  distribution for additional information about the GNU GPL.
*
*  For using this framework with software that can not be combined with the GNU
*  GPL please contact mail@cbastuck about acquiring a commercial license.
*
*  See http://www.cbastuck.de/ for more information.
*
\**************************************************************************/

#include "MP3ReaderModule.h"

#include <fstream>

extern "C"
{
  #include <mad.h>
}

namespace CLAY{ namespace MODULE{

/*
 * This is a private message structure. A generic pointer to this structure
 * is passed to each of the callback functions. Put here any data you need
 * to access from within the callbacks.
 */
struct buffer 
{
  unsigned char const* start;
  unsigned long length;
  unsigned long bytesWritten;
  MODEL::SampleBuffer* target;
  unsigned long samplesWritten;
};


/*
 * This is the input callback. The purpose of this callback is to (re)fill
 * the stream buffer which is to be decoded. In this example, an entire file
 * has been mapped into memory, so we just call mad_stream_buffer() with the
 * address and length of the mapping. When this callback is called a second
 * time, we are finished decoding.
 */
//---------------------------------------------input
static enum mad_flow input(void *data, struct mad_stream *stream)
{
  struct buffer *buffer = (struct buffer*)data;

  if (buffer->bytesWritten == buffer->length)
  {
    return MAD_FLOW_STOP;
  }

  mad_stream_buffer(stream, buffer->start, buffer->length);

  buffer->bytesWritten = buffer->length;

  return MAD_FLOW_CONTINUE;
}

/*
 * The following utility routine performs simple rounding, clipping, and
 * scaling of MAD's high-resolution samples down to 16 bits. It does not
 * perform any dithering or noise shaping, which would be recommended to
 * obtain any exceptional audio quality. It is therefore not recommended to
 * use this routine if high-quality output is desired.
 */
//---------------------------------------------scale
static inline signed int scale(mad_fixed_t sample)
{
  /* round */
  sample += (1L << (MAD_F_FRACBITS - 16));

  /* clip */
  if (sample >= MAD_F_ONE)
    sample = MAD_F_ONE - 1;
  else if (sample < -MAD_F_ONE)
    sample = -MAD_F_ONE;

  /* quantize */
  return sample >> (MAD_F_FRACBITS + 1 - 16);
}


/*
 * This is the output callback function. It is called after each frame of
 * MPEG audio data has been completely decoded. The purpose of this callback
 * is to output (or play) the decoded PCM audio.
 */
//---------------------------------------------output
static enum mad_flow output(void *data,
		                        struct mad_header const *header,
		                        struct mad_pcm *pcm)
{
//std::ofstream os("C:\\Temp\\os.raw", std::ios::binary | std::ios::out | std::ios::app);

  buffer* pData = (buffer*)data;
  MODEL::SampleBuffer* pTarget = pData->target;

  unsigned int nchannels, nsamples;
  mad_fixed_t const *left_ch, *right_ch;

  /* pcm->samplerate contains the sampling frequency */

  nchannels = pcm->channels;
  nsamples  = pcm->length;
  left_ch   = pcm->samples[0];
  right_ch  = pcm->samples[1];
 
  pTarget->resize(pData->samplesWritten + nsamples);

  for(unsigned int i=0; nsamples--; ++i) 
  {
    /* output sample(s) in 16-bit signed little-endian PCM */
    signed int sample = scale(*left_ch++);
    //os.put((sample >> 0) & 0xff);
    //os.put((sample >> 8) & 0xff);
    pTarget->setSample(0, pData->samplesWritten+i, (float)sample / 32768.f);

    if (nchannels == 2) 
    {
      sample = scale(*right_ch++);
      //os.put((sample >> 0) & 0xff);
      //os.put((sample >> 8) & 0xff);
      pTarget->setSample(1, pData->samplesWritten+i, (float)sample / 32768.f);
    }
  }

  pData->samplesWritten += pcm->length; //i.e. nsamples

  return MAD_FLOW_CONTINUE;
}

/*
 * This is the error callback function. It is called whenever a decoding
 * error occurs. The error is indicated by stream->error; the list of
 * possible MAD_ERROR_* errors can be found in the mad.h (or stream.h)
 * header file.
 */
//---------------------------------------------error
static enum mad_flow error(void *data, struct mad_stream *stream, struct mad_frame *frame)
{
  struct buffer *buffer = (struct buffer*)data;

  fprintf(stderr, "decoding error 0x%04x (%s) at byte offset %u\n",
	  stream->error, mad_stream_errorstr(stream),
	  stream->this_frame - buffer->start);

  /* return MAD_FLOW_BREAK here to stop decoding (and propagate an error) */

  return MAD_FLOW_CONTINUE;
}

//---------------------------------------------header
static enum mad_flow header(void *data, struct mad_header const * header)
{
  buffer* pData = (buffer*)data;
  MODEL::SampleBuffer* pTarget = pData->target;

  if(!pTarget->getNumSamples()) //do this only once
  {
    switch(header->mode)
    {
    case MAD_MODE_SINGLE_CHANNEL:
      pTarget->setLayout(MODEL::SampleBuffer::eMonoLayout);
      break;

    case MAD_MODE_JOINT_STEREO:
      pTarget->setLayout(MODEL::SampleBuffer::eStereoLayout);
      break;

    case MAD_MODE_DUAL_CHANNEL:
      pTarget->setLayout(MODEL::SampleBuffer::eStereoLayout);
      break;

    case MAD_MODE_STEREO:
      pTarget->setLayout(MODEL::SampleBuffer::eStereoLayout);
      break;
    }
    pTarget->setSampleRate(header->samplerate);

    //try to estimate the decoded buffer size to prevent unnecessary allocations
    float fEstimatedDuration = (float)pData->length / (float)(header->bitrate / 8);
    unsigned int uEstimatedBytes = (unsigned int)ceil(header->samplerate * fEstimatedDuration * sizeof(MODEL::SampleBuffer::tSample));
    pTarget->reserve(uEstimatedBytes);
  }

  return MAD_FLOW_CONTINUE;
}

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------MP3ReaderModule
MP3ReaderModule::MP3ReaderModule(const tString& sRuntimeID)
  : tBase(sRuntimeID)
{
}

//---------------------------------------------registerModuleOutputs
void MP3ReaderModule::registerModuleOutputs()
{
  registerOutput(new tOutputType(), "sample-out");
}

//---------------------------------------------reset
void MP3ReaderModule::reset()
{
  tOutputType* pOutput = static_cast<tOutputType*>(getOutput("sample-out"));
  pOutput->getData().clear();
}

//---------------------------------------------loadMP3
bool MP3ReaderModule::loadMP3(const tString& sFn)
{
  tOutputType* pOutput = static_cast<tOutputType*>(getOutput("sample-out"));
  if(pOutput)
  {
    return loadMP3(sFn, pOutput->getData());
  }
  return false;
}

//---------------------------------------------loadMP3
bool MP3ReaderModule::loadMP3(const tString& sFn, MODEL::SampleBuffer& aDst)
{
  std::ifstream is(sFn.c_str(), std::ios::binary | std::ios::in);
  if(is)
  {
    is.seekg(0, std::ios::end);
    unsigned int uNumBytes = (unsigned int) is.tellg();
    is.seekg(std::ios::beg);

    unsigned char* pBuffer = new unsigned char[uNumBytes];
    is.read(reinterpret_cast<char*>(pBuffer), uNumBytes);

    bool bSuccess = decode(pBuffer, uNumBytes, aDst);
    //aDst.writeRaw(sFn + ".raw"); //TESTING
    delete[] pBuffer;
    return bSuccess;
  }

  return false;
}

/*
 * This is the function to perform all the decoding.
 * It instantiates a decoder object and configures it with the input,
 * output, and error callback functions above. A single call to
 * mad_decoder_run() continues until a callback function returns
 * MAD_FLOW_STOP (to stop decoding) or MAD_FLOW_BREAK (to stop decoding and
 * signal an error).
 */
//---------------------------------------------decode
bool MP3ReaderModule::decode(unsigned char const* start, unsigned long length, MODEL::SampleBuffer& aDst)
{
  struct buffer buffer;
  struct mad_decoder decoder;

  /* initialize our private message structure */

  buffer.start  = start;
  buffer.length = length;
  buffer.target = &aDst;
  buffer.samplesWritten = 0;
  buffer.bytesWritten = 0;

  /* configure input, output, and error functions */

  mad_decoder_init(&decoder, &buffer,
		               input, header, 0 /* filter */, output,
		               error, 0 /* message */);

  /* start decoding */

  int result = mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);

  /* release the decoder */

  mad_decoder_finish(&decoder);

  return result == 0;
}

} }
