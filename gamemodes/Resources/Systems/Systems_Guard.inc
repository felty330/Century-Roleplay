#define MIN_MONEY_AMOUNT            (1)
#define MAX_MONEY_AMOUNT            (5000000)
#define DEFAULT_MONEY_AMOUNT        (0)
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
    if(!IsValidPlayerMoney(playerid))
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
    if(!IsValidPlayerMoney(playerid))
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
    if(!IsValidPlayerMoney(playerid))
        return 0;

    if(PlayerData[playerid][pMoney] < amount)
        return 0;

    return SetPlayerMoneyEx(playerid, PlayerData[playerid][pMoney] - amount);
}


