//+------------------------------------------------------------------+
//|                                                 MetaTrader 5 API |
//|                             Copyright 2000-2024, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
#include "MT5APIConfigParam.h"
//+------------------------------------------------------------------+
//| Datafeed module description                                      |
//+------------------------------------------------------------------+
class IMTConFeederModule
  {
public:
   //--- necessary fields flags
   enum EnFeedersFieldFlags
     {
      FEED_FIELD_SERVER=1,            // server field
      FEED_FIELD_LOGIN =2,            // login field
      FEED_FIELD_PASS  =4,            // password field
      FEED_FIELD_PARAM =8,            // parameters
      //--- enumeration borders
      FEED_FIELD_NONE  =0,
      FEED_FIELD_ALL   =FEED_FIELD_SERVER|FEED_FIELD_LOGIN|FEED_FIELD_PASS|FEED_FIELD_PARAM
     };
   //--- common methods
   virtual void      Release(void)=0;
   virtual MTAPIRES  Assign(const IMTConFeederModule* param)=0;
   virtual MTAPIRES  Clear(void)=0;
   //--- default datafeed name
   virtual LPCWSTR   Name(void) const=0;
   //--- vendor name
   virtual LPCWSTR   Vendor(void) const=0;
   //--- datafeed description
   virtual LPCWSTR   Description(void) const=0;
   //--- datafeed file name
   virtual LPCWSTR   Module(void) const=0;
   //--- default feed server address
   virtual LPCWSTR   FeedServer(void) const=0;
   //--- default feed server login
   virtual LPCWSTR   FeedLogin(void) const=0;
   //--- default feed server password
   virtual LPCWSTR   FeedPassword(void) const=0;
   //--- datafeed version
   virtual UINT      Version(void) const=0;
   //--- datafeed available modes (IMTConFeeder::EnFeedersFlags)
   virtual UINT      Modes(void) const=0;
   //--- changeable EnFeedersFieldFlags
   virtual UINT      Fields(void) const=0;
   //--- default datafeed parameters
   virtual UINT      ParameterTotal(void) const=0;
   virtual MTAPIRES  ParameterNext(const UINT pos,IMTConParam* param) const=0;
   virtual MTAPIRES  ParameterGet(LPCWSTR name,IMTConParam* param) const=0;
   //--- explicit destructor is prohibited
protected:
                    ~IMTConFeederModule(void) {}
  };
//+------------------------------------------------------------------+
//| Symbols name and price translation config                        |
//+------------------------------------------------------------------+
class IMTConFeederTranslate
  {
public:
   //--- common methods
   virtual void      Release(void)=0;
   virtual MTAPIRES  Assign(const IMTConFeederTranslate* param)=0;
   virtual MTAPIRES  Clear(void)=0;
   //--- symbol name in datafeed
   virtual LPCWSTR   Source(void) const=0;
   virtual MTAPIRES  Source(LPCWSTR source)=0;
   //--- symbol name in MT5
   virtual LPCWSTR   Symbol(void) const=0;
   virtual MTAPIRES  Symbol(LPCWSTR symbol)=0;
   //--- bid markup in points
   virtual INT       BidMarkup(void) const=0;
   virtual MTAPIRES  BidMarkup(const INT markup)=0;
   //--- ask markup in points
   virtual INT       AskMarkup(void) const=0;
   virtual MTAPIRES  AskMarkup(const INT markup)=0;
   //--- digits
   virtual UINT      Digits(void) const=0;
   //--- explicit destructor is prohibited
protected:
                    ~IMTConFeederTranslate(void) {}
  };
//+------------------------------------------------------------------+
//| Datafeed config interface                                        |
//+------------------------------------------------------------------+
class IMTConFeeder
  {
public:
   //--- datafeed working flags
   enum EnFeedersFlags
     {
      FEED_FLAG_QUOTES        =0x0000001,    // feeder should send quotes
      FEED_FLAG_NEWS          =0x0000002,    // feeder should send news
      FEED_FLAG_REMOTE        =0x0000008,    // feeder works as remote service
      FEED_FLAG_TRIAL         =0x0000010,    // feeder works in trial mode
      FEED_FLAG_INTERNAL      =0x0000020,    // feeder works as part of history server
      FEED_FLAG_IMPORT_SYMBOLS=0x0000040,    // allow to import symbols
      //--- flags borders
      FEED_FLAG_NONE   =0,
      FEED_FLAG_ALL    =FEED_FLAG_QUOTES|FEED_FLAG_NEWS|FEED_FLAG_REMOTE|FEED_FLAG_TRIAL|FEED_FLAG_INTERNAL|FEED_FLAG_IMPORT_SYMBOLS
     };
   //--- datafeed working modes
   enum EnFeedersMode
     {
      FEEDER_DISABLED  =0,
      FEEDER_ENABLED   =1,
      //--- enumeration borders
      FEEDER_FIRST     =FEEDER_DISABLED,
      FEEDER_LAST      =FEEDER_ENABLED
     };
   //--- common methods
   virtual void      Release(void)=0;
   virtual MTAPIRES  Assign(const IMTConFeeder* param)=0;
   virtual MTAPIRES  Clear(void)=0;
   //--- datafeed name
   virtual LPCWSTR   Name(void) const=0;
   virtual MTAPIRES  Name(LPCWSTR name)=0;
   //--- datafeed module filename
   virtual LPCWSTR   Module(void) const=0;
   virtual MTAPIRES  Module(LPCWSTR name)=0;
   //--- feed server address
   virtual LPCWSTR   FeedServer(void) const=0;
   virtual MTAPIRES  FeedServer(LPCWSTR server)=0;
   //--- feed server login
   virtual LPCWSTR   FeedLogin(void) const=0;
   virtual MTAPIRES  FeedLogin(LPCWSTR login)=0;
   //--- feed server password
   virtual LPCWSTR   FeedPassword(void) const=0;
   virtual MTAPIRES  FeedPassword(LPCWSTR password)=0;
   //--- EnFeedersMode
   virtual UINT      Mode(void) const=0;
   virtual MTAPIRES  Mode(const UINT mode)=0;
   //--- EnFeedersFlags
   virtual UINT      Flags(void) const=0;
   virtual MTAPIRES  Flags(const UINT flags)=0;
   //--- comma separated news keywords
   virtual LPCWSTR   Keywords(void) const=0;
   virtual MTAPIRES  Keywords(LPCWSTR keywords)=0;
   //--- news category
   virtual LPCWSTR   Categories(void) const=0;
   virtual MTAPIRES  Categories(LPCWSTR categories)=0;
   //--- obsolete value
   virtual UINT      ObsoleteValue(void) const=0;
   virtual MTAPIRES  ObsoleteValue(const UINT value)=0;
   //--- datafeed reconnect timeout
   virtual UINT      TimeoutReconnect(void) const=0;
   virtual MTAPIRES  TimeoutReconnect(const UINT timeout)=0;
   //--- datafeed sleep timeout
   virtual UINT      TimeoutSleep(void) const=0;
   virtual MTAPIRES  TimeoutSleep(const UINT timeout)=0;
   //--- reconnect attempts before timeout
   virtual UINT      TimeoutAttempts(void) const=0;
   virtual MTAPIRES  TimeoutAttempts(const UINT attempts)=0;
   //--- datafeed additional parameters access
   virtual MTAPIRES  ParameterAdd(IMTConParam* param)=0;
   virtual MTAPIRES  ParameterUpdate(const UINT pos,const IMTConParam* param)=0;
   virtual MTAPIRES  ParameterDelete(const UINT pos)=0;
   virtual MTAPIRES  ParameterClear(void)=0;
   virtual MTAPIRES  ParameterShift(const UINT pos,const int shift)=0;
   virtual UINT      ParameterTotal(void) const=0;
   virtual MTAPIRES  ParameterNext(const UINT pos,IMTConParam* param) const=0;
   virtual MTAPIRES  ParameterGet(LPCWSTR name,IMTConParam* param) const=0;
   //--- list of symbols for translating quotes
   virtual MTAPIRES  SymbolAdd(LPCWSTR path)=0;
   virtual MTAPIRES  SymbolUpdate(const UINT pos,LPCWSTR path)=0;
   virtual MTAPIRES  SymbolDelete(const UINT pos)=0;
   virtual MTAPIRES  SymbolClear(void)=0;
   virtual MTAPIRES  SymbolShift(const UINT pos,const int shift)=0;
   virtual UINT      SymbolTotal(void) const=0;
   virtual LPCWSTR   SymbolNext(const UINT pos) const=0;
   //--- list of symbols translations
   virtual MTAPIRES  TranslateAdd(IMTConFeederTranslate* param)=0;
   virtual MTAPIRES  TranslateUpdate(const UINT pos,const IMTConFeederTranslate* param)=0;
   virtual MTAPIRES  TranslateDelete(const UINT pos)=0;
   virtual MTAPIRES  TranslateClear(void)=0;
   virtual MTAPIRES  TranslateShift(const UINT pos,const int shift)=0;
   virtual UINT      TranslateTotal(void) const=0;
   virtual MTAPIRES  TranslateNext(const UINT pos,IMTConFeederTranslate* param) const=0;
   virtual MTAPIRES  TranslateGet(LPCWSTR symbol,IMTConFeederTranslate* param) const=0;
   //--- gateway server address
   virtual LPCWSTR   GatewayServer(void) const=0;
   virtual MTAPIRES  GatewayServer(LPCWSTR server)=0;
   //--- gateway server login
   virtual UINT64    GatewayLogin(void) const=0;
   virtual MTAPIRES  GatewayLogin(UINT64 login)=0;
   //--- gateway server password
   virtual LPCWSTR   GatewayPassword(void) const=0;
   virtual MTAPIRES  GatewayPassword(LPCWSTR password)=0;
   //--- feeder state information
   virtual bool      StateConnected(void) const=0;
   virtual UINT      StateReceivedTicks(void) const=0;
   virtual UINT      StateReceivedBooks(void) const=0;
   virtual UINT      StateReceivedNews(void) const=0;
   virtual UINT      StateTrafficIn(void) const=0;
   virtual UINT      StateTrafficOut(void) const=0;
   //--- explicit destructor is prohibited
protected:
                    ~IMTConFeeder(void) {}
  };
//+------------------------------------------------------------------+
//| Datafeed config events notification interface                    |
//+------------------------------------------------------------------+
class IMTConFeederSink
  {
public:
   virtual void      OnFeederAdd(const IMTConFeeder*    /*feeder*/) {  }
   virtual void      OnFeederUpdate(const IMTConFeeder* /*feeder*/) {  }
   virtual void      OnFeederDelete(const IMTConFeeder* /*feeder*/) {  }
   virtual void      OnFeederSync(void)                             {  }
  };
//+------------------------------------------------------------------+

