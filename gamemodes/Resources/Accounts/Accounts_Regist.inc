
FUNC:OnBanChecked(playerid, race_check)
{
    if(race_check != g_MysqlRaceCheck[playerid]) return KickEx(playerid);

    if(cache_num_rows() > 0)
    {
        new reason[128], admin[24], ban_date;
        cache_get_value_name(0, "reason", reason);
        cache_get_value_name(0, "admin", admin);
        cache_get_value_name_int(0, "ban_date", ban_date);

        new string[512];
        format(string, sizeof(string), "{ffffff}Akun UCP kamu ({04d400}%s{ffffff}) telah diblokir permanen dari server.\n\nAdmin: {bd0000}%s\n{ffffff}Alasan: {bd0000}%s\n{ffffff}Timestamp: {bd0000}%d\n\nJika ini adalah kesalahan, silahkan hubungi admin di Discord.", PlayerData[playerid][pUCP], admin, reason, ban_date);
        Dialog_Show(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "ACCOUNT BANNED", string, "Close", "");
        return KickEx(playerid);
    }

    new query[160];
    mysql_format(c_SQL, query, sizeof(query), "SELECT id, admin, password FROM ucp WHERE ucp_name = '%e' LIMIT 1", PlayerData[playerid][pUCP]);
    mysql_tquery(c_SQL, query, "OnUCPChecked", "dd", playerid, g_MysqlRaceCheck[playerid]);
    return 1;
}


FUNC:OnUCPChecked(playerid, race_check)
{
    if(race_check != g_MysqlRaceCheck[playerid]) return KickEx(playerid);
    
    if(cache_num_rows() > 0)
    {
        cache_get_value_name_int(0, "admin", PlayerData[playerid][pAdmin]);
        cache_get_value_name(0, "password", PlayerData[playerid][pPassword]);

        ShowLoginDialog(playerid);
    }
    else
    {
        ShowRegisterDialog(playerid);
    }
    return 1;
}

FUNC:OnPasswordHashed(playerid)
{
    if(!IsPlayerConnected(playerid))
      return 0;
    bcrypt_get_hash(PlayerData[playerid][pPassword], BCRYPT_HASH_LENGTH);
    new query[256];
    mysql_format(c_SQL, query, sizeof(query), "INSERT INTO ucp (ucp_name, password) VALUES ('%e', '%e')", PlayerData[playerid][pUCP], PlayerData[playerid][pPassword]);
    mysql_tquery(c_SQL, query, "OnUCPRegistered", "dd", playerid, g_MysqlRaceCheck[playerid]);
    return 1;
}

FUNC:OnUCPRegistered(playerid, race_check)
{
    if(!IsPlayerConnected(playerid))
      return 0;
    if(race_check != g_MysqlRaceCheck[playerid]) return KickEx(playerid);

    if(mysql_errno(c_SQL) == ERROR_DUPLICATE) {
      SendError(playerid, "Nama UCP sudah digunakan. Silahkan reconnect dan gunakan nama lain.");
      return KickEx(playerid);
    }
    
    if(cache_affected_rows() < 1)
    {
        SendError(playerid, "Register gagal. Silahkan reconnect dan coba lagi.");
        return KickEx(playerid);
    }

    CheckUserCharacter(playerid);
    return 1;
}

FUNC:OnPasswordVerify(playerid, bool:success)
{
    if(!IsPlayerConnected(playerid))
      return 0;
    if(success)
    {
        CheckUserCharacter(playerid);
        return 1;
    }


    PlayerData[playerid][pLoginAttempts]++;

    if(PlayerData[playerid][pLoginAttempts] >= LOGIN_MAX_ATTEMPTS)
    {
        ShowLoginFailedDialog(playerid);
        return KickEx(playerid);
    }

    ShowLoginDialog(playerid, true);
    return 1;
}

FUNC:CheckUserCharacter(playerid) {
    g_MysqlRaceCheck[playerid]++;

    new query[160];
    mysql_format(c_SQL, query, sizeof(query), "SELECT username FROM players WHERE ucp = '%e' LIMIT %d", PlayerData[playerid][pUCP], MAX_CHARS);
    mysql_tquery(c_SQL, query, "OnCharacterLoaded", "dd", playerid, g_MysqlRaceCheck[playerid]);
    return 1;
}

FUNC:ShowCharacterList(playerid) {
    new name[268], str[64], count;

    name[0] = EOS;
    for(new i = 0; i < MAX_CHARS; i++)
    {
        if(PlayerCharacter[playerid][i][0] != EOS)
        {
            format(str, sizeof(str), "%s\n", PlayerCharacter[playerid][i]);
            strcat(name, str);
            count++;
        }
    }

    if(count < MAX_CHARS)
    {
        strcat(name, "< Buat Karakter >");
    }

    Dialog_Show(playerid, DIALOG_CHARLIST, DIALOG_STYLE_LIST, "CCR: Character Slot", name, "Pilih", "Keluar");
    return 1;
}

FUNC:OnUsernameChecked(playerid, const name[], race_check)
{
    if(!IsPlayerConnected(playerid))
        return 0;

    if(race_check != g_MysqlRaceCheck[playerid])
        return 0;

    if(cache_num_rows() > 0)
    {
        PlayerData[playerid][pTempCharName][0] = EOS;
        return ShowCreateCharacterDialog(playerid, true, "Nama character sudah digunakan.");
    }

    format(PlayerData[playerid][pCharName], MAX_PLAYER_NAME, "%s", name);
    PlayerData[playerid][pTempCharName][0] = EOS;
    ShowBirthDateDialog(playerid);
    return 1;
}

FUNC:CreateCharacterAccount(playerid)
{
    if(PlayerData[playerid][pCharSetup] != CHAR_FINISHED)
      return KickEx(playerid);
    
    new query[512];
    mysql_format(c_SQL, query, sizeof(query), "INSERT INTO players (username, ucp, skin, gender, birth_day, birth_month, birth_year, weight, height, origin, pos_x, pos_y, pos_z, pos_a, interior, virtual_world) VALUES ('%e', '%e', %d, '%e', %d, %d, %d, %d, %d, '%e', %f, %f, %f, %f, %d, %d)",
        PlayerData[playerid][pCharName],
        PlayerData[playerid][pUCP],
        PlayerData[playerid][pSkin],
        PlayerData[playerid][pGender],
        PlayerData[playerid][pBirthDay],
        PlayerData[playerid][pBirthMonth],
        PlayerData[playerid][pBirthYear],
        PlayerData[playerid][pWeight],
        PlayerData[playerid][pHeight],
        PlayerData[playerid][pOrigin],
        DEFAULT_SPAWN_X,
        DEFAULT_SPAWN_Y,
        DEFAULT_SPAWN_Z,
        DEFAULT_SPAWN_A,
        0,
        0
    );
    mysql_tquery(c_SQL, query, "OnInsertCharacter", "dd", playerid, g_MysqlRaceCheck[playerid]);
    return 1;
}

FUNC:OnInsertCharacter(playerid, race_check)
{
    if(!IsPlayerConnected(playerid))
      return 0;
      
    if(race_check != g_MysqlRaceCheck[playerid]) return KickEx(playerid);
    
    if(mysql_errno(c_SQL) == ERROR_DUPLICATE) {
      SendError(playerid, "Nama UCP sudah digunakan. Silahkan gunakan nama lain.");
      return KickEx(playerid);
    }
    if(cache_affected_rows() < 1)
    {
        SendError(playerid, "Gagal membuat character. Silahkan coba lagi.");
        return CheckUserCharacter(playerid);
    }
    PlayerData[playerid][pID] = cache_insert_id();
    if(PlayerData[playerid][pID] < 1)
    {
        SendError(playerid, "Gagal membuat character. Silahkan coba lagi.");
        return KickEx(playerid);
    }

    SetPlayerName(playerid, PlayerData[playerid][pCharName]);
    
    PlayerData[playerid][isLogin] = true;
    PlayerData[playerid][pCharSetup] = CHAR_NONE;
    CreateSetupCharacter(playerid);
    return 1;
}
