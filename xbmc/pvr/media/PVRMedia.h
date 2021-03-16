/*
 *  Copyright (C) 2012-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "threads/CriticalSection.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

class CVideoDatabase;

namespace PVR
{
class CPVRClient;
class CPVREpgInfoTag;
class CPVRMediaTag;
class CPVRMediaTagUid;
class CPVRMediaPath;

class CPVRMedia
{
public:
  CPVRMedia();
  virtual ~CPVRMedia();

  /*!
    * @brief (re)load the media from the clients.
    * @return the number of media loaded.
    */
  int Load();

  /*!
    * @brief unload all media.
    */
  void Unload();

  /*!
    * @brief client has delivered a new/updated media tag.
    * @param tag The media tag
    * @param client The client the media tag belongs to.
    */
  void UpdateFromClient(const std::shared_ptr<CPVRMediaTag>& tag, const CPVRClient& client);

  /*!
    * @brief refresh the media list from the clients.
    */
  void Update();

  int GetNumTVMedia() const;
  bool HasDeletedTVMedia() const;
  int GetNumRadioMedia() const;
  bool HasDeletedRadioMedia() const;

  /*!
    * @brief Set a mediaTag's watched state
    * @param mediaTag The mediaTag
    * @param bWatched True to set watched, false to set unwatched state
    * @return True on success, false otherwise
    */
  bool MarkWatched(const std::shared_ptr<CPVRMediaTag>& mediaTag, bool bWatched);

  /*!
    * @brief Reset a mediaTag's resume point, if any
    * @param mediaTag The mediaTag
    * @return True on success, false otherwise
    */
  bool ResetResumePoint(const std::shared_ptr<CPVRMediaTag>& mediaTag);

  /*!
    * @brief Get a list of all media
    * @return the list of all media
    */
  std::vector<std::shared_ptr<CPVRMediaTag>> GetAll() const;

  std::shared_ptr<CPVRMediaTag> GetByPath(const std::string& path) const;
  std::shared_ptr<CPVRMediaTag> GetById(int iClientId, const std::string& strMediaTagId) const;
  std::shared_ptr<CPVRMediaTag> GetById(unsigned int iId) const;

private:
  mutable CCriticalSection m_critSection;
  bool m_bIsUpdating = false;
  std::map<CPVRMediaTagUid, std::shared_ptr<CPVRMediaTag>> m_media;
  unsigned int m_iLastId = 0;
  std::unique_ptr<CVideoDatabase> m_database;
  bool m_bDeletedTVMedia = false;
  bool m_bDeletedRadioMedia = false;
  unsigned int m_iTVMedia = 0;
  unsigned int m_iRadioMedia = 0;

  void UpdateFromClients();

  /*!
    * @brief Get/Open the video database.
    * @return A reference to the video database.
    */
  CVideoDatabase& GetVideoDatabase();

  /*!
    * @brief Set a mediaTag's play count
    * @param mediaTag The mediaTag
    * @param count The new play count
    * @return True on success, false otherwise
    */
  bool SetMediaPlayCount(const std::shared_ptr<CPVRMediaTag>& mediaTag, int count);

  /*!
    * @brief Increment a mediaTag's play count
    * @param mediaTag The mediaTag
    * @return True on success, false otherwise
    */
  bool IncrementMediaPlayCount(const std::shared_ptr<CPVRMediaTag>& mediaTag);

  /*!
    * @brief special value for parameter count of method ChangeMediaPlayCount
    */
  static const int INCREMENT_PLAY_COUNT = -1;

  /*!
    * @brief change the play count of the given mediaTag
    * @param mediaTag The mediaTag
    * @param count The new play count or INCREMENT_PLAY_COUNT to denote that the current play count is to be incremented by one
    * @return true if the play count was changed successfully
    */
  bool ChangeMediaPlayCount(const std::shared_ptr<CPVRMediaTag>& mediaTag, int count);
};
} // namespace PVR
