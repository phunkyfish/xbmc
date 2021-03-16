/*
 *  Copyright (C) 2012-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "PVRMediaTag.h"

#include "ServiceBroker.h"
#include "guilib/LocalizeStrings.h"
#include "pvr/PVRManager.h"
#include "pvr/addons/PVRClient.h"
#include "pvr/channels/PVRChannel.h"
#include "pvr/channels/PVRChannelGroupsContainer.h"
#include "pvr/epg/Epg.h"
#include "pvr/media/PVRMediaPath.h"
#include "pvr/providers/PVRProvider.h"
#include "pvr/providers/PVRProviders.h"
#include "settings/AdvancedSettings.h"
#include "settings/SettingsComponent.h"
#include "utils/StringUtils.h"
#include "utils/Variant.h"
#include "utils/log.h"
#include "video/VideoDatabase.h"

#include <memory>
#include <string>
#include <vector>

using namespace PVR;

CPVRMediaTagUid::CPVRMediaTagUid(int iClientId, const std::string& strMediaTagId)
  : m_iClientId(iClientId), m_strMediaTagId(strMediaTagId)
{
}

bool CPVRMediaTagUid::operator>(const CPVRMediaTagUid& right) const
{
  return (m_iClientId == right.m_iClientId) ? m_strMediaTagId > right.m_strMediaTagId
                                            : m_iClientId > right.m_iClientId;
}

bool CPVRMediaTagUid::operator<(const CPVRMediaTagUid& right) const
{
  return (m_iClientId == right.m_iClientId) ? m_strMediaTagId < right.m_strMediaTagId
                                            : m_iClientId < right.m_iClientId;
}

bool CPVRMediaTagUid::operator==(const CPVRMediaTagUid& right) const
{
  return m_iClientId == right.m_iClientId && m_strMediaTagId == right.m_strMediaTagId;
}

bool CPVRMediaTagUid::operator!=(const CPVRMediaTagUid& right) const
{
  return m_iClientId != right.m_iClientId || m_strMediaTagId != right.m_strMediaTagId;
}

CPVRMediaTag::CPVRMediaTag()
{
  Reset();
}

CPVRMediaTag::CPVRMediaTag(const PVR_MEDIA_TAG& mediaTag, unsigned int iClientId)
{
  Reset();

  m_strMediaTagId = mediaTag.strMediaTagId;
  m_strTitle = mediaTag.strTitle;
  m_strShowTitle = mediaTag.strEpisodeName;
  m_iSeason = mediaTag.iSeriesNumber;
  m_iEpisode = mediaTag.iEpisodeNumber;
  m_iEpisodePart = mediaTag.iEpisodePartNumber;
  if (mediaTag.iYear > 0)
    SetYear(mediaTag.iYear);
  m_iClientId = iClientId;
  m_mediaTagTime =
      mediaTag.mediaTagTime +
      CServiceBroker::GetSettingsComponent()->GetAdvancedSettings()->m_iPVRTimeCorrection;
  // Deleted mediaTag is placed at the root of the deleted view
  m_strDirectory = mediaTag.bIsDeleted ? "" : mediaTag.strDirectory;
  m_strPlot = mediaTag.strPlot;
  m_strPlotOutline = mediaTag.strPlotOutline;
  m_strProviderName = mediaTag.strProviderName;
  m_strIconPath = mediaTag.strIconPath;
  m_strThumbnailPath = mediaTag.strThumbnailPath;
  m_strFanartPath = mediaTag.strFanartPath;
  m_bIsDeleted = mediaTag.bIsDeleted;
  m_iClientProviderUniqueId = mediaTag.iClientProviderUid;
  if (strlen(mediaTag.strFirstAired) > 0)
    m_firstAired.SetFromW3CDateTime(mediaTag.strFirstAired);
  m_iFlags = mediaTag.iFlags;
  if (mediaTag.sizeInBytes >= 0)
    m_sizeInBytes = mediaTag.sizeInBytes;

  m_strOriginalTitle = mediaTag.strOriginalTitle;
  m_cast = Tokenize(mediaTag.strCast);
  m_directors = Tokenize(mediaTag.strDirector);
  m_writers = Tokenize(mediaTag.strWriter);
  m_strIMDBNumber = mediaTag.strIMDBNumber;
  m_strSeriesLink = mediaTag.strSeriesLink;
  m_strParentalRatingCode = mediaTag.strParentalRatingCode;
  m_iParentalRating = mediaTag.iParentalRating;
  m_iStarRating = mediaTag.iStarRating;

  SetGenre(mediaTag.iGenreType, mediaTag.iGenreSubType, mediaTag.strGenreDescription);
  CVideoInfoTag::SetPlayCount(mediaTag.iPlayCount);
  CVideoInfoTag::SetResumePoint(mediaTag.iLastPlayedPosition, mediaTag.iDuration, "");
  SetDuration(mediaTag.iDuration);

  if (mediaTag.mediaClass != PVR_MEDIA_TAG_CLASS_UNKNOWN)
  {
    m_bRadio = mediaTag.mediaClass == PVR_MEDIA_TAG_CLASS_AUDIO;
  }
  else
  {
    if (mediaTag.mediaType != PVR_MEDIA_TAG_TYPE_UNKNOWN)
    {
      m_bRadio = (mediaTag.mediaType == PVR_MEDIA_TAG_TYPE_MUSIC) ||
                 (mediaTag.mediaType == PVR_MEDIA_TAG_TYPE_RADIO_SHOW) ||
                 (mediaTag.mediaType == PVR_MEDIA_TAG_TYPE_PODCAST);
    }
    else
    {
      const std::shared_ptr<CPVRClient> client =
          CServiceBroker::GetPVRManager().GetClient(m_iClientId);
      bool bSupportsRadio = client && client->GetClientCapabilities().SupportsRadio();
      if (bSupportsRadio && client && client->GetClientCapabilities().SupportsTV())
      {
        CLog::Log(LOGWARNING, "Unable to determine media class. Defaulting to TV.");
        m_bRadio = false; // Assume TV.
      }
      else
      {
        m_bRadio = bSupportsRadio;
      }
    }
  }
  m_mediaType = mediaTag.mediaType;


  UpdatePath();
}

bool CPVRMediaTag::operator==(const CPVRMediaTag& right) const
{
  CSingleLock lock(m_critSection);
  return (this == &right) ||
      (m_strMediaTagId == right.m_strMediaTagId &&
       m_iClientId == right.m_iClientId &&
       m_strProviderName == right.m_strProviderName &&
       m_mediaTagTime == right.m_mediaTagTime &&
       GetDuration() == right.GetDuration() &&
       m_strPlotOutline == right.m_strPlotOutline &&
       m_strPlot == right.m_strPlot &&
       m_strDirectory == right.m_strDirectory &&
       m_strFileNameAndPath == right.m_strFileNameAndPath &&
       m_strTitle == right.m_strTitle &&
       m_strShowTitle == right.m_strShowTitle &&
       m_iSeason == right.m_iSeason &&
       m_iEpisode == right.m_iEpisode &&
       m_iEpisodePart == right.m_iEpisodePart &&
       GetPremiered() == right.GetPremiered() &&
       m_strIconPath == right.m_strIconPath &&
       m_strThumbnailPath == right.m_strThumbnailPath &&
       m_strFanartPath == right.m_strFanartPath &&
       m_iMediaTagId == right.m_iMediaTagId &&
       m_bIsDeleted == right.m_bIsDeleted &&
       m_iClientProviderUniqueId == right.m_iClientProviderUniqueId &&
       m_bRadio == right.m_bRadio &&
       m_mediaType == right.m_mediaType &&
       m_genre == right.m_genre &&
       m_iGenreType == right.m_iGenreType &&
       m_iGenreSubType == right.m_iGenreSubType &&
       m_strOriginalTitle == right.m_strOriginalTitle &&
       m_cast == right.m_cast &&
       m_directors == right.m_directors &&
       m_writers == right.m_writers &&
       m_strIMDBNumber == right.m_strIMDBNumber &&
       m_strSeriesLink == right.m_strSeriesLink &&
       m_strParentalRatingCode == right.m_strParentalRatingCode &&
       m_iParentalRating == right.m_iParentalRating &&
       m_iStarRating == right.m_iStarRating &&
       m_firstAired == right.m_firstAired &&
       m_iFlags == right.m_iFlags &&
       m_sizeInBytes == right.m_sizeInBytes);
}

bool CPVRMediaTag::operator!=(const CPVRMediaTag& right) const
{
  return !(*this == right);
}

void CPVRMediaTag::Serialize(CVariant& value) const
{
  CVideoInfoTag::Serialize(value);

  value["provider"] = m_strProviderName;
  value["directory"] = m_strDirectory;
  value["icon"] = m_strIconPath;
  value["starttime"] = m_mediaTagTime.IsValid() ? m_mediaTagTime.GetAsDBDateTime() : "";
  value["mediaTagid"] = m_iMediaTagId;
  value["isdeleted"] = m_bIsDeleted;
  value["radio"] = m_bRadio;
  value["genre"] = m_genre;

  if (!value.isMember("art"))
    value["art"] = CVariant(CVariant::VariantTypeObject);
  if (!m_strThumbnailPath.empty())
    value["art"]["thumb"] = m_strThumbnailPath;
  if (!m_strFanartPath.empty())
    value["art"]["fanart"] = m_strFanartPath;

  value["clientid"] = m_iClientId;
}

void CPVRMediaTag::ToSortable(SortItem& sortable, Field field) const
{
  CSingleLock lock(m_critSection);
  if (field == FieldSize)
    sortable[FieldSize] = m_sizeInBytes;
  else
    CVideoInfoTag::ToSortable(sortable, field);
}

void CPVRMediaTag::Reset()
{
  m_strMediaTagId.clear();
  m_iClientId = -1;
  m_strProviderName.clear();
  m_strDirectory.clear();
  m_strFileNameAndPath.clear();
  m_strIconPath.clear();
  m_strThumbnailPath.clear();
  m_strFanartPath.clear();
  m_bGotMetaData = false;
  m_iMediaTagId = 0;
  m_bIsDeleted = false;
  m_iSeason = -1;
  m_iEpisode = -1;
  m_iEpisodePart = -1;
  m_cast.clear();
  m_directors.clear();
  m_writers.clear();
  m_strIMDBNumber.clear();
  m_strSeriesLink.clear();
  m_strParentalRatingCode.clear();
  m_iParentalRating = 0;
  m_iStarRating = 0;
  m_iClientProviderUniqueId = PVR_PROVIDER_INVALID_UID;
  m_bRadio = false;
  m_mediaType = PVR_MEDIA_TAG_TYPE_UNKNOWN;
  m_iFlags = PVR_MEDIA_TAG_FLAG_UNDEFINED;
  {
    CSingleLock lock(m_critSection);
    m_sizeInBytes = 0;
  }

  m_mediaTagTime.Reset();
  CVideoInfoTag::Reset();
}

bool CPVRMediaTag::Delete()
{
  std::shared_ptr<CPVRClient> client = CServiceBroker::GetPVRManager().GetClient(m_iClientId);
  return client && (client->DeleteMediaTag(*this) == PVR_ERROR_NO_ERROR);
}

bool CPVRMediaTag::Undelete()
{
  const std::shared_ptr<CPVRClient> client = CServiceBroker::GetPVRManager().GetClient(m_iClientId);
  return client && (client->UndeleteMediaTag(*this) == PVR_ERROR_NO_ERROR);
}

bool CPVRMediaTag::Rename(const std::string& strNewName)
{
  m_strTitle = strNewName;
  const std::shared_ptr<CPVRClient> client = CServiceBroker::GetPVRManager().GetClient(m_iClientId);
  return client && (client->RenameMediaTag(*this) == PVR_ERROR_NO_ERROR);
}

bool CPVRMediaTag::SetPlayCount(int count)
{
  const std::shared_ptr<CPVRClient> client = CServiceBroker::GetPVRManager().GetClient(m_iClientId);
  if (client && client->GetClientCapabilities().SupportsMediaPlayCount())
  {
    if (client->SetMediaTagPlayCount(*this, count) != PVR_ERROR_NO_ERROR)
      return false;
  }

  return CVideoInfoTag::SetPlayCount(count);
}

bool CPVRMediaTag::IncrementPlayCount()
{
  const std::shared_ptr<CPVRClient> client = CServiceBroker::GetPVRManager().GetClient(m_iClientId);
  if (client && client->GetClientCapabilities().SupportsMediaPlayCount())
  {
    if (client->SetMediaTagPlayCount(*this, CVideoInfoTag::GetPlayCount() + 1) !=
        PVR_ERROR_NO_ERROR)
      return false;
  }

  return CVideoInfoTag::IncrementPlayCount();
}

bool CPVRMediaTag::SetResumePoint(const CBookmark& resumePoint)
{
  const std::shared_ptr<CPVRClient> client = CServiceBroker::GetPVRManager().GetClient(m_iClientId);
  if (client && client->GetClientCapabilities().SupportsMediaLastPlayedPosition())
  {
    if (client->SetMediaTagLastPlayedPosition(*this, lrint(resumePoint.timeInSeconds)) !=
        PVR_ERROR_NO_ERROR)
      return false;
  }

  return CVideoInfoTag::SetResumePoint(resumePoint);
}

bool CPVRMediaTag::SetResumePoint(double timeInSeconds,
                                  double totalTimeInSeconds,
                                  const std::string& playerState /* = "" */)
{
  const std::shared_ptr<CPVRClient> client = CServiceBroker::GetPVRManager().GetClient(m_iClientId);
  if (client && client->GetClientCapabilities().SupportsMediaLastPlayedPosition())
  {
    if (client->SetMediaTagLastPlayedPosition(*this, lrint(timeInSeconds)) != PVR_ERROR_NO_ERROR)
      return false;
  }

  return CVideoInfoTag::SetResumePoint(timeInSeconds, totalTimeInSeconds, playerState);
}

CBookmark CPVRMediaTag::GetResumePoint() const
{
  const std::shared_ptr<CPVRClient> client = CServiceBroker::GetPVRManager().GetClient(m_iClientId);
  if (client && client->GetClientCapabilities().SupportsMediaLastPlayedPosition() &&
      m_resumePointRefetchTimeout.IsTimePast())
  {
    // @todo: root cause should be fixed. details: https://github.com/xbmc/xbmc/pull/14961
    m_resumePointRefetchTimeout.Set(
        10000); // update resume point from backend at most every 10 secs

    int pos = -1;
    client->GetMediaTagLastPlayedPosition(*this, pos);

    if (pos >= 0)
    {
      CBookmark resumePoint(CVideoInfoTag::GetResumePoint());
      resumePoint.timeInSeconds = pos;
      CPVRMediaTag* pThis = const_cast<CPVRMediaTag*>(this);
      pThis->CVideoInfoTag::SetResumePoint(resumePoint);
    }
  }
  return CVideoInfoTag::GetResumePoint();
}

bool CPVRMediaTag::UpdateMediaTagSize()
{
  const std::shared_ptr<CPVRClient> client = CServiceBroker::GetPVRManager().GetClient(m_iClientId);
  if (client && client->GetClientCapabilities().SupportsMediaSize() &&
      m_mediaTagSizeRefetchTimeout.IsTimePast())
  {
    // @todo: root cause should be fixed. details: https://github.com/xbmc/xbmc/pull/14961
    m_mediaTagSizeRefetchTimeout.Set(10000); // update size from backend at most every 10 secs

    int64_t sizeInBytes = -1;
    client->GetMediaTagSize(*this, sizeInBytes);

    CSingleLock lock(m_critSection);
    if (sizeInBytes >= 0 && sizeInBytes != m_sizeInBytes)
    {
      CSingleLock lock(m_critSection);
      m_sizeInBytes = sizeInBytes;
      return true;
    }
  }

  return false;
}

void CPVRMediaTag::UpdateMetadata(CVideoDatabase& db, const CPVRClient& client)
{
  if (m_bGotMetaData || !db.IsOpen())
    return;

  if (!client.GetClientCapabilities().SupportsMediaPlayCount())
    CVideoInfoTag::SetPlayCount(db.GetPlayCount(m_strFileNameAndPath));

  if (!client.GetClientCapabilities().SupportsMediaLastPlayedPosition())
  {
    CBookmark resumePoint;
    if (db.GetResumeBookMark(m_strFileNameAndPath, resumePoint))
      CVideoInfoTag::SetResumePoint(resumePoint);
  }

  m_bGotMetaData = true;
}

std::vector<PVR_EDL_ENTRY> CPVRMediaTag::GetEdl() const
{
  std::vector<PVR_EDL_ENTRY> edls;

  const std::shared_ptr<CPVRClient> client = CServiceBroker::GetPVRManager().GetClient(m_iClientId);
  if (client && client->GetClientCapabilities().SupportsMediaEdl())
    client->GetMediaTagEdl(*this, edls);

  return edls;
}

void CPVRMediaTag::Update(const CPVRMediaTag& tag, const CPVRClient& client)
{
  m_strMediaTagId = tag.m_strMediaTagId;
  m_iClientId = tag.m_iClientId;
  m_strTitle = tag.m_strTitle;
  m_strShowTitle = tag.m_strShowTitle;
  m_iSeason = tag.m_iSeason;
  m_iEpisode = tag.m_iEpisode;
  SetPremiered(tag.GetPremiered());
  m_mediaTagTime = tag.m_mediaTagTime;
  m_strDirectory = tag.m_strDirectory;
  m_strPlot = tag.m_strPlot;
  m_strPlotOutline = tag.m_strPlotOutline;
  m_iEpisodePart = tag.m_iEpisodePart;
  m_strOriginalTitle = tag.m_strOriginalTitle;
  m_cast = tag.m_cast;
  m_directors = tag.m_directors;
  m_writers = tag.m_writers;
  m_strIMDBNumber = tag.m_strIMDBNumber;
  m_strSeriesLink = tag.m_strSeriesLink;
  m_strParentalRatingCode = tag.m_strParentalRatingCode;
  m_iParentalRating = tag.m_iParentalRating;
  m_iStarRating = tag.m_iStarRating;
  m_strProviderName = tag.m_strProviderName;
  m_genre = tag.m_genre;
  m_strIconPath = tag.m_strIconPath;
  m_strThumbnailPath = tag.m_strThumbnailPath;
  m_strFanartPath = tag.m_strFanartPath;
  m_bIsDeleted = tag.m_bIsDeleted;
  m_iClientProviderUniqueId = tag.m_iClientProviderUniqueId;
  m_bRadio = tag.m_bRadio;
  m_mediaType = tag.m_mediaType;
  m_firstAired = tag.m_firstAired;
  m_iFlags = tag.m_iFlags;
  {
    CSingleLock lock(m_critSection);
    m_sizeInBytes = tag.m_sizeInBytes;
  }

  if (client.GetClientCapabilities().SupportsMediaPlayCount())
    CVideoInfoTag::SetPlayCount(tag.GetLocalPlayCount());

  if (client.GetClientCapabilities().SupportsMediaLastPlayedPosition())
    CVideoInfoTag::SetResumePoint(tag.GetLocalResumePoint());

  SetDuration(tag.GetDuration());

  if (m_iGenreType == EPG_GENRE_USE_STRING || m_iGenreSubType == EPG_GENRE_USE_STRING)
  {
    /* No type/subtype. Use the provided description */
    m_genre = tag.m_genre;
  }
  else
  {
    /* Determine genre description by type/subtype */
    m_genre = StringUtils::Split(
        CPVREpg::ConvertGenreIdToString(tag.m_iGenreType, tag.m_iGenreSubType),
        CServiceBroker::GetSettingsComponent()->GetAdvancedSettings()->m_videoItemSeparator);
  }

  //Old Method of identifying TV show title and subtitle using m_strDirectory and strPlotOutline (deprecated)
  std::string strShow = StringUtils::Format("%s - ", g_localizeStrings.Get(20364).c_str());
  if (StringUtils::StartsWithNoCase(m_strPlotOutline, strShow))
  {
    CLog::Log(LOGWARNING, "PVR addon provides episode name in strPlotOutline which is deprecated");
    std::string strEpisode = m_strPlotOutline;
    std::string strTitle = m_strDirectory;

    size_t pos = strTitle.rfind('/');
    strTitle.erase(0, pos + 1);
    strEpisode.erase(0, strShow.size());
    m_strTitle = strTitle;
    pos = strEpisode.find('-');
    strEpisode.erase(0, pos + 2);
    m_strShowTitle = strEpisode;
  }

  UpdatePath();
}

void CPVRMediaTag::UpdatePath()
{
  m_strFileNameAndPath =
      CPVRMediaPath(m_bIsDeleted, m_bRadio, m_strDirectory, m_strTitle, m_iSeason, m_iEpisode,
                    GetYear(), m_strShowTitle, m_strProviderName, m_mediaTagTime, m_strMediaTagId);
}

const CDateTime& CPVRMediaTag::MediaTagTimeAsLocalTime() const
{
  static CDateTime tmp;
  tmp.SetFromUTCDateTime(m_mediaTagTime);

  return tmp;
}

std::string CPVRMediaTag::GetTitleFromURL(const std::string& url)
{
  return CPVRMediaPath(url).GetTitle();
}

int CPVRMediaTag::ClientProviderUniqueId() const
{
  return m_iClientProviderUniqueId;
}

std::string CPVRMediaTag::ProviderName() const
{
  return m_strProviderName;
}

int CPVRMediaTag::ClientID() const
{
  return m_iClientId;
}

void CPVRMediaTag::SetGenre(int iGenreType, int iGenreSubType, const std::string& strGenre)
{
  m_iGenreType = iGenreType;
  m_iGenreSubType = iGenreSubType;

  if ((iGenreType == EPG_GENRE_USE_STRING || iGenreSubType == EPG_GENRE_USE_STRING) &&
      !strGenre.empty())
  {
    /* Type and sub type are not given. Use the provided genre description if available. */
    m_genre = StringUtils::Split(
        strGenre,
        CServiceBroker::GetSettingsComponent()->GetAdvancedSettings()->m_videoItemSeparator);
  }
  else
  {
    /* Determine the genre description from the type and subtype IDs */
    m_genre = StringUtils::Split(
        CPVREpg::ConvertGenreIdToString(iGenreType, iGenreSubType),
        CServiceBroker::GetSettingsComponent()->GetAdvancedSettings()->m_videoItemSeparator);
  }
}

const std::string CPVRMediaTag::GetGenresLabel() const
{
  return StringUtils::Join(
      m_genre, CServiceBroker::GetSettingsComponent()->GetAdvancedSettings()->m_videoItemSeparator);
}

CDateTime CPVRMediaTag::FirstAired() const
{
  return m_firstAired;
}

void CPVRMediaTag::SetYear(int year)
{
  if (year > 0)
    m_premiered = CDateTime(year, 1, 1, 0, 0, 0);
}

int CPVRMediaTag::GetYear() const
{
  return m_premiered.GetYear();
}

bool CPVRMediaTag::HasYear() const
{
  return m_premiered.IsValid();
}

bool CPVRMediaTag::IsNew() const
{
  return (m_iFlags & PVR_MEDIA_TAG_FLAG_IS_NEW) > 0;
}

bool CPVRMediaTag::IsPremiere() const
{
  return (m_iFlags & PVR_MEDIA_TAG_FLAG_IS_PREMIERE) > 0;
}

bool CPVRMediaTag::IsLive() const
{
  return (m_iFlags & PVR_MEDIA_TAG_FLAG_IS_LIVE) > 0;
}

bool CPVRMediaTag::IsFinale() const
{
  return (m_iFlags & PVR_MEDIA_TAG_FLAG_IS_FINALE) > 0;
}

int64_t CPVRMediaTag::GetSizeInBytes() const
{
  CSingleLock lock(m_critSection);
  return m_sizeInBytes;
}

PVR_MEDIA_TAG_TYPE CPVRMediaTag::MediaType() const
{
  CSingleLock lock(m_critSection);
  return m_mediaType;
}

const std::string CPVRMediaTag::GetCastLabel() const
{
  // Note: see CVideoInfoTag::GetCast for reference implementation.
  std::string strLabel;
  for (const auto& castEntry : m_cast)
    strLabel += StringUtils::Format("%s\n", castEntry.c_str());

  return StringUtils::TrimRight(strLabel, "\n");
}

const std::string CPVRMediaTag::GetDirectorsLabel() const
{
  return StringUtils::Join(
      m_directors,
      CServiceBroker::GetSettingsComponent()->GetAdvancedSettings()->m_videoItemSeparator);
}

const std::string CPVRMediaTag::GetWritersLabel() const
{
  return StringUtils::Join(
      m_writers,
      CServiceBroker::GetSettingsComponent()->GetAdvancedSettings()->m_videoItemSeparator);
}

std::string CPVRMediaTag::IMDBNumber() const
{
  return m_strIMDBNumber;
}

std::string CPVRMediaTag::SeriesLink() const
{
  return m_strSeriesLink;
}

std::string CPVRMediaTag::ParentalRatingCode() const
{
  return m_strParentalRatingCode;
}

int CPVRMediaTag::ParentalRating() const
{
  return m_iParentalRating;
}

int CPVRMediaTag::StarRating() const
{
  return m_iStarRating;
}

const std::vector<std::string> CPVRMediaTag::Tokenize(const std::string& str)
{
  return StringUtils::Split(str.c_str(), EPG_STRING_TOKEN_SEPARATOR);
}

const std::string CPVRMediaTag::DeTokenize(const std::vector<std::string>& tokens)
{
  return StringUtils::Join(tokens, EPG_STRING_TOKEN_SEPARATOR);
}

std::shared_ptr<CPVRProvider> CPVRMediaTag::GetDefaultProvider() const
{
  return CServiceBroker::GetPVRManager().Providers()->GetByClient(m_iClientId,
                                                                  PVR_PROVIDER_INVALID_UID);
}

bool CPVRMediaTag::HasClientProvider() const
{
  CSingleLock lock(m_critSection);
  return m_iClientProviderUniqueId != PVR_PROVIDER_INVALID_UID;
}

std::shared_ptr<CPVRProvider> CPVRMediaTag::GetProvider() const
{
  return CServiceBroker::GetPVRManager().Providers()->GetByClient(m_iClientId,
                                                                  m_iClientProviderUniqueId);
}
