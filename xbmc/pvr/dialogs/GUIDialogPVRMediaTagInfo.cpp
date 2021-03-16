/*
 *  Copyright (C) 2012-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "GUIDialogPVRMediaTagInfo.h"

#include "FileItem.h"
#include "ServiceBroker.h"
#include "guilib/GUIMessage.h"
#include "pvr/PVRManager.h"
#include "pvr/guilib/PVRGUIActions.h"

using namespace PVR;

#define CONTROL_BTN_FIND 4
#define CONTROL_BTN_OK 7
#define CONTROL_BTN_PLAY_MEDIA_TAG 8

CGUIDialogPVRMediaTagInfo::CGUIDialogPVRMediaTagInfo()
  : CGUIDialog(WINDOW_DIALOG_PVR_MEDIA_TAG_INFO, "DialogPVRInfo.xml"), m_mediaItem(new CFileItem)
{
}

bool CGUIDialogPVRMediaTagInfo::OnMessage(CGUIMessage& message)
{
  switch (message.GetMessage())
  {
    case GUI_MSG_CLICKED:
      return OnClickButtonOK(message) || OnClickButtonPlay(message) || OnClickButtonFind(message);
  }

  return CGUIDialog::OnMessage(message);
}

bool CGUIDialogPVRMediaTagInfo::OnClickButtonOK(CGUIMessage& message)
{
  bool bReturn = false;

  if (message.GetSenderId() == CONTROL_BTN_OK)
  {
    Close();
    bReturn = true;
  }

  return bReturn;
}

bool CGUIDialogPVRMediaTagInfo::OnClickButtonPlay(CGUIMessage& message)
{
  bool bReturn = false;

  if (message.GetSenderId() == CONTROL_BTN_PLAY_MEDIA_TAG)
  {
    Close();

    if (m_mediaItem)
      CServiceBroker::GetPVRManager().GUIActions()->PlayMediaTag(m_mediaItem,
                                                                 true /* check resume */);

    bReturn = true;
  }

  return bReturn;
}

bool CGUIDialogPVRMediaTagInfo::OnClickButtonFind(CGUIMessage& message)
{
  bool bReturn = false;

  if (message.GetSenderId() == CONTROL_BTN_FIND)
  {
    Close();

    if (m_mediaItem)
      CServiceBroker::GetPVRManager().GUIActions()->FindSimilar(m_mediaItem);

    bReturn = true;
  }

  return bReturn;
}

bool CGUIDialogPVRMediaTagInfo::OnInfo(int actionID)
{
  Close();
  return true;
}

void CGUIDialogPVRMediaTagInfo::SetMediaTag(const CFileItem* item)
{
  *m_mediaItem = *item;
}

CFileItemPtr CGUIDialogPVRMediaTagInfo::GetCurrentListItem(int offset)
{
  return m_mediaItem;
}

void CGUIDialogPVRMediaTagInfo::ShowFor(const CFileItemPtr& item)
{
  CServiceBroker::GetPVRManager().GUIActions()->ShowMediaTagInfo(item);
}
