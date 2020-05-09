/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "Network.h"

#include "ServiceBroker.h"
#include "URL.h"
#include "addons/binary-addons/AddonDll.h"
#include "addons/kodi-addon-dev-kit/include/kodi/Network.h"
#include "filesystem/CurlFile.h"
#include "network/DNSNameCache.h"
#include "network/Network.h"
#include "utils/HttpHeader.h"
#include "utils/URIUtils.h"
#include "utils/log.h"

namespace ADDON
{

void Interface_Network::Init(AddonGlobalInterface *addonInterface)
{
  addonInterface->toKodi->kodi_network = new AddonToKodiFuncTable_kodi_network();

  addonInterface->toKodi->kodi_network->wake_on_lan = wake_on_lan;
  addonInterface->toKodi->kodi_network->get_ip_address = get_ip_address;
  addonInterface->toKodi->kodi_network->get_hostname = get_hostname;
  addonInterface->toKodi->kodi_network->get_http_header = get_http_header;
  addonInterface->toKodi->kodi_network->get_mime_type = get_mime_type;
  addonInterface->toKodi->kodi_network->get_content_type = get_content_type;
  addonInterface->toKodi->kodi_network->get_cookies = get_cookies;
  addonInterface->toKodi->kodi_network->is_local_host = is_local_host;
  addonInterface->toKodi->kodi_network->is_host_on_lan = is_host_on_lan;
  addonInterface->toKodi->kodi_network->dns_lookup = dns_lookup;
  addonInterface->toKodi->kodi_network->url_encode = url_encode;
  addonInterface->toKodi->kodi_network->http_header_create = http_header_create;
  addonInterface->toKodi->kodi_network->http_header_free = http_header_free;
}

void Interface_Network::DeInit(AddonGlobalInterface* addonInterface)
{
  if (addonInterface->toKodi) /* <-- needed as long as the old addon way is used */
  {
    delete addonInterface->toKodi->kodi_network;
    addonInterface->toKodi->kodi_network = nullptr;
  }
}

bool Interface_Network::wake_on_lan(void* kodiBase, const char* mac)
{
  CAddonDll* addon = static_cast<CAddonDll*>(kodiBase);
  if (addon == nullptr || mac == nullptr)
  {
    CLog::Log(LOGERROR, "Interface_Network::{} - invalid data (addon='{}', mac='{}')", __FUNCTION__,
              kodiBase, static_cast<const void*>(mac));
    return false;
  }

  return CServiceBroker::GetNetwork().WakeOnLan(mac);
}

char* Interface_Network::get_ip_address(void* kodiBase)
{
  CAddonDll* addon = static_cast<CAddonDll*>(kodiBase);
  if (addon == nullptr)
  {
    CLog::Log(LOGERROR, "Interface_Network::{} - invalid data (addon='{}')", __FUNCTION__,
              kodiBase);
    return nullptr;
  }

  std::string titleIP;
  CNetworkInterface* iface = CServiceBroker::GetNetwork().GetFirstConnectedInterface();
  if (iface)
    titleIP = iface->GetCurrentIPAddress();
  else
    titleIP = "127.0.0.1";

  char* buffer = nullptr;
  if (!titleIP.empty())
    buffer = strdup(titleIP.c_str());
  return buffer;
}

char* Interface_Network::get_hostname(void* kodiBase)
{
  CAddonDll* addon = static_cast<CAddonDll*>(kodiBase);
  if (addon == nullptr)
  {
    CLog::Log(LOGERROR, "Interface_Network::{} - invalid data (addon='{}')", __FUNCTION__,
              kodiBase);
    return nullptr;
  }

  std::string hostname;
  if (!CServiceBroker::GetNetwork().GetHostName(hostname))
    return nullptr;

  char* buffer = nullptr;
  if (!hostname.empty())
    buffer = strdup(hostname.c_str());
  return buffer;
}

bool Interface_Network::get_mime_type(void* kodiBase,
                                      const char* url,
                                      char** content,
                                      const char* useragent)
{
  CAddonDll* addon = static_cast<CAddonDll*>(kodiBase);
  if (addon == nullptr || url == nullptr || content == nullptr || useragent == nullptr)
  {
    CLog::Log(
        LOGERROR,
        "Interface_Network::{} - invalid data (addon='{}', url='{}', content='{}', useragent='{}')",
        __FUNCTION__, kodiBase, static_cast<const void*>(url), static_cast<const void*>(content),
        static_cast<const void*>(useragent));
    return false;
  }

  std::string kodiContent;
  bool ret = XFILE::CCurlFile::GetMimeType(CURL(url), kodiContent, useragent);
  if (ret && !kodiContent.empty())
  {
    *content = strdup(kodiContent.c_str());
  }
  return ret;
}

bool Interface_Network::get_content_type(void* kodiBase,
                                         const char* url,
                                         char** content,
                                         const char* useragent)
{
  CAddonDll* addon = static_cast<CAddonDll*>(kodiBase);
  if (addon == nullptr || url == nullptr || content == nullptr || useragent == nullptr)
  {
    CLog::Log(
        LOGERROR,
        "Interface_Network::{} - invalid data (addon='{}', url='{}', content='{}', useragent='{}')",
        __FUNCTION__, kodiBase, static_cast<const void*>(url), static_cast<const void*>(content),
        static_cast<const void*>(useragent));
    return false;
  }

  std::string kodiContent;
  bool ret = XFILE::CCurlFile::GetContentType(CURL(url), kodiContent, useragent);
  if (ret && !kodiContent.empty())
  {
    *content = strdup(kodiContent.c_str());
  }
  return ret;
}

bool Interface_Network::get_cookies(void* kodiBase, const char* url, char** cookies)
{
  CAddonDll* addon = static_cast<CAddonDll*>(kodiBase);
  if (addon == nullptr || url == nullptr || cookies == nullptr)
  {
    CLog::Log(LOGERROR, "Interface_Network::{} - invalid data (addon='{}', url='{}', cookies='{}')",
              __FUNCTION__, kodiBase, static_cast<const void*>(url),
              static_cast<const void*>(cookies));
    return false;
  }

  std::string kodiCookies;
  bool ret = XFILE::CCurlFile::GetCookies(CURL(url), kodiCookies);
  if (ret && !kodiCookies.empty())
  {
    *cookies = strdup(kodiCookies.c_str());
  }
  return ret;
}

bool Interface_Network::is_local_host(void* kodiBase, const char* hostname)
{
  CAddonDll* addon = static_cast<CAddonDll*>(kodiBase);
  if (addon == nullptr || hostname == nullptr)
  {
    CLog::Log(LOGERROR, "Interface_Network::{} - invalid data (addon='{}', hostname='{}')",
              __FUNCTION__, kodiBase, static_cast<const void*>(hostname));
    return false;
  }

  return CServiceBroker::GetNetwork().IsLocalHost(hostname);
}

bool Interface_Network::is_host_on_lan(void* kodiBase, const char* hostname, bool offLineCheck)
{
  CAddonDll* addon = static_cast<CAddonDll*>(kodiBase);
  if (addon == nullptr || hostname == nullptr)
  {
    CLog::Log(LOGERROR, "Interface_Network::{} - invalid data (addon='{}', hostname='{}')",
              __FUNCTION__, kodiBase, static_cast<const void*>(hostname));
    return false;
  }

  return URIUtils::IsHostOnLAN(hostname, offLineCheck);
}

char* Interface_Network::dns_lookup(void* kodiBase, const char* url, bool* ret)
{
  CAddonDll* addon = static_cast<CAddonDll*>(kodiBase);
  if (addon == nullptr || url == nullptr || ret == nullptr)
  {
    CLog::Log(LOGERROR, "Interface_Network::{} - invalid data (addon='{}', url='{}', ret='{}')",
              __FUNCTION__, kodiBase, static_cast<const void*>(url), static_cast<void*>(ret));
    return nullptr;
  }

  std::string string;
  *ret = CDNSNameCache::Lookup(url, string);
  char* buffer = nullptr;
  if (!string.empty())
    buffer = strdup(string.c_str());
  return buffer;
}

char* Interface_Network::url_encode(void* kodiBase, const char* url)
{
  CAddonDll* addon = static_cast<CAddonDll*>(kodiBase);
  if (addon == nullptr || url == nullptr)
  {
    CLog::Log(LOGERROR, "Interface_Network::{} - invalid data (addon='{}', url='{}')", __FUNCTION__,
              kodiBase, static_cast<const void*>(url));
    return nullptr;
  }

  std::string string = CURL::Encode(url);
  char* buffer = nullptr;
  if (!string.empty())
    buffer = strdup(string.c_str());
  return buffer;
}

bool Interface_Network::get_http_header(void* kodiBase,
                                        const char* url,
                                        struct KODI_HTTP_HEADER* headers)
{
  CAddonDll* addon = static_cast<CAddonDll*>(kodiBase);
  if (addon == nullptr || url == nullptr || headers == nullptr || headers->handle == nullptr)
  {
    CLog::Log(LOGERROR,
              "Interface_Network::{} - invalid data (addon='{}', url='{}', headers='{}', "
              "headers->handle='{}')",
              __FUNCTION__, kodiBase, static_cast<const void*>(url),
              static_cast<const void*>(headers), headers->handle);
    return false;
  }

  CHttpHeader* httpHeader = static_cast<CHttpHeader*>(headers->handle);
  return XFILE::CCurlFile::GetHttpHeader(CURL(url), *httpHeader);
}

bool Interface_Network::http_header_create(void* kodiBase, struct KODI_HTTP_HEADER* headers)
{
  CAddonDll* addon = static_cast<CAddonDll*>(kodiBase);
  if (addon == nullptr || headers == nullptr)
  {
    CLog::Log(LOGERROR, "Interface_Network::{} - invalid data (addon='{}', headers='{}')",
              __FUNCTION__, kodiBase, static_cast<const void*>(headers));
    return false;
  }

  headers->handle = new CHttpHeader;
  headers->parse = http_header_parse;
  headers->add_param = http_header_add_param;
  headers->get_value = http_header_get_value;
  headers->get_values = http_header_get_values;
  headers->get_header = http_header_get_header;
  headers->get_mime_type = http_header_get_mime_type;
  headers->get_charset = http_header_get_charset;
  headers->get_proto_line = http_header_get_proto_line;
  headers->is_header_done = http_header_is_header_done;
  headers->clear = http_header_clear;

  return true;
}

void Interface_Network::http_header_free(void* kodiBase, struct KODI_HTTP_HEADER* headers)
{
  CAddonDll* addon = static_cast<CAddonDll*>(kodiBase);
  if (addon == nullptr || headers == nullptr)
  {
    CLog::Log(LOGERROR, "Interface_Network::{} - invalid data (addon='{}', headers='{}')",
              __FUNCTION__, kodiBase, static_cast<const void*>(headers));
    return;
  }

  delete static_cast<CHttpHeader*>(headers->handle);
  headers->handle = nullptr;
}

void Interface_Network::http_header_parse(void* kodiBase, void* handle, const char* data)
{
  CAddonDll* addon = static_cast<CAddonDll*>(kodiBase);
  if (addon == nullptr || handle == nullptr || data == nullptr)
  {
    CLog::Log(LOGERROR, "Interface_Network::{} - invalid data (addon='{}', handle='{}', data='{}')",
              __FUNCTION__, kodiBase, handle, static_cast<const void*>(data));
    return;
  }

  static_cast<CHttpHeader*>(handle)->Parse(data);
}

void Interface_Network::http_header_add_param(
    void* kodiBase, void* handle, const char* param, const char* value, bool overwrite)
{
  CAddonDll* addon = static_cast<CAddonDll*>(kodiBase);
  if (addon == nullptr || handle == nullptr || param == nullptr || value == nullptr)
  {
    CLog::Log(
        LOGERROR,
        "Interface_Network::{} - invalid data (addon='{}', handle='{}', param='{}', value='{}')",
        __FUNCTION__, kodiBase, handle, static_cast<const void*>(param),
        static_cast<const void*>(value));
    return;
  }

  static_cast<CHttpHeader*>(handle)->AddParam(param, value);
}

char* Interface_Network::http_header_get_value(void* kodiBase, void* handle, const char* param)
{
  CAddonDll* addon = static_cast<CAddonDll*>(kodiBase);
  if (addon == nullptr || handle == nullptr || param == nullptr)
  {
    CLog::Log(LOGERROR,
              "Interface_Network::{} - invalid data (addon='{}', handle='{}', param='{}')",
              __FUNCTION__, kodiBase, handle, static_cast<const void*>(param));
    return nullptr;
  }

  std::string string = static_cast<CHttpHeader*>(handle)->GetValue(param);

  char* buffer = nullptr;
  if (!string.empty())
    buffer = strdup(string.c_str());
  return buffer;
}

char** Interface_Network::http_header_get_values(void* kodiBase,
                                                 void* handle,
                                                 const char* param,
                                                 int* length)
{
  CAddonDll* addon = static_cast<CAddonDll*>(kodiBase);
  if (addon == nullptr || handle == nullptr || param == nullptr || length == nullptr)
  {
    CLog::Log(
        LOGERROR,
        "Interface_Network::{} - invalid data (addon='{}', handle='{}', param='{}', length='{}')",
        __FUNCTION__, kodiBase, handle, static_cast<const void*>(param),
        static_cast<const void*>(length));
    return nullptr;
  }


  std::vector<std::string> values = static_cast<CHttpHeader*>(handle)->GetValues(param);
  *length = values.size();
  char** ret = static_cast<char**>(malloc(sizeof(char*) * values.size()));
  for (int i = 0; i < *length; ++i)
  {
    ret[i] = strdup(values[i].c_str());
  }
  return ret;
}

char* Interface_Network::http_header_get_header(void* kodiBase, void* handle)
{
  CAddonDll* addon = static_cast<CAddonDll*>(kodiBase);
  if (addon == nullptr || handle == nullptr)
  {
    CLog::Log(LOGERROR, "Interface_Network::{} - invalid data (addon='{}', handle='{}')",
              __FUNCTION__, kodiBase, handle);
    return nullptr;
  }

  std::string string = static_cast<CHttpHeader*>(handle)->GetHeader();

  char* buffer = nullptr;
  if (!string.empty())
    buffer = strdup(string.c_str());
  return buffer;
}

char* Interface_Network::http_header_get_mime_type(void* kodiBase, void* handle)
{
  CAddonDll* addon = static_cast<CAddonDll*>(kodiBase);
  if (addon == nullptr || handle == nullptr)
  {
    CLog::Log(LOGERROR, "Interface_Network::{} - invalid data (addon='{}', handle='{}')",
              __FUNCTION__, kodiBase, handle);
    return nullptr;
  }

  std::string string = static_cast<CHttpHeader*>(handle)->GetMimeType();

  char* buffer = nullptr;
  if (!string.empty())
    buffer = strdup(string.c_str());
  return buffer;
}

char* Interface_Network::http_header_get_charset(void* kodiBase, void* handle)
{
  CAddonDll* addon = static_cast<CAddonDll*>(kodiBase);
  if (addon == nullptr || handle == nullptr)
  {
    CLog::Log(LOGERROR, "Interface_Network::{} - invalid data (addon='{}', handle='{}')",
              __FUNCTION__, kodiBase, handle);
    return nullptr;
  }

  std::string string = static_cast<CHttpHeader*>(handle)->GetCharset();

  char* buffer = nullptr;
  if (!string.empty())
    buffer = strdup(string.c_str());
  return buffer;
}

char* Interface_Network::http_header_get_proto_line(void* kodiBase, void* handle)
{
  CAddonDll* addon = static_cast<CAddonDll*>(kodiBase);
  if (addon == nullptr || handle == nullptr)
  {
    CLog::Log(LOGERROR, "Interface_Network::{} - invalid data (addon='{}', handle='{}')",
              __FUNCTION__, kodiBase, handle);
    return nullptr;
  }

  std::string string = static_cast<CHttpHeader*>(handle)->GetProtoLine();

  char* buffer = nullptr;
  if (!string.empty())
    buffer = strdup(string.c_str());
  return buffer;
}

bool Interface_Network::http_header_is_header_done(void* kodiBase, void* handle)
{
  CAddonDll* addon = static_cast<CAddonDll*>(kodiBase);
  if (addon == nullptr || handle == nullptr)
  {
    CLog::Log(LOGERROR, "Interface_Network::{} - invalid data (addon='{}', handle='{}')",
              __FUNCTION__, kodiBase, handle);
    return false;
  }

  return static_cast<CHttpHeader*>(handle)->IsHeaderDone();
}

void Interface_Network::http_header_clear(void* kodiBase, void* handle)
{
  CAddonDll* addon = static_cast<CAddonDll*>(kodiBase);
  if (addon == nullptr || handle == nullptr)
  {
    CLog::Log(LOGERROR, "Interface_Network::{} - invalid data (addon='{}', handle='{}')",
              __FUNCTION__, kodiBase, handle);
    return;
  }

  static_cast<CHttpHeader*>(handle)->Clear();
}

} /* namespace ADDON */
