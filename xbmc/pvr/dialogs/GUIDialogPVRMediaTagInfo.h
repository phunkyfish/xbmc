/*
 *  Copyright (C) 2012-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "guilib/GUIDialog.h"

class CFileItem;
class CGUIMessage;

namespace PVR
{
class CGUIDialogPVRMediaTagInfo : public CGUIDialog
{
public:
  CGUIDialogPVRMediaTagInfo();
  ~CGUIDialogPVRMediaTagInfo() override = default;
  bool OnMessage(CGUIMessage& message) override;
  bool OnInfo(int actionID) override;
  bool HasListItems() const override { return true; }
  CFileItemPtr GetCurrentListItem(int offset = 0) override;

  void SetMediaTag(const CFileItem& item);

private:
  bool OnClickButtonFind(CGUIMessage& message);
  bool OnClickButtonOK(CGUIMessage& message);
  bool OnClickButtonPlay(CGUIMessage& message);

  CFileItemPtr m_mediaItem;
};
} // namespace PVR
