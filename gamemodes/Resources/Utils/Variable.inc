//		[Enum]		//
enum E_PLAYER_DATA
{
	pID,

	pUCP[MAX_PLAYER_NAME + 1],
	pPassword[BCRYPT_HASH_LENGTH],
	pAdmin,
	bool:pAdminDuty,
	pIP[16],

	//Characters
	pCharacter,
	pCharName[MAX_PLAYER_NAME + 1],
	pTempCharName[MAX_PLAYER_NAME + 1],
	Float:pPos[4],
	pLastExit,
	Float:pHealth,
	Float:pArmour,
	pInterior,
	pVirtualWorld,
	pSkin,
	pMoney,
	pLevel,
	pBirthDay,
	pBirthMonth,
	pBirthYear,
	pGender[16],
	pOrigin[32],
	pWeight,
	pHeight,
	pGuns[13],
	pAmmo[13],
	pWeapon,

	//Temp
	bool:isLogin,
	bool:isOfficialClient,
	bool:pWeaponDirty,
	pLoginAttempts,
	pCharSetup
};
new PlayerData[MAX_PLAYERS][E_PLAYER_DATA];
new g_MysqlRaceCheck[MAX_PLAYERS];
new PlayerCharacter[MAX_PLAYERS][MAX_CHARS][MAX_PLAYER_NAME + 1];
new g_TickCount[MAX_PLAYERS];

//Restart
new bool:g_ServerRestarting = false;