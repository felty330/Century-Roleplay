#include <open.mp>
#include <a_mysql>
#include <samp_bcrypt>
#include <easyDialog>
#include <streamer>
#include <sscanf2>
#include <Pawn.CMD>
#include <YSI_Data\y_iterate>

//		[Modules]		//
#include "Resources\Utils\Header"

main() {}

//		[Core]		//
#include "Resources\Header"

public OnGameModeInit() {
	new gm[64];
	format(gm,  sizeof(gm), "game.mode %s", TEXT_GAMEMODE);
	SendRconCommand(gm);
	DatabaseConnect();
	DisableInteriorEnterExits();
	ShowPlayerMarkers(PLAYER_MARKERS_MODE_STREAMED);
	return 1;
}

public OnGameModeExit() {
	mysql_close(c_SQL);
	return 1;
}

public OnPlayerConnect(playerid) {
	g_MysqlRaceCheck[playerid]++;
	ResetVariables(playerid);

	//Validate
	GetPlayerName(playerid, PlayerData[playerid][pUCP], MAX_PLAYER_NAME);
	if(!CheckPlayerUCPName(playerid))
		return 0;

	if(IsPlayerUsingOfficialClient(playerid)) {
		PlayerData[playerid][isOfficialClient] = true;
	}
	SetPlayerCameraPos(playerid, 155.3337, -1776.4384, 14.8978);
	SetPlayerCameraLookAt(playerid, 156.2734, -1776.0850, 14.2128);
	InterpolateCameraLookAt(playerid, 156.2734, -1776.0850, 14.2128, 156.2713, -1776.0797, 14.7078, 5000, CAMERA_MOVE);

	new query[160];
    mysql_format(c_SQL, query, sizeof(query), "SELECT admin, reason, ban_date FROM bans WHERE ucp = '%e' LIMIT 1", PlayerData[playerid][pUCP]);
    mysql_tquery(c_SQL, query, "OnBanChecked", "dd", playerid, g_MysqlRaceCheck[playerid]);
	return 1;
}

public OnPlayerDisconnect(playerid, reason) {
	g_MysqlRaceCheck[playerid]++;
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
	ResetVariables(playerid);
	return 1;
} 

public OnPlayerRequestClass(playerid, classid)
{
    return 1;
}

public OnPlayerSpawn(playerid) {
	if(!IsPlayerConnected(playerid)) {
		TogglePlayerControllable(playerid, true);
		return 0;
	}


	SetPlayerInterior(playerid, PlayerData[playerid][pInterior]);
	SetPlayerVirtualWorld(playerid, PlayerData[playerid][pVirtualWorld]);
	SetPlayerSkin(playerid, PlayerData[playerid][pSkin]);
	SetPlayerScore(playerid, PlayerData[playerid][pLevel]);
	return 1;
}

public OnPlayerDeath(playerid) {

	return 1;
}


public OnPlayerRequestSpawn(playerid)
{
	if(!IsPlayerConnected(playerid)) {
		SendError(playerid, "Kamu tidak di izinkan menekan tombol spawn!.");
		return 0;
	}

	if(!PlayerData[playerid][isLogin])
		return 0;

	if(PlayerData[playerid][pID] < 1) {
		SendError(playerid, "Kamu belum memilih karakter!.");
		return 0;
	}
	return 1;
}

public OnPlayerText(playerid, text[])
{	
	if(!IsPlayerConnected(playerid))
		return 0;
	if(!PlayerData[playerid][isLogin] || PlayerData[playerid][pID] < 1)
		return 0;
	if(isnull(text)) return 0;
	new msg[256];
	format(msg, sizeof(msg), "%s [%d]: %s", PlayerData[playerid][pCharName], playerid, text);

	SendNearbyMessage(playerid, LOCAL_CHAT_RADIUS, COLOR_WHITE, msg);
	return 0;
}

public OnPlayerEnterCheckpoint(playerid) {
	return 1;
}

public OnPlayerCommandPerformed(playerid, cmd[], params[], result, flags) {
	if(!IsPlayerConnected(playerid)) {
		SendError(playerid, "Kamu harus login dan memilih character terlebih dahulu!!");
		return KickEx(playerid);
	}
	if(result == -1)
    {
        SendClientMessage(playerid, COLOR_YELLOW, "[Server]{ffffff} Perintah tidak ditemukan, /help untuk melihat perintah yang ada!");
        return 0;
    }
	return 1;
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ) {
	if(!IsPlayerConnected(playerid))
		return SendError(playerid, "Kamu belum siap menggunakan fitur ini!");
	if(PlayerData[playerid][pAdmin] < 2) {
		SendError(playerid, "Kamu bukan admin/staff");
	} else {
		SetPlayerPos(playerid, fX, fY+2.0, fZ);
		SendClientMessage(playerid, COLOR_YELLOW, "[Server] Kamu melakukan teleportasi melalui map!");
	}
	return 1;
}
