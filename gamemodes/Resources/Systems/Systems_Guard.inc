enum E_GUARD_TYPE
{
    GUARD_UCP,
    GUARD_CHARACTER,
    GUARD_BASIC,
    GUARD_ACTION,
    GUARD_ADMIN,
    GUARD_ADMIN_DUTY
};

#define RequireGuard(%0, %1) if(!Guard(%0, %1)) return 1;

#define MIN_MONEY_AMOUNT    (1)
#define MAX_MONEY_AMOUNT    (5000000)
#define DEFAULT_MONEY_AMOUNT    (0)

stock GuardError(playerid, const message[])
{
    SendClientMessage(playerid, COLOR_RED, message);
    return 1;
}

stock bool:IsPlayerUCPLogged(playerid)
{
    return IsPlayerConnected(playerid) && PlayerData[playerid][isLogin];
}

stock bool:IsPlayerCharacterLoaded(playerid)
{
    return IsPlayerUCPLogged(playerid) && PlayerData[playerid][pID] > 0;
}

stock bool:IsPlayerAliveState(playerid)
{
    if(GetPlayerState(playerid) == PLAYER_STATE_WASTED)
        return false;

    if(PlayerData[playerid][pHealth] <= 0.0)
        return false;

    return true;
}

stock bool:Guard(playerid, E_GUARD_TYPE:type, admin_level = 1)
{
    if(!IsPlayerConnected(playerid))
        return false;

    switch(type)
    {
        case GUARD_UCP:
        {
            if(!PlayerData[playerid][isLogin])
                return GuardError(playerid, "Kamu belum login."), false;
        }
        case GUARD_CHARACTER:
        {
            if(!PlayerData[playerid][isLogin])
                return GuardError(playerid, "Kamu belum login."), false;

            if(PlayerData[playerid][pID] <= 0)
                return GuardError(playerid, "Kamu belum memilih karakter."), false;
        }
        case GUARD_BASIC:
        {
            if(!Guard(playerid, GUARD_CHARACTER))
                return false;

            if(!IsPlayerAliveState(playerid))
                return GuardError(playerid, "Kamu sedang tidak sadarkan diri."), false;
        }
        case GUARD_ACTION:
        {
            if(!Guard(playerid, GUARD_BASIC))
                return false;

            /*
                Nanti kalau variable sudah ada, tambahkan di sini:
                pCuffed, pFrozen, pInjured, pJailed, pInDialog.
            */
        }
        case GUARD_ADMIN:
        {
            if(!PlayerData[playerid][isLogin])
                return GuardError(playerid, "Kamu belum login."), false;

            if(PlayerData[playerid][pAdmin] < admin_level)
                return GuardError(playerid, "Kamu tidak memiliki akses."), false;
        }
        case GUARD_ADMIN_DUTY:
        {
            if(!PlayerData[playerid][isLogin])
                return GuardError(playerid, "Kamu belum login."), false;
            
            if(PlayerData[playerid][pAdmin] < admin_level)
                return GuardError(playerid, "Kamu tidak memiliki akses."), false;

            if(!PlayerData[playerid][pAdminDuty])
                return GuardError(playerid, "Kamu harus admin duty terlebih dahulu."), false;
        }
    }
    return true;
}

stock bool:GuardSilent(playerid, E_GUARD_TYPE:type, admin_level = 1)
{
    if(!IsPlayerConnected(playerid))
        return false;

    switch(type)
    {
        case GUARD_UCP:
        {
            return PlayerData[playerid][isLogin];
        }

        case GUARD_CHARACTER:
        {
            return PlayerData[playerid][isLogin] && PlayerData[playerid][pID] > 0;
        }

        case GUARD_BASIC, GUARD_ACTION:
        {
            return PlayerData[playerid][isLogin] &&
                PlayerData[playerid][pID] > 0 &&
                IsPlayerAliveState(playerid);
        }
        case GUARD_ADMIN:
        {
            return PlayerData[playerid][isLogin] && PlayerData[playerid][pAdmin] >= admin_level;
        }
        case GUARD_ADMIN_DUTY:
        {
            return PlayerData[playerid][isLogin] && PlayerData[playerid][pAdmin] >= admin_level && PlayerData[playerid][pAdminDuty];
        }
    }
    return false;
}

stock bool:IsValidMoneyAmount(amount)
{
    return (amount >= MIN_MONEY_AMOUNT && amount <= MAX_MONEY_AMOUNT);
}

stock bool:IsValidPlayerMoney(amount)
{
    return (amount >= DEFAULT_MONEY_AMOUNT && amount <= MAX_MONEY_AMOUNT);
}

stock SetPlayerMoneyEx(playerid, amount)
{
    if(!IsPlayerConnected(playerid))
        return 0;
    if(!IsValidPlayerMoney(amount))
        return 0;

    PlayerData[playerid][pMoney] = amount;
    ResetPlayerMoney(playerid);
    GivePlayerMoney(playerid, PlayerData[playerid][pMoney]);
    return 1;
}

stock GivePlayerMoneyEx(playerid, amount)
{
    if(!IsPlayerConnected(playerid))
        return 0;

    if(!IsValidMoneyAmount(amount))
        return 0;
    if(!IsValidPlayerMoney(PlayerData[playerid][pMoney]))
        return 0;

    if(PlayerData[playerid][pMoney] > (MAX_MONEY_AMOUNT - amount))
        return 0;

    return SetPlayerMoneyEx(playerid, PlayerData[playerid][pMoney] + amount);
}

stock TakePlayerMoneyEx(playerid, amount)
{
    if(!IsPlayerConnected(playerid))
        return 0;

    if(!IsValidMoneyAmount(amount))
        return 0;
    if(!IsValidPlayerMoney(PlayerData[playerid][pMoney]))
        return 0;

    if(PlayerData[playerid][pMoney] < amount)
        return 0;

    return SetPlayerMoneyEx(playerid, PlayerData[playerid][pMoney] - amount);
}