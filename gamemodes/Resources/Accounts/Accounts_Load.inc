FUNC:OnCharacterLoaded(playerid, race_check)
{
    if(race_check != g_MysqlRaceCheck[playerid]) return KickEx(playerid);

    new rows = cache_num_rows();
    if(rows > MAX_CHARS) rows = MAX_CHARS;

    for(new i = 0; i < MAX_CHARS; i++)
    {
        PlayerCharacter[playerid][i][0] = EOS;
    }

    for(new k = 0; k < rows; k++)
    {
        cache_get_value_name(k, "username", PlayerCharacter[playerid][k]);
    }
    
    SendClientMessage(playerid, COLOR_WHITE, "");
    SendClientMessage(playerid, COLOR_WHITE, "");
    SendClientMessage(playerid, COLOR_WHITE, "");
    SendClientMessage(playerid, COLOR_WHITE, "");
    SendClientMessage(playerid, COLOR_WHITE, "");
    SendClientMessage(playerid, COLOR_WHITE, "");
    SendClientMessage(playerid, COLOR_WHITE, "");
    SendClientMessage(playerid, COLOR_WHITE, "");
    SendClientMessage(playerid, COLOR_WHITE, "");
    SendClientMessage(playerid, COLOR_WHITE, "");
    SendClientMessage(playerid, COLOR_WHITE, "");
    SendClientMessage(playerid, COLOR_WHITE, "");
    SendClientMessage(playerid, COLOR_WHITE, "");
    SendClientMessage(playerid, COLOR_WHITE, "");
    SendClientMessage(playerid, COLOR_WHITE, "");
    SendClientMessage(playerid, COLOR_WHITE, "");
    SendClientMessage(playerid, COLOR_WHITE, "");
    SendClientMessage(playerid, COLOR_WHITE, "");
    SendClientMessage(playerid, COLOR_WHITE, "");
    SendClientMessage(playerid, COLOR_WHITE, "");

    ShowCharacterList(playerid);
    return 1;
}

FUNC:OnCharacterDataLoaded(playerid, race_check)
{
    if(!IsPlayerConnected(playerid))
      return 0;
    if(race_check != g_MysqlRaceCheck[playerid]) return KickEx(playerid);
    new ip[120];
    PlayerLoadTime(playerid, "Success Load Character");

    if(cache_num_rows() < 1)
    {
        SendError(playerid, "Character tidak ditemukan atau bukan milik UCP kamu.");
        return KickEx(playerid);
    }

    cache_get_value_name_int(0, "id", PlayerData[playerid][pID]);
    if(PlayerData[playerid][pID] < 1)
    {
        SendError(playerid, "Akun ini tidak terdaftar dalam database.");
        return KickEx(playerid);
    }

    cache_get_value_name(0, "username", PlayerData[playerid][pCharName], MAX_PLAYER_NAME + 1);
    GetPlayerIp(playerid, ip, sizeof(ip));
    cache_get_value_name(0, "ip", PlayerData[playerid][pIP]);
    cache_get_value_name_int(0, "admin", PlayerData[playerid][pAdmin]);
    cache_get_value_name_float(0, "pos_x", PlayerData[playerid][pPos][0]);
    cache_get_value_name_float(0, "pos_y", PlayerData[playerid][pPos][1]);
    cache_get_value_name_float(0, "pos_z", PlayerData[playerid][pPos][2]);
    cache_get_value_name_float(0, "pos_a", PlayerData[playerid][pPos][3]);

    cache_get_value_name_int(0, "interior", PlayerData[playerid][pInterior]);
    cache_get_value_name_int(0, "virtual_world", PlayerData[playerid][pVirtualWorld]);
    cache_get_value_name_int(0, "skin", PlayerData[playerid][pSkin]);
    cache_get_value_name_int(0, "money", PlayerData[playerid][pMoney]);
    cache_get_value_name_int(0, "level", PlayerData[playerid][pLevel]);
    cache_get_value_name_int(0, "last_exit", PlayerData[playerid][pLastExit]);
    cache_get_value_name_int(0, "weight", PlayerData[playerid][pWeight]);
    cache_get_value_name_int(0, "height", PlayerData[playerid][pHeight]);
    cache_get_value_name(0, "origin", PlayerData[playerid][pOrigin]);
    cache_get_value_name_int(0, "birth_day", PlayerData[playerid][pBirthDay]);
    cache_get_value_name_int(0, "birth_month", PlayerData[playerid][pBirthMonth]);
    cache_get_value_name_int(0, "birth_year", PlayerData[playerid][pBirthYear]);
    cache_get_value_name(0, "gender", PlayerData[playerid][pGender]);

    PlayerData[playerid][isLogin] = true;
    PlayerData[playerid][pCharSetup] = CHAR_NONE;

    SetPlayerInterior(playerid, PlayerData[playerid][pInterior]);
    SetPlayerVirtualWorld(playerid, PlayerData[playerid][pVirtualWorld]);
    SetPlayerScore(playerid, PlayerData[playerid][pLevel]);
    SetPlayerMoneyEx(playerid, PlayerData[playerid][pMoney]);
    SetTimerEx("ApplyCharacterName", 300, false, "d", playerid);
    SetSpawnInfo(playerid, NO_TEAM, PlayerData[playerid][pSkin],
        PlayerData[playerid][pPos][0],
        PlayerData[playerid][pPos][1],
        PlayerData[playerid][pPos][2],
        PlayerData[playerid][pPos][3],
        WEAPON_FIST, 0, WEAPON_FIST, 0, WEAPON_FIST, 0
    );
    SpawnPlayer(playerid);

    return 1;
}
