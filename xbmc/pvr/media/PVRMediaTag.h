/*
 *  Copyright (C) 2012-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

/*
 * DESCRIPTION:
 *
 * CPVRMediaTagInfoTag is part of the Kodi PVR system to support mediaTag entrys,
 * stored on a other Backend like VDR or MythTV.
 *
 * The mediaTag information tag holds data about name, length, mediaTag time
 * and so on of recorded stream stored on the backend.
 *
 * The filename string is used to by the PVRManager and passed to VideoPlayer
 * to stream data from the backend to Kodi.
 *
 * It is a also CVideoInfoTag and some of his variables must be set!
 *
 */

#include "XBDateTime.h"
#include "addons/kodi-dev-kit/include/kodi/c-api/addon-instance/pvr/pvr_media.h"
#include "threads/CriticalSection.h"
#include "threads/SystemClock.h"
#include "video/Bookmark.h"
#include "video/VideoInfoTag.h"

#include <memory>
#include <string>
#include <vector>

class CVideoDatabase;

struct PVR_EDL_ENTRY;
struct PVR_MEDIA_TAG;

namespace PVR
{
class CPVRChannel;
class CPVRClient;
class CPVRProvider;

/*!
* @brief Representation of a CPVRMediaTag unique ID.
*/
class CPVRMediaTagUid final
{
public:
  int m_iClientId; /*!< ID of the backend */
  std::string m_strMediaTagId; /*!< unique ID of the mediaTag on the client */

  CPVRMediaTagUid(int iClientId, const std::string& strMediaTagId);

  bool operator>(const CPVRMediaTagUid& right) const;
  bool operator<(const CPVRMediaTagUid& right) const;
  bool operator==(const CPVRMediaTagUid& right) const;
  bool operator!=(const CPVRMediaTagUid& right) const;
};

class CPVRMediaTag final : public CVideoInfoTag
{
public:
  int m_iClientId; /*!< ID of the backend */
  std::string m_strMediaTagId; /*!< unique ID of the mediaTag on the client */
  std::string m_strProviderName; /*!< name of the provider this media is from the client */
  std::string m_strDirectory; /*!< directory of this mediaTag on the client */
  std::string m_strIconPath; /*!< icon path */
  std::string m_strThumbnailPath; /*!< thumbnail path */
  std::string m_strFanartPath; /*!< fanart path */
  unsigned m_iMediaTagId; /*!< id that won't change while xbmc is running */
  std::vector<std::string> m_cast; /*!< cast */
  std::vector<std::string> m_directors; /*!< director(s) */
  std::vector<std::string> m_writers; /*!< writer(s) */
  std::string m_strIMDBNumber; /*!< imdb number */
  std::string m_strSeriesLink; /*!< series link */
  std::string m_strParentalRatingCode; /*!< parental rating code */

  CPVRMediaTag();
  CPVRMediaTag(const PVR_MEDIA_TAG& mediaTag, unsigned int iClientId);

private:
  CPVRMediaTag(const CPVRMediaTag& tag) = delete;
  CPVRMediaTag& operator=(const CPVRMediaTag& other) = delete;

public:
  bool operator==(const CPVRMediaTag& right) const;
  bool operator!=(const CPVRMediaTag& right) const;

  void Serialize(CVariant& value) const override;

  // ISortable implementation
  void ToSortable(SortItem& sortable, Field field) const override;

  /*!
   * @brief Reset this tag to it's initial state.
   */
  void Reset();

  /*!
   * @brief Delete this mediaTag on the client (if supported).
   * @return True if it was deleted successfully, false otherwise.
   */
  bool Delete();

  /*!
   * @brief Undelete this mediaTag on the client (if supported).
   * @return True if it was undeleted successfully, false otherwise.
   */
  bool Undelete();

  /*!
   * @brief Rename this mediaTag on the client (if supported).
   * @param strNewName The new name.
   * @return True if it was renamed successfully, false otherwise.
   */
  bool Rename(const std::string& strNewName);

  /*!
   * @brief Set this mediaTag's play count. The value will be transferred to the backend if it supports server-side play counts.
   * @param count play count.
   * @return True if play count was set successfully, false otherwise.
   */
  bool SetPlayCount(int count) override;

  /*!
   * @brief Increment this mediaTag's play count. The value will be transferred to the backend if it supports server-side play counts.
   * @return True if play count was increased successfully, false otherwise.
   */
  bool IncrementPlayCount() override;

  /*!
   * @brief Set this mediaTag's play count without transferring the value to the backend, even if it supports server-side play counts.
   * @param count play count.
   * @return True if play count was set successfully, false otherwise.
   */
  bool SetLocalPlayCount(int count) { return CVideoInfoTag::SetPlayCount(count); }

  /*!
   * @brief Get this mediaTag's local play count. The value will not be obtained from the backend, even if it supports server-side play counts.
   * @return the play count.
   */
  int GetLocalPlayCount() const { return CVideoInfoTag::GetPlayCount(); }

  /*!
   * @brief Set this mediaTag's resume point. The value will be transferred to the backend if it supports server-side resume points.
   * @param resumePoint resume point.
   * @return True if resume point was set successfully, false otherwise.
   */
  bool SetResumePoint(const CBookmark& resumePoint) override;

  /*!
   * @brief Set this mediaTag's resume point. The value will be transferred to the backend if it supports server-side resume points.
   * @param timeInSeconds the time of the resume point
   * @param totalTimeInSeconds the total time of the video
   * @param playerState the player state
   * @return True if resume point was set successfully, false otherwise.
   */
  bool SetResumePoint(double timeInSeconds,
                      double totalTimeInSeconds,
                      const std::string& playerState = "") override;

  /*!
   * @brief Get this mediaTag's resume point. The value will be obtained from the backend if it supports server-side resume points.
   * @return the resume point.
   */
  CBookmark GetResumePoint() const override;

  /*!
   * @brief Update this mediaTag's size. The value will be obtained from the backend if it supports server-side size retrieval.
   * @return true if the the updated value is differnt, false otherwise.
   */
  bool UpdateMediaTagSize();

  /*!
   * @brief Get this mediaTag's local resume point. The value will not be obtained from the backend even if it supports server-side resume points.
   * @return the resume point.
   */
  CBookmark GetLocalResumePoint() const { return CVideoInfoTag::GetResumePoint(); }

  /*!
   * @brief Retrieve the edit decision list (EDL) of a mediaTag on the backend.
   * @return The edit decision list (empty on error)
   */
  std::vector<PVR_EDL_ENTRY> GetEdl() const;

  /*!
   * @brief Get the resume point and play count from the database if the
   * client doesn't handle it itself.
    * @param db The database to read the data from.
    * @param client The client this media tag belongs to.
   */
  void UpdateMetadata(CVideoDatabase& db, const CPVRClient& client);

  /*!
   * @brief Update this tag with the contents of the given tag.
   * @param tag The new tag info.
   * @param client The client this media tag belongs to.
   */
  void Update(const CPVRMediaTag& tag, const CPVRClient& client);

  /*!
   * @brief Retrieve the mediaTag start as UTC time
   * @return the mediaTag start time
   */
  const CDateTime& MediaTagTimeAsUTC() const { return m_mediaTagTime; }

  /*!
   * @brief Retrieve the mediaTag start as local time
   * @return the mediaTag start time
   */
  const CDateTime& MediaTagTimeAsLocalTime() const;

  /*!
   * @brief Retrieve the mediaTag title from the URL path
   * @param url the URL for the mediaTag
   * @return Title of the mediaTag
   */
  static std::string GetTitleFromURL(const std::string& url);

  /*!
   * @brief If deleted but can be undeleted it is true
   */
  bool IsDeleted() const { return m_bIsDeleted; }

  /*!
   * @brief Check whether this is a tv or radio mediaTag
   * @return true if this is a radio mediaTag, false if this is a tv mediaTag
   */
  bool IsRadio() const { return m_bRadio; }

  /*!
  * @brief Get the uid of the provider on the client on which this mediaTag is from
  * @return the client uid of the provider or PVR_PROVIDER_INVALID_UID
  */
  int ClientProviderUniqueId() const;

  /*!
    * @brief Get the client provider name for this media tag
    * @return m_strProviderName The name for this media tags provider
    */
  std::string ProviderName() const;

  /*!
   * @brief the identifier of the client that serves this mediaTag
   * @return the client identifier
   */
  int ClientID() const;

  /*!
   * @brief Retrieve the mediaTag Episode Name
   * @note Returns an empty string if no Episode Name was provided by the PVR client
   */
  std::string EpisodeName() const { return m_strShowTitle; }

  /*!
   * @brief set the genre for this mediaTag.
   * @param iGenreType The genre type ID. If set to EPG_GENRE_USE_STRING, set genre to the value provided with strGenre. Otherwise, compile the genre string from the values given by iGenreType and iGenreSubType
   * @param iGenreSubType The genre subtype ID
   * @param strGenre The genre
   */
  void SetGenre(int iGenreType, int iGenreSubType, const std::string& strGenre);

  /*!
   * @brief Get the genre type ID of this mediaTag.
   * @return The genre type ID.
   */
  int GenreType() const { return m_iGenreType; }

  /*!
   * @brief Get the genre subtype ID of this mediaTag.
   * @return The genre subtype ID.
   */
  int GenreSubType() const { return m_iGenreSubType; }

  /*!
   * @brief Get the genre as human readable string.
   * @return The genre.
   */
  const std::vector<std::string> Genre() const { return m_genre; }

  /*!
   * @brief Get the genre(s) of this mediaTag as formatted string.
   * @return The genres label.
   */
  const std::string GetGenresLabel() const;

  /*!
   * @brief Get the first air date of this mediaTag.
   * @return The first air date.
   */
  CDateTime FirstAired() const;

  /*!
   * @brief Get the premiere year of this mediaTag.
   * @return The premiere year
   */
  int GetYear() const override;

  /*!
   * @brief Set the premiere year of this mediaTag.
   * @param year The premiere year
   */
  void SetYear(int year) override;

  /*!
   * @brief Check if the premiere year of this mediaTag is valid
   * @return True if the mediaTag has as valid premiere date, false otherwise
   */
  bool HasYear() const override;

  /*!
   * @brief Check whether this mediaTag will be flagged as new.
   * @return True if this mediaTag will be flagged as new, false otherwise
   */
  bool IsNew() const;

  /*!
   * @brief Check whether this mediaTag will be flagged as a premiere.
   * @return True if this mediaTag will be flagged as a premiere, false otherwise
   */
  bool IsPremiere() const;

  /*!
   * @brief Check whether this mediaTag will be flagged as a finale.
   * @return True if this mediaTag will be flagged as a finale, false otherwise
   */
  bool IsFinale() const;

  /*!
   * @brief Check whether this mediaTag will be flagged as live.
   * @return True if this mediaTag will be flagged as live, false otherwise
   */
  bool IsLive() const;

  /*!
   * @brief Return the flags (PVR_MEDIA_TAG_FLAG_*) of this mediaTag as a bitfield.
   * @return the flags.
   */
  unsigned int Flags() const { return m_iFlags; }

  /*!
   * @brief Return the size of this mediaTag in bytes.
   * @return the size in bytes.
   */
  int64_t GetSizeInBytes() const;

  /*!
    * @brief Get the cast of this event as formatted string.
    * @return The cast label.
    */
  const std::string GetCastLabel() const;

  /*!
    * @brief Get the director(s) of this event as formatted string.
    * @return The directors label.
    */
  const std::string GetDirectorsLabel() const;

  /*!
    * @brief Get the writer(s) of this event as formatted string.
    * @return The writers label.
    */
  const std::string GetWritersLabel() const;

  /*!
    * @brief Get the imdbnumber of this event.
    * @return The imdbnumber.
    */
  std::string IMDBNumber() const;

  /*!
    * @brief The series link for this event.
    * @return The series link or empty string, if not available.
    */
  std::string SeriesLink() const;

  /*!
    * @brief Get the parental rating code of this event.
    * @return The parental rating code.
    */
  std::string ParentalRatingCode() const;

  /*!
    * @brief Get the parental rating of this event.
    * @return The parental rating.
    */
  int ParentalRating() const;

  /*!
    * @brief Get the star rating of this event.
    * @return The star rating.
    */
  int StarRating() const;

  /*!
   * @brief Return the mediaTag type.
   * @return the type of media.
   */
  PVR_MEDIA_TAG_TYPE MediaType() const;

  /*!
   * @brief Mark a mediaTag as dirty/clean.
   * @param bDirty true to mark as dirty, false to mark as clean.
   */
  void SetDirty(bool bDirty) { m_bDirty = bDirty; }

  /*!
   * @brief Return whether the mediaTag is marked dirty.
   * @return true if dirty, false otherwise.
   */
  bool IsDirty() const { return m_bDirty; }

  /*!
    * @brief Split the given string into tokens. Interpretes occurences of EPG_STRING_TOKEN_SEPARATOR in the string as separator.
    * @param str The string to tokenize.
    * @return the tokens.
    */
  static const std::vector<std::string> Tokenize(const std::string& str);

  /*!
    * @brief Combine the given strings to a single string. Inserts EPG_STRING_TOKEN_SEPARATOR as separator.
    * @param tokens The tokens.
    * @return the combined string.
    */
  static const std::string DeTokenize(const std::vector<std::string>& tokens);

  /*!
    * @brief Get the default provider of this media tag. The default
    *        provider represents the PVR add-on itself.
    * @return The default provider of this media tag
    */
  std::shared_ptr<CPVRProvider> GetDefaultProvider() const;

  /*!
    * @brief Whether or not this media tag has provider set by the client.
    * @return True if a provider was set by the client, false otherwise.
    */
  bool HasClientProvider() const;

  /*!
    * @brief Get the provider of this media tag. This may be the default provider or a
    *        custom provider set by the client. If @ref "HasClientProvider()" returns true
    *        the provider will be custom from the client, otherwise the default provider.
    * @return The provider of this media tag
    */
  std::shared_ptr<CPVRProvider> GetProvider() const;

private:
  void UpdatePath();

  CDateTime m_mediaTagTime; /*!< start time of the mediaTag */
  bool m_bGotMetaData;
  bool m_bIsDeleted; /*!< set if entry is a deleted mediaTag which can be undelete */
  int m_iClientProviderUniqueId; /*!< provider uid associated with this mediaTag on the client */
  bool m_bRadio; /*!< radio or tv mediaTag */
  int m_iGenreType = 0; /*!< genre type */
  int m_iGenreSubType = 0; /*!< genre subtype */
  int m_iEpisodePart = -1; /*!< episode part number */
  int m_iParentalRating; /*!< parental rating */
  int m_iStarRating; /*!< star rating */
  mutable XbmcThreads::EndTime m_resumePointRefetchTimeout;
  unsigned int m_iFlags = 0; /*!< the flags applicable to this mediaTag */
  mutable XbmcThreads::EndTime m_mediaTagSizeRefetchTimeout;
  int64_t m_sizeInBytes = 0; /*!< the size of the mediaTag in bytes */
  PVR_MEDIA_TAG_TYPE m_mediaType = PVR_MEDIA_TAG_TYPE_UNKNOWN;
  bool m_bDirty = false;

  mutable CCriticalSection m_critSection;
};
} // namespace PVR
