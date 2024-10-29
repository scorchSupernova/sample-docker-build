
#pragma once
#include "stdafx.h"
//+------------------------------------------------------------------+
//| Manager                                                          |
//+------------------------------------------------------------------+
class CManager
{
private:
    enum              constants
    {
        MT5_CONNECT_TIMEOUT = 30000,       // connect timeout in milliseconds
    };


public:
    IMTManagerAPI* m_manager;
    CMTManagerAPIFactory m_factory;
    IMTDeal*  deal;
    IMTDealArray* m_deal_array;
    IMTUser* m_user;
    IMTAccount* m_account;
    IMTAccountArray* a_array;
    IMTUserArray* u_array;
    IMTTickSink* tick;
    IMTConSymbol* symbol;
    IMTOrder* order;
    IMTOrderArray* or_array;
    IMTPosition* pos;
    IMTPositionArray* pos_array;

    CManager(void);
    ~CManager(void);
    bool              Initialize();
    bool              Login(LPCWSTR server, UINT64 login, LPCWSTR password);
    void              Logout();
    bool              DealerBalance(const UINT64 login, const double amount, const UINT type, const LPCWSTR comment, bool deposit);
    bool              GetUserDeal(IMTDealArray*& deals, const UINT64 login, SYSTEMTIME& time_from, SYSTEMTIME& time_to);
    bool              GetUserInfo(UINT64 login);
    IMTAccount* GetAccountInfo(UINT64 login);
    std::string       convertUserToJson(const IMTUser* user,const IMTAccount* account);
    UINT             GetTotalUser();
    bool             GetUserLoginsByGroup(UINT64*& logins, UINT& logins_total);
    bool             GetAllUserLogin();
    bool             GetActiveTrades(UINT64 login);
    bool             GetPositionRequestsByLogins(const UINT64* logins, UINT logins_total);
    bool             GetAccountRequestsByLogins(const UINT64* logins, UINT logins_total);
    std::string      convertTradeDetailsToJson(const IMTPosition* position,bool isOpen);
    std::string      convertDealToJson(const IMTDeal* deal,bool isUpdate);

private:
    void              Shutdown();

};
//+------------------------------------------------------------------+
