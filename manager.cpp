
#include "stdafx.h"
#include "Manager.h"
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
CManager::CManager(void) : m_manager(NULL), m_deal_array(NULL), m_user(NULL), m_account(NULL), u_array(NULL)
{
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
CManager::~CManager(void)
{
    Shutdown();
}
//+------------------------------------------------------------------+
//| Initialize library                                               |
//+------------------------------------------------------------------+
bool CManager::Initialize()
{
    MTAPIRES  res = MT_RET_OK_NONE;
    CMTStr256 message;
    //--- loading manager API
    if ((res = m_factory.Initialize(NULL)) != MT_RET_OK)
    {
        message.Format(L"Loading manager API failed (%u)", res);
        return(false);
    }
    //--- creating manager interface
    if ((res = m_factory.CreateManager(MTManagerAPIVersion, &m_manager)) != MT_RET_OK)
    {
        m_factory.Shutdown();
        message.Format(L"Creating manager interface failed (%u)", res);
        return(false);
    }
    //--- create deal array
    if (!(deal = m_manager->DealCreate()))
    {
        m_manager->LoggerOut(MTLogErr, L"DealCreateArray fail");
        return(false);
    }
    //--- create deal array
    if (!(m_deal_array = m_manager->DealCreateArray()))
    {
        m_manager->LoggerOut(MTLogErr, L"DealCreateArray fail");
        return(false);
    }
    //--- create user interface
    if (!(m_user = m_manager->UserCreate()))
    {
        m_manager->LoggerOut(MTLogErr, L"UserCreate fail");
        return(false);
    }
    //--- create account interface
    if (!(m_account = m_manager->UserCreateAccount()))
    {
        m_manager->LoggerOut(MTLogErr, L"UserCreateAccount fail");
        return(false);
    }
    //--- create an object of an array of accounts
    if (!(a_array = m_manager->UserCreateAccountArray()))
    {
        m_manager->LoggerOut(MTLogErr, L"UserCreateAccounts fail");
        return(false);
    }
    //--- Create an object of an array of client records.
    if (!(u_array = m_manager->UserCreateArray()))
    {
        m_manager->LoggerOut(MTLogErr, L"UserCreate Array fail");
        return(false);
    }
    //--- Create symbol interface
    if (!(symbol = m_manager->SymbolCreate())) {
        m_manager->LoggerOut(MTLogErr, L"SymbolCreate fail");
        return(false);
    }
    //--- Create position  interface
    if (!(pos = m_manager->PositionCreate())) {
        m_manager->LoggerOut(MTLogErr, L"Position create fail");
        return(false);
    }
    //--- Create position array  interface
    if (!(pos_array = m_manager->PositionCreateArray())) {
        m_manager->LoggerOut(MTLogErr, L"Position array create fail");
        return(false);
    }


    //--- all right
    return(true);
}
//+------------------------------------------------------------------+
//| Login                                                            |
//+------------------------------------------------------------------+
bool CManager::Login(LPCWSTR server, UINT64 login, LPCWSTR password)
{
    //--- connect
    MTAPIRES res = m_manager->Connect(server, login, password, NULL,
        IMTManagerAPI::PUMP_MODE_SYMBOLS |
        IMTManagerAPI::PUMP_MODE_GROUPS |
        IMTManagerAPI::PUMP_MODE_USERS |
        IMTManagerAPI::PUMP_MODE_ACTIVITY |
        IMTManagerAPI::PUMP_MODE_ORDERS |
        IMTManagerAPI::PUMP_MODE_POSITIONS |
        IMTManagerAPI::PUMP_MODE_FULL,
        MT5_CONNECT_TIMEOUT);
    if (res != MT_RET_OK)
    {
        m_manager->LoggerOut(MTLogErr, L"Connection failed (%u)", res);
        return(false);
    }

    std::wcout << "Inited Server: " << server << " login: " << login << std::endl;
    return(true);
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
void CManager::Logout()
{
    //--- disconnect manager
    if (m_manager)
        m_manager->Disconnect();
}
//+------------------------------------------------------------------+
//| Shutdown                                                         |
//+------------------------------------------------------------------+
void CManager::Shutdown()
{
    if (m_deal_array)
    {
        m_deal_array->Release();
        m_deal_array = NULL;
    }
    if (m_manager)
    {
        m_manager->Release();
        m_manager = NULL;
    }
    if (m_user)
    {
        m_user->Release();
        m_user = NULL;
    }
    if (m_account)
    {
        m_account->Release();
        m_account = NULL;
    }
    m_factory.Shutdown();
}
//+------------------------------------------------------------------+
//| Get array of dealer balance operation                            |
//+------------------------------------------------------------------+
bool CManager::GetUserDeal(IMTDealArray*& deals, const UINT64 login, SYSTEMTIME& time_from, SYSTEMTIME& time_to)
{
    //--- request array
    MTAPIRES res = m_manager->DealRequest(login, SMTTime::STToTime(time_from), SMTTime::STToTime(time_to), m_deal_array);
    if (res != MT_RET_OK)
    {
        m_manager->LoggerOut(MTLogErr, L"DealRequest fail(%u)", res);
        return(false);
    }
    //---
    deals = m_deal_array;
    return(true);
}
//+------------------------------------------------------------------+
//| Get user info string                                             |
//+------------------------------------------------------------------+
bool CManager::GetUserInfo(UINT64 login)
{

    std::wstringstream str;
    //--- request user from server
    m_user->Clear();
    MTAPIRES res = m_manager->UserRequest(login, m_user);
    if (res != MT_RET_OK)
    {
        m_manager->LoggerOut(MTLogErr, L"UserRequest error (%u)", res);
        return(false);
    }

    //--- format string
    str << "login: " << m_user->Login() << " Name: " << m_user->Name() << " Balance: " << m_user->Balance() << " Credit: " << m_user->Credit();
    //std::wcout << str.str() << std::endl;

    return(true);
}
//+------------------------------------------------------------------+
//| Get user info string                                             |
//+------------------------------------------------------------------+
IMTAccount* CManager::GetAccountInfo(UINT64 login)
{
    std::stringstream str;
    //--- request account from server
    m_account->Clear();
    MTAPIRES res = m_manager->UserAccountRequest(login, m_account);
    if (res != MT_RET_OK)
    {
        m_manager->LoggerOut(MTLogErr, L"UserAccountRequest error (%u)", res);
        return nullptr;
    }

    //--- format string
    str << "Login: " << m_account->Login() << " Balance: " << m_account->Balance() << " Equity: " << m_account->Equity() << " Margin: " << m_account->Margin();
    //std::cout << str.str() << std::endl;

    return m_account;
}
//+------------------------------------------------------------------+
//| Dealer operation                                                 |
//+------------------------------------------------------------------+
bool CManager::DealerBalance(const UINT64 login, const double amount, const UINT type, const LPCWSTR comment, bool deposit)
{
    UINT64 deal_id = 0;
    //--- dealer operation
    MTAPIRES res = m_manager->DealerBalance(login, deposit ? amount : -amount, type, comment, deal_id);
    if (res != MT_RET_REQUEST_DONE)
    {
        m_manager->LoggerOut(MTLogErr, L"DealerBalance failed (%u)", res);
        return(false);
    }
    //---
    return(true);
}
//+------------------------------------------------------------------+
//| Get total user                                                   |
//+------------------------------------------------------------------+
UINT CManager::GetTotalUser() {
    UINT res = m_manager->UserTotal();
    return res;
}
//+------------------------------------------------------------------+
//| Get All User under group                                       |
//+------------------------------------------------------------------+
bool CManager::GetUserLoginsByGroup(UINT64*& logins, UINT& logins_total) {

    MTAPIRES res = m_manager->UserLogins(m_user->Group(), logins, logins_total);

    if (res != MT_RET_OK)
    {
        m_manager->LoggerOut(MTLogErr, L"UserLogins failed (%u)", res);
        return(false);
    }

    return true;

}
//+------------------------------------------------------------------+
//| Get User all user under a login                                  |
//+------------------------------------------------------------------+
bool CManager::GetAllUserLogin() {

    MTAPIRES res = m_manager->UserGetByGroup(L"*", u_array);

    if (res != MT_RET_OK) {
        m_manager->LoggerOut(MTLogErr, L"User get by group failed (%u)", res);
        return(false);
    }

    return true;
}

bool CManager::GetPositionRequestsByLogins(const UINT64* logins, UINT logins_total) {

    MTAPIRES res = m_manager->PositionRequestByLogins(logins, logins_total, pos_array);

    if (res != MT_RET_OK) {
        m_manager->LoggerOut(MTLogErr, L"Position get by users login failed (%u)", res);
        return(false);
    }

    /*
    for (UINT i = 0; i < pos_array->Total(); i++) {
        const IMTPosition* position = pos_array->Next(i); // get the position at index i
        std::cout << "id: " << position->Position() << std::endl;
    }
    */

    return true;
}


bool CManager::GetAccountRequestsByLogins(const UINT64* logins, UINT logins_total) {

    MTAPIRES res = m_manager->UserAccountRequestByLogins(logins, logins_total, a_array);

    if (res != MT_RET_OK) {
        m_manager->LoggerOut(MTLogErr, L"Accounts get by users login failed (%u)", res);
        return(false);
    }

    return true;

}


//+------------------------------------------------------------------+
//| Get all active trade                                             |
//+------------------------------------------------------------------+
bool CManager::GetActiveTrades(UINT64 login) {
    // get all open positions of the specified login
    MTAPIRES res = m_manager->PositionGet(login, pos_array);

    if (res != MT_RET_OK) {
        m_manager->LoggerOut(MTLogErr, L"Failed to get active trades (%u)", res);
        return false;
    }

    return true;
}

//+------------------------------------------------------------------+
//| Convert User data to json                                        |
//+------------------------------------------------------------------+

std::string  CManager::convertUserToJson(const IMTUser* user, const IMTAccount* account)
{
    nlohmann::json j = {
     {"balance", account->Balance()},
     {"equity", account->Equity()},
     //{"group", std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>().to_bytes(user->Group())},
     //{"last_day_balance", user->BalancePrevDay()},
     //{"last_day_equity", user->EquityPrevDay()},
    // {"last_month_balance", user->BalancePrevMonth()},
     //{"last_month_equity", user->EquityPrevMonth()}
    };

    return j.dump();
}

std::string CManager::convertTradeDetailsToJson(const IMTPosition* position, bool isOpen) {
    
    std::chrono::time_point<std::chrono::system_clock> timestamp = std::chrono::system_clock::now();
    std::time_t timestamp_seconds = std::chrono::system_clock::to_time_t(timestamp);
    std::tm* ptm = std::gmtime(&timestamp_seconds);
    char buffer[32];
    std::strftime(buffer, 32, "%Y.%m.%d %H:%M:%S", ptm);

    std::string open_time_str;
    std::time_t open_time_t = static_cast<std::time_t>(position->TimeCreate());
    std::tm open_time_tm = *std::localtime(&open_time_t); // convert to local time
    std::stringstream open_time_ss;
    open_time_ss << std::put_time(&open_time_tm, "%Y.%m.%d %H:%M:%S");

    std::tm close_time_tm = *std::localtime(&timestamp_seconds); // convert to local time
    std::stringstream close_time_ss;

    if (isOpen) {
        close_time_ss << "1970.01.01 00:00:00";
    }
    else {
        close_time_ss << std::put_time(&close_time_tm, "%Y.%m.%d %H:%M:%S");
    }

    std::string close_time_str = close_time_ss.str(); // get string representation of close time


    double pips;

    if (position->Action() == 1) {
        pips = (position->PriceOpen() - position->PriceCurrent()) * pow(10, position->Digits());
    }
    else {
        pips = (position->PriceCurrent() - position->PriceOpen()) * pow(10, position->Digits());
    }

    std::stringstream ss;
    ss << "{"
        << "\"object\": {"
        << "\"close_price\": " << position->PriceCurrent() << ","
        << "\"close_time\": " << (isOpen ? 0 : timestamp_seconds) << ","
        << "\"close_time_str\": \"" << close_time_ss.str() << "\","
        << "\"commission\": " << 0 << "," // jani na
        << "\"digits\": " << position->Digits() << ","
        << "\"login\": " << position->Login() << ","
        << "\"lots\": " << position->Volume() / 10000 << ","
        << "\"open_price\": " << position->PriceOpen() << ","
        << "\"open_time\": " << position->TimeCreate() << ","
        << "\"open_time_str\": \"" << open_time_ss.str() << "\","
        << "\"pips\": " << pips << "," // jani na
        << "\"profit\": " << position->Profit() << ","
        << "\"reason\": " << position->Reason() << ","
        << "\"sl\": " << position->PriceSL() << ","
        << "\"state\": " << 0 << "," // jani na
        << "\"swap\": " << position->Storage() << ","
        << "\"symbol\": \"" << std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>().to_bytes(position->Symbol()) << "\","
        << "\"ticket\": " << position->Position() << ","
        << "\"tp\": " << position->PriceTP() << ","
        << "\"type\": " << position->Action() << ","
        << "\"type_str\": \"" << (position->Action() == 1 ? "Sell" : "Buy") << "\","
        << "\"volume\": " << position->Volume() / 100
        << "},"
        << "\"type\": \"order\""
        << "}";


    return ss.str();
}

std::string CManager::convertDealToJson(const IMTDeal* deal,bool isUpdate) {

    std::chrono::time_point<std::chrono::system_clock> timestamp = std::chrono::system_clock::now();
    std::time_t timestamp_seconds = std::chrono::system_clock::to_time_t(timestamp);
    std::tm* ptm = std::gmtime(&timestamp_seconds);
    char buffer[32];
    std::strftime(buffer, 32, "%Y.%m.%d %H:%M:%S", ptm);

    std::string open_time_str;
    std::time_t open_time_t = static_cast<std::time_t>(deal->Time());
    std::tm open_time_tm = *std::localtime(&open_time_t); // convert to local time
    std::stringstream open_time_ss;
    open_time_ss << std::put_time(&open_time_tm, "%Y.%m.%d %H:%M:%S");

    std::tm close_time_tm = *std::localtime(&timestamp_seconds); // convert to local time
    std::stringstream close_time_ss;


     close_time_ss << std::put_time(&close_time_tm, "%Y.%m.%d %H:%M:%S");
    std::string close_time_str = close_time_ss.str(); // get string representation of close time


    double pips;

    if (deal->Action() == 1) {
        pips = (deal->Price() - deal->PricePosition()) * pow(10, deal->Digits());
    }
    else {
        pips = (deal->PricePosition() - deal->Price()) * pow(10, deal->Digits());
    }

    std::stringstream ss;
    ss << "{"
        << "\"object\": {"
        << "\"close_price\": " << deal->PricePosition() << ","
        << "\"close_time\": " << timestamp_seconds << ","
        << "\"close_time_str\": \"" << close_time_ss.str() << "\","
        << "\"commission\": " << deal->Commission() << ","
        << "\"digits\": " << deal->Digits() << ","
        << "\"login\": " << deal->Login() << ","
        << "\"lots\": " << deal->Volume() / 10000 << ","
        << "\"open_price\": " << deal->Price() << ","
        << "\"open_time\": " << deal->Time() << ","
        << "\"open_time_str\": \"" << open_time_ss.str() << "\","
        << "\"pips\": " << pips << ","
        << "\"profit\": " << deal->Profit() << ","
        << "\"reason\": " << deal->Reason() << ","
        << "\"sl\": " << deal->PriceSL() << ","
        << "\"state\": " << 0 << ","
        << "\"swap\": " << deal->Storage() << ","
        << "\"symbol\": \"" << std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>().to_bytes(deal->Symbol()) << "\","
        << "\"ticket\": " << (isUpdate ? deal->Deal() : deal->PositionID()) << ","
        << "\"tp\": " << deal->PriceTP() << ","
        << "\"type\": " << deal->Action() << ","
        << "\"type_str\": \"" << (deal->Action() == 1 ? "Sell" : "Buy") << "\","
        << "\"volume\": " << deal->Volume() / 100
        << "},"
        << "\"type\": \"order\""
        << "}";


    return ss.str();
}