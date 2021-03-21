/*
 *  Copyright (C) 2012-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "PVRProvider.h"

#include "ServiceBroker.h"
#include "guilib/LocalizeStrings.h"
#include "pvr/PVRDatabase.h"
#include "pvr/PVRManager.h"
#include "pvr/addons/PVRClient.h"
#include "pvr/addons/PVRClients.h"
#include "utils/Variant.h"
#include "utils/log.h"

#include <memory>
#include <string>

using namespace PVR;

CPVRProvider::CPVRProvider(int iUniqueId, int iClientId)
  : m_iProviderId(PVR_PROVIDER_INVALID_DB_ID),
    m_iUniqueId(iUniqueId),
    m_iClientId(iClientId),
    m_providerType(PVR_PROVIDER_TYPE_UNKNOWN),
    m_bIsClientProvider(false)
{
}

CPVRProvider::CPVRProvider(const PVR_PROVIDER& provider, int iClientId)
  : m_iProviderId(PVR_PROVIDER_INVALID_DB_ID),
    m_iUniqueId(provider.iUniqueId),
    m_iClientId(iClientId),
    m_strProviderName(provider.strProviderName),
    m_providerType(provider.providerType),
    m_strIconPath(provider.strIconPath),
    m_strCountry(provider.strCountry),
    m_strLanguage(provider.strLanguage),
    m_bIsClientProvider(false)
{
}

CPVRProvider::CPVRProvider(int iClientId,
                           const std::string& addonProviderName,
                           const std::string& addonIconPath,
                           const std::string& addonThumbPath)
  : m_iProviderId(PVR_PROVIDER_INVALID_DB_ID),
    m_iUniqueId(PVR_PROVIDER_ADDON_UID),
    m_iClientId(iClientId),
    m_strProviderName(addonProviderName),
    m_providerType(PVR_PROVIDER_TYPE_ADDON),
    m_strIconPath(addonIconPath),
    m_bIsClientProvider(true),
    m_strThumbPath(addonThumbPath)
{
}

bool CPVRProvider::operator==(const CPVRProvider& right) const
{
  return (m_iUniqueId == right.m_iUniqueId && m_iClientId == right.m_iClientId);
}

/**
 * Compare not equal for two CPVRProvider
 */
bool CPVRProvider::operator!=(const CPVRProvider& right) const
{
  return !(*this == right);
}

void CPVRProvider::Serialize(CVariant& value) const
{
  value["providerid"] = m_iProviderId;
  value["clientid"] = m_iClientId;
  value["providername"] = m_strProviderName;
  switch (m_providerType)
  {
    case PVR_PROVIDER_TYPE_ADDON:
      value["providertype"] = "addon";
      break;
    case PVR_PROVIDER_TYPE_SATELLITE:
      value["providertype"] = "satellite";
      break;
    case PVR_PROVIDER_TYPE_CABLE:
      value["providertype"] = "cable";
      break;
    case PVR_PROVIDER_TYPE_AERIAL:
      value["providertype"] = "aerial";
      break;
    case PVR_PROVIDER_TYPE_IPTV:
      value["providertype"] = "iptv";
      break;
    default:
      value["state"] = "unknown";
      break;
  }
  value["iconpath"] = m_strIconPath;
  value["country"] = m_strCountry;
  value["language"] = m_strLanguage;
}

int CPVRProvider::GetProviderId() const
{
  return m_iProviderId;
}

bool CPVRProvider::SetProviderId(int iProviderId)
{
  CSingleLock lock(m_critSection);

  if (m_iProviderId != iProviderId)
  {
    m_iProviderId = iProviderId;
    return true;
  }

  return false;
}

int CPVRProvider::GetUniqueId() const
{
  CSingleLock lock(m_critSection);
  return m_iUniqueId;
}

int CPVRProvider::GetClientId() const
{
  CSingleLock lock(m_critSection);
  return m_iClientId;
}

std::string CPVRProvider::GetProviderName() const
{
  CSingleLock lock(m_critSection);
  return m_strProviderName;
}

bool CPVRProvider::SetProviderName(const std::string& strProviderName)
{
  CSingleLock lock(m_critSection);
  if (m_strProviderName != strProviderName)
  {
    m_strProviderName = strProviderName;

    return true;
  }

  return false;
}

PVR_PROVIDER_TYPE CPVRProvider::GetProviderType() const
{
  CSingleLock lock(m_critSection);
  return m_providerType;
}

bool CPVRProvider::SetProviderType(PVR_PROVIDER_TYPE providerType)
{
  CSingleLock lock(m_critSection);
  if (m_providerType != providerType)
  {
    m_providerType = providerType;

    return true;
  }

  return false;
}

std::string CPVRProvider::GetIconPath() const
{
  CSingleLock lock(m_critSection);
  return m_strIconPath;
}

bool CPVRProvider::SetIconPath(const std::string& strIconPath)
{
  CSingleLock lock(m_critSection);
  if (m_strIconPath != strIconPath)
  {
    m_strIconPath = strIconPath;

    return true;
  }

  return false;
}

std::string CPVRProvider::GetCountry() const
{
  CSingleLock lock(m_critSection);
  return m_strCountry;
}

bool CPVRProvider::SetCountry(const std::string& strCountry)
{
  CSingleLock lock(m_critSection);
  if (m_strCountry != strCountry)
  {
    m_strCountry = strCountry;

    return true;
  }

  return false;
}

std::string CPVRProvider::GetLanguage() const
{
  CSingleLock lock(m_critSection);
  return m_strLanguage;
}

bool CPVRProvider::SetLanguage(const std::string& strLanguage)
{
  CSingleLock lock(m_critSection);
  if (m_strLanguage != strLanguage)
  {
    m_strLanguage = strLanguage;

    return true;
  }

  return false;
}

bool CPVRProvider::Persist(bool updateRecord /* = false */)
{
  CSingleLock lock(m_critSection);
  const std::shared_ptr<CPVRDatabase> database = CServiceBroker::GetPVRManager().GetTVDatabase();
  if (database)
    return database->Persist(*this, updateRecord);

  return false;
}

bool CPVRProvider::DeleteFromDatabase()
{
  CSingleLock lock(m_critSection);
  const std::shared_ptr<CPVRDatabase> database = CServiceBroker::GetPVRManager().GetTVDatabase();
  if (database)
    return database->Delete(*this);

  return false;
}

bool CPVRProvider::UpdateEntry(const std::shared_ptr<CPVRProvider>& fromProvider,
                               ProviderUpdateMode updateMode)
{
  bool m_bChanged = false;
  CSingleLock lock(m_critSection);

  if (updateMode == ProviderUpdateMode::BY_DATABASE)
  {
    m_iProviderId = fromProvider->m_iProviderId;

    m_strProviderName = fromProvider->m_strProviderName;
    m_providerType = fromProvider->m_providerType;
    m_strIconPath = fromProvider->m_strIconPath;

    if (fromProvider->m_bIsClientProvider)
    {
      m_strThumbPath = fromProvider->m_strThumbPath;
      m_bIsClientProvider = fromProvider->m_bIsClientProvider;
    }

    m_strCountry = fromProvider->m_strCountry;
    m_strLanguage = fromProvider->m_strLanguage;
  }
  else if (updateMode == ProviderUpdateMode::BY_CLIENT)
  {
    if (m_strProviderName != fromProvider->m_strProviderName)
    {
      m_strProviderName = fromProvider->m_strProviderName;
      m_bChanged = true;
    }

    if (m_providerType != fromProvider->m_providerType)
    {
      m_providerType = fromProvider->m_providerType;
      m_bChanged = true;
    }

    if (m_strIconPath != fromProvider->m_strIconPath)
    {
      m_strIconPath = fromProvider->m_strIconPath;
      m_bChanged = true;
    }

    if (fromProvider->m_bIsClientProvider)
    {
      m_strThumbPath = fromProvider->m_strThumbPath;
      m_bIsClientProvider = fromProvider->m_bIsClientProvider;
    }

    if (m_strCountry != fromProvider->m_strCountry)
    {
      m_strCountry = fromProvider->m_strCountry;
      m_bChanged = true;
    }

    if (m_strLanguage != fromProvider->m_strLanguage)
    {
      m_strLanguage = fromProvider->m_strLanguage;
      m_bChanged = true;
    }
  }

  return m_bChanged;
}

bool CPVRProvider::HasThumbPath() const
{
  return (m_providerType == PVR_PROVIDER_TYPE_ADDON && !m_strThumbPath.empty());
}