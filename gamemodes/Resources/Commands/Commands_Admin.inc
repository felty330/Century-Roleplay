CMD:a(playerid, params[])
{
	if(!PlayerData[playerid][isLogin] || PlayerData[playerid][pID] < 1)
		return SendError(playerid, "Kamu harus login dan memilih character terlebih dahulu!");

	if(PlayerData[playerid][pAdmin] < 1)
		return PermissionError(playerid);

	if(isnull(params))
		return SendSyntax(playerid, "/a <text>");

	if(strlen(params) > 128)
		return SendError(playerid, "Pesan terlalu banyak(Maksimal 128 kata)");

	new name[MAX_PLAYER_NAME + 1],
		message[192],
		line[96],
		split = -1;

	GetPlayerName(playerid, name, sizeof(name));

	if(strlen(params) > 70)
	{
		split = strfind(params, " ", false, 60);

		if(split == -1 || split > 70)
			split = 70;

		format(line, sizeof(line), "%s", params[split + 1]);
		params[split] = EOS;
	}

	format(message, sizeof(message), ""RED_E"[Admin Chat] %d "LG_E"%s: "LB_E"%s",
		GetAdminRank(playerid),
		PlayerData[playerid][pCharName],
		playerid,
		params
	);

	foreach(new i : Player)
	{
		if(PlayerData[i][isLogin] && PlayerData[i][pAdmin] > 0)
			SendClientMessage(i, COLOR_LB, message);
	}

	if(split != -1 && line[0] != EOS)
	{
		foreach(new i : Player)
		{
			if(PlayerData[i][isLogin] && PlayerData[i][pAdmin] > 0)
				SendClientMessage(i, COLOR_LB, line);
		}
	}
	return 1;
}

CMD:setadminlevel(playerid, params[])
{
	if(!PlayerData[playerid][isLogin] || PlayerData[playerid][pID] < 1)
		return SendError(playerid, "Kamu harus login dan memilih character terlebih dahulu!");
	if(PlayerData[playerid][pAdmin] < 6)
		return SendError(playerid, "Kamu tidak ada izin untuk menggunakan perintah ini!.");

	new level, otherid;
	if(sscanf(params, "ud", otherid, level))
		return SendSyntax(playerid, "/setadminlevel <playerid/name> <level 0 - 6>");

	if(!IsPlayerConnected(otherid))
		return SendError(playerid, "Player tidak ada di kota/offline");

	if(!PlayerData[otherid][isLogin] || PlayerData[otherid][pID] < 1)
		return SendError(playerid, "Player tersebut belum masuk kota.");

	if(level < 0 || level > 6)
		return SendError(playerid, "Level admin hanya 0 sampai 6");

	PlayerData[otherid][pAdmin] = level;

	new query[128];
	mysql_format(c_SQL, query, sizeof(query), "UPDATE ucp SET admin = %d WHERE ucp_name = '%e' LIMIT 1",
		level,
		PlayerData[otherid][pUCP]
	);
	mysql_tquery(c_SQL, query);

	SendServer(playerid, "Kamu mengubah level admin %s menjadi %d.", PlayerData[otherid][pUCP], otherid, level);

	SendServer(otherid, "%s mengubah level admin kamu menjadi %d.", PlayerData[otherid][pUCP], playerid, level);
	return 1;
}

CMD:goto(playerid, params[])
{
	if(!PlayerData[playerid][isLogin] || PlayerData[playerid][pID] < 1)
		return SendError(playerid, "Kamu harus login terlebih dahulu.");

	if(PlayerData[playerid][pAdmin] < 1)
		return PermissionError(playerid);

	new otherid;

	if(sscanf(params, "u", otherid))
		return SendSyntax(playerid, "/goto <playerid/nama>");

	if(!IsPlayerConnected(otherid))
		return SendError(playerid, "Player tidak online.");

	if(!PlayerData[otherid][isLogin] || PlayerData[otherid][pID] < 1)
		return SendError(otherid, "Player tersebut belum login.");

	new Float:x, Float:y, Float:z;
	GetPlayerPos(otherid, x, y, z);

	SetPlayerInterior(playerid, GetPlayerInterior(otherid));
	SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(otherid));
	SetPlayerPos(playerid, x + 1.0, y + 1.0, z);

	SendServer(playerid, "Kamu teleport ke %s.", PlayerData[otherid][pCharName], otherid);
	return 1;
}

CMD:gethere(playerid, params[])
{
	if(!PlayerData[playerid][isLogin] || PlayerData[playerid][pID] < 1)
		return SendError(playerid, "Kamu harus login terlebih dahulu.");

	if(PlayerData[playerid][pAdmin] < 2)
		return PermissionError(playerid);

	new otherid;

	if(sscanf(params, "u", otherid))
		return SendSyntax(playerid, "/gethere <playerid/nama>");

	if(!IsPlayerConnected(otherid))
		return SendError(playerid, "Player tidak ada di kota/offline");

	if(!PlayerData[otherid][isLogin] || PlayerData[otherid][pID] < 1)
		return SendError(otherid, "Player tersebut belum login.");

	if(otherid == playerid)
		return SendError(playerid, "Tidak bisa menarik diri sendiri!");

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);

	SetPlayerInterior(otherid, GetPlayerInterior(playerid));
	SetPlayerVirtualWorld(otherid, GetPlayerVirtualWorld(playerid));
	SetPlayerPos(otherid, x + 1.0, y + 1.0, z);

	SendServer(playerid, "Kamu menarik %s ke posisimu", PlayerData[otherid][pCharName], otherid);
	SendServer(playerid, "%s menarik kamu ke posisinya", PlayerData[playerid][pCharName], playerid);
	return 1;
}

CMD:setskin(playerid, params[])
{
	if(!PlayerData[playerid][isLogin] || PlayerData[playerid][pID] < 1)
		return SendError(playerid, "Kamu harus login terlebih dahulu.");

	if(PlayerData[playerid][pAdmin] < 3)
		return PermissionError(playerid);

	new otherid, skinid;

	if(sscanf(params,"ud", otherid, skinid))
		return SendSyntax(playerid, "/setskin <playerid/nama> <skinid>");

	if(!IsPlayerConnected(otherid))
		return SendError(playerid, "Player tidak online.");

	if(!PlayerData[otherid][isLogin] || PlayerData[otherid][pID] < 1)
		return SendError(playerid, "Player tersebut belum login.");

	if(skinid < 0 || skinid > 311)
		return SendError(playerid, "Skin ID tidak valid!.");

	PlayerData[otherid][pSkin] = skinid;
	SetPlayerSkin(playerid, skinid);

	SendServer(playerid, "Kamu mengubah skin %s menjadi %d", PlayerData[otherid][pCharName], skinid);
	SendServer(otherid, "%s mengubah skin kamu menjadi %d", PlayerData[playerid][pCharName], skinid);
	return 1;
}

CMD:setmoney(playerid, params[])
{
	if(!PlayerData[playerid][isLogin] || PlayerData[playerid][pID] < 1)
		return SendError(playerid, "Kamu harus login terlebih dahulu.");

	if(PlayerData[playerid][pAdmin] < 5)
		return PermissionError(playerid);
	
	new otherid, amount;

	if(sscanf(params, "ud", otherid, amount))
		return SendSyntax(playerid, "/setmoney <playerid/nama> <amount>");

	if(!IsPlayerConnected(otherid))
		return SendError(playerid, "Player tidak online.");

	if(amount < MIN_MONEY_AMOUNT || amount > MAX_MONEY_AMOUNT)
		return SendError(playerid, "Jumlah uang tidak valid!.");

	SetPlayerMoneyEx(otherid, amount);
	SendServer(playerid, "Kamu mengubah jumlah uang %s menjadi $%d", PlayerData[otherid][pCharName], amount);
	SendServer(otherid, "%s mengubah jumlah uang kamu menjadi $%d", PlayerData[playerid][pCharName], amount);
	return 1;
}

CMD:revive(playerid, params[])
{
	if(!PlayerData[playerid][isLogin] || PlayerData[playerid][pID] < 1)
		return SendError(playerid, "Kamu harus login terlebih dahulu.");

	if(PlayerData[playerid][pAdmin] < 2)
		return PermissionError(playerid);

	new otherid;

	if(sscanf(params, "u", otherid))
		return SendSyntax(playerid, "/revive <playerid/nama>");

	if(!IsPlayerConnected(otherid))
		return SendError(playerid, "Player tidak online.");

	if(!PlayerData[otherid][isLogin] || PlayerData[otherid][pID] < 1)
		return SendError(playerid, "Player tersebut belum login.");

	SetPlayerHealth(otherid, 100.0);
	SendServer(playerid, "Kamu revive %s.", PlayerData[otherid][pCharName]);
	SendServer(playerid, "%s revive kamu.", PlayerData[playerid][pCharName]);
	return 1;
}

CMD:freeze(playerid, params[])
{
	if(!PlayerData[playerid][isLogin] || PlayerData[playerid][pID] < 1)
		return SendError(playerid, "Kamu harus login terlebih dahulu.");

	if(PlayerData[playerid][pAdmin] < 2)
		return PermissionError(playerid);

	new otherid;

	if(sscanf(params, "u", otherid))
		return SendSyntax(playerid, "/freeze <playerid/nama>");

	if(!IsPlayerConnected(otherid))
		return SendError(playerid, "Player tidak online.");

	if(!PlayerData[otherid][isLogin] || PlayerData[otherid][pID] < 1)
		return SendError(playerid, "Player tersebut belum login.");

	TogglePlayerControllable(otherid, false);
	SendServer(playerid, "Kamu freeze %s.", PlayerData[otherid][pCharName]);
	SendServer(otherid, "Admin/Staff %s freeze kamu.", PlayerData[playerid][pCharName]);
	return 1;
}

CMD:unfreeze(playerid, params[])
{
	if(!PlayerData[playerid][isLogin] || PlayerData[playerid][pID] < 1)
		return SendError(playerid, "Kamu harus login terlebih dahulu.");

	if(PlayerData[playerid][pAdmin] < 2)
		return PermissionError(playerid);

	new otherid;

	if(sscanf(params, "u", otherid))
		return SendSyntax(playerid, "/unfreeze <playerid/nama>");

	if(!IsPlayerConnected(otherid))
		return SendError(playerid, "Player tidak online.");

	if(!PlayerData[otherid][isLogin] || PlayerData[otherid][pID] < 1)
		return SendError(playerid, "Player tersebut belum login.");

	TogglePlayerControllable(otherid, true);
	SendServer(playerid, "Kamu unfreeze %s.", PlayerData[otherid][pCharName]);
	SendServer(otherid, "Admin/Staff %s unfreeze kamu.", PlayerData[playerid][pCharName]);
	return 1;
}

CMD:slap(playerid, params[])
{
	if(!PlayerData[playerid][isLogin] || PlayerData[playerid][pID] < 1)
		return SendError(playerid, "Kamu harus login terlebih dahulu.");

	if(PlayerData[playerid][pAdmin] < 2)
		return PermissionError(playerid);

	new otherid;

	if(sscanf(params, "u", otherid))
		return SendSyntax(playerid, "/slap <playerid/nama>");

	if(!IsPlayerConnected(otherid))
		return SendError(playerid, "Player tidak online.");

	if(!PlayerData[otherid][isLogin] || PlayerData[otherid][pID] < 1)
		return SendError(playerid, "Player tersebut belum login.");

	new Float:x, Float:y, Float:z;
	GetPlayerPos(otherid, x, y, z);
	SetPlayerPos(otherid, x, y + 3.0, z);

	SendServer(playerid, "Kamu slap %s.", PlayerData[otherid][pCharName]);
	return 1;
}

CMD:kick(playerid, params[])
{
	if(!PlayerData[playerid][isLogin] || PlayerData[playerid][pID] < 1)
		return SendError(playerid, "Kamu harus login terlebih dahulu.");

	if(PlayerData[playerid][pAdmin] < 2)
		return PermissionError(playerid);

	new otherid, reason[64];

	if(sscanf(params, "u", otherid, reason))
		return SendSyntax(playerid, "/kick <playerid/nama> <reason>");

	if(!IsPlayerConnected(otherid))
		return SendError(playerid, "Player tidak online.");

	if(!PlayerData[otherid][isLogin] || PlayerData[otherid][pID] < 1)
		return SendError(playerid, "Player tersebut belum login.");

	if(otherid == playerid)
		return SendError(playerid, "Tidak bisa kick diri sendiri.");
	if(PlayerData[otherid][pAdmin] >= PlayerData[playerid][pAdmin])
		return SendError(playerid, "Kamu tidak bisa kick admin dengan level yang sama atau level tinggi.");

	SendClientMessageToAllEx(COLOR_RED, "SERVER: %s telah ditendang oleh admin %s. Reason: %s", PlayerData[otherid][pCharName], PlayerData[playerid][pCharName], reason);

	KickEx(otherid);
	return 1;
}