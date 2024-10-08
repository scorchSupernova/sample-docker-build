//+------------------------------------------------------------------+
//|                                                   BalanceExample |
//|                             Copyright 2000-2022, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS
//+------------------------------------------------------------------+
//| Windows versions                                                 |
//+------------------------------------------------------------------+
#define WINVER               0x0601          // Change this to the appropriate value to target other versions of Windows.
#define _WIN32_WINNT         0x0601          // Change this to the appropriate value to target other versions of Windows.
#define _WIN32_WINDOWS       0x0601          // Change this to the appropriate value to target Windows Me or later.
#define _WIN32_IE            0x0601          // Change this to the appropriate value to target other versions of IE.
#define  WIN32_LEAN_AND_MEAN
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
#include <afxwin.h>         // MFC core and standard components
#include <process.h>
#include <afxcmn.h>
#include <afxdtctl.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>  
#include <fmt/format.h>
#include <fstream>
#include <codecvt>
#include <locale>
#include <string>
#include <memory>
#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <hiredis/hiredis.h>
#include "nlohmann/json.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <curl/curl.h>
#include <string>
#include <stdexcept>
#include <librdkafka/rdkafka.h>
#include <librdkafka/rdkafkacpp.h>

//--- MT5 Manager API
#include "Include\MT5APIManager.h"
//+------------------------------------------------------------------+
//| Manifest                                                         |
//+------------------------------------------------------------------+
#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
//+------------------------------------------------------------------+
