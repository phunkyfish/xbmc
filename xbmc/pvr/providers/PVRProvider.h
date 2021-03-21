/*
 *  Copyright (C) 2012-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "addons/kodi-dev-kit/include/kodi/c-api/addon-instance/pvr/pvr_providers.h"
#include "threads/CriticalSection.h"
#include "utils/ISerializable.h"

#include <memory>
#include <string>

struct PVR_TIMER;

namespace PVR
{
class CPVRChannel;
class CPVRClient;
class CPVREpgInfoTag;

enum class ProviderUpdateMode
{
  BY_CLIENT,
  BY_DATABASE
};

static constexpr int PVR_PROVIDER_ADDON_UID = -1;
static constexpr int PVR_PROVIDER_INVALID_DB_ID = -1;

class CPVRProvider final : public ISerializable
{
public:
  CPVRProvider(int iUniqueId, int iClientId);
  CPVRProvider(const PVR_PROVIDER& provider, int iClientId);
  CPVRProvider(int iClientId,
               const std::string& addonProviderName,
               const std::string& addonIconPath,
               const std::string& addonThumbPath);

  bool operator==(const CPVRProvider& right) const;
  bool operator!=(const CPVRProvider& right) const;

  void Serialize(CVariant& value) const override;

  /*!
   * @brief The database id of this provider
   *
   * A unique identifier for this provider.
   * It can be used to find the same provider on this clients backend
   *
   * @return The database id of this provider
   */
  int GetProviderId() const;

  /*!
   * @brief Set the database id of this provider
   * @param iProviderId The new ID.
   * @return True if the something changed, false otherwise.
   */
  bool SetProviderId(int iProviderId);

  /*!
   * @brief A unique identifier for this provider.
   *
   * A unique identifier for this provider.
   * It can be used to find the same provider on this clients backend
   *
   * @return The Unique ID.
   */
  int GetUniqueId() const;

  /*!
   * @return The identifier of the client that supplies this provider.
   */
  int GetClientId() const;

  /*!
   * @return The name of the provider. Can be user provided or the backend name
   */
  std::string GetProviderName() const;

  /*!
   * @brief Set the name of the provider.
   * @param providerName The new name of the provider.
   * @return True if the something changed, false otherwise.
   */
  bool SetProviderName(const std::string& iProviderName);

  /*!
   * @brief Checks whether this provider has a type other than
   *        PVR_PROVIDER_TYPE_UNKNOWN
   * @return True if this provider has a type other than unknown, false otherwise
   */
  bool HasProviderType() const { return m_providerType != PVR_PROVIDER_TYPE_UNKNOWN; }

  /*!
   * @brief Gets the type of this provider.
   * @return the type of this provider.
   */
  PVR_PROVIDER_TYPE GetProviderType() const;

  /*!
   * @brief Sets the type of this provider.
   * @param providerType the new provider type.
   * @return True if the something changed, false otherwise.
   */
  bool SetProviderType(PVR_PROVIDER_TYPE providerType);

  /*!
   * @brief Get the iconpath for this provider's icon
   * @return iconpath for this provider's icon
   */
  std::string GetIconPath() const;

  /*!
   * @brief Set the path for this icon
   * @param strIconPath The new path of the icon.
   * @return true if the icon path was updated successfully
   */
  bool SetIconPath(const std::string& strIconPath);

  /*!
   * @brief Get this provider's country
   * @return This provider's country
   */
  std::string GetCountry() const;

  /*!
   * @brief Set the country
   * @param strCountry The new country for this provider
   * @return true if the country was updated successfully
   */
  bool SetCountry(const std::string& strCountry);

  /*!
   * @brief Get this provider's language
   * @return This provider's language
   */
  std::string GetLanguage() const;

  /*!
   * @brief Set the language
   * @param strLanguage The new language for this provider
   * @return true if the language was updated successfully
   */
  bool SetLanguage(const std::string& strLanguage);

  /*!
   * @brief Get if this provider's has thumb image path. Note only PVR add-on providers.
   * @return True is this add-on provider has a thumb image path, false otherwise.
   */
  bool HasThumbPath() const;

  /*!
   * @brief Get this provider's thumb image path. Note only PVR add-on providers will set this value.
   * @return This add-on provider's thumb image path.
   */
  std::string GetThumbPath() const { return m_strThumbPath; }

  /*!
   * @brief Whether a provider is a default provider of a PVR Client add-on or not
   * @return True is this provider is of a PVR Client add-on, false otherwise.
   */
  bool IsClientProvider() const { return m_bIsClientProvider; }

  /*!
   * @brief updates this provider from the provided entry
   * @param fromProvider A provider containing the data that shall be merged into this provider's data.
   * @param updateMode update as User, Client or DB
   * @return true if the provider was updated successfully
   */
  bool UpdateEntry(const std::shared_ptr<CPVRProvider>& fromProvider,
                   ProviderUpdateMode updateMode);

  /*!
   * @brief Persist this provider in the local database.
   * @param updateRecord True if an existing record should be updated, false for an insert
   * @return True on success, false otherwise.
   */
  bool Persist(bool updateRecord = false);

  /*!
   * @brief Delete this provider from the local database.
   * @return True on success, false otherwise.
   */
  bool DeleteFromDatabase();

  mutable CCriticalSection m_critSection;

private:
  CPVRProvider(const CPVRProvider& provider) = delete;
  CPVRProvider& operator=(const CPVRProvider& orig) = delete;

  int m_iProviderId; /*!< the identifier given to this provider by the TV database */

  int m_iUniqueId; /*!< @brief unique ID of the provider on the backend */
  int m_iClientId; /*!< @brief ID of the backend */
  std::string m_strProviderName; /*!< @brief name of this provider */
  PVR_PROVIDER_TYPE m_providerType; /*!< @brief service type for this provider */
  std::string m_strIconPath; /*!< @brief the path to the icon for this provider */
  std::string m_strCountry; /*!< @brief the country for this provider (empty if undefined) */
  std::string m_strLanguage; /*!< @brief the language for this provider (empty if undefined) */
  bool m_bIsClientProvider; /*!< the provider is a default provider of a PVR Client add-on */
  std::string m_strThumbPath; /*!< a thumb image path for providers that are PVR add-ons */

  std::string m_strPath; /*!< the path for this provider */
};
} // namespace PVR
