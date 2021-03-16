/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "../../AddonBase.h"
#include "../../c-api/addon-instance/pvr.h"

//¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
// "C++" Definitions group 5 - PVR media
#ifdef __cplusplus

namespace kodi
{
namespace addon
{

//==============================================================================
/// @defgroup cpp_kodi_addon_pvr_Defs_MediaTag_PVRMediaTag class PVRMediaTag
/// @ingroup cpp_kodi_addon_pvr_Defs_MediaTag
/// @brief **Data structure with available media data**\n
/// With this, media related data are transferred between addon and Kodi
/// and can also be used by the addon itself.
///
/// The related values here are automatically initiated to defaults and need
/// only be set if supported and used.
///
/// ----------------------------------------------------------------------------
///
/// @copydetails cpp_kodi_addon_pvr_Defs_MediaTag_PVRMediaTag_Help
///
///@{
class PVRMediaTag : public CStructHdl<PVRMediaTag, PVR_MEDIA_TAG>
{
  friend class CInstancePVRClient;

public:
  /*! \cond PRIVATE */
  PVRMediaTag()
  {
    m_cStructure->iSeriesNumber = PVR_MEDIA_TAG_INVALID_SERIES_EPISODE;
    m_cStructure->iEpisodeNumber = PVR_MEDIA_TAG_INVALID_SERIES_EPISODE;
    m_cStructure->iEpisodePartNumber = PVR_MEDIA_TAG_INVALID_SERIES_EPISODE;
    m_cStructure->mediaTagTime = 0;
    m_cStructure->iDuration = PVR_MEDIA_TAG_VALUE_NOT_AVAILABLE;
    m_cStructure->iGenreType = PVR_MEDIA_TAG_VALUE_NOT_AVAILABLE;
    m_cStructure->iGenreSubType = PVR_MEDIA_TAG_VALUE_NOT_AVAILABLE;
    m_cStructure->iPlayCount = PVR_MEDIA_TAG_VALUE_NOT_AVAILABLE;
    m_cStructure->iLastPlayedPosition = PVR_MEDIA_TAG_VALUE_NOT_AVAILABLE;
    m_cStructure->iParentalRating = 0;
    m_cStructure->iStarRating = 0;
    m_cStructure->bIsDeleted = false;
    m_cStructure->iClientProviderUid = PVR_PROVIDER_INVALID_UID;
    m_cStructure->mediaClass = PVR_MEDIA_TAG_CLASS_UNKNOWN;
    m_cStructure->mediaType = PVR_MEDIA_TAG_TYPE_UNKNOWN;
    m_cStructure->iFlags = 0;
    m_cStructure->sizeInBytes = PVR_MEDIA_TAG_VALUE_NOT_AVAILABLE;
  }
  PVRMediaTag(const PVRMediaTag& mediaTag) : CStructHdl(mediaTag) {}
  /*! \endcond */

  /// @defgroup cpp_kodi_addon_pvr_Defs_MediaTag_PVRMediaTag_Help Value Help
  /// @ingroup cpp_kodi_addon_pvr_Defs_MediaTag_PVRMediaTag
  ///
  /// <b>The following table contains values that can be set with @ref cpp_kodi_addon_pvr_Defs_MediaTag_PVRMediaTag :</b>
  /// | Name | Type | Set call | Get call | Usage
  /// |------|------|----------|----------|-----------
  /// | **MediaTag id** | `std::string` | @ref PVRMediaTag::SetMediaTagId "SetMediaTagId" | @ref PVRMediaTag::GetMediaTagId "GetMediaTagId" | *required to set*
  /// | **Title** | `std::string` | @ref PVRMediaTag::SetTitle "SetTitle" | @ref PVRMediaTag::GetTitle "GetTitle" | *required to set*
  /// | **Episode name** | `std::string` | @ref PVRMediaTag::SetEpisodeName "SetEpisodeName" | @ref PVRMediaTag::GetEpisodeName "GetEpisodeName" | *optional*
  /// | **Series number** | `int` | @ref PVRMediaTag::SetSeriesNumber "SetSeriesNumber" | @ref PVRMediaTag::GetSeriesNumber "GetSeriesNumber" | *optional*
  /// | **Episode number** | `int` | @ref PVRMediaTag::SetEpisodeNumber "SetEpisodeNumber" | @ref PVRMediaTag::GetEpisodeNumber "GetEpisodeNumber" | *optional*
  /// | **Episode part number** | `int` | @ref PVRMediaTag::SetEpisodePartNumber "SetEpisodePartNumber" | @ref PVRMediaTag::GetEpisodePArtNumber "GetEpisodePartNumber" | *optional*
  /// | **Year** | `int` | @ref PVRMediaTag::SetYear "SetYear" | @ref PVRMediaTag::GetYear "GetYear" | *optional*
  /// | **Directory** | `std::string` | @ref PVRMediaTag::SetDirectory "SetDirectory" | @ref PVRMediaTag::GetDirectory "GetDirectory" | *optional*
  /// | **Plot outline** | `std::string` | @ref PVRMediaTag::SetPlotOutline "SetPlotOutline" | @ref PVRMediaTag::GetPlotOutline "GetPlotOutline" | *optional*
  /// | **Plot** | `std::string` | @ref PVRMediaTag::SetPlot "SetPlot" | @ref PVRMediaTag::GetPlot "GetPlot" | *optional*
  /// | **Original title** | `std::string` | @ref PVRMediaTag::SetOriginalTitle "SetOriginalTitle" | @ref PVRMediaTag::GetOriginalTitle "GetOriginalTitle" | *optional*
  /// | **Cast** | `std::string` | @ref PVRMediaTag::SetCast "SetCast" | @ref PVRMediaTag::GetCast "GetCast" | *optional*
  /// | **Director** | `std::string` | @ref PVRMediaTag::SetDirector "SetDirector" | @ref PVRMediaTag::GetDirector "GetDirector" | *optional*
  /// | **Writer** | `std::string` | @ref PVRMediaTag::SetWriter "SetWriter" | @ref PVRMediaTag::GetWriter "GetWriter" | *optional*
  /// | **IMDB number** | `std::string` | @ref PVRMediaTag::SetIMDBNumber "SetIMDBNumber" | @ref PVRMediaTag::GetIMDBNumber "GetIMDBNumber" | *optional*
  /// | **Series link** | `std::string` | @ref PVRMediaTag::SetSeriesLink "SetSeriesLink" | @ref PVRMediaTag::GetSeriesLink "GetSeriesLink" | *optional*
  /// | **Parental rating code** | `int` | @ref PVRMediaTag::SetParentalRatingCode "SetParentalRatingCode" | @ref PVRMediaTag::GetParentalRatingCode "GetParentalRatingCode" | *optional*
  /// | **Parental rating** | `int` | @ref PVRMediaTag::SetParentalRating "SetParentalRating" | @ref PVRMediaTag::GetParentalRating "GetParentalRating" | *optional*
  /// | **Star rating** | `int` | @ref PVRMediaTag::SetStarRating "SetStarRating" | @ref PVRMediaTag::GetStarRating "GetStarRating" | *optional*
  /// | **Genre description** | `std::string` | @ref PVRMediaTag::SetGenreDescription "SetGenreDescription" | @ref PVRMediaTag::GetGenreDescription "GetGenreDescription" | *optional*
  /// | **Provider name** | `std::string` | @ref PVRMediaTag::SetProviderName "SetProviderName" | @ref PVRMediaTag::GetProviderName "GetProviderName" | *optional*
  /// | **Icon path** | `std::string` | @ref PVRMediaTag::SetIconPath "SetIconPath" | @ref PVRMediaTag::GetIconPath "GetIconPath" | *optional*
  /// | **Thumbnail path** | `std::string` | @ref PVRMediaTag::SetThumbnailPath "SetThumbnailPath" | @ref PVRMediaTag::GetThumbnailPath "GetThumbnailPath" | *optional*
  /// | **Fanart path** | `std::string` | @ref PVRMediaTag::SetFanartPath "SetFanartPath" | @ref PVRMediaTag::GetFanartPath "GetFanartPath" | *optional*
  /// | **MediaTag time** | `time_t` | @ref PVRMediaTag::SetMediaTagTime "SetMediaTagTime" | @ref PVRMediaTag::GetMediaTagTime "GetMediaTagTime" | *optional*
  /// | **Duration** | `int` | @ref PVRMediaTag::SetDuration "SetDuration" | @ref PVRMediaTag::GetDuration "GetDuration" | *optional*
  /// | **Genre type** | `int` | @ref PVRMediaTag::SetGenreType "SetGenreType" | @ref PVRMediaTag::GetGenreType "GetGenreType" | *optional*
  /// | **Genre sub type** | `int` | @ref PVRMediaTag::SetGenreSubType "SetGenreSubType" | @ref PVRMediaTag::GetGenreSubType "GetGenreSubType" | *optional*
  /// | **Play count** | `int` | @ref PVRMediaTag::SetPlayCount "SetPlayCount" | @ref PVRMediaTag::GetPlayCount "GetPlayCount" | *optional*
  /// | **Last played position** | `int` | @ref PVRMediaTag::SetLastPlayedPosition "SetLastPlayedPosition" | @ref PVRMediaTag::GetLastPlayedPosition "GetLastPlayedPosition" | *optional*
  /// | **Is deleted** | `bool` | @ref PVRMediaTag::SetIsDeleted "SetIsDeleted" | @ref PVRMediaTag::GetIsDeleted "GetIsDeleted" | *optional*
  /// | **Provider unique id** | `int` | @ref PVRMediaTag::SetProviderUniqueId "SetProviderUniqueId" | @ref PVRMediaTag::GetProviderUniqueId "GetProviderUniqueId" | *optional*
  /// | **Media class** | @ref PVR_MEDIA_TAG_CLASS | @ref PVRMediaTag::SetMediaClass "SetMediaClass" | @ref PVRMediaTag::GetMediaClass "GetMediaClass" | *optional*
  /// | **Media type** | @ref PVR_MEDIA_TAG_TYPE | @ref PVRMediaTag::SetMediaType "SetMediaType" | @ref PVRMediaTag::GetMediaType "GetMediaType" | *optional*
  /// | **First aired** | `std::string` | @ref PVRMediaTag::SetFirstAired "SetFirstAired" | @ref PVRMediaTag::GetFirstAired "GetFirstAired" | *optional*
  /// | **Flags** | `std::string` | @ref PVRMediaTag::SetFlags "SetFlags" | @ref PVRMediaTag::GetFlags "GetFlags" | *optional*
  /// | **Size in bytes** | `std::string` | @ref PVRMediaTag::SetSizeInBytes "SetSizeInBytes" | @ref PVRMediaTag::GetSizeInBytes "GetSizeInBytes" | *optional*

  /// @addtogroup cpp_kodi_addon_pvr_Defs_MediaTag_PVRMediaTag
  ///@{

  /// @brief **required**\n
  /// Unique identifier of the mediaTag on the client.
  void SetMediaTagId(const std::string& mediaTagId)
  {
    strncpy(m_cStructure->strMediaTagId, mediaTagId.c_str(),
            sizeof(m_cStructure->strMediaTagId) - 1);
  }

  /// @brief To get with @ref SetMediaTagId changed values.
  std::string GetMediaTagId() const { return m_cStructure->strMediaTagId; }

  /// @brief **required**\n
  /// The title of this mediaTag.
  void SetTitle(const std::string& title)
  {
    strncpy(m_cStructure->strTitle, title.c_str(), sizeof(m_cStructure->strTitle) - 1);
  }

  /// @brief To get with @ref SetTitle changed values.
  std::string GetTitle() const { return m_cStructure->strTitle; }

  /// @brief **optional**\n
  /// Episode name (also known as subtitle).
  void SetEpisodeName(const std::string& episodeName)
  {
    strncpy(m_cStructure->strEpisodeName, episodeName.c_str(),
            sizeof(m_cStructure->strEpisodeName) - 1);
  }

  /// @brief To get with @ref SetEpisodeName changed values.
  std::string GetEpisodeName() const { return m_cStructure->strEpisodeName; }

  /// @brief **optional**\n
  /// Series number (usually called season).
  ///
  /// Set to "0" for specials/pilot. For 'invalid' see @ref SetEpisodeNumber or set to -1.
  void SetSeriesNumber(int seriesNumber) { m_cStructure->iSeriesNumber = seriesNumber; }

  /// @brief To get with @ref SetSeriesNumber changed values.
  int GetSeriesNumber() const { return m_cStructure->iSeriesNumber; }

  /// @brief **optional**\n
  /// Eepisode number within the "iSeriesNumber" season.
  ///
  /// For 'invalid' set to -1 or seriesNumber=episodeNumber=0 to show both are invalid.
  void SetEpisodeNumber(int episodeNumber) { m_cStructure->iEpisodeNumber = episodeNumber; }

  /// @brief To get with @ref SetEpisodeNumber changed values.
  int GetEpisodeNumber() const { return m_cStructure->iEpisodeNumber; }

  /// @brief **optional**\n
  /// Episode part number within the "iSeriesNumber" season.
  ///
  /// For 'invalid' set to -1 or seriesNumber=episodeNumber=episodeNumber==0 to show all are invalid.
  void SetEpisodePartNumber(int episodePartNumber)
  {
    m_cStructure->iEpisodePartNumber = episodePartNumber;
  }

  /// @brief To get with @ref SetEpisodePartNumber changed values.
  int GetEpisodePartNumber() const { return m_cStructure->iEpisodePartNumber; }

  /// @brief **optional**\n
  /// Year of first release (use to identify a specific movie re-make) / first
  /// airing for TV shows.
  ///
  /// Set to '0' for invalid.
  void SetYear(int year) { m_cStructure->iYear = year; }

  /// @brief To get with @ref SetYear changed values.
  int GetYear() const { return m_cStructure->iYear; }

  /// @brief **optional**\n
  ///
  /// Directory of this mediaTag on the client.
  void SetDirectory(const std::string& directory)
  {
    strncpy(m_cStructure->strDirectory, directory.c_str(), sizeof(m_cStructure->strDirectory) - 1);
  }

  /// @brief To get with @ref SetDirectory changed values.
  std::string GetDirectory() const { return m_cStructure->strDirectory; }

  /// @brief **optional**\n
  /// Plot outline name.
  void SetPlotOutline(const std::string& plotOutline)
  {
    strncpy(m_cStructure->strPlotOutline, plotOutline.c_str(),
            sizeof(m_cStructure->strPlotOutline) - 1);
  }

  /// @brief To get with @ref SetPlotOutline changed values.
  std::string GetPlotOutline() const { return m_cStructure->strPlotOutline; }

  /// @brief **optional**\n
  /// Plot name.
  void SetPlot(const std::string& plot)
  {
    strncpy(m_cStructure->strPlot, plot.c_str(), sizeof(m_cStructure->strPlot) - 1);
  }

  /// @brief To get with @ref SetPlot changed values.
  std::string GetPlot() const { return m_cStructure->strPlot; }

  /// @brief **optional**\n
  /// Original title.
  void SetOriginalTitle(const std::string& strOriginalTitle)
  {
    strncpy(m_cStructure->strOriginalTitle, strOriginalTitle.c_str(),
            sizeof(m_cStructure->strOriginalTitle) - 1);
  }

  /// @brief To get with @ref SetOriginalTitle changed values
  std::string GetOriginalTitle() const { return m_cStructure->strOriginalTitle; }

  /// @brief **optional**\n
  /// Cast name(s).
  ///
  /// @note Use @ref EPG_STRING_TOKEN_SEPARATOR to separate different persons.
  void SetCast(const std::string& strCast)
  {
    strncpy(m_cStructure->strCast, strCast.c_str(), sizeof(m_cStructure->strCast) - 1);
  }

  /// @brief To get with @ref SetCast changed values
  std::string GetCast() const { return m_cStructure->strCast; }

  /// @brief **optional**\n
  /// Director name(s).
  ///
  /// @note Use @ref EPG_STRING_TOKEN_SEPARATOR to separate different persons.
  void SetDirector(const std::string& strDirector)
  {
    strncpy(m_cStructure->strDirector, strDirector.c_str(), sizeof(m_cStructure->strDirector) - 1);
  }

  /// @brief To get with @ref SetDirector changed values.
  std::string GetDirector() const { return m_cStructure->strDirector; }

  /// @brief **optional**\n
  /// Writer name(s).
  ///
  /// @note Use @ref EPG_STRING_TOKEN_SEPARATOR to separate different persons.
  void SetWriter(const std::string& strWriter)
  {
    strncpy(m_cStructure->strWriter, strWriter.c_str(), sizeof(m_cStructure->strWriter) - 1);
  }

  /// @brief To get with @ref SetDirector changed values
  std::string GetWriter() const { return m_cStructure->strWriter; }

  /// @brief **optional**\n
  /// [IMDB](https://en.wikipedia.org/wiki/IMDb) identification number.
  void SetIMDBNumber(const std::string& strIMDBNumber)
  {
    strncpy(m_cStructure->strIMDBNumber, strIMDBNumber.c_str(),
            sizeof(m_cStructure->strIMDBNumber) - 1);
  }

  /// @brief To get with @ref SetIMDBNumber changed values.
  std::string GetIMDBNumber() const { return m_cStructure->strIMDBNumber; }

  /// @brief **optional**\n
  /// Series link for this media tag.
  void SetSeriesLink(const std::string& strSeriesLink)
  {
    strncpy(m_cStructure->strSeriesLink, strSeriesLink.c_str(),
            sizeof(m_cStructure->strSeriesLink) - 1);
  }

  /// @brief To get with @ref SetSeriesLink changed values.
  std::string GetSeriesLink() const { return m_cStructure->strSeriesLink; }

  /// @brief **required**\n
  /// This media tag's parental rating code.
  void SetParentalRatingCode(const std::string& strParentalRatingCode)
  {
    strncpy(m_cStructure->strParentalRatingCode, strParentalRatingCode.c_str(),
            sizeof(m_cStructure->strParentalRatingCode) - 1);
  }

  /// @brief To get with @ref SetParentalRatingCode changed values.
  std::string GetParentalRatingCode() const { return m_cStructure->strParentalRatingCode; }

  /// @brief **optional**\n
  /// Parental rating.
  void SetParentalRating(int parentalRating) { m_cStructure->iParentalRating = parentalRating; }

  /// @brief To get with @ref SetParentalRatinge changed values.
  int GetParentalRating() const { return m_cStructure->iParentalRating; }

  /// @brief **optional**\n
  /// Star rating.
  void SetStarRating(int starRating) { m_cStructure->iStarRating = starRating; }

  /// @brief To get with @ref SetStarRating changed values.
  int GetStarRating() const { return m_cStructure->iStarRating; }

  /// @brief **optional**\n
  /// Channel name.
  void SetProviderName(const std::string& providerName)
  {
    strncpy(m_cStructure->strProviderName, providerName.c_str(),
            sizeof(m_cStructure->strProviderName) - 1);
  }

  /// @brief To get with @ref SetProviderName changed values.
  std::string GetProviderName() const { return m_cStructure->strProviderName; }

  /// @brief **optional**\n
  /// Channel logo (icon) path.
  void SetIconPath(const std::string& iconPath)
  {
    strncpy(m_cStructure->strIconPath, iconPath.c_str(), sizeof(m_cStructure->strIconPath) - 1);
  }

  /// @brief To get with @ref SetIconPath changed values.
  std::string GetIconPath() const { return m_cStructure->strIconPath; }

  /// @brief **optional**\n
  /// Thumbnail path.
  void SetThumbnailPath(const std::string& thumbnailPath)
  {
    strncpy(m_cStructure->strThumbnailPath, thumbnailPath.c_str(),
            sizeof(m_cStructure->strThumbnailPath) - 1);
  }

  /// @brief To get with @ref SetThumbnailPath changed values.
  std::string GetThumbnailPath() const { return m_cStructure->strThumbnailPath; }

  /// @brief **optional**\n
  /// Fanart path.
  void SetFanartPath(const std::string& fanartPath)
  {
    strncpy(m_cStructure->strFanartPath, fanartPath.c_str(),
            sizeof(m_cStructure->strFanartPath) - 1);
  }

  /// @brief To get with @ref SetFanartPath changed values.
  std::string GetFanartPath() const { return m_cStructure->strFanartPath; }

  /// @brief **optional**\n
  /// Start time of the mediaTag.
  void SetMediaTagTime(time_t mediaTagTime) { m_cStructure->mediaTagTime = mediaTagTime; }

  /// @brief To get with @ref SetMediaTagTime changed values.
  time_t GetMediaTagTime() const { return m_cStructure->mediaTagTime; }

  /// @brief **optional**\n
  /// Duration of the mediaTag in seconds.
  void SetDuration(int duration) { m_cStructure->iDuration = duration; }

  /// @brief To get with @ref SetDuration changed values.
  int GetDuration() const { return m_cStructure->iDuration; }

  /// @brief **optional**\n
  /// Genre type.
  ///
  /// Use @ref EPG_GENRE_USE_STRING if type becomes given by @ref SetGenreDescription.
  ///
  /// @note If confirmed that backend brings the types in [ETSI EN 300 468](https://www.etsi.org/deliver/etsi_en/300400_300499/300468/01.14.01_60/en_300468v011401p.pdf)
  /// conform values, can be @ref EPG_EVENT_CONTENTMASK ignored and to set here
  /// with backend value.
  ///
  ///
  /// --------------------------------------------------------------------------
  ///
  /// **Example 1:**
  /// ~~~~~~~~~~~~~{.cpp}
  /// kodi::addon::PVRMediaTag tag;
  /// tag.SetGenreType(EPG_EVENT_CONTENTMASK_MOVIEDRAMA);
  /// ~~~~~~~~~~~~~
  ///
  /// --------------------------------------------------------------------------
  ///
  /// **Example 2** (in case of other, not ETSI EN 300 468 conform genre types):
  /// ~~~~~~~~~~~~~{.cpp}
  /// kodi::addon::PVRMediaTag tag;
  /// tag.SetGenreType(EPG_GENRE_USE_STRING);
  /// tag.SetGenreDescription("My special genre name"); // Should use (if possible) kodi::GetLocalizedString(...) to have match user language.
  /// ~~~~~~~~~~~~~
  ///
  void SetGenreType(int genreType) { m_cStructure->iGenreType = genreType; }

  /// @brief To get with @ref SetGenreType changed values.
  int GetGenreType() const { return m_cStructure->iGenreType; }

  /// @brief **optional**\n
  /// Genre sub type.
  ///
  /// Subtypes groups related to set by @ref SetGenreType:
  /// | Main genre type | List with available sub genre types
  /// |-----------------|-----------------------------------------
  /// | @ref EPG_EVENT_CONTENTMASK_UNDEFINED | Nothing, should be 0
  /// | @ref EPG_EVENT_CONTENTMASK_MOVIEDRAMA | @ref EPG_EVENT_CONTENTSUBMASK_MOVIEDRAMA
  /// | @ref EPG_EVENT_CONTENTMASK_NEWSCURRENTAFFAIRS | @ref EPG_EVENT_CONTENTSUBMASK_NEWSCURRENTAFFAIRS
  /// | @ref EPG_EVENT_CONTENTMASK_SHOW | @ref EPG_EVENT_CONTENTSUBMASK_SHOW
  /// | @ref EPG_EVENT_CONTENTMASK_SPORTS | @ref EPG_EVENT_CONTENTSUBMASK_SPORTS
  /// | @ref EPG_EVENT_CONTENTMASK_CHILDRENYOUTH | @ref EPG_EVENT_CONTENTSUBMASK_CHILDRENYOUTH
  /// | @ref EPG_EVENT_CONTENTMASK_MUSICBALLETDANCE | @ref EPG_EVENT_CONTENTSUBMASK_MUSICBALLETDANCE
  /// | @ref EPG_EVENT_CONTENTMASK_ARTSCULTURE | @ref EPG_EVENT_CONTENTSUBMASK_ARTSCULTURE
  /// | @ref EPG_EVENT_CONTENTMASK_SOCIALPOLITICALECONOMICS | @ref EPG_EVENT_CONTENTSUBMASK_SOCIALPOLITICALECONOMICS
  /// | @ref EPG_EVENT_CONTENTMASK_EDUCATIONALSCIENCE | @ref EPG_EVENT_CONTENTSUBMASK_EDUCATIONALSCIENCE
  /// | @ref EPG_EVENT_CONTENTMASK_LEISUREHOBBIES | @ref EPG_EVENT_CONTENTSUBMASK_LEISUREHOBBIES
  /// | @ref EPG_EVENT_CONTENTMASK_SPECIAL | @ref EPG_EVENT_CONTENTSUBMASK_SPECIAL
  /// | @ref EPG_EVENT_CONTENTMASK_USERDEFINED | Can be defined by you
  /// | @ref EPG_GENRE_USE_STRING | **Kodi's own value**, which declares that the type with @ref SetGenreDescription is given.
  ///
  /// --------------------------------------------------------------------------
  ///
  /// **Example:**
  /// ~~~~~~~~~~~~~{.cpp}
  /// kodi::addon::PVRMediaTag tag;
  /// tag.SetGenreType(EPG_EVENT_CONTENTMASK_MUSICBALLETDANCE);
  /// tag.SetGenreSubType(EPG_EVENT_CONTENTSUBMASK_MUSICBALLETDANCE_JAZZ);
  /// ~~~~~~~~~~~~~
  ///
  void SetGenreSubType(int genreSubType) { m_cStructure->iGenreSubType = genreSubType; }

  /// @brief To get with @ref SetGenreSubType changed values.
  int GetGenreSubType() const { return m_cStructure->iGenreSubType; }

  /// @brief **optional**\n
  /// To set own genre description name.
  ///
  /// Will be used only when genreType == @ref EPG_GENRE_USE_STRING or
  /// genreSubType == @ref EPG_GENRE_USE_STRING.
  ///
  /// Use @ref EPG_STRING_TOKEN_SEPARATOR to separate different genres.
  ///
  /// In case of other, not [ETSI EN 300 468](https://www.etsi.org/deliver/etsi_en/300400_300499/300468/01.14.01_60/en_300468v011401p.pdf)
  /// conform genre types or something special.
  ///
  /// --------------------------------------------------------------------------
  ///
  /// **Example:**
  /// ~~~~~~~~~~~~~{.cpp}
  /// kodi::addon::PVRMediaTag tag;
  /// tag.SetGenreType(EPG_GENRE_USE_STRING);
  /// tag.SetGenreDescription("Action" + EPG_STRING_TOKEN_SEPARATOR + "Thriller");
  /// ~~~~~~~~~~~~~
  ///
  void SetGenreDescription(const std::string& genreDescription)
  {
    strncpy(m_cStructure->strGenreDescription, genreDescription.c_str(),
            sizeof(m_cStructure->strGenreDescription) - 1);
  }

  /// @brief To get with @ref SetGenreDescription changed values.
  std::string GetGenreDescription() const { return m_cStructure->strGenreDescription; }

  /// @brief **optional**\n
  /// Play count of this mediaTag on the client.
  void SetPlayCount(int playCount) { m_cStructure->iPlayCount = playCount; }

  /// @brief To get with @ref SetPlayCount changed values.
  int GetPlayCount() const { return m_cStructure->iPlayCount; }

  /// @brief **optional**\n
  /// Last played position of this mediaTag on the client.
  void SetLastPlayedPosition(int lastPlayedPosition)
  {
    m_cStructure->iLastPlayedPosition = lastPlayedPosition;
  }

  /// @brief To get with @ref SetLastPlayedPosition changed values.
  int GetLastPlayedPosition() const { return m_cStructure->iLastPlayedPosition; }

  /// @brief **optional**\n
  /// Shows this mediaTag is deleted and can be undelete.
  void SetIsDeleted(int isDeleted) { m_cStructure->bIsDeleted = isDeleted; }

  /// @brief To get with @ref SetIsDeleted changed values.
  int GetIsDeleted() const { return m_cStructure->bIsDeleted; }

  /// @brief **optional**\n
  /// Unique identifier of the channel for this mediaTag. @ref PVR_CHANNEL_INVALID_UID
  /// denotes that channel uid is not available.
  void SetProviderUniqueId(int providerUniqueId) { m_cStructure->iClientProviderUid = providerUniqueId; }

  /// @brief To get with @ref SetProviderUniqueId changed values
  int GetProviderUniqueId() const { return m_cStructure->iClientProviderUid; }

  /// @brief **optional**\n
  /// Media class.
  ///
  /// Set to @ref PVR_MEDIA_TAG_CLASS_UNKNOWN if the class cannot be
  /// determined.
  ///
  /// --------------------------------------------------------------------------
  ///
  /// Example:
  /// ~~~~~~~~~~~~~{.cpp}
  /// kodi::addon::PVRMediaTag tag;
  /// tag.SetMediaClass(PVR_MEDIA_TAG_CLASS_VIDEO);
  /// ~~~~~~~~~~~~~
  ///
  void SetMediaClass(PVR_MEDIA_TAG_CLASS mediaClass) { m_cStructure->mediaClass = mediaClass; }

  /// @brief To get with @ref SetMediaClass changed values
  PVR_MEDIA_TAG_CLASS GetMediaClass() const { return m_cStructure->mediaClass; }

  /// @brief **optional**\n
  /// Media type.
  ///
  /// Set to @ref PVR_MEDIA_TAG_TYPE_UNKNOWN if the type cannot be
  /// determined. Each media belongs to a media class.
  ///
  /// --------------------------------------------------------------------------
  ///
  /// Example:
  /// ~~~~~~~~~~~~~{.cpp}
  /// kodi::addon::PVRMediaTag tag;
  /// tag.SetMediaType(PVR_MEDIA_TAG_TYPE_VIDEO);
  /// ~~~~~~~~~~~~~
  ///
  void SetMediaType(PVR_MEDIA_TAG_TYPE mediaType) { m_cStructure->mediaType = mediaType; }

  /// @brief To get with @ref SetMediaType changed values
  PVR_MEDIA_TAG_TYPE GetMediaType() const { return m_cStructure->mediaType; }

  /// @brief **optional**\n
  /// First aired date of this mediaTag.
  ///
  /// Used only for display purposes. Specify in W3C date format "YYYY-MM-DD".
  ///
  /// --------------------------------------------------------------------------
  ///
  /// Example:
  /// ~~~~~~~~~~~~~{.cpp}
  /// kodi::addon::PVRMediaTag tag;
  /// tag.SetFirstAired(1982-10-22);
  /// ~~~~~~~~~~~~~
  ///
  void SetFirstAired(const std::string& firstAired)
  {
    strncpy(m_cStructure->strFirstAired, firstAired.c_str(),
            sizeof(m_cStructure->strFirstAired) - 1);
  }

  /// @brief To get with @ref SetFirstAired changed values
  std::string GetFirstAired() const { return m_cStructure->strFirstAired; }

  /// @brief **optional**\n
  /// Bit field of independent flags associated with the mediaTag.
  ///
  /// See @ref cpp_kodi_addon_pvr_Defs_MediaTag_PVR_MEDIA_TAG_FLAG for
  /// available bit flags.
  ///
  /// --------------------------------------------------------------------------
  ///
  /// @copydetails cpp_kodi_addon_pvr_Defs_MediaTag_PVR_MEDIA_TAG_FLAG
  ///
  void SetFlags(unsigned int flags) { m_cStructure->iFlags = flags; }

  /// @brief To get with @ref SetFlags changed values.
  unsigned int GetFlags() const { return m_cStructure->iFlags; }

  /// @brief **optional**\n
  /// Size of the mediaTag in bytes.
  void SetSizeInBytes(int64_t sizeInBytes) { m_cStructure->sizeInBytes = sizeInBytes; }

  /// @brief To get with @ref SetSizeInBytes changed values.
  int64_t GetSizeInBytes() const { return m_cStructure->sizeInBytes; }
  ///@}

private:
  PVRMediaTag(const PVR_MEDIA_TAG* mediaTag) : CStructHdl(mediaTag) {}
  PVRMediaTag(PVR_MEDIA_TAG* mediaTag) : CStructHdl(mediaTag) {}
};
///@}
//------------------------------------------------------------------------------

//==============================================================================
/// @defgroup cpp_kodi_addon_pvr_Defs_MediaTag_PVRMediaResultSet class PVRMediaResultSet
/// @ingroup cpp_kodi_addon_pvr_Defs_MediaTag_PVRMediaTag
/// @brief **PVR add-on mediaTag transfer class**\n
/// To transfer the content of @ref kodi::addon::CInstancePVRClient::GetMedia().
///
/// @note This becomes only be used on addon call above, not usable outside on
/// addon itself.
///@{
class PVRMediaResultSet
{
public:
  /*! \cond PRIVATE */
  PVRMediaResultSet() = delete;
  PVRMediaResultSet(const AddonInstance_PVR* instance, ADDON_HANDLE handle)
    : m_instance(instance), m_handle(handle)
  {
  }
  /*! \endcond */

  /// @addtogroup cpp_kodi_addon_pvr_Defs_MediaTag_PVRMediaResultSet
  ///@{

  /// @brief To add and give content from addon to Kodi on related call.
  ///
  /// @param[in] tag The to transferred data.
  void Add(const kodi::addon::PVRMediaTag& tag)
  {
    m_instance->toKodi->TransferMediaTagEntry(m_instance->toKodi->kodiInstance, m_handle, tag);
  }

  ///@}

private:
  const AddonInstance_PVR* m_instance = nullptr;
  const ADDON_HANDLE m_handle;
};
///@}
//------------------------------------------------------------------------------

} /* namespace addon */
} /* namespace kodi */

#endif /* __cplusplus */
