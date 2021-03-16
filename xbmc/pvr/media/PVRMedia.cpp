/*
 *  Copyright (C) 2012-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "PVRMedia.h"

#include "ServiceBroker.h"
#include "pvr/PVRManager.h"
#include "pvr/addons/PVRClients.h"
#include "pvr/epg/EpgInfoTag.h"
#include "pvr/media/PVRMediaPath.h"
#include "pvr/media/PVRMediaTag.h"
#include "threads/SingleLock.h"
#include "utils/URIUtils.h"
#include "utils/log.h"
#include "video/VideoDatabase.h"

#include <memory>
#include <utility>
#include <vector>

using namespace PVR;

CPVRMedia::CPVRMedia() = default;

CPVRMedia::~CPVRMedia()
{
  if (m_database && m_database->IsOpen())
    m_database->Close();
}

void CPVRMedia::UpdateFromClients()
{
  CSingleLock lock(m_critSection);

  for (const auto& mediaTag : m_media)
    mediaTag.second->SetDirty(true);

  std::vector<int> failedClients;
  CServiceBroker::GetPVRManager().Clients()->GetMedia(this, false, failedClients);
  CServiceBroker::GetPVRManager().Clients()->GetMedia(this, true, failedClients);

  // remove media that were deleted at the backend
  for (auto it = m_media.begin(); it != m_media.end();)
  {
    if ((*it).second->IsDirty() && std::find(failedClients.begin(), failedClients.end(),
                                             (*it).second->ClientID()) == failedClients.end())
      it = m_media.erase(it);
    else
      ++it;
  }
}

int CPVRMedia::Load()
{
  Unload();
  Update();
  return m_media.size();
}

void CPVRMedia::Unload()
{
  CSingleLock lock(m_critSection);
  m_bDeletedTVMedia = false;
  m_bDeletedRadioMedia = false;
  m_iTVMedia = 0;
  m_iRadioMedia = 0;
  m_media.clear();
}

void CPVRMedia::Update()
{
  CSingleLock lock(m_critSection);
  if (m_bIsUpdating)
    return;
  m_bIsUpdating = true;
  lock.Leave();

  CLog::LogFC(LOGDEBUG, LOGPVR, "Updating media");
  UpdateFromClients();

  lock.Enter();
  m_bIsUpdating = false;
  lock.Leave();

  CServiceBroker::GetPVRManager().PublishEvent(PVREvent::MediaInvalidated);
}

int CPVRMedia::GetNumTVMedia() const
{
  CSingleLock lock(m_critSection);
  return m_iTVMedia;
}

bool CPVRMedia::HasDeletedTVMedia() const
{
  CSingleLock lock(m_critSection);
  return m_bDeletedTVMedia;
}

int CPVRMedia::GetNumRadioMedia() const
{
  CSingleLock lock(m_critSection);
  return m_iRadioMedia;
}

bool CPVRMedia::HasDeletedRadioMedia() const
{
  CSingleLock lock(m_critSection);
  return m_bDeletedRadioMedia;
}

std::vector<std::shared_ptr<CPVRMediaTag>> CPVRMedia::GetAll() const
{
  std::vector<std::shared_ptr<CPVRMediaTag>> media;

  CSingleLock lock(m_critSection);
  for (const auto& mediaTagEntry : m_media)
  {
    media.emplace_back(mediaTagEntry.second);
  }

  return media;
}

std::shared_ptr<CPVRMediaTag> CPVRMedia::GetById(unsigned int iId) const
{
  CSingleLock lock(m_critSection);
  for (const auto& mediaTag : m_media)
  {
    if (iId == mediaTag.second->m_iMediaTagId)
      return mediaTag.second;
  }

  return {};
}

std::shared_ptr<CPVRMediaTag> CPVRMedia::GetByPath(const std::string& path) const
{
  CSingleLock lock(m_critSection);

  CPVRMediaPath recPath(path);
  if (recPath.IsValid())
  {
    bool bDeleted = recPath.IsDeleted();
    bool bRadio = recPath.IsRadio();

    for (const auto& mediaTag : m_media)
    {
      std::shared_ptr<CPVRMediaTag> current = mediaTag.second;
      // Omit media not matching criteria
      if (!URIUtils::PathEquals(path, current->m_strFileNameAndPath) ||
          bDeleted != current->IsDeleted() || bRadio != current->IsRadio())
        continue;

      return current;
    }
  }

  return {};
}

std::shared_ptr<CPVRMediaTag> CPVRMedia::GetById(int iClientId,
                                                 const std::string& strMediaTagId) const
{
  std::shared_ptr<CPVRMediaTag> retVal;
  CSingleLock lock(m_critSection);
  const auto it = m_media.find(CPVRMediaTagUid(iClientId, strMediaTagId));
  if (it != m_media.end())
    retVal = it->second;

  return retVal;
}

void CPVRMedia::UpdateFromClient(const std::shared_ptr<CPVRMediaTag>& tag,
                                 const CPVRClient& client)
{
  CSingleLock lock(m_critSection);

  if (tag->IsDeleted())
  {
    if (tag->IsRadio())
      m_bDeletedRadioMedia = true;
    else
      m_bDeletedTVMedia = true;
  }

  std::shared_ptr<CPVRMediaTag> existingTag = GetById(tag->m_iClientId, tag->m_strMediaTagId);
  if (existingTag)
  {
    existingTag->Update(*tag, client);
    existingTag->SetDirty(false);
  }
  else
  {
    tag->UpdateMetadata(GetVideoDatabase(), client);
    tag->m_iMediaTagId = ++m_iLastId;
    m_media.insert({CPVRMediaTagUid(tag->m_iClientId, tag->m_strMediaTagId), tag});
    if (tag->IsRadio())
      ++m_iRadioMedia;
    else
      ++m_iTVMedia;
  }
}

bool CPVRMedia::SetMediaPlayCount(const std::shared_ptr<CPVRMediaTag>& mediaTag, int count)
{
  return ChangeMediaPlayCount(mediaTag, count);
}

bool CPVRMedia::IncrementMediaPlayCount(const std::shared_ptr<CPVRMediaTag>& mediaTag)
{
  return ChangeMediaPlayCount(mediaTag, INCREMENT_PLAY_COUNT);
}

bool CPVRMedia::ChangeMediaPlayCount(const std::shared_ptr<CPVRMediaTag>& mediaTag, int count)
{
  if (mediaTag)
  {
    CSingleLock lock(m_critSection);

    CVideoDatabase& db = GetVideoDatabase();
    if (db.IsOpen())
    {
      if (count == INCREMENT_PLAY_COUNT)
        mediaTag->IncrementPlayCount();
      else
        mediaTag->SetPlayCount(count);

      // Clear resume bookmark
      if (mediaTag->GetPlayCount() > 0)
      {
        db.ClearBookMarksOfFile(mediaTag->m_strFileNameAndPath, CBookmark::RESUME);
        mediaTag->SetResumePoint(CBookmark());
      }

      CServiceBroker::GetPVRManager().PublishEvent(PVREvent::MediaInvalidated);
      return true;
    }
  }

  return false;
}

bool CPVRMedia::MarkWatched(const std::shared_ptr<CPVRMediaTag>& mediaTag, bool bWatched)
{
  if (bWatched)
    return IncrementMediaPlayCount(mediaTag);
  else
    return SetMediaPlayCount(mediaTag, 0);
}

bool CPVRMedia::ResetResumePoint(const std::shared_ptr<CPVRMediaTag>& mediaTag)
{
  bool bResult = false;

  if (mediaTag)
  {
    CSingleLock lock(m_critSection);

    CVideoDatabase& db = GetVideoDatabase();
    if (db.IsOpen())
    {
      bResult = true;

      db.ClearBookMarksOfFile(mediaTag->m_strFileNameAndPath, CBookmark::RESUME);
      mediaTag->SetResumePoint(CBookmark());

      CServiceBroker::GetPVRManager().PublishEvent(PVREvent::MediaInvalidated);
    }
  }
  return bResult;
}

CVideoDatabase& CPVRMedia::GetVideoDatabase()
{
  if (!m_database)
  {
    m_database.reset(new CVideoDatabase());
    m_database->Open();

    if (!m_database->IsOpen())
      CLog::LogF(LOGERROR, "Failed to open the video database");
  }

  return *m_database;
}
