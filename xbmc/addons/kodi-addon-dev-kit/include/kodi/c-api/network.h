/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

  typedef struct KODI_HTTP_HEADER
  {
    void* handle;

    void (*parse)(void* kodiBase, void* handle, const char* data);
    void (*add_param)(
        void* kodiBase, void* handle, const char* param, const char* value, bool overwrite);
    char* (*get_value)(void* kodiBase, void* handle, const char* param);
    char** (*get_values)(void* kodiBase, void* handle, const char* param, int* length);
    char* (*get_header)(void* kodiBase, void* handle);
    char* (*get_mime_type)(void* kodiBase, void* handle);
    char* (*get_charset)(void* kodiBase, void* handle);
    char* (*get_proto_line)(void* kodiBase, void* handle);
    bool (*is_header_done)(void* kodiBase, void* handle);
    void (*clear)(void* kodiBase, void* handle);
  } KODI_HTTP_HEADER;

  /*
   * For interface between add-on and kodi.
   *
   * This structure defines the addresses of functions stored inside Kodi which
   * are then available for the add-on to call
   *
   * All function pointers there are used by the C++ interface functions below.
   * You find the set of them on xbmc/addons/interfaces/General.cpp
   *
   * Note: For add-on development itself this is not needed
   */
  typedef struct AddonToKodiFuncTable_kodi_network
  {
    bool (*wake_on_lan)(void* kodiBase, const char* mac);
    char* (*get_ip_address)(void* kodiBase);
    char* (*dns_lookup)(void* kodiBase, const char* url, bool* ret);
    char* (*url_encode)(void* kodiBase, const char* url);
    char* (*get_hostname)(void* kodiBase);
    bool (*is_local_host)(void* kodiBase, const char* hostname);
    bool (*is_host_on_lan)(void* kodiBase, const char* hostname, bool offLineCheck);
    bool (*get_http_header)(void* kodiBase, const char* url, KODI_HTTP_HEADER* headers);
    bool (*get_mime_type)(void* kodiBase, const char* url, char** content, const char* useragent);
    bool (*get_content_type)(void* kodiBase,
                             const char* url,
                             char** content,
                             const char* useragent);
    bool (*get_cookies)(void* kodiBase, const char* url, char** cookies);
    bool (*http_header_create)(void* kodiBase, KODI_HTTP_HEADER* headers);
    void (*http_header_free)(void* kodiBase, KODI_HTTP_HEADER* headers);
  } AddonToKodiFuncTable_kodi_network;

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */
