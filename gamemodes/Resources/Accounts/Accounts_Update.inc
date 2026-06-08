UpdateDataChars(playerid)
{
    if(!IsPlayerConnected(playerid))
      return 0;

    if(!PlayerData[playerid][isLogin])
      return 0;
    if(PlayerData[playerid][pID] < 1)
      return 0;
    
    new query[1024];
    mysql_format(c_SQL, query, sizeof(query), "UPDATE players SET \
        pos_x = %f, pos_y = %f, pos_z = %f, pos_a = %f, \
        interior = %d, virtual_world = %d, skin = %d, money = %d, \
        level = %d, last_exit = %d \
        WHERE id = %d LIMIT 1",
        PlayerData[playerid][pPos][0],
        PlayerData[playerid][pPos][1],
        PlayerData[playerid][pPos][2],
        PlayerData[playerid][pPos][3],
        PlayerData[playerid][pInterior],
        PlayerData[playerid][pVirtualWorld],
        PlayerData[playerid][pSkin],
        PlayerData[playerid][pMoney],
        PlayerData[playerid][pLevel],
        PlayerData[playerid][pLastExit],
        PlayerData[playerid][pID]
    );
    mysql_tquery(c_SQL, query);
    return 1;
}

stock SavePlayerForExit(playerid)
{
    if(!GuardSilent(playerid, GUARD_CHARACTER))
        return 0;

    GetPlayerPos(playerid,
        PlayerData[playerid][pPos][0],
        PlayerData[playerid][pPos][1],
        PlayerData[playerid][pPos][2]
    );
    GetPlayerFacingAngle(playerid, PlayerData[playerid][pPos][3]);

    PlayerData[playerid][pInterior] = GetPlayerInterior(playerid);
    PlayerData[playerid][pVirtualWorld] = GetPlayerVirtualWorld(playerid);
    PlayerData[playerid][pLastExit] = gettime();

    UpdateDataChars(playerid);
    return 1;
}

stock SaveAllCharacters()
{
    foreach(new i: Player)
    {
        SavePlayerForExit(i);
    }
    return 1;
}