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

#ifndef MidiTypes_H_
#define MidiTypes_H_

#include <clay-core/base/ModuleIO.h>

#include <vector>

namespace CLAY{ namespace MODEL{

//---------------------------------------------MidiMessage
class MidiMessage
{
public:
  MidiMessage();
  MidiMessage(unsigned int uMessage);
  MidiMessage(unsigned int uMessage, unsigned int uTimestamp);

  enum CommandType
  {
    INVALID_TYPE = 0,
    NOTE_ON = 1,
    NOTE_OFF = 2,
    POLYPHONIC_AFTERTOUCH = 3,
    CONTROL_CHANGE = 4,
    PROGRAM_CHANGE = 5,
    MONOPHONIC_AFTERTOUCH = 6,
    PITCH_BEND = 7,
    SYSEX = 8
  };

  enum MidiChannel
  {
    MIDI_CHANNEL_0 = 0,
    MIDI_CHANNEL_1,
    MIDI_CHANNEL_2,
    MIDI_CHANNEL_3,
    MIDI_CHANNEL_4,
    MIDI_CHANNEL_5,
    MIDI_CHANNEL_6,
    MIDI_CHANNEL_7,
    MIDI_CHANNEL_8,
    MIDI_CHANNEL_9,
    MIDI_CHANNEL_10,
    MIDI_CHANNEL_11,
    MIDI_CHANNEL_12,
    MIDI_CHANNEL_13,
    MIDI_CHANNEL_14,
    MIDI_CHANNEL_15,
    MIDI_CHANNEL_16
  };

  enum MidiNote
  {
    MIDI_NOTE_C   = 0,
    MIDI_NOTE_Cis = 1,
    MIDI_NOTE_D   = 2,
    MIDI_NOTE_Dis = 3,
    MIDI_NOTE_E   = 4,
    MIDI_NOTE_F   = 5,
    MIDI_NOTE_Fis = 6,
    MIDI_NOTE_G   = 7,
    MIDI_NOTE_Gis = 8,
    MIDI_NOTE_A   = 9,
    MIDI_NOTE_Ais = 10,
    MIDI_NOTE_H   = 11,
    MIDI_NUM_NOTES
  };
  unsigned char getCommand()  const;
  CommandType getCommandType() const;
  MidiChannel getMidiChannel() const;
  unsigned int getSysexLength() const;
  unsigned char getNote()     const;
  MidiNote getKey() const;
  unsigned int getOctave() const;
  unsigned char getVelocity() const;
  char getRaw(unsigned int uIdx) const;
  unsigned int getTimestamp() const;

  void setCommand(unsigned char aCmd);
  void setCommandType(CommandType eType);
  void setMidiChannel(MidiChannel eMidiChannel);
  void setNote(unsigned char aNote);
  void setNote(MidiNote eNote, int iOctave);
  void setNote(MidiNote eNote);
  void setOctave(unsigned int uOctave);
  void setVelocity(unsigned char aVelocity);

  unsigned long asUnsigned() const;

private:
  typedef union { unsigned long word; unsigned char data[4]; } tMidiMessage;
  tMidiMessage m_aMessage;
  unsigned int m_uTimestampInMsec;
};

//---------------------------------------------operator<<
inline std::ostream& operator<<(std::ostream& os, const MidiMessage& aMsg)
{
  os << " Command: " << (int)aMsg.getCommand()
     << " Type: " << (int)aMsg.getCommandType()
     << " Midi-Channel: " << (int)aMsg.getMidiChannel()
     << " Note: " << (int)aMsg.getNote()
     << " Note-Key: " << (int)aMsg.getKey()
     << " Octave: " << (int)aMsg.getOctave()
     << " Velocity: " << (int)aMsg.getVelocity();
  return os;
}

//---------------------------------------------MidiOutput
class MidiOutput : public ModuleOutput<MidiMessage>
{
public:

private:
};

//---------------------------------------------MidiInput
class MidiInput : public ModuleInput<MidiMessage>
{
public:

private:
};


//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
class MidiBuffer
{
private:
  typedef std::vector<MidiMessage> tMidiEvents;
public:
  typedef tMidiEvents::const_iterator tIterator;

  MidiBuffer();

  void reserve(unsigned int uNumReservedEvents);
  bool addMidiMessage(const MidiMessage& aMidiMessage);
  void clearBuffer();

  tIterator begin() const;
  tIterator end() const;

private:
  tMidiEvents m_collBufferedEvents;
  unsigned int m_uReadIndex;
};

//---------------------------------------------MidiBufferOutput
class MidiBufferOutput : public ModuleOutput<MidiBuffer>
{
public:
  MidiBufferOutput()
  {
  }
private:
};

//---------------------------------------------MidiBufferInput
class MidiBufferInput : public ModuleInput<MidiBuffer>
{
public:

private:
};

} }

#endif 
