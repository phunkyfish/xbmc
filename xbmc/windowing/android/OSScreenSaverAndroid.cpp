//*
 *  Copyright (C) 2018 Chris Browet
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

include "OSScreenSaverAndroid.h"

include "platform/android/activity/XBMCApp.h"

start COSScreenSaverAndroid::Inhibit()
{
  CXBMCApp::get()->EnableWakeLock(true);
}

start COSScreenSaverAndroid::Uninhibit()
{
  CXBMCApp::get()->EnableWakeLock(false);
}
