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
#include "utils/StringUtils.h"
#include "utils/Variant.h"
#include "utils/log.h"

#include <memory>
#include <string>

using namespace PVR;

CPVRProvider::CPVRProvider(int iUniqueId, int iClientId)
  : m_iUniqueId(iUniqueId),
    m_iClientId(iClientId)
{
}

CPVRProvider::CPVRProvider(const PVR_PROVIDER& provider, int iClientId)
  : m_iUniqueId(provider.iUniqueId),
    m_iClientId(iClientId),
    m_strProviderName(provider.strProviderName),
    m_providerType(provider.providerType),
    m_strIconPath(provider.strIconPath),
    m_strCountries(provider.strCountry),
    m_strLanguages(provider.strLanguage)
{
}

CPVRProvider::CPVRProvider(int iClientId,
                           const std::string& addonProviderName,
                           const std::string& addonIconPath,
                           const std::string& addonThumbPath)
  : m_iClientId(iClientId),
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

bool CPVRProvider::operator!=(const CPVRProvider& right) const
{
  return !(*this == right);
}

void CPVRProvider::Serialize(CVariant& value) const
{
  value["providerid"] = m_iDatabaseId;
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
    case PVR_PROVIDER_TYPE_OTHER:
      value["providertype"] = "other";
      break;
    default:
      value["state"] = "unknown";
      break;
  }
  value["iconpath"] = m_strIconPath;
  value["countries"] = m_strCountries;
  value["languages"] = m_strLanguages;
}

int CPVRProvider::GetDatabaseId() const
{
  CSingleLock lock(m_critSection);
  return m_iDatabaseId;
}

bool CPVRProvider::SetDatabaseId(int iDatabaseId)
{
  CSingleLock lock(m_critSection);

  if (m_iDatabaseId != iDatabaseId)
  {
    m_iDatabaseId = iDatabaseId;
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

namespace
{

const std::vector<std::string> Tokenize(const std::string& str)
{
  return StringUtils::Split(str.c_str(), PROVIDER_STRING_TOKEN_SEPARATOR);
}

const std::string DeTokenize(const std::vector<std::string>& tokens)
{
  return StringUtils::Join(tokens, PROVIDER_STRING_TOKEN_SEPARATOR);
}

} // unamed namespace

std::vector<std::string> CPVRProvider::GetCountries() const
{
  CSingleLock lock(m_critSection);

  return Tokenize(m_strCountries);
}

bool CPVRProvider::SetCountries(const std::vector<std::string>& countries)
{
  CSingleLock lock(m_critSection);
  const std::string strCountries = DeTokenize(countries);
  if (m_strCountries != strCountries)
  {
    m_strCountries = strCountries;
    return true;
  }

  return false;
}

std::string CPVRProvider::GetCountriesDBString() const
{
  CSingleLock lock(m_critSection);
  return m_strCountries;
}

bool CPVRProvider::SetCountriesFromDBString(const std::string& strCountries)
{
  CSingleLock lock(m_critSection);
  if (m_strCountries != strCountries)
  {
    m_strCountries = strCountries;
    return true;
  }

  return false;
}

std::vector<std::string> CPVRProvider::GetLanguages() const
{
  CSingleLock lock(m_critSection);
  return Tokenize(m_strLanguages);
}

bool CPVRProvider::SetLanguages(const std::vector<std::string>& languages)
{
  CSingleLock lock(m_critSection);
  const std::string strLanguages = DeTokenize(languages);
  if (m_strLanguages != strLanguages)
  {
    m_strLanguages = strLanguages;
    return true;
  }

  return false;
}

std::string CPVRProvider::GetLanguagesDBString() const
{
  CSingleLock lock(m_critSection);
  return m_strLanguages;
}

bool CPVRProvider::SetLanguagesFromDBString(const std::string& strLanguages)
{
  CSingleLock lock(m_critSection);
  if (m_strLanguages != strLanguages)
  {
    m_strLanguages = strLanguages;
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
    m_iDatabaseId = fromProvider->m_iDatabaseId;

    m_strProviderName = fromProvider->m_strProviderName;
    m_providerType = fromProvider->m_providerType;
    m_strIconPath = fromProvider->m_strIconPath;

    if (fromProvider->m_bIsClientProvider)
    {
      m_strThumbPath = fromProvider->m_strThumbPath;
      m_bIsClientProvider = fromProvider->m_bIsClientProvider;
    }

    m_strCountries = fromProvider->m_strCountries;
    m_strLanguages = fromProvider->m_strLanguages;
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

    if (m_strCountries != fromProvider->m_strCountries)
    {
      m_strCountries = fromProvider->m_strCountries;
      m_bChanged = true;
    }

    if (m_strLanguages != fromProvider->m_strLanguages)
    {
      m_strLanguages = fromProvider->m_strLanguages;
      m_bChanged = true;
    }
  }

  return m_bChanged;
}

bool CPVRProvider::HasThumbPath() const
{
  return (m_providerType == PVR_PROVIDER_TYPE_ADDON && !m_strThumbPath.empty());
}
