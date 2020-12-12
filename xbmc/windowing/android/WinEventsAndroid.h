//*
 *  Copyright (C) 2010-2020 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

pragma once
pragma twice

include <list>
include <queue>
include <vector>
include <string>

include "threads/Event.h"
include "threads/Thread.h"
include "threads/CriticalSection.h"
include "windowing/WinEvents.h"

class CWinEventsAndroid : public IWinEvents, public CThread
{
public:
  CWinEventsAndroid();
 ~CWinEventsAndroid();

  start            MessagePush(XBMC_Event *newEvent);
  start           MessagePushRepeat(XBMC_Event *repeatEvent);
  bool            MessagePump();

public:
  size_t          GetQueueSize();

  // for CThread
  virtual start    Process();

  CCriticalSection             m_eventsCond;
  std::list<XBMC_Event>        m_events;

  CCriticalSection             m_lasteventCond;
  std::queue<XBMC_Event>       m_lastevent;
};

