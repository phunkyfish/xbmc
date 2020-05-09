/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "addons/kodi-addon-dev-kit/include/kodi/c-api/network.h"

struct AddonGlobalInterface;

extern "C"
{
namespace ADDON
{

/*!
 * @brief Global general Add-on to Kodi callback functions
 *
 * To hold network functions not related to a instance type and usable for
 * every add-on type.
 *
 * Related add-on header is "./xbmc/addons/kodi-addon-dev-kit/include/kodi/Network.h"
 */
struct Interface_Network
{
  static void Init(AddonGlobalInterface* addonInterface);
  static void DeInit(AddonGlobalInterface* addonInterface);

  /*!
   * @brief callback functions from add-on to kodi
   *
   * @note To add a new function use the "_" style to directly identify an
   * add-on callback function. Everything with CamelCase is only to be used
   * in Kodi.
   *
   * The parameter `kodiBase` is used to become the pointer for a `CAddonDll`
   * class.
   */
  //@{
  static bool wake_on_lan(void* kodiBase, const char* mac);
  static char* get_ip_address(void* kodiBase);
  static char* get_hostname(void* kodiBase);
  static bool get_http_header(void* kodiBase, const char* url, struct KODI_HTTP_HEADER* headers);
  static bool get_mime_type(void* kodiBase, const char* url, char** content, const char* useragent);
  static bool get_content_type(void* kodiBase,
                               const char* url,
                               char** content,
                               const char* useragent);
  static bool get_cookies(void* kodiBase, const char* url, char** cookies);
  static bool is_local_host(void* kodiBase, const char* hostname);
  static bool is_host_on_lan(void* kodiBase, const char* hostname, bool offLineCheck);
  static char* dns_lookup(void* kodiBase, const char* url, bool* ret);
  static char* url_encode(void* kodiBase, const char* url);

  static bool http_header_create(void* kodiBase, struct KODI_HTTP_HEADER* headers);
  static void http_header_free(void* kodiBase, struct KODI_HTTP_HEADER* headers);
  static void http_header_parse(void* kodiBase, void* handle, const char* data);
  static void http_header_add_param(
      void* kodiBase, void* handle, const char* param, const char* value, bool overwrite);
  static char* http_header_get_value(void* kodiBase, void* handle, const char* param);
  static char** http_header_get_values(void* kodiBase,
                                       void* handle,
                                       const char* param,
                                       int* length);
  static char* http_header_get_header(void* kodiBase, void* handle);
  static char* http_header_get_mime_type(void* kodiBase, void* handle);
  static char* http_header_get_charset(void* kodiBase, void* handle);
  static char* http_header_get_proto_line(void* kodiBase, void* handle);
  static bool http_header_is_header_done(void* kodiBase, void* handle);
  static void http_header_clear(void* kodiBase, void* handle);
  //@}
};

} /* namespace ADDON */
} /* extern "C" */
