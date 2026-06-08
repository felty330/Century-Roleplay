/*==========================================================
										
							Dialog 	
										
==========================================================*/

enum
{
    DIALOG_UNUSED,
    DIALOG_REGISTER,
    DIALOG_REGISTER_CONFIRM,
    DIALOG_CHARLIST,
    DIALOG_CREATE_CHARACTER,
    DIALOG_LOGIN,
    DIALOG_AGE,
    DIALOG_ORIGIN,
    DIALOG_GENDER,
    DIALOG_WEIGHT,
    DIALOG_HEIGHT,

    //Validate
    DIALOG_IMPORTANT_UCP
};

Dialog:DIALOG_UNUSED(playerid, response, listitem, inputtext[]) {
	if(response) return 1;
	return 1;
}

Dialog:DIALOG_REGISTER(playerid, response, listitem, inputtext[]) {
	if(!response) return KickEx(playerid);
	new passwordErr = GetRegisterPasswordErr(inputtext);

	if(passwordErr != REGISTER_ERROR_NONE) {
		ShowRegisterDialog(playerid, passwordErr);
		return 1;
	}

	bcrypt_hash(playerid, "OnPasswordHashed", inputtext, BCRYPT_COST);
	return 1;
}

Dialog:DIALOG_LOGIN(playerid, response, listitem, inputtext[])
{
    if(!response) return KickEx(playerid);
    if(PlayerData[playerid][pPassword][0] == EOS) return KickEx(playerid);

    bcrypt_verify(playerid, "OnPasswordVerify", inputtext, PlayerData[playerid][pPassword]);
    return 1;
}

Dialog:DIALOG_CHARLIST(playerid, response, listitem, inputtext[]) {
	if(!response) return KickEx(playerid);
	if(listitem < 0 || listitem >= MAX_CHARS)
		return KickEx(playerid);

	if(PlayerCharacter[playerid][listitem][0] == EOS)
		return ShowCreateCharacterDialog(playerid);

	strcopy(PlayerData[playerid][pCharName], PlayerCharacter[playerid][listitem]);

	StartTickCount(playerid);

	new query[160];
	mysql_format(c_SQL, query, sizeof(query), "SELECT * FROM players WHERE username = '%e' AND ucp = '%e' LIMIT 1", PlayerCharacter[playerid][listitem], PlayerData[playerid][pUCP]);
	mysql_tquery(c_SQL, query, "OnCharacterDataLoaded", "dd", playerid, g_MysqlRaceCheck[playerid]);
	return 1;
}

Dialog:DIALOG_CREATE_CHARACTER(playerid, response, listitem, inputtext[])
{
    if(!response)
        return CheckUserCharacter(playerid);

    if(PlayerData[playerid][pCharSetup] != CHAR_NAME)
        return KickEx(playerid);

    new check = CheckCharacterName(inputtext);

    if(check != CHARSNAME_PASS)
        return ShowCharacterNameWarning(playerid, check);

    format(PlayerData[playerid][pTempCharName], MAX_PLAYER_NAME + 1, "%s", inputtext);

    g_MysqlRaceCheck[playerid]++;

    new query[160];
    mysql_format(c_SQL, query, sizeof(query),
        "SELECT id FROM players WHERE username = '%e' LIMIT 1",
        PlayerData[playerid][pTempCharName]
    );

    mysql_tquery(c_SQL, query, "OnUsernameChecked", "dsd",
        playerid,
        PlayerData[playerid][pTempCharName],
        g_MysqlRaceCheck[playerid]
    );

    return 1;
}

Dialog:DIALOG_AGE(playerid, response, listitem, inputtext[])
{
    if(!response) return ShowCreateCharacterDialog(playerid);
    if(PlayerData[playerid][pCharSetup] != CHAR_BIRTHDATE)
      return KickEx(playerid);
    ParseBirthDate(playerid, inputtext);
    return 1;
}

Dialog:DIALOG_ORIGIN(playerid, response, listitem, inputtext[])
{
    if(!response) {
        PlayerData[playerid][pCharSetup] = CHAR_BIRTHDATE;
        return ShowBirthDateDialog(playerid);
    }
    
    if(PlayerData[playerid][pCharSetup] != CHAR_ORIGIN)
        return KickEx(playerid);

    if(listitem < 0 || listitem >= 9)
        return KickEx(playerid);

    format(PlayerData[playerid][pOrigin], 32, "%s", inputtext);
    PlayerData[playerid][pCharSetup] = CHAR_WEIGHT;
    return ShowWeightDialog(playerid);
}

Dialog:DIALOG_WEIGHT(playerid, response, listitem, inputtext[])
{
    if(!response)
    {
        PlayerData[playerid][pCharSetup] = CHAR_ORIGIN;
        return ShowOriginDialog(playerid);
    }
    if(PlayerData[playerid][pCharSetup] != CHAR_WEIGHT)
      return KickEx(playerid);
    new weight = strval(inputtext);
    if(weight < 40 || weight > 200)
    {
        return ShowWeightDialog(playerid, true);
    }

    PlayerData[playerid][pWeight] = weight;
    PlayerData[playerid][pCharSetup] = CHAR_HEIGHT;
    return ShowHeightDialog(playerid);
}

Dialog:DIALOG_HEIGHT(playerid, response, listitem, inputtext[])
{
    if(!response)
    {
        PlayerData[playerid][pCharSetup] = CHAR_WEIGHT;
        return ShowWeightDialog(playerid);
    }
    if(PlayerData[playerid][pCharSetup] != CHAR_HEIGHT)
      return KickEx(playerid);

    new height = strval(inputtext);
    if(height < 140 || height > 220)
    {
        return ShowHeightDialog(playerid, true);
    }

    PlayerData[playerid][pHeight] = height;
    PlayerData[playerid][pCharSetup] = CHAR_GENDER;
    return ShowGenderDialog(playerid);
}

Dialog:DIALOG_GENDER(playerid, response, listitem, inputtext[])
{
    if(!response)
    {
        PlayerData[playerid][pCharSetup] = CHAR_HEIGHT;
        return ShowHeightDialog(playerid);
    }
    if(PlayerData[playerid][pCharSetup] != CHAR_GENDER)
      return KickEx(playerid);
    switch(listitem)
    {
        case 0:
        {
            format(PlayerData[playerid][pGender], 16, "Laki-Laki");
            PlayerData[playerid][pSkin] = 26;
            PlayerData[playerid][pCharSetup] = CHAR_FINISHED;
            CreateCharacterAccount(playerid);
        }
        case 1:
        {
            format(PlayerData[playerid][pGender], 16, "Perempuan");
            PlayerData[playerid][pSkin] = 12;
            PlayerData[playerid][pCharSetup] = CHAR_FINISHED;
            CreateCharacterAccount(playerid);
        }
        default:
        {
            return KickEx(playerid);
        }
    }
    return 1;
}

Dialog:DIALOG_WARNING_UCP(playerid, response, listitem, inputtext[])
{
    SetTimerEx("KickBadUCPName", 300, false, "d", playerid);
    return 1;
}
