/*
 *  Copyright (C) 2005-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "AddonBase.h"
#include "c-api/network.h"

#ifdef __cplusplus

//==============================================================================
/// @defgroup cpp_kodi_network  Interface - kodi::network
/// @ingroup cpp
/// @brief **Network functions**\n
/// The network module offers functions that allow you to control it.
///
/// It has the header @ref Network.h "#include <kodi/Network.h>" be included
/// to enjoy it.
///
//------------------------------------------------------------------------------

//==============================================================================
/// @defgroup cpp_kodi_network_Defs Definitions, structures and enumerators
/// @ingroup cpp_kodi_network
/// @brief **Network definition values**\n
/// All to network associated data structures.
///
//------------------------------------------------------------------------------

namespace kodi
{
namespace network
{

//¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
// "C++" related filesystem definitions
//{{{

//==============================================================================
/// @defgroup cpp_kodi_network_HttpHeader class HttpHeader
/// @ingroup cpp_kodi_vfs_Defs
/// @brief
///
//@{
class HttpHeader
{
public:
  //==========================================================================
  /// @brief Http header parser class constructor
  ///
  HttpHeader()
  {
    using namespace ::kodi::addon;

    CAddonBase::m_interface->toKodi->kodi_network->http_header_create(
        CAddonBase::m_interface->toKodi->kodiBase, &m_handle);
  }
  //--------------------------------------------------------------------------

  //==========================================================================
  /// @brief Destructor
  ///
  ~HttpHeader()
  {
    using namespace ::kodi::addon;

    CAddonBase::m_interface->toKodi->kodi_network->http_header_free(
        CAddonBase::m_interface->toKodi->kodiBase, &m_handle);
  }
  //--------------------------------------------------------------------------

  //==========================================================================
  /// @brief
  ///
  /// @param[in] data
  ///
  void Parse(const std::string& data)
  {
    using namespace ::kodi::addon;

    if (!m_handle.handle)
      return;

    return m_handle.parse(CAddonBase::m_interface->toKodi->kodiBase, m_handle.handle, data.c_str());
  }
  //--------------------------------------------------------------------------

  //==========================================================================
  /// @brief
  ///
  /// @param[in] param
  /// @param[in] value
  /// @param[in] overwrite [opt]
  ///
  void AddParam(const std::string& param, const std::string& value, const bool overwrite = false)
  {
    using namespace ::kodi::addon;

    if (!m_handle.handle)
      return;

    return m_handle.add_param(CAddonBase::m_interface->toKodi->kodiBase, m_handle.handle,
                              param.c_str(), value.c_str(), overwrite);
  }
  //--------------------------------------------------------------------------

  //==========================================================================
  /// @brief
  ///
  /// @param[in] param
  /// @return
  ///
  std::string GetValue(const std::string& param) const
  {
    using namespace ::kodi::addon;

    if (!m_handle.handle)
      return "";

    std::string protoLine;
    char* string = m_handle.get_value(CAddonBase::m_interface->toKodi->kodiBase, m_handle.handle,
                                      param.c_str());
    if (string != nullptr)
    {
      protoLine = string;
      CAddonBase::m_interface->toKodi->free_string(CAddonBase::m_interface->toKodi->kodiBase,
                                                   string);
    }
    return protoLine;
  }
  //--------------------------------------------------------------------------

  //==========================================================================
  /// @brief
  ///
  /// @param[in] param
  /// @return
  ///
  std::vector<std::string> GetValues(const std::string& param) const
  {
    using namespace kodi::addon;

    if (!m_handle.handle)
      return std::vector<std::string>();

    int numValues;
    char** res(m_handle.get_values(CAddonBase::m_interface->toKodi->kodiBase, m_handle.handle,
                                   param.c_str(), &numValues));
    if (res)
    {
      std::vector<std::string> vecReturn;
      for (int i = 0; i < numValues; ++i)
      {
        vecReturn.emplace_back(res[i]);
      }
      CAddonBase::m_interface->toKodi->free_string_array(CAddonBase::m_interface->toKodi->kodiBase,
                                                         res, numValues);
      return vecReturn;
    }
    return std::vector<std::string>();
  }
  //--------------------------------------------------------------------------

  //==========================================================================
  /// @brief
  ///
  /// @return
  ///
  std::string GetHeader() const
  {
    using namespace ::kodi::addon;

    if (!m_handle.handle)
      return "";

    std::string protoLine;
    char* string = m_handle.get_header(CAddonBase::m_interface->toKodi->kodiBase, m_handle.handle);
    if (string != nullptr)
    {
      protoLine = string;
      CAddonBase::m_interface->toKodi->free_string(CAddonBase::m_interface->toKodi->kodiBase,
                                                   string);
    }
    return protoLine;
  }
  //--------------------------------------------------------------------------

  //==========================================================================
  /// @brief
  ///
  /// @return
  ///
  std::string GetMimeType() const
  {
    using namespace ::kodi::addon;

    if (!m_handle.handle)
      return "";

    std::string protoLine;
    char* string =
        m_handle.get_mime_type(CAddonBase::m_interface->toKodi->kodiBase, m_handle.handle);
    if (string != nullptr)
    {
      protoLine = string;
      CAddonBase::m_interface->toKodi->free_string(CAddonBase::m_interface->toKodi->kodiBase,
                                                   string);
    }
    return protoLine;
  }
  //--------------------------------------------------------------------------

  //==========================================================================
  /// @brief
  ///
  /// @return
  ///
  std::string GetCharset() const
  {
    using namespace ::kodi::addon;

    if (!m_handle.handle)
      return "";

    std::string protoLine;
    char* string = m_handle.get_charset(CAddonBase::m_interface->toKodi->kodiBase, m_handle.handle);
    if (string != nullptr)
    {
      protoLine = string;
      CAddonBase::m_interface->toKodi->free_string(CAddonBase::m_interface->toKodi->kodiBase,
                                                   string);
    }
    return protoLine;
  }
  //--------------------------------------------------------------------------

  //==========================================================================
  /// @brief
  ///
  /// @return
  ///
  std::string GetProtoLine() const
  {
    using namespace ::kodi::addon;

    if (!m_handle.handle)
      return "";

    std::string protoLine;
    char* string =
        m_handle.get_proto_line(CAddonBase::m_interface->toKodi->kodiBase, m_handle.handle);
    if (string != nullptr)
    {
      protoLine = string;
      CAddonBase::m_interface->toKodi->free_string(CAddonBase::m_interface->toKodi->kodiBase,
                                                   string);
    }
    return protoLine;
  }
  //--------------------------------------------------------------------------

  //==========================================================================
  /// @brief
  ///
  /// @return
  ///
  bool IsHeaderDone() const
  {
    using namespace ::kodi::addon;

    if (!m_handle.handle)
      return false;

    return m_handle.is_header_done(CAddonBase::m_interface->toKodi->kodiBase, m_handle.handle);
  }
  //--------------------------------------------------------------------------

  //==========================================================================
  /// @brief
  ///
  void Clear()
  {
    using namespace ::kodi::addon;

    if (!m_handle.handle)
      return;

    m_handle.clear(CAddonBase::m_interface->toKodi->kodiBase, m_handle.handle);
  }
  //--------------------------------------------------------------------------

  KODI_HTTP_HEADER m_handle;
};
//@}
//----------------------------------------------------------------------------

//============================================================================
/// @ingroup cpp_kodi_network
/// @brief Send WakeOnLan magic packet.
///
/// @param[in] mac Network address of the host to wake.
/// @return True if the magic packet was successfully sent, false otherwise.
///
inline bool WakeOnLan(const std::string& mac)
{
  using namespace ::kodi::addon;

  return CAddonBase::m_interface->toKodi->kodi_network->wake_on_lan(
      CAddonBase::m_interface->toKodi->kodiBase, mac.c_str());
}
//----------------------------------------------------------------------------

//============================================================================
/// @ingroup cpp_kodi_network
/// @brief To the current own ip address as a string.
///
/// @return Own system ip.
///
///
/// ------------------------------------------------------------------------
///
/// **Example:**
/// ~~~~~~~~~~~~~{.cpp}
/// #include <kodi/Network.h>
/// ...
/// std::string ipAddress = kodi::network::GetIPAddress();
/// fprintf(stderr, "My IP is '%s'\n", ipAddress.c_str());
/// ...
/// ~~~~~~~~~~~~~
///
inline std::string GetIPAddress()
{
  using namespace ::kodi::addon;

  std::string ip;
  char* string = CAddonBase::m_interface->toKodi->kodi_network->get_ip_address(
      CAddonBase::m_interface->toKodi->kodiBase);
  if (string != nullptr)
  {
    ip = string;
    CAddonBase::m_interface->toKodi->free_string(CAddonBase::m_interface->toKodi->kodiBase, string);
  }
  return ip;
}
//----------------------------------------------------------------------------

//============================================================================
/// @ingroup cpp_kodi_network
/// @brief Return our hostname.
///
/// @return String about hostname, empty in case of error
///
///
/// ------------------------------------------------------------------------
///
/// **Example:**
/// ~~~~~~~~~~~~~{.cpp}
/// #include <kodi/Network.h>
/// ...
/// std::string hostname = kodi::network::GetHostname();
/// fprintf(stderr, "My hostname is '%s'\n", hostname.c_str());
/// ...
/// ~~~~~~~~~~~~~
///
inline std::string GetHostname()
{
  using namespace ::kodi::addon;

  std::string ip;
  char* string = CAddonBase::m_interface->toKodi->kodi_network->get_hostname(
      CAddonBase::m_interface->toKodi->kodiBase);
  if (string != nullptr)
  {
    ip = string;
    CAddonBase::m_interface->toKodi->free_string(CAddonBase::m_interface->toKodi->kodiBase, string);
  }
  return ip;
}
//----------------------------------------------------------------------------

//============================================================================
/// @ingroup cpp_kodi_network
/// @brief
///
/// @param[in] url URL source of the data
/// @param[out] header The class @ref cpp_kodi_network_Defs_HttpHeader
/// @return true if successfully done, otherwise false
///
///
/// ------------------------------------------------------------------------
///
/// **Example:**
/// ~~~~~~~~~~~~~{.cpp}
/// #include <kodi/Network.h>
/// ...
/// kodi::network::HttpHeader header;
/// std::string url = "";
/// bool ret = kodi::network::GetHttpHeader(url, header);
/// ...
/// ~~~~~~~~~~~~~
///
inline bool GetHttpHeader(const std::string& url, HttpHeader& header)
{
  using namespace ::kodi::addon;

  return CAddonBase::m_interface->toKodi->kodi_network->get_http_header(
      CAddonBase::m_interface->toKodi->kodiBase, url.c_str(), &header.m_handle);
}
//----------------------------------------------------------------------------

//============================================================================
/// @ingroup cpp_kodi_network
/// @brief Get file mime type.
///
/// @param[in] url URL source of the data
/// @param[out] content
/// @param[in] useragent [opt]
/// @return true if successfully done, otherwise false
///
///
/// ------------------------------------------------------------------------
///
/// **Example:**
/// ~~~~~~~~~~~~~{.cpp}
/// #include <kodi/Network.h>
/// ...
/// std::string hostname = kodi::network::GetHostname();
/// fprintf(stderr, "My hostname is '%s'\n", hostname.c_str());
/// ...
/// ~~~~~~~~~~~~~
///
inline bool GetMimeType(const std::string& url,
                        std::string& content,
                        const std::string& useragent = "")
{
  using namespace ::kodi::addon;

  char* cContent;
  bool ret = CAddonBase::m_interface->toKodi->kodi_network->get_mime_type(
      CAddonBase::m_interface->toKodi->kodiBase, url.c_str(), &cContent, useragent.c_str());
  if (cContent != nullptr)
  {
    content = cContent;
    CAddonBase::m_interface->toKodi->free_string(CAddonBase::m_interface->toKodi->kodiBase,
                                                 cContent);
  }
  return ret;
}
//----------------------------------------------------------------------------

//============================================================================
/// @ingroup cpp_kodi_network
/// @brief Get file content-type.
///
/// @param[in] url URL source of the data
/// @param[out] content The returned type
/// @param[in] useragent [opt]
/// @return true if successfully done, otherwise false
///
///
/// ------------------------------------------------------------------------
///
/// **Example:**
/// ~~~~~~~~~~~~~{.cpp}
/// #include <kodi/Network.h>
/// ...
/// std::string url = "";
/// std::string content;
/// bool ret = kodi::network::GetCookies(url, content);
/// fprintf(stderr, "Content type from URL '%s' is '%s' (return was %s)\n",
///         url.c_str(), content.c_str(), ret ? "true" : "false");
/// ...
/// ~~~~~~~~~~~~~
///
inline bool GetContentType(const std::string& url,
                           std::string& content,
                           const std::string& useragent = "")
{
  using namespace ::kodi::addon;

  char* cContent;
  bool ret = CAddonBase::m_interface->toKodi->kodi_network->get_content_type(
      CAddonBase::m_interface->toKodi->kodiBase, url.c_str(), &cContent, useragent.c_str());
  if (cContent != nullptr)
  {
    content = cContent;
    CAddonBase::m_interface->toKodi->free_string(CAddonBase::m_interface->toKodi->kodiBase,
                                                 cContent);
  }
  return ret;
}
//----------------------------------------------------------------------------

//============================================================================
/// @ingroup cpp_kodi_network
/// @brief Get cookies stored by CURL in RFC 2109 format.
///
/// @param[in] url URL source of the data
/// @param[out] cookies The text list of available cookies
/// @return true if successfully done, otherwise false
///
///
/// ------------------------------------------------------------------------
///
/// **Example:**
/// ~~~~~~~~~~~~~{.cpp}
/// #include <kodi/Network.h>
/// ...
/// std::string url = "";
/// std::string cookies;
/// bool ret = kodi::network::GetCookies(url, cookies);
/// fprintf(stderr, "Cookies from URL '%s' are '%s' (return was %s)\n",
///         url.c_str(), cookies.c_str(), ret ? "true" : "false");
/// ...
/// ~~~~~~~~~~~~~
///
inline bool GetCookies(const std::string& url, std::string& cookies)
{
  using namespace ::kodi::addon;

  char* cCookies;
  bool ret = CAddonBase::m_interface->toKodi->kodi_network->get_cookies(
      CAddonBase::m_interface->toKodi->kodiBase, url.c_str(), &cCookies);
  if (cCookies != nullptr)
  {
    cookies = cCookies;
    CAddonBase::m_interface->toKodi->free_string(CAddonBase::m_interface->toKodi->kodiBase,
                                                 cCookies);
  }
  return ret;
}
//----------------------------------------------------------------------------

//============================================================================
/// @ingroup cpp_kodi_network
/// @brief Check given name or ip address corresponds to localhost.
///
/// @param[in] hostname Hostname to check
/// @return Return true if given name or ip address corresponds to localhost
///
///
/// ------------------------------------------------------------------------
///
/// **Example:**
/// ~~~~~~~~~~~~~{.cpp}
/// #include <kodi/Network.h>
/// ...
/// if (kodi::network::IsLocalHost("127.0.0.1"))
///   fprintf(stderr, "Is localhost\n");
/// ...
/// ~~~~~~~~~~~~~
///
inline bool IsLocalHost(const std::string& hostname)
{
  using namespace ::kodi::addon;

  return CAddonBase::m_interface->toKodi->kodi_network->is_local_host(
      CAddonBase::m_interface->toKodi->kodiBase, hostname.c_str());
}
//----------------------------------------------------------------------------

//==============================================================================
/// @ingroup cpp_kodi_network
/// @brief Checks whether the specified path refers to a local network.
///
/// @param[in] hostname Hostname to check
/// @param[in] offLineCheck Check if in private range, see https://en.wikipedia.org/wiki/Private_network
/// @return True if host is on a LAN, false otherwise
///
inline bool IsHostOnLAN(const std::string& hostname, bool offLineCheck = false)
{
  using namespace kodi::addon;

  return CAddonBase::m_interface->toKodi->kodi_network->is_host_on_lan(
      CAddonBase::m_interface->toKodi->kodiBase, hostname.c_str(), offLineCheck);
}
//------------------------------------------------------------------------------

//============================================================================
/// @ingroup cpp_kodi_network
/// @brief URL encodes the given string
///
/// This function converts the given input string to a URL encoded string and
/// returns that as a new allocated string. All input characters that are
/// not a-z, A-Z, 0-9, '-', '.', '_' or '~' are converted to their "URL escaped"
/// version (%NN where NN is a two-digit hexadecimal number).
///
/// @param[in] url The code of the message to get.
/// @return Encoded URL string
///
///
/// ------------------------------------------------------------------------
///
/// **Example:**
/// ~~~~~~~~~~~~~{.cpp}
/// #include <kodi/Network.h>
/// ...
/// std::string encodedUrl = kodi::network::URLEncode("François");
/// fprintf(stderr, "Encoded URL is '%s'\n", encodedUrl.c_str());
/// ...
/// ~~~~~~~~~~~~~
/// For example, the string: François ,would be encoded as: Fran%C3%A7ois
///
inline std::string URLEncode(const std::string& url)
{
  using namespace ::kodi::addon;

  std::string retString;
  char* string = CAddonBase::m_interface->toKodi->kodi_network->url_encode(
      CAddonBase::m_interface->toKodi->kodiBase, url.c_str());
  if (string != nullptr)
  {
    retString = string;
    CAddonBase::m_interface->toKodi->free_string(CAddonBase::m_interface->toKodi->kodiBase, string);
  }
  return retString;
}
//----------------------------------------------------------------------------

//============================================================================
/// @ingroup cpp_kodi_network
/// @brief Lookup URL in DNS cache
///
/// This test will get DNS record for a domain. The DNS lookup is done directly
/// against the domain's authoritative name server, so changes to DNS Records
/// should show up instantly. By default, the DNS lookup tool will return an
/// IP address if you give it a name (e.g. www.example.com)
///
/// @param[in] hostName   The code of the message to get.
/// @param[out] ipAddress Returned address
/// @return true if successfull
///
///
/// ------------------------------------------------------------------------
///
/// **Example:**
/// ~~~~~~~~~~~~~{.cpp}
/// #include <kodi/Network.h>
/// ...
/// std::string ipAddress;
/// bool ret = kodi::network::DNSLookup("www.google.com", ipAddress);
/// fprintf(stderr, "DNSLookup returned for www.google.com the IP '%s', call was %s\n", ipAddress.c_str(), ret ? "ok" : "failed");
/// ...
/// ~~~~~~~~~~~~~
///
inline bool DNSLookup(const std::string& hostName, std::string& ipAddress)
{
  using namespace ::kodi::addon;

  bool ret = false;
  char* string = CAddonBase::m_interface->toKodi->kodi_network->dns_lookup(
      CAddonBase::m_interface->toKodi->kodiBase, hostName.c_str(), &ret);
  if (string != nullptr)
  {
    ipAddress = string;
    CAddonBase::m_interface->toKodi->free_string(CAddonBase::m_interface->toKodi->kodiBase, string);
  }
  return ret;
}
//----------------------------------------------------------------------------

} /* namespace network */
} /* namespace kodi */

#endif /* __cplusplus */
