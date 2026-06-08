PlayerLoadTime(playerid, const reason[] = "") {
	new loadtime = GetTickCount() - g_TickCount[playerid];
	new msg[128];
	format(msg, sizeof(msg), "[Character Load] %s: %dms", reason, loadtime);
	SendClientMessage(playerid, COLOR_GREEN, msg);
	return 1;
}

CreateSetupCharacter(playerid)
{
    if(!PlayerData[playerid][isLogin]) return SendError(playerid, "Kamu tidak memiliki akses untuk melakukan ini!");

    PlayerData[playerid][pPos][0] = DEFAULT_SPAWN_X;
    PlayerData[playerid][pPos][1] = DEFAULT_SPAWN_Y;
    PlayerData[playerid][pPos][2] = DEFAULT_SPAWN_Z;
    PlayerData[playerid][pPos][3] = DEFAULT_SPAWN_A;
    PlayerData[playerid][pInterior] = 0;
    PlayerData[playerid][pVirtualWorld] = 0;

    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerMoneyEx(playerid, 5000);
    SetSpawnInfo(playerid, NO_TEAM, PlayerData[playerid][pSkin],
        DEFAULT_SPAWN_X, DEFAULT_SPAWN_Y,
        DEFAULT_SPAWN_Z, DEFAULT_SPAWN_A,
        WEAPON_FIST, 0, WEAPON_FIST, 0, WEAPON_FIST, 0
    );
    SpawnPlayer(playerid);
    return 1;
}

ResetVariables(playerid)
{
    PlayerData[playerid][pID] = -1;
    PlayerData[playerid][pUCP][0] = EOS;
    PlayerData[playerid][pPassword][0] = EOS;
    PlayerData[playerid][pAdmin] = 0;
    PlayerData[playerid][pAdminDuty] = false;

    PlayerData[playerid][pCharacter] = -1;
    PlayerData[playerid][pCharName][0] = EOS;
    PlayerData[playerid][pSkin] = 0;
    PlayerData[playerid][pMoney] = 0;
    PlayerData[playerid][pLevel] = 0;

    PlayerData[playerid][pPos][0] = 0.0;
    PlayerData[playerid][pPos][1] = 0.0;
    PlayerData[playerid][pPos][2] = 0.0;
    PlayerData[playerid][pPos][3] = 0.0;
    PlayerData[playerid][pInterior] = 0;
    PlayerData[playerid][pVirtualWorld] = 0;

    PlayerData[playerid][pBirthDay] = 0;
    PlayerData[playerid][pBirthMonth] = 0;
    PlayerData[playerid][pBirthYear] = 0;
    format(PlayerData[playerid][pGender], 16, "Laki-Laki");
    PlayerData[playerid][pLastExit] = 0;
    PlayerData[playerid][pWeight] = 0;
    PlayerData[playerid][pHeight] = 0;
    PlayerData[playerid][pOrigin][0] = EOS;
    PlayerData[playerid][pCharSetup] = CHAR_NONE;

    PlayerData[playerid][isLogin] = false;
    PlayerData[playerid][pLoginAttempts] = 0;
    PlayerData[playerid][isOfficialClient] = false;

    for(new i = 0; i < MAX_CHARS; i++)
    {
        PlayerCharacter[playerid][i][0] = EOS;
    }
    ResetPlayerMoney(playerid);
    return 1;
}

CheckPlayerUCPName(playerid) {
    new check = CheckUCPName(PlayerData[playerid][pUCP]);
    if(check != UCPNAME_PASS)
    {
        ShowUCPNameWarning(playerid, check);
        return 0;
    }
    return 1;
}
stock GetAdminRank(playerid)
{
    new level[32];

    switch(PlayerData[playerid][pAdmin])
    {
        case 1: level = "Helper";
        case 2: level = "Moderator";
        case 3: level = "Admin";
        case 4: level = "Senior Admin";
        case 5: level = "Head Admin";
        case 6: level = "Owner";
        default: level = "Player";
    }
    return level;
}