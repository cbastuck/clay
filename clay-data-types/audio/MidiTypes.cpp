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

#include "MidiTypes.h"

namespace CLAY{ namespace MODEL{

//---------------------------------------------MidiMessage
MidiMessage::MidiMessage()
{
  memset(&m_aMessage, 0, sizeof(m_aMessage));
  m_uTimestampInMsec = 0;
}

//---------------------------------------------MidiMessage
MidiMessage::MidiMessage(unsigned int uMessage)
{
  memcpy(&m_aMessage, &uMessage, sizeof(m_aMessage));
  m_uTimestampInMsec = 0;
}

//---------------------------------------------MidiMessage
MidiMessage::MidiMessage(unsigned int uMessage, unsigned int uTimestampInMsec)
{
  memcpy(&m_aMessage, &uMessage, sizeof(m_aMessage));
  m_uTimestampInMsec = uTimestampInMsec;
}

//---------------------------------------------getCommand
unsigned char MidiMessage::getCommand() const
{
  return m_aMessage.data[0];
}

//---------------------------------------------getCommandType
MidiMessage::CommandType MidiMessage::getCommandType() const
{
  unsigned char c = 0xF0 & m_aMessage.data[0];
  if(c == 0x80)
    return NOTE_OFF;
  else if(c == 0x90)
    return NOTE_ON;
  else if(c == 0xA0)
    return POLYPHONIC_AFTERTOUCH;
  else if(c == 0xB0)
    return CONTROL_CHANGE;
  else if(c == 0xC0)
    return PROGRAM_CHANGE;
  else if(c == 0xD0)
    return MONOPHONIC_AFTERTOUCH;
  else if(c == 0xE0)
    return PITCH_BEND;
  else if(c == 0xF0)
    return SYSEX;
  return INVALID_TYPE;
}
    
//---------------------------------------------getMidiChannel
MidiMessage::MidiChannel MidiMessage::getMidiChannel() const
{
  unsigned char c = 0x0F & m_aMessage.data[0];
  return (MidiChannel)c;
}

//---------------------------------------------getSysexLength
unsigned int MidiMessage::getSysexLength() const
{
  return m_aMessage.data[1];
}

//---------------------------------------------getNote
unsigned char MidiMessage::getNote() const
{
  return m_aMessage.data[1];
}

//---------------------------------------------getKey
MidiMessage::MidiNote MidiMessage::getKey() const
{
  return (MidiNote)(getNote() % MIDI_NUM_NOTES);
}

//---------------------------------------------getOctave
unsigned int MidiMessage::getOctave() const
{
  return getNote() / MIDI_NUM_NOTES;
}

//---------------------------------------------getVelocity
unsigned char MidiMessage::getVelocity() const
{
  return m_aMessage.data[2];
}

//---------------------------------------------getRaw
char MidiMessage::getRaw(unsigned int uIdx) const
{
  return m_aMessage.data[uIdx];
}

//---------------------------------------------getTimestamp
unsigned int MidiMessage::getTimestamp() const
{
  return m_uTimestampInMsec;
}

//---------------------------------------------setCommand
void MidiMessage::setCommand(unsigned char aCmd)
{
  m_aMessage.data[0] = aCmd;
}

//---------------------------------------------setCommandType
void MidiMessage::setCommandType(MidiMessage::CommandType eType)
{
  unsigned char& c = m_aMessage.data[0];
  c &= 0x0F; //clear the current type
  if(eType == NOTE_OFF)
    c |= 0x80;
  else if(eType == NOTE_ON)
    c |= 0x90;
  else if(eType == POLYPHONIC_AFTERTOUCH)
    c |= 0xA0;
  else if(eType == CONTROL_CHANGE)
    c |= 0xB0;
  else if(eType == PROGRAM_CHANGE)
    c |= 0xC0;
  else if(eType == MONOPHONIC_AFTERTOUCH)
    c |= 0xD0;
  else if(eType == PITCH_BEND)
    c |= 0xE0;
  else if(eType == SYSEX)
    c |= 0xF0;
}

//---------------------------------------------setMidiChannel
void MidiMessage::setMidiChannel(MidiChannel eMidiChannel)
{
  unsigned char& c = m_aMessage.data[0];
  c &= 0xF0; //clear the existing channel
  c |= eMidiChannel;
  //int i = eMidiChannel;
  //i = i << 4;
  //unsigned char n = i & 0x0F;
  //c |= n;
}

//---------------------------------------------setNote
void MidiMessage::setNote(unsigned char aNote)
{
  m_aMessage.data[1] = aNote;
}

//---------------------------------------------setNote
void MidiMessage::setNote(MidiNote eNote, int iOctave)
{
  setNote(iOctave * MIDI_NUM_NOTES + eNote);
}

//---------------------------------------------setNote
void MidiMessage::setNote(MidiNote eNote)
{
  unsigned int uCurrentOctave = getNote() / MIDI_NUM_NOTES;
  setNote(eNote, uCurrentOctave);
}

//---------------------------------------------setOctave
void MidiMessage::setOctave(unsigned int uOctave)
{
  unsigned int uCurrentKey = getNote() % MIDI_NUM_NOTES;
  setNote((MidiNote)uCurrentKey, uOctave);
}

//---------------------------------------------setVelocity
void MidiMessage::setVelocity(unsigned char aVelocity)
{
  m_aMessage.data[2] = aVelocity;
}

//---------------------------------------------asUnsigned
unsigned long MidiMessage::asUnsigned() const
{
  return m_aMessage.word;
}

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------MidiBuffer
MidiBuffer::MidiBuffer()
  : m_uReadIndex(0)
{
}

//---------------------------------------------reserve
void MidiBuffer::reserve(unsigned int uNumReservedEvents)
{
  m_collBufferedEvents.reserve(uNumReservedEvents);
}

//---------------------------------------------addMidiMessage
bool MidiBuffer::addMidiMessage(const MidiMessage& aMidiMessage)
{
  if(m_collBufferedEvents.size() < m_collBufferedEvents.capacity())
  {
    m_collBufferedEvents.push_back(aMidiMessage);
    return true;
  }
  return false; //the buffer is full 
}

//---------------------------------------------clearBuffer
void MidiBuffer::clearBuffer()
{
  m_collBufferedEvents.clear();
}

//---------------------------------------------begin
MidiBuffer::tIterator MidiBuffer::begin() const
{
  return m_collBufferedEvents.begin() + m_uReadIndex;
}

//---------------------------------------------end
MidiBuffer::tIterator MidiBuffer::end() const
{
  return m_collBufferedEvents.end();
}

} }
