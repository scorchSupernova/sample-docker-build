//+------------------------------------------------------------------+
//|                                                MetaTrader 5 API  |
//|                             Copyright 2000-2024, MetaQuotes Ltd. |
//|                                               www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//+------------------------------------------------------------------+
//| Commission tier config                                           |
//+------------------------------------------------------------------+
class IMTConCommTier
  {
public:
   //--- commission charge mode
   enum EnCommissionMode
     {
      COMM_MONEY_DEPOSIT       =0,  // in money, in group deposit currency
      COMM_MONEY_SYMBOL_BASE   =1,  // in money, in symbol base currency
      COMM_MONEY_SYMBOL_PROFIT =2,  // in money, in symbol profit currency
      COMM_MONEY_SYMBOL_MARGIN =3,  // in money, in symbol margin currency
      COMM_PIPS                =4,  // in pips
      COMM_PERCENT             =5,  // in percent
      COMM_MONEY_SPECIFIED     =6,  // in money, in specified currency
      COMM_PERCENT_PROFIT      =7,  // in profit percent
      //--- enumeration borders
      COMM_FIRST               =COMM_MONEY_DEPOSIT,
      COMM_LAST                =COMM_PERCENT_PROFIT
     };
   //--- commission type by volume
   enum EnCommissionVolumeType
     {
      COMM_TYPE_DEAL           =0,  // commission per deal
      COMM_TYPE_VOLUME         =1,  // commission per volume
      //--- enumeration borders
      COMM_TYPE_FIRST          =COMM_TYPE_DEAL,
      COMM_TYPE_LAST           =COMM_TYPE_VOLUME
     };
   //--- common methods
   virtual void      Release(void)=0;
   virtual MTAPIRES  Assign(const IMTConCommTier* group)=0;
   virtual MTAPIRES  Clear(void)=0;
   //--- EnCommissionMode
   virtual UINT      Mode(void) const=0;
   virtual MTAPIRES  Mode(const UINT mode)=0;
   //--- EnCommissionVolumeType
   virtual UINT      Type(void) const=0;
   virtual MTAPIRES  Type(const UINT type)=0;
   //--- commission value
   virtual double    Value(void) const=0;
   virtual MTAPIRES  Value(const double value)=0;
   //--- minimal commission value
   virtual double    Minimal(void) const=0;
   virtual MTAPIRES  Minimal(const double value)=0;
   //--- tier range from
   virtual double    RangeFrom(void) const=0;
   virtual MTAPIRES  RangeFrom(const double value)=0;
   //--- tier range to
   virtual double    RangeTo(void) const=0;
   virtual MTAPIRES  RangeTo(const double value)=0;
   //--- commission currency (for Mode==COMM_MONEY_SPECIFIED)
   virtual LPCWSTR   Currency(void) const=0;
   virtual MTAPIRES  Currency(LPCWSTR currency)=0;
   //--- maximal commission value
   virtual double    Maximal(void) const=0;
   virtual MTAPIRES  Maximal(const double value)=0;
   //--- explicit destructor is prohibited
protected:
                    ~IMTConCommTier(void) {}
  };
//+------------------------------------------------------------------+
//| Commission config interface                                      |
//+------------------------------------------------------------------+
class IMTConCommission
  {
public:
   //--- commission mode
   enum EnCommMode
     {
      COMM_STANDARD            =0,  // standard commission
      COMM_AGENT               =1,  // agent commission
      COMM_FEE                 =2,  // fee
      //--- enumeration borders
      COMM_FIRST               =COMM_STANDARD,
      COMM_LAST                =COMM_FEE
     };
   //--- commission range mode
   enum EnCommRangeMode
     {
      COMM_RANGE_VOLUME         =0,  // range in volumes
      COMM_RANGE_TURNOVER_MONEY =1,  // turnover in money
      COMM_RANGE_TURNOVER_VOLUME=2,  // turnover in volume
      COMM_RANGE_VALUE          =3,  // range in values
      COMM_RANGE_PROFIT         =4,  // range in profit
      //--- enumeration borders
      COMM_RANGE_FIRST         =COMM_RANGE_VOLUME,
      COMM_RANGE_LAST          =COMM_RANGE_PROFIT
     };
   //--- commission charge modes
   enum EnCommChargeMode
     {
      COMM_CHARGE_DAILY        =0, // charge at the end of daily
      COMM_CHARGE_MONTHLY      =1, // charge at the end of month
      COMM_CHARGE_INSTANT      =2, // charge instantly
      //--- enumeration borders
      COMM_CHARGE_FIRST        =COMM_CHARGE_DAILY,
      COMM_CHARGE_LAST         =COMM_CHARGE_INSTANT
     };
   //--- deal entry mode
   enum EnCommEntryMode
     {
      COMM_ENTRY_ALL           =0, // both in and out
      COMM_ENTRY_IN            =1, // in deals
      COMM_ENTRY_OUT           =2, // out deals
      //--- enumeration borders
      COMM_ENTRY_FIRST         =COMM_ENTRY_ALL,
      COMM_ENTRY_LAST          =COMM_ENTRY_OUT
     };
   //--- deal action mode
   enum EnCommActionMode
     {
      COMM_ACTION_ALL          =0,
      COMM_ACTION_BUY          =1,
      COMM_ACTION_SELL         =2,
      //--- enumeration borders
      COMM_ACTION_FIRST        =COMM_ACTION_ALL,
      COMM_ACTION_LAST         =COMM_ACTION_SELL
     };
   //--- deal profit mode
   enum EnCommProfitMode
     {
      COMM_PROFIT_ALL          =0,
      COMM_PROFIT_PROFIT       =1,
      COMM_PROFIT_LOSS         =2,
      //--- enumeration borders
      COMM_PROFIT_FIRST        =COMM_PROFIT_ALL,
      COMM_PROFIT_LAST         =COMM_PROFIT_LOSS
     };
   //--- deal reason
   enum EnCommReasonFlags
     {
      COMM_REASON_FLAG_NONE            =0x00000000,   // none
      COMM_REASON_FLAG_CLIENT          =0x00000001,   // client terminal
      COMM_REASON_FLAG_EXPERT          =0x00000002,   // expert
      COMM_REASON_FLAG_DEALER          =0x00000004,   // dealer
      COMM_REASON_FLAG_EXTERNAL_CLIENT =0x00000008,   // external client
      COMM_REASON_FLAG_MOBILE          =0x00000010,   // mobile terminal
      COMM_REASON_FLAG_WEB             =0x00000020,   // web terminal
      COMM_REASON_FLAG_SIGNAL          =0x00000040,   // signal service
      //--- enumeration borders
      COMM_REASON_FLAG_ALL             =COMM_REASON_FLAG_CLIENT | COMM_REASON_FLAG_EXPERT | COMM_REASON_FLAG_DEALER | COMM_REASON_FLAG_EXTERNAL_CLIENT | COMM_REASON_FLAG_MOBILE | COMM_REASON_FLAG_WEB | COMM_REASON_FLAG_SIGNAL
     };
   //--- common methods
   virtual void      Release(void)=0;
   virtual MTAPIRES  Assign(const IMTConCommission* group)=0;
   virtual MTAPIRES  Clear(void)=0;
   //--- commission name
   virtual LPCWSTR   Name(void) const=0;
   virtual MTAPIRES  Name(LPCWSTR name)=0;
   //--- description
   virtual LPCWSTR   Description(void) const=0;
   virtual MTAPIRES  Description(LPCWSTR descr)=0;
   //--- symbols path
   virtual LPCWSTR   Path(void) const=0;
   virtual MTAPIRES  Path(LPCWSTR path)=0;
   //--- EnCommMode
   virtual UINT      Mode(void) const=0;
   virtual MTAPIRES  Mode(const UINT mode)=0;
   //--- EnCommRangeMode
   virtual UINT      RangeMode(void) const=0;
   virtual MTAPIRES  RangeMode(const UINT mode)=0;
   //--- EnCommChargeMode
   virtual UINT      ChargeMode(void) const=0;
   virtual MTAPIRES  ChargeMode(const UINT mode)=0;
   //--- commission tiers
   virtual MTAPIRES  TierAdd(IMTConCommTier* tier)=0;
   virtual MTAPIRES  TierUpdate(const UINT pos,const IMTConCommTier* tier)=0;
   virtual MTAPIRES  TierDelete(const UINT pos)=0;
   virtual MTAPIRES  TierClear(void)=0;
   virtual MTAPIRES  TierShift(const UINT pos,const int shift)=0;
   virtual UINT      TierTotal(void) const=0;
   virtual MTAPIRES  TierNext(const UINT pos,IMTConCommTier* tier) const=0;
   //---- turnover calculation currency
   virtual LPCWSTR   TurnoverCurrency(void) const=0;
   virtual MTAPIRES  TurnoverCurrency(LPCWSTR currency)=0;
   //--- EnCommEntryMode
   virtual UINT      EntryMode(void) const=0;
   virtual MTAPIRES  EntryMode(const UINT mode)=0;
   //--- EnCommActionMode
   virtual UINT      ActionMode(void) const=0;
   virtual MTAPIRES  ActionMode(const UINT mode)=0;
   //--- EnCommProfitMode
   virtual UINT      ProfitMode(void) const=0;
   virtual MTAPIRES  ProfitMode(const UINT mode)=0;
   //--- EnCommReasonFlags
   virtual UINT      ReasonFlags(void) const=0;
   virtual MTAPIRES  ReasonFlags(const UINT flags)=0;
   //--- explicit destructor is prohibited
protected:
                    ~IMTConCommission(void) {}
  };
//+------------------------------------------------------------------+
//| Symbols configuration for clients group                          |
//+------------------------------------------------------------------+
class IMTConGroupSymbol
  {
public:
   //--- Requests Execution flags
   enum EnREFlags
     {
      RE_FLAGS_NONE           =0,  // none
      RE_FLAGS_ORDER          =1,  // confirm orders after price confirmation
      //--- enumeration borders
      RE_FLAGS_ALL            =RE_FLAGS_ORDER
     };
   //--- Permissions
   enum EnPermissionsFlags
     {
      PERMISSION_NONE         =0,  // none
      PERMISSION_BOOK         =1,  // allow books for group
      //--- enumeration borders
      PERMISSION_DEFAULT      =PERMISSION_BOOK,
      PERMISSION_ALL          =PERMISSION_BOOK
     };

public:
   //--- common methods
   virtual void      Release(void)=0;
   virtual MTAPIRES  Assign(const IMTConGroupSymbol* group)=0;
   virtual MTAPIRES  Clear(void)=0;
   //--- setup default values for all fields
   virtual MTAPIRES  Default(void)=0;
   //--- symbol or symbol groups path
   virtual LPCWSTR   Path(void) const=0;
   virtual MTAPIRES  Path(LPCWSTR path)=0;
   //--- IMTConSymbol::EnTradeMode
   virtual UINT      TradeMode(void) const=0;
   virtual MTAPIRES  TradeMode(const UINT mode)=0;
   virtual UINT      TradeModeDefault(void) const=0;
   //--- IMTConSymbol::EnCalcMode
   virtual UINT      ExecMode(void) const=0;
   virtual MTAPIRES  ExecMode(const UINT mode)=0;
   virtual UINT      ExecModeDefault(void) const=0;
   //--- IMTConSymbol::EnTradeFillingFlags
   virtual UINT      FillFlags(void) const=0;
   virtual MTAPIRES  FillFlags(const UINT flags)=0;
   virtual UINT      FillFlagsDefault(void) const=0;
   //--- IMTConSymbol::EnTradeExpirationFlags
   virtual UINT      ExpirFlags(void) const=0;
   virtual MTAPIRES  ExpirFlags(const UINT flags)=0;
   virtual UINT      ExpirFlagsDefault(void) const=0;
   //--- spread difference (0 - floating spread)
   virtual INT       SpreadDiff(void) const=0;
   virtual MTAPIRES  SpreadDiff(const INT spread)=0;
   virtual INT       SpreadDiffDefault(void) const=0;
   //--- spread difference balance
   virtual INT       SpreadDiffBalance(void) const=0;
   virtual MTAPIRES  SpreadDiffBalance(const INT spread)=0;
   virtual INT       SpreadDiffBalanceDefault(void) const=0;
   //--- stops level
   virtual INT       StopsLevel(void) const=0;
   virtual MTAPIRES  StopsLevel(const INT level)=0;
   virtual INT       StopsLevelDefault(void) const=0;
   //--- freeze level
   virtual INT       FreezeLevel(void) const=0;
   virtual MTAPIRES  FreezeLevel(const INT level)=0;
   virtual INT       FreezeLevelDefault(void) const=0;
   //--- minimal volume
   virtual UINT64    VolumeMin(void) const=0;
   virtual MTAPIRES  VolumeMin(const UINT64 volume)=0;
   virtual UINT64    VolumeMinDefault(void) const=0;
   //--- maximal volume
   virtual UINT64    VolumeMax(void) const=0;
   virtual MTAPIRES  VolumeMax(const UINT64 volume)=0;
   virtual UINT64    VolumeMaxDefault(void) const=0;
   //--- volume step
   virtual UINT64    VolumeStep(void) const=0;
   virtual MTAPIRES  VolumeStep(const UINT64 volume)=0;
   virtual UINT64    VolumeStepDefault(void) const=0;
   //--- cumulative positions and orders limit
   virtual UINT64    VolumeLimit(void) const=0;
   virtual MTAPIRES  VolumeLimit(const UINT64 volume)=0;
   virtual UINT64    VolumeLimitDefault(void) const=0;
   //--- IMTConSymbol::EnMarginFlags
   virtual UINT      MarginFlags(void) const=0;
   virtual MTAPIRES  MarginFlags(const UINT flags)=0;
   virtual UINT      MarginFlagsDefault(void) const=0;
   //--- initial margin
   virtual double    MarginInitial(void) const=0;
   virtual MTAPIRES  MarginInitial(const double margin)=0;
   virtual double    MarginInitialDefault(void) const=0;
   //--- maintenance margin
   virtual double    MarginMaintenance(void) const=0;
   virtual MTAPIRES  MarginMaintenance(const double margin)=0;
   virtual double    MarginMaintenanceDefault(void) const=0;
   //--- long orders and positions margin rate
   virtual double    MarginLong(void) const=0;
   virtual MTAPIRES  MarginLong(const double margin)=0;
   virtual double    MarginLongDefault(void) const=0;
   //--- short orders and positions margin rate
   virtual double    MarginShort(void) const=0;
   virtual MTAPIRES  MarginShort(const double margin)=0;
   virtual double    MarginShortDefault(void) const=0;
   //--- limit orders and positions margin rate
   virtual double    MarginLimit(void) const=0;
   virtual MTAPIRES  MarginLimit(const double margin)=0;
   virtual double    MarginLimitDefault(void) const=0;
   //--- stop orders and positions margin rate
   virtual double    MarginStop(void) const=0;
   virtual MTAPIRES  MarginStop(const double margin)=0;
   virtual double    MarginStopDefault(void) const=0;
   //--- stop-limit orders and positions margin rate
   virtual double    MarginStopLimit(void) const=0;
   virtual MTAPIRES  MarginStopLimit(const double margin)=0;
   virtual double    MarginStopLimitDefault(void) const=0;
   //--- IMTConSymbol::EnSwapMode
   virtual UINT      SwapMode(void) const=0;
   virtual MTAPIRES  SwapMode(const UINT mode)=0;
   virtual UINT      SwapModeDefault(void) const=0;
   //--- long positions swaps rate
   virtual double    SwapLong(void) const=0;
   virtual MTAPIRES  SwapLong(const double swap)=0;
   virtual double    SwapLongDefault(void) const=0;
   //--- short positions swaps rate
   virtual double    SwapShort(void) const=0;
   virtual MTAPIRES  SwapShort(const double swap)=0;
   virtual double    SwapShortDefault(void) const=0;
   //--- 3 time swaps day, EnSwapDay (obsolete)
   virtual int       Swap3Day(void) const=0;
   virtual MTAPIRES  Swap3Day(const int day)=0;
   virtual int       Swap3DayDefault(void) const=0;
   //--- request execution timeout
   virtual UINT      RETimeout(void) const=0;
   virtual MTAPIRES  RETimeout(const UINT timeout)=0;
   virtual UINT      RETimeoutDefault(void) const=0;
   //--- instant execution check mode
   virtual UINT      IECheckMode(void) const=0;
   virtual MTAPIRES  IECheckMode(const UINT mode)=0;
   virtual UINT      IECheckModeDefault(void) const=0;
   //--- instant execution timeout
   virtual UINT      IETimeout(void) const=0;
   virtual MTAPIRES  IETimeout(const UINT timeout)=0;
   virtual UINT      IETimeoutDefault(void) const=0;
   //--- instant execution profit slippage
   virtual UINT      IESlipProfit(void) const=0;
   virtual MTAPIRES  IESlipProfit(const UINT slippage)=0;
   virtual UINT      IESlipProfitDefault(void) const=0;
   //--- instant execution losing slippage
   virtual UINT      IESlipLosing(void) const=0;
   virtual MTAPIRES  IESlipLosing(const UINT slippage)=0;
   virtual UINT      IESlipLosingDefault(void) const=0;
   //--- instant execution max volume
   virtual UINT64    IEVolumeMax(void) const=0;
   virtual MTAPIRES  IEVolumeMax(const UINT64 volume)=0;
   virtual UINT64    IEVolumeMaxDefault(void) const=0;
   //--- IMTConSymbol::EnOrderFlags
   virtual UINT      OrderFlags(void) const=0;
   virtual MTAPIRES  OrderFlags(const UINT flags)=0;
   virtual UINT      OrderFlagsDefault(void) const=0;
   //--- orders and positions margin rates
   virtual double    MarginRateInitial(const UINT type) const=0;
   virtual MTAPIRES  MarginRateInitial(const UINT type,const double margin_rate)=0;
   virtual double    MarginRateInitialDefault(void) const=0;
   //--- orders and positions margin rates
   virtual double    MarginRateMaintenance(const UINT type) const=0;
   virtual MTAPIRES  MarginRateMaintenance(const UINT type,const double margin_rate)=0;
   virtual double    MarginRateMaintenanceDefault(void) const=0;
   //--- trade symbol liquidity rate in margin calculation
   virtual double    MarginRateLiquidity(void) const=0;
   virtual MTAPIRES  MarginRateLiquidity(const double rate)=0;
   virtual double    MarginRateLiquidityDefault(void) const=0;
   //--- request execution flags IMTConGroupSymbol::EnREFlags
   virtual UINT      REFlags(void) const=0;
   virtual MTAPIRES  REFlags(const UINT flags)=0;
   virtual UINT      REFlagsDefault(void) const=0;
   //--- hedged positions margin rate
   virtual double    MarginHedged(void) const=0;
   virtual MTAPIRES  MarginHedged(const double margin)=0;
   virtual double    MarginHedgedDefault(void) const=0;
   //--- permissions flags
   virtual UINT      PermissionsFlags(void) const=0;
   virtual MTAPIRES  PermissionsFlags(const UINT flags)=0;
   //--- margin currency rate
   virtual double    MarginRateCurrency(void) const=0;
   virtual MTAPIRES  MarginRateCurrency(const double margin_rate)=0;
   virtual double    MarginRateCurrencyDefault(void) const=0;
   //--- book depth limit
   virtual UINT      BookDepthLimit(void) const=0;
   virtual MTAPIRES  BookDepthLimit(const UINT depth)=0;
   //--- instant execution flags
   virtual UINT      IEFlags(void) const=0;
   virtual MTAPIRES  IEFlags(const UINT flags)=0;
   virtual UINT      IEFlagsDefault(void) const=0;
   //--- minimal volume
   virtual UINT64    VolumeMinExt(void) const=0;
   virtual MTAPIRES  VolumeMinExt(const UINT64 volume)=0;
   virtual UINT64    VolumeMinExtDefault(void) const=0;
   //--- maximal volume
   virtual UINT64    VolumeMaxExt(void) const=0;
   virtual MTAPIRES  VolumeMaxExt(const UINT64 volume)=0;
   virtual UINT64    VolumeMaxExtDefault(void) const=0;
   //--- volume step
   virtual UINT64    VolumeStepExt(void) const=0;
   virtual MTAPIRES  VolumeStepExt(const UINT64 volume)=0;
   virtual UINT64    VolumeStepExtDefault(void) const=0;
   //--- cumulative positions and orders limit
   virtual UINT64    VolumeLimitExt(void) const=0;
   virtual MTAPIRES  VolumeLimitExt(const UINT64 volume)=0;
   virtual UINT64    VolumeLimitExtDefault(void) const=0;
   //--- instant execution max volume
   virtual UINT64    IEVolumeMaxExt(void) const=0;
   virtual MTAPIRES  IEVolumeMaxExt(const UINT64 volume)=0;
   virtual UINT64    IEVolumeMaxExtDefault(void) const=0;
   //--- days in year
   virtual UINT      SwapYearDays(void) const=0;
   virtual MTAPIRES  SwapYearDays(const UINT days)=0;
   virtual UINT      SwapYearDaysDefault(void) const=0;
   //--- swap flags
   virtual UINT      SwapFlags(void) const=0;
   virtual MTAPIRES  SwapFlags(const UINT flags)=0;
   virtual UINT      SwapFlagsDefault(void) const=0;
   //--- swap rate for Sunday
   virtual double    SwapRateSunday(void) const=0;
   virtual MTAPIRES  SwapRateSunday(const double rate)=0;
   virtual double    SwapRateSundayDefault(void) const=0;
   //--- swap rate for Monday
   virtual double    SwapRateMonday(void) const=0;
   virtual MTAPIRES  SwapRateMonday(const double rate)=0;
   virtual double    SwapRateMondayDefault(void) const=0;
   //--- swap rate for Tuesday
   virtual double    SwapRateTuesday(void) const=0;
   virtual MTAPIRES  SwapRateTuesday(const double rate)=0;
   virtual double    SwapRateTuesdayDefault(void) const=0;
   //--- swap rate for Wednesday
   virtual double    SwapRateWednesday(void) const=0;
   virtual MTAPIRES  SwapRateWednesday(const double rate)=0;
   virtual double    SwapRateWednesdayDefault(void) const=0;
   //--- swap rate for Thursday
   virtual double    SwapRateThursday(void) const=0;
   virtual MTAPIRES  SwapRateThursday(const double rate)=0;
   virtual double    SwapRateThursdayDefault(void) const=0;
   //--- swap rate for Friday
   virtual double    SwapRateFriday(void) const=0;
   virtual MTAPIRES  SwapRateFriday(const double rate)=0;
   virtual double    SwapRateFridayDefault(void) const=0;
   //--- swap rate for Saturday
   virtual double    SwapRateSaturday(void) const=0;
   virtual MTAPIRES  SwapRateSaturday(const double rate)=0;
   virtual double    SwapRateSaturdayDefault(void) const=0;
   //--- explicit destructor is prohibited
protected:
                    ~IMTConGroupSymbol(void) {}
  };
//+------------------------------------------------------------------+
//| Group configuration interface                                    |
//+------------------------------------------------------------------+
class IMTConGroup
  {
public:
   //--- group permissions flags
   enum EnPermissionsFlags
     {
      PERMISSION_NONE              =0x00000000,  // default
      PERMISSION_CERT_CONFIRM      =0x00000001,  // certificate confirmation neccessary
      PERMISSION_ENABLE_CONNECTION =0x00000002,  // clients connections allowed
      PERMISSION_RESET_PASSWORD    =0x00000004,  // reset password after first logon
      PERMISSION_FORCED_OTP_USAGE  =0x00000008,  // forced usage OTP
      PERMISSION_RISK_WARNING      =0x00000010,  // show risk warning window on start
      PERMISSION_REGULATION_PROTECT=0x00000020,  // country-specific regulatory protection
      PERMISSION_NOTIFY_DEALS      =0x00000040,  // allow deals notification by push message
      PERMISSION_NOTIFY_ORDERS     =0x00000080,  // allow orders notification by push message
      PERMISSION_NOTIFY_BALANCES   =0x00000100,  // allow balances notification by push message
      //--- all push notifications
      PERMISSION_NOTIFY_ALL        =PERMISSION_NOTIFY_DEALS|PERMISSION_NOTIFY_ORDERS|PERMISSION_NOTIFY_BALANCES,
      //--- enumeration borders
      PERMISSION_ALL         =PERMISSION_CERT_CONFIRM     | PERMISSION_ENABLE_CONNECTION | PERMISSION_RESET_PASSWORD|
      PERMISSION_FORCED_OTP_USAGE | PERMISSION_RISK_WARNING      | PERMISSION_REGULATION_PROTECT | PERMISSION_NOTIFY_DEALS |
      PERMISSION_NOTIFY_ORDERS | PERMISSION_NOTIFY_BALANCES
     };
   //--- authorization mode
   enum EnAuthMode
     {
      AUTH_STANDARD               =0,  // standard authorization
      AUTH_RSA1024                =1,  // RSA1024 certificate
      AUTH_RSA2048                =2,  // RSA2048 certificate
      AUTH_RSA_CUSTOM             =3,  // RSA custom
      //--- enumeration borders
      AUTH_FIRST                  =AUTH_STANDARD,
      AUTH_LAST                   =AUTH_RSA2048
     };
   //--- One-Time-Password mode
   enum EnAuthOTPMode
     {
      AUTH_OTP_DISABLED           =0,
      AUTH_OTP_TOTP_SHA256        =1,
      AUTH_OTP_TOTP_SHA256_WEB    =2,
      //--- enumeration borders
      AUTH_OTP_FIRST              =AUTH_OTP_DISABLED,
      AUTH_OTP_LAST               =AUTH_OTP_TOTP_SHA256_WEB
     };
   //--- reports generation mode
   enum EnReportsMode
     {
      REPORTS_DISABLED            =0,  // reports disabled
      REPORTS_FULL                =1,  // End of Day & End of Month
      REPORTS_EOD_ONLY            =2,  // End of Day only
      REPORTS_EOM_ONLY            =3,  // End of Month only
      //--- enumeration borders
      REPORTS_FIRST               =REPORTS_DISABLED,
      REPORTS_LAST                =REPORTS_EOM_ONLY
     };
   //--- reports generation flags
   enum EnReportsFlags
     {
      REPORTSFLAGS_NONE           =0,  // none
      REPORTSFLAGS_EMAIL          =1,  // send reports through email
      REPORTSFLAGS_SUPPORT        =2,  // send reports copies on support email
      REPORTSFLAGS_STATEMENTS     =4,  // generate reports
      //--- enumeration borders
      REPORTSFLAGS_ALL            =REPORTSFLAGS_NONE|REPORTSFLAGS_EMAIL|REPORTSFLAGS_STATEMENTS
     };
   //--- news modes
   enum EnNewsMode
     {
      NEWS_MODE_DISABLED          =0,  // disable news
      NEWS_MODE_HEADERS           =1,  // enable only news headers
      NEWS_MODE_FULL              =2,  // enable full news
      //--- enumeration borders
      NEWS_MODE_FIRST             =NEWS_MODE_DISABLED,
      NEWS_MODE_LAST              =NEWS_MODE_FULL
     };
   //--- internal email modes
   enum EnMailMode
     {
      MAIL_MODE_DISABLED          =0,  // disable internal email
      MAIL_MODE_FULL              =1,  // enable internal email
      //--- enumeration borders
      MAIL_MODE_FIRST             =MAIL_MODE_DISABLED,
      MAIL_MODE_LAST              =MAIL_MODE_FULL
     };
   //--- client history limits
   enum EnHistoryLimit
     {
      TRADE_HISTORY_ALL           =0,  // unlimited
      TRADE_HISTORY_MONTHS_1      =1,  // one month
      TRADE_HISTORY_MONTHS_3      =2,  // 3 months
      TRADE_HISTORY_MONTHS_6      =3,  // 6 months
      TRADE_HISTORY_YEAR_1        =4,  // 1 year
      TRADE_HISTORY_YEAR_2        =5,  // 2 years
      TRADE_HISTORY_YEAR_3        =6,  // 3 years
      //--- enumeration borders
      TRADE_HISTORY_FIRST         =TRADE_HISTORY_ALL,
      TRADE_HISTORY_LAST          =TRADE_HISTORY_YEAR_3
     };
   //--- free margin calculation modes
   enum EnFreeMarginMode
     {
      FREE_MARGIN_NOT_USE_PL      =0,  // don't use floating profit and loss
      FREE_MARGIN_USE_PL          =1,  // use floating profit and loss
      FREE_MARGIN_PROFIT          =2,  // use floating profit only
      FREE_MARGIN_LOSS            =3,  // use floating loss only
      //--- enumeration borders
      FREE_MARGIN_FIRST           =FREE_MARGIN_NOT_USE_PL,
      FREE_MARGIN_LAST            =FREE_MARGIN_LOSS
     };
   //--- EnTransferMode
   enum EnTransferMode
     {
      TRANSFER_MODE_DISABLED      =0,
      TRANSFER_MODE_NAME          =1,
      TRANSFER_MODE_GROUP         =2,
      TRANSFER_MODE_NAME_GROUP    =3,
      //--- enumeration borders
      TRANSFER_MODE_FIRST         =TRANSFER_MODE_DISABLED,
      TRANSFER_MODE_LAST          =TRANSFER_MODE_NAME_GROUP
     };
   //--- stop-out mode
   enum EnStopOutMode
     {
      STOPOUT_PERCENT             =0,  // stop-out in percent
      STOPOUT_MONEY               =1,  // stop-out in money
      //--- enumeration borders
      STOPOUT_FIRST               =STOPOUT_PERCENT,
      STOPOUT_LAST                =STOPOUT_MONEY
     };
   //--- fixed profit calculation in free margin
   enum EnMarginFreeProfitMode
     {
      FREE_MARGIN_PROFIT_PL       =0,  // both fixed loss and profit on free margin
      FREE_MARGIN_PROFIT_LOSS     =1,  // only fixed loss on free margin
      //--- enumeration borders
      FREE_MARGIN_PROFIT_FIRST    =FREE_MARGIN_PROFIT_PL,
      FREE_MARGIN_PROFIT_LAST     =FREE_MARGIN_PROFIT_LOSS
     };
   //--- group risk management mode 
   enum EnMarginMode
     {
      MARGIN_MODE_RETAIL           =0, // Retail FX, Retail CFD, Retail Futures
      MARGIN_MODE_EXCHANGE_DISCOUNT=1, // Exchange, margin discount rates based 
      MARGIN_MODE_RETAIL_HEDGED    =2, // Retail FX, Retail CFD, Retail Futures with hedged positions
      //--- enumeration borders
      MARGIN_MODE_FIRST           =MARGIN_MODE_RETAIL,
      MARGIN_MODE_LAST            =MARGIN_MODE_RETAIL_HEDGED
     };
   //--- margin calculation flags
   enum EnMarginFlags
     {
      MARGIN_FLAGS_NONE           =0,        // none
      MARGIN_FLAGS_CLEAR_ACC      =1,        // clear accumulated profit at end of day
      //--- enumeration borders
      MARGIN_FLAGS_ALL            =MARGIN_FLAGS_CLEAR_ACC
     };
   //--- trade rights flags
   enum EnTradeFlags
     {
      TRADEFLAGS_NONE             =0x00000000,  // none
      TRADEFLAGS_SWAPS            =0x00000001,  // allow swaps charges
      TRADEFLAGS_TRAILING         =0x00000002,  // allow trailing stops
      TRADEFLAGS_EXPERTS          =0x00000004,  // allow expert advisors
      TRADEFLAGS_EXPIRATION       =0x00000008,  // allow orders expiration
      TRADEFLAGS_SIGNALS_ALL      =0x00000010,  // allow trade signals
      TRADEFLAGS_SIGNALS_OWN      =0x00000020,  // allow trade signals only from own server
      TRADEFLAGS_SO_COMPENSATION  =0x00000040,  // allow negative balance compensation after stop out
      TRADEFLAGS_SO_FULLY_HEDGED  =0x00000080,  // allow stop out fully hegded accounts
      TRADEFLAGS_FIFO_CLOSE       =0x00000100,  // allow to close positions by FIFO rule
      TRADEFLAGS_HEDGE_PROHIBIT   =0x00000200,  // prohibit hedged positions
      TRADEFLAGS_DEAL_COST        =0x00000400,  // calculate and show deals cost
      TRADEFLAGS_SO_COMPENSATION_CREDIT=0x00000800, // allow credit compensation after stop out
      //--- enumeration borders
      TRADEFLAGS_DEFAULT          =TRADEFLAGS_SWAPS|TRADEFLAGS_TRAILING|TRADEFLAGS_EXPERTS|TRADEFLAGS_EXPIRATION|TRADEFLAGS_SIGNALS_ALL,
      TRADEFLAGS_ALL              =TRADEFLAGS_SWAPS|TRADEFLAGS_TRAILING|TRADEFLAGS_EXPERTS|TRADEFLAGS_EXPIRATION|TRADEFLAGS_SIGNALS_ALL|TRADEFLAGS_SIGNALS_OWN|
      TRADEFLAGS_SO_COMPENSATION|TRADEFLAGS_SO_FULLY_HEDGED|TRADEFLAGS_FIFO_CLOSE|TRADEFLAGS_HEDGE_PROHIBIT|TRADEFLAGS_DEAL_COST|TRADEFLAGS_SO_COMPENSATION_CREDIT
     };
   //--- common methods
   virtual void      Release(void)=0;
   virtual MTAPIRES  Assign(const IMTConGroup* group)=0;
   virtual MTAPIRES  Clear(void)=0;
   //--- group name
   virtual LPCWSTR   Group(void) const=0;
   virtual MTAPIRES  Group(LPCWSTR group)=0;
   //--- group trade server ID
   virtual UINT64    Server(void) const=0;
   virtual MTAPIRES  Server(const UINT64 server)=0;
   //--- EnPermissionsFlags
   virtual UINT64    PermissionsFlags(void) const=0;
   virtual MTAPIRES  PermissionsFlags(const UINT64 flags)=0;
   //--- EnAuthMode
   virtual UINT      AuthMode(void) const=0;
   virtual MTAPIRES  AuthMode(const UINT mode)=0;
   //--- minimal password length
   virtual UINT      AuthPasswordMin(void) const=0;
   virtual MTAPIRES  AuthPasswordMin(const UINT mode)=0;
   //--- company name
   virtual LPCWSTR   Company(void) const=0;
   virtual MTAPIRES  Company(LPCWSTR company)=0;
   //--- company web page URL
   virtual LPCWSTR   CompanyPage(void) const=0;
   virtual MTAPIRES  CompanyPage(LPCWSTR page)=0;
   //--- company email
   virtual LPCWSTR   CompanyEmail(void) const=0;
   virtual MTAPIRES  CompanyEmail(LPCWSTR email)=0;
   //--- company support site URL
   virtual LPCWSTR   CompanySupportPage(void) const=0;
   virtual MTAPIRES  CompanySupportPage(LPCWSTR page)=0;
   //--- company support email
   virtual LPCWSTR   CompanySupportEmail(void) const=0;
   virtual MTAPIRES  CompanySupportEmail(LPCWSTR email)=0;
   //--- company catalog name (for reports and email templates)
   virtual LPCWSTR   CompanyCatalog(void) const=0;
   virtual MTAPIRES  CompanyCatalog(LPCWSTR catalog)=0;
   //--- deposit currency
   virtual LPCWSTR   Currency(void) const=0;
   virtual MTAPIRES  Currency(LPCWSTR currency)=0;
   virtual UINT      CurrencyDigits(void) const=0;
   //--- EnReportsMode
   virtual UINT      ReportsMode(void) const=0;
   virtual MTAPIRES  ReportsMode(const UINT mode)=0;
   //--- EnReportsFlags
   virtual UINT64    ReportsFlags(void) const=0;
   virtual MTAPIRES  ReportsFlags(const UINT64 flags)=0;
   //--- reports SMTP server address:ports 
   virtual LPCWSTR   ReportsSMTP(void) const=0;
   virtual MTAPIRES  ReportsSMTP(LPCWSTR server)=0;
   //--- reports SMTP server login
   virtual LPCWSTR   ReportsSMTPLogin(void) const=0;
   virtual MTAPIRES  ReportsSMTPLogin(LPCWSTR login)=0;
   //--- reports SMTP server password
   virtual LPCWSTR   ReportsSMTPPass(void) const=0;
   virtual MTAPIRES  ReportsSMTPPass(LPCWSTR password)=0;
   //--- EnNewsMode
   virtual UINT      NewsMode(void) const=0;
   virtual MTAPIRES  NewsMode(const UINT mode)=0;
   //--- news category filter string
   virtual LPCWSTR   NewsCategory(void) const=0;
   virtual MTAPIRES  NewsCategory(LPCWSTR category)=0;
   //--- allowed news languages (Windows API LANGID used)
   virtual MTAPIRES  NewsLangAdd(const UINT language)=0;
   virtual MTAPIRES  NewsLangUpdate(const UINT pos,const UINT language)=0;
   virtual MTAPIRES  NewsLangDelete(const UINT pos)=0;
   virtual MTAPIRES  NewsLangClear(void)=0;
   virtual UINT      NewsLangTotal(void) const=0;
   virtual UINT      NewsLangNext(const UINT pos) const=0;
   //--- EnMailMode
   virtual UINT      MailMode(void) const=0;
   virtual MTAPIRES  MailMode(const UINT mode)=0;
   //--- EnTradeFlags
   virtual UINT64    TradeFlags(void) const=0;
   virtual MTAPIRES  TradeFlags(const UINT64 flags)=0;
   //--- interest rate for free deposit money
   virtual double    TradeInterestrate(void) const=0;
   virtual MTAPIRES  TradeInterestrate(const double rate)=0;
   //--- virtual credit
   virtual double    TradeVirtualCredit(void) const=0;
   virtual MTAPIRES  TradeVirtualCredit(const double credit)=0;
   //--- EnFreeMarginMode
   virtual UINT      MarginFreeMode(void) const=0;
   virtual MTAPIRES  MarginFreeMode(const UINT freemode)=0;
   //--- EnStopOutMode
   virtual UINT      MarginSOMode(void) const=0;
   virtual MTAPIRES  MarginSOMode(const UINT level)=0;
   //--- Margin Call level value
   virtual double    MarginCall(void) const=0;
   virtual MTAPIRES  MarginCall(const double level)=0;
   //--- Sto-Out level value
   virtual double    MarginStopOut(void) const=0;
   virtual MTAPIRES  MarginStopOut(const double level)=0;
   //--- default demo accounts leverage
   virtual UINT      DemoLeverage(void) const=0;
   virtual MTAPIRES  DemoLeverage(const UINT leverage)=0;
   //--- default demo accounts deposit
   virtual double    DemoDeposit(void) const=0;
   virtual MTAPIRES  DemoDeposit(const double deposit)=0;
   //--- EnHistoryLimit
   virtual UINT      LimitHistory(void) const=0;
   virtual MTAPIRES  LimitHistory(const UINT limit)=0;
   //--- max. order limit
   virtual UINT      LimitOrders(void) const=0;
   virtual MTAPIRES  LimitOrders(const UINT limit)=0;
   //--- max. selected symbols limit
   virtual UINT      LimitSymbols(void) const=0;
   virtual MTAPIRES  LimitSymbols(const UINT limit)=0;
   //--- commissions
   virtual MTAPIRES  CommissionAdd(IMTConCommission* commission)=0;
   virtual MTAPIRES  CommissionUpdate(const UINT pos,const IMTConCommission* commission)=0;
   virtual MTAPIRES  CommissionDelete(const UINT pos)=0;
   virtual MTAPIRES  CommissionClear(void)=0;
   virtual MTAPIRES  CommissionShift(const UINT pos,const int shift)=0;
   virtual UINT      CommissionTotal(void) const=0;
   virtual MTAPIRES  CommissionNext(const UINT pos,IMTConCommission* commission) const=0;
   virtual MTAPIRES  CommissionGet(LPCWSTR name,IMTConCommission* commission) const=0;
   //--- groups symbols settings
   virtual MTAPIRES  SymbolAdd(IMTConGroupSymbol* symbol)=0;
   virtual MTAPIRES  SymbolUpdate(const UINT pos,const IMTConGroupSymbol* symbol)=0;
   virtual MTAPIRES  SymbolDelete(const UINT pos)=0;
   virtual MTAPIRES  SymbolClear(void)=0;
   virtual MTAPIRES  SymbolShift(const UINT pos,const int shift)=0;
   virtual UINT      SymbolTotal(void) const=0;
   virtual MTAPIRES  SymbolNext(const UINT pos,IMTConGroupSymbol* symbol) const=0;
   virtual MTAPIRES  SymbolGet(LPCWSTR name,IMTConGroupSymbol* symbol) const=0;
   //--- margin free profit accounting mode
   virtual UINT      MarginFreeProfitMode(void) const=0;
   virtual MTAPIRES  MarginFreeProfitMode(const UINT mode)=0;
   //--- group risk management mode - EnMarginMode
   virtual UINT      MarginMode(void) const=0;
   virtual MTAPIRES  MarginMode(const UINT mode)=0;
   //--- OTP authentication mode - EnAuthOTPMode
   virtual UINT      AuthOTPMode(void) const=0;
   virtual MTAPIRES  AuthOTPMode(const UINT mode)=0;
   //--- deposit transfer mode - EnTransferMode
   virtual UINT      TradeTransferMode(void) const=0;
   virtual MTAPIRES  TradeTransferMode(const UINT mode)=0;
   //--- margin calculation flags EnMarginFlags
   virtual UINT64    MarginFlags(void) const=0;
   virtual MTAPIRES  MarginFlags(const UINT64 flags)=0;
   //--- max. positions limit
   virtual UINT      LimitPositions(void) const=0;
   virtual MTAPIRES  LimitPositions(const UINT limit)=0;
   //--- deposit currency digits
   virtual MTAPIRES  CurrencyDigitsSet(const UINT currency_digits)=0;
   //--- reports SMTP email account
   virtual LPCWSTR   ReportsEmail(void) const=0;
   virtual MTAPIRES  ReportsEmail(LPCWSTR email)=0;
   //--- company deposit URL
   virtual LPCWSTR   CompanyDepositPage(void) const=0;
   virtual MTAPIRES  CompanyDepositPage(LPCWSTR url)=0;
   //--- company deposit URL
   virtual LPCWSTR   CompanyWithdrawalPage(void) const=0;
   virtual MTAPIRES  CompanyWithdrawalPage(LPCWSTR url)=0;
   //--- demo groups in days, orders and positions will be deleted after this period
   virtual UINT      DemoInactivityPeriod(void) const=0;
   virtual MTAPIRES  DemoInactivityPeriod(const UINT period)=0;
   //--- floating leverage profile name
   virtual LPCWSTR   MarginFloatingLeverage(void) const=0;
   virtual MTAPIRES  MarginFloatingLeverage(LPCWSTR name)=0;
   //--- explicit destructor is prohibited
protected:
                    ~IMTConGroup(void) {}
  };
//+------------------------------------------------------------------+
//| Group configuration array interface                              |
//+------------------------------------------------------------------+
class IMTConGroupArray
  {
public:
   //--- common methods
   virtual void      Release(void)=0;
   virtual MTAPIRES  Assign(const IMTConGroupArray* array)=0;
   virtual MTAPIRES  Clear(void)=0;
   //--- add
   virtual MTAPIRES  Add(IMTConGroup* record)=0;
   virtual MTAPIRES  AddCopy(const IMTConGroup* record)=0;
   virtual MTAPIRES  Add(IMTConGroupArray* array)=0;
   virtual MTAPIRES  AddCopy(const IMTConGroupArray* array)=0;
   //--- delete & detach
   virtual MTAPIRES  Delete(const UINT pos)=0;
   virtual IMTConGroup* Detach(const UINT pos)=0;
   //--- update
   virtual MTAPIRES  Update(const UINT pos,IMTConGroup* record)=0;
   virtual MTAPIRES  UpdateCopy(const UINT pos,const IMTConGroup* record)=0;
   virtual MTAPIRES  Shift(const UINT pos,const int shift)=0;
   //--- data access
   virtual UINT      Total(void) const=0;
   virtual IMTConGroup*  Next(const UINT index) const=0;
   //--- sorting and search
   virtual MTAPIRES  Sort(MTSortFunctionPtr sort_function)=0;
   virtual int       Search(const void *key,MTSortFunctionPtr sort_function) const=0;
   virtual int       SearchGreatOrEq(const void *key,MTSortFunctionPtr sort_function) const=0;
   virtual int       SearchGreater(const void *key,MTSortFunctionPtr sort_function) const=0;
   virtual int       SearchLessOrEq(const void *key,MTSortFunctionPtr sort_function) const=0;
   virtual int       SearchLess(const void *key,MTSortFunctionPtr sort_function) const=0;
   virtual int       SearchLeft(const void *key,MTSortFunctionPtr sort_function) const=0;
   virtual int       SearchRight(const void *key,MTSortFunctionPtr sort_function) const=0;
   //--- explicit destructor is prohibited
protected:
                    ~IMTConGroupArray(void) {}
  };
//+------------------------------------------------------------------+
//| Group config events notification interface                       |
//+------------------------------------------------------------------+
class IMTConGroupSink
  {
public:
   virtual void      OnGroupAdd(const IMTConGroup*    /*config*/) {  }
   virtual void      OnGroupUpdate(const IMTConGroup* /*config*/) {  }
   virtual void      OnGroupDelete(const IMTConGroup* /*config*/) {  }
   virtual void      OnGroupSync(void)                            {  }
   //--- config modification hooks (only for Server API)
   virtual MTAPIRES  HookGroupAdd(const UINT64 /*login*/,IMTConGroup* /*new_cfg*/)                               { return(MT_RET_OK); }
   virtual MTAPIRES  HookGroupUpdate(const UINT64 /*login*/,const IMTConGroup* /*cfg*/,IMTConGroup* /*new_cfg*/) { return(MT_RET_OK); }
   virtual MTAPIRES  HookGroupDelete(const UINT64 /*login*/,const IMTConGroup* /*cfg*/)                          { return(MT_RET_OK); }
  };
//+------------------------------------------------------------------+ 
