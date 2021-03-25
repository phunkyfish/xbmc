/*
 *  Copyright (C) 2012-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "PVRProviders.h"

#include "ServiceBroker.h"
#include "pvr/PVRDatabase.h"
#include "pvr/PVRManager.h"
#include "pvr/addons/PVRClient.h"
#include "pvr/addons/PVRClients.h"
#include "pvr/providers/PVRProvider.h"
#include "settings/Settings.h"
#include "threads/SingleLock.h"
#include "utils/log.h"

#include <memory>
#include <string>
#include <vector>

using namespace PVR;

bool CPVRProvidersContainer::UpdateFromClient(const std::shared_ptr<CPVRProvider>& provider)
{
  CSingleLock lock(m_critSection);
  const std::shared_ptr<CPVRProvider> providerToUpdate =
      GetByClient(provider->GetClientId(), provider->GetUniqueId());
  if (providerToUpdate)
  {
    return providerToUpdate->UpdateEntry(provider, ProviderUpdateMode::BY_CLIENT);
  }
  else
  {
    provider->SetDatabaseId(++m_iLastId);
    InsertEntry(provider, ProviderUpdateMode::BY_CLIENT);
  }

  return true;
}

std::shared_ptr<CPVRProvider> CPVRProvidersContainer::GetByClient(int iClientId,
                                                                  int iUniqueId) const
{
  CSingleLock lock(m_critSection);
  for (const auto& provider : m_providers)
  {
    if (provider->GetClientId() == iClientId && provider->GetUniqueId() == iUniqueId)
    {
      return provider;
    }
  }

  return {};
}

void CPVRProvidersContainer::InsertEntry(const std::shared_ptr<CPVRProvider>& newProvider,
                                         ProviderUpdateMode updateMode)
{
  bool found = false;
  for (auto& provider : m_providers)
  {
    if (provider->GetClientId() == newProvider->GetClientId() &&
        provider->GetUniqueId() == newProvider->GetUniqueId())
    {
      found = true;
      provider->UpdateEntry(newProvider, updateMode);
    }
  }

  if (!found)
  {
    m_providers.emplace_back(newProvider);
  }
}

std::vector<std::shared_ptr<CPVRProvider>> CPVRProvidersContainer::GetProvidersList() const
{
  CSingleLock lock(m_critSection);
  return m_providers;
}


bool CPVRProviders::Load()
{
  // unload previous providers
  Unload();

  // load providers from database
  bool bReturn = LoadFromDatabase();

  // update from clients
  Update();

  return bReturn;
}

void CPVRProviders::Unload()
{
  // remove all tags
  CSingleLock lock(m_critSection);
  m_providers.clear();
}

bool CPVRProviders::LoadFromDatabase()
{
  const std::shared_ptr<CPVRDatabase> database = CServiceBroker::GetPVRManager().GetTVDatabase();
  if (database)
  {
    bool bChanged = false;

    CPVRProviders providers;
    database->Get(providers);

    for (auto& provider : providers.GetProvidersList())
    {
      bChanged |= (CheckAndAddEntry(provider, ProviderUpdateMode::BY_DATABASE) != nullptr);
    }
  }
  return true;
}

bool CPVRProviders::Update()
{
  {
    CSingleLock lock(m_critSection);
    if (m_bIsUpdating)
      return false;
    m_bIsUpdating = true;
  }

  // Default client providers are the add-ons themselves, we retrieve both enabled
  // and disbled add-ons as we don't want them removed from the DB
  CPVRProviders newAddonProviderList;
  std::vector<int> disabledClients;
  std::vector<CVariant> clientProviderInfos =
      CServiceBroker::GetPVRManager().Clients()->GetClientProviderInfos();
  CLog::LogFC(LOGDEBUG, LOGPVR, "Adding default providers, found %d PVR add-ons",
              clientProviderInfos.size());
  for (const auto& clientInfo : clientProviderInfos)
  {
    auto addonProvider = std::make_shared<CPVRProvider>(
        clientInfo["clientid"].asInteger(), clientInfo["name"].asString(),
        clientInfo["icon"].asString(), clientInfo["thumb"].asString());

    newAddonProviderList.CheckAndAddEntry(addonProvider, ProviderUpdateMode::BY_CLIENT);

    if (!clientInfo["enabled"].asBoolean())
      disabledClients.emplace_back(clientInfo["clientid"].asInteger());
  }
  UpdateDefaultEntries(newAddonProviderList);

  // Client providers are retrieved from the clients
  CLog::LogFC(LOGDEBUG, LOGPVR, "Updating providers");
  CPVRProvidersContainer newProviderList;
  std::vector<int> failedClients;
  CServiceBroker::GetPVRManager().Clients()->GetProviders(&newProviderList, failedClients);
  return UpdateClientEntries(newProviderList, failedClients, disabledClients);
}

bool CPVRProviders::UpdateDefaultEntries(const CPVRProvidersContainer& newProviders)
{
  bool bChanged = false;

  CSingleLock lock(m_critSection);

  // go through the provider list and check for updated or new providers
  for (const auto& newProvider : newProviders.GetProvidersList())
  {
    bChanged |= (CheckAndPersistEntry(newProvider, ProviderUpdateMode::BY_CLIENT) != nullptr);
  }

  // check for deleted providers
  for (std::vector<std::shared_ptr<CPVRProvider>>::iterator it = m_providers.begin();
       it != m_providers.end();)
  {
    const std::shared_ptr<CPVRProvider> provider = *it;
    if (!newProviders.GetByClient(provider->GetClientId(), provider->GetUniqueId()))
    {
      // provider was not found
      bool bIgnoreProvider = false;

      // ignore add-on any providers that are no PVR Client addon providers
      if (!provider->IsClientProvider())
        bIgnoreProvider = true;

      if (bIgnoreProvider)
      {
        ++it;
        continue;
      }

      CLog::LogFC(LOGDEBUG, LOGPVR, "Deleted provider {} on client {}", provider->GetUniqueId(),
                  provider->GetClientId());

      (*it)->DeleteFromDatabase();
      it = m_providers.erase(it);

      bChanged |= true;
    }
    else
    {
      ++it;
    }
  }

  return bChanged;
}

bool CPVRProviders::UpdateClientEntries(const CPVRProvidersContainer& newProviders,
                                        const std::vector<int>& failedClients,
                                        const std::vector<int>& disabledClients)
{
  bool bChanged = false;

  CSingleLock lock(m_critSection);

  // go through the provider list and check for updated or new providers
  for (const auto& newProvider : newProviders.GetProvidersList())
  {
    bChanged |= (CheckAndPersistEntry(newProvider, ProviderUpdateMode::BY_CLIENT) != nullptr);
  }

  // check for deleted providers
  for (auto it = m_providers.begin(); it != m_providers.end();)
  {
    const std::shared_ptr<CPVRProvider> provider = *it;
    if (!newProviders.GetByClient(provider->GetClientId(), provider->GetUniqueId()))
    {
      // provider was not found
      bool bIgnoreProvider = false;
      for (const auto& failedClient : failedClients)
      {
        if (failedClient == provider->GetClientId())
        {
          bIgnoreProvider = true;
          break;
        }
      }

      for (const auto& disabledClient : disabledClients)
      {
        if (disabledClient == provider->GetClientId())
        {
          bIgnoreProvider = true;
          break;
        }
      }

      // ignore add-on providers as they are a special case
      if (provider->IsClientProvider())
        bIgnoreProvider = true;

      if (bIgnoreProvider)
      {
        ++it;
        continue;
      }

      CLog::LogFC(LOGDEBUG, LOGPVR, "Deleted provider {} on client {}", provider->GetUniqueId(),
                  provider->GetClientId());

      (*it)->DeleteFromDatabase();
      it = m_providers.erase(it);

      bChanged = true;
    }
    else
    {
      ++it;
    }
  }

  m_bIsUpdating = false;

  return bChanged;
}

std::shared_ptr<CPVRProvider> CPVRProviders::CheckAndAddEntry(
    const std::shared_ptr<CPVRProvider>& newProvider, ProviderUpdateMode updateMode)
{
  bool bChanged = false;

  CSingleLock lock(m_critSection);
  std::shared_ptr<CPVRProvider> provider =
      GetByClient(newProvider->GetClientId(), newProvider->GetUniqueId());
  if (provider)
  {
    bChanged = provider->UpdateEntry(newProvider, updateMode);
  }
  else
  {
    // We don't set an id as this came from the DB so it has one already
    InsertEntry(newProvider, updateMode);

    if (newProvider->GetDatabaseId() > m_iLastId)
      m_iLastId = newProvider->GetDatabaseId();

    provider = newProvider;
    bChanged = true;
  }

  if (bChanged)
    return provider;

  return {};
}

std::shared_ptr<CPVRProvider> CPVRProviders::CheckAndPersistEntry(
    const std::shared_ptr<CPVRProvider>& newProvider, ProviderUpdateMode updateMode)
{
  bool bChanged = false;

  CSingleLock lock(m_critSection);
  std::shared_ptr<CPVRProvider> provider =
      GetByClient(newProvider->GetClientId(), newProvider->GetUniqueId());
  if (provider)
  {
    bChanged = provider->UpdateEntry(newProvider, updateMode);

    if (bChanged)
      provider->Persist(true);

    CLog::LogFC(LOGDEBUG, LOGPVR, "Updated provider {} on client {}", newProvider->GetUniqueId(),
                newProvider->GetClientId());
  }
  else
  {
    newProvider->SetDatabaseId(++m_iLastId);
    InsertEntry(newProvider, updateMode);

    newProvider->Persist();

    CLog::LogFC(LOGDEBUG, LOGPVR, "Added provider {} on client {}", newProvider->GetUniqueId(),
                newProvider->GetClientId());

    provider = newProvider;
    bChanged = true;
  }

  if (bChanged)
    return provider;

  return {};
}

bool CPVRProviders::PersistUserChanges(const std::vector<std::shared_ptr<CPVRProvider>>& providers)
{
  for (auto provider : providers)
  {
    provider->Persist(true);

    CLog::LogFC(LOGDEBUG, LOGPVR, "Updated provider {} on client {}", provider->GetUniqueId(),
                provider->GetClientId());
  }

  return true;
}

std::shared_ptr<CPVRProvider> CPVRProviders::GetById(unsigned int iProviderId) const
{
  CSingleLock lock(m_critSection);
  for (const auto& provider : m_providers)
  {
    if (provider->GetDatabaseId() == iProviderId)
      return provider;
  }

  return {};
}

void CPVRProviders::RemoveEntry(const std::shared_ptr<CPVRProvider>& provider)
{
  CSingleLock lock(m_critSection);

  std::remove_if(m_providers.begin(), m_providers.end(),
                 [&provider](const std::shared_ptr<CPVRProvider>& providerToRemove) {
                   return provider->GetClientId() == providerToRemove->GetClientId() &&
                          provider->GetUniqueId() == providerToRemove->GetUniqueId();
                 });
}
