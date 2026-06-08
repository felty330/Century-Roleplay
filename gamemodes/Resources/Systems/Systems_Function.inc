FUNC:_KickPlayer(playerid) {
	Kick(playerid);
	return 1;
}

FUNC:KickBadUCPName(playerid)
{
    if(IsPlayerConnected(playerid))
        Kick(playerid);

    return 1;
}