/*==========================================================
                                           
                   Account Dialog Show  
                                           
==========================================================*/
stock GetOriginList(output[], size)
{
    format(output, size,
        "Indonesia\n\
America\n\
United Kingdom\n\
Japan\n\
Korea\n\
China\n\
Mexico\n\
Russia\n\
Germany\n\
France\n\
Africa"
    );
    return 1;
}

stock ShowRegisterConfirmDialog(playerid, bool:error = false)
{
    new string[512];

    if(error)
    {
        format(string, sizeof(string),
            YELLOW_E "Century Roleplay Account\n\n"\
            RED_E "Konfirmasi password tidak sama.\n\n"\
            WHITE_E "Masukkan ulang password yang sama untuk menyelesaikan register."
        );
    }
    else
    {
        format(string, sizeof(string),
            YELLOW_E "Century Roleplay Account\n\n"\
            WHITE_E "Masukkan ulang password yang sama untuk konfirmasi register."
        );
    }

    return Dialog_Show(playerid, DIALOG_REGISTER_CONFIRM, DIALOG_STYLE_PASSWORD,
        "Century: Confirm Password", string, "Confirm", "Kembali"
    );
}

stock ShowRegisterDialog(playerid, error = REGISTER_ERROR_NONE)
{
    new string[512], errorText[160];

    switch(error)
    {
        case REGISTER_ERROR_EMPTY_PASSWORD:
        {
            format(errorText, sizeof(errorText),
                RED_E "Password tidak boleh kosong.\n"
            );
        }
        case REGISTER_ERROR_SHORT_PASSWORD:
        {
            format(errorText, sizeof(errorText),
                RED_E "Password terlalu pendek.\n"\
                WHITE_E "Minimal password adalah " YELLOW_E "%d karakter" WHITE_E ".\n",
                PASSWORD_MIN_LENGTH
            );
        }
        case REGISTER_ERROR_LONG_PASSWORD:
        {
            format(errorText, sizeof(errorText),
                RED_E "Password terlalu panjang.\n"\
                WHITE_E "Maksimal password adalah " YELLOW_E "%d karakter" WHITE_E ".\n",
                PASSWORD_MAX_LENGTH
            );
        }
        case REGISTER_ERROR_NO_LETTER:
        {
            format(errorText, sizeof(errorText),
                RED_E "Password wajib mengandung minimal 1 huruf.\n"
            );
        }
        case REGISTER_ERROR_NO_NUMBER:
        {
            format(errorText, sizeof(errorText),
                RED_E "Password wajib mengandung minimal 1 angka.\n"
            );
        }
    }

    format(string, sizeof(string),
        YELLOW_E "Century Roleplay Account\n\n"\
        WHITE_E "UCP Name      : " GREEN_E "%s\n"\
        WHITE_E "Account Status: " RED_E "Belum Terdaftar\n\n"\
        "%s"\
        WHITE_E "Buat password untuk mendaftarkan UCP ini.\n"\
        GREY_E "Minimal %d karakter, maksimal %d karakter. Gunakan kombinasi huruf dan angka.",
        PlayerData[playerid][pUCP],
        errorText,
        PASSWORD_MIN_LENGTH,
        PASSWORD_MAX_LENGTH
    );

    return Dialog_Show(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD,
        "Century: Register Account", string, "Register", "Keluar"
    );
}

stock ShowLoginDialog(playerid, bool:error = false)
{
    new string[512];

    if(error)
    {
        format(string, sizeof(string),
            YELLOW_E "Century Roleplay Account\n\n"\
            WHITE_E "UCP Name      : " GREEN_E "%s\n"\
            WHITE_E "Account Status: " GREEN_E "Terdaftar\n\n"\
            RED_E "Password salah. Percobaan %d/%d.\n\n"\
            WHITE_E "Masukkan password dengan benar untuk masuk ke server.",
            PlayerData[playerid][pUCP], PlayerData[playerid][pLoginAttempts], LOGIN_MAX_ATTEMPTS
        );
    }
    else
    {
        format(string, sizeof(string),
            YELLOW_E "Century Roleplay Account\n\n"\
            WHITE_E "UCP Name      : " GREEN_E "%s\n"\
            WHITE_E "Account Status: " GREEN_E "Terdaftar\n\n"\
            WHITE_E "Masukkan password UCP kamu untuk login.\n"\
            GREY_E "Jangan pernah membagikan password ke siapapun.",
            PlayerData[playerid][pUCP]
        );
    }

    return Dialog_Show(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Century: Login Account", string, "Login", "Keluar");
}

stock ShowLoginFailedDialog(playerid)
{
    new string[512];
    format(string, sizeof(string),
        YELLOW_E "Century Roleplay Account\n\n"\
        WHITE_E "UCP Name      : " GREEN_E "%s\n"\
        WHITE_E "Account Status: " GREEN_E "Terdaftar\n\n"\
        RED_E "Kamu salah password sebanyak %d kali.\n\n"\
        GREY_E "Jika lupa password, ajukan reset password melalui Discord staff.",
        PlayerData[playerid][pUCP], LOGIN_MAX_ATTEMPTS
    );
    return Dialog_Show(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "Century: Login Failed", string, "Tutup", "");
}

stock ShowCreateCharacterDialog(playerid, bool:error = false, const error_text[] = "")
{
    new string[512];

    if(error)
    {
        format(string, sizeof(string),
            YELLOW_E "Create Character\n\n"\
            RED_E "%s\n\n"\
            WHITE_E "Format nama wajib: Firstname_Lastname\n"\
            GREY_E "Contoh: Labian_Lasan",
            error_text
        );
    }
    else
    {
        format(string, sizeof(string),
            YELLOW_E "Create Character\n\n"\
            WHITE_E "Masukkan nama character baru.\n\n"\
            WHITE_E "Format nama wajib: Firstname_Lastname\n"\
            GREY_E "Contoh: Labian_Lasan"
        );
    }
    PlayerData[playerid][pCharSetup] = CHAR_NAME;

    return Dialog_Show(playerid, DIALOG_CREATE_CHARACTER, DIALOG_STYLE_INPUT, "Century: Character Name", string, "Lanjut", "Kembali");
}

stock ShowBirthDateDialog(playerid, bool:error = false)
{
    new string[512];
    PlayerData[playerid][pCharSetup] = CHAR_BIRTHDATE;
    
    if(error)
    {
        format(string, sizeof(string),
            YELLOW_E "Character Identity\n\n"\
            RED_E "Tanggal lahir tidak valid.\n\n"\
            WHITE_E "Gunakan format: DD/MM/YYYY\n"\
            GREY_E "Contoh: 16/01/2005"
        );
    }
    else
    {
        format(string, sizeof(string),
            YELLOW_E "Character Identity\n\n"\
            WHITE_E "Masukkan tanggal lahir character.\n\n"\
            WHITE_E "Format: DD/MM/YYYY\n"\
            GREY_E "Contoh: 16/01/2005"
        );
    }

    return Dialog_Show(playerid, DIALOG_AGE, DIALOG_STYLE_INPUT, "Century: Birth Date", string, "Lanjut", "Kembali");
}

stock ShowOriginDialog(playerid)
{
    new list[192];
    GetOriginList(list, sizeof(list));
    return Dialog_Show(playerid, DIALOG_ORIGIN, DIALOG_STYLE_LIST, "Century: Character Origin", list, "Pilih", "Kembali");
}

stock ShowWeightDialog(playerid, bool:error = false)
{
    new string[512];

    if(error)
    {
        format(string, sizeof(string),
            YELLOW_E "Character Body\n\n"\
            RED_E "Berat badan tidak valid.\n\n"\
            WHITE_E "Range berat badan: " YELLOW_E "40 - 200 kg\n"\
            GREY_E "Masukkan angka saja. Contoh: 70"
        );
    }
    else
    {
        format(string, sizeof(string),
            YELLOW_E "Character Body\n\n"\
            WHITE_E "Masukkan berat badan character.\n\n"\
            WHITE_E "Range: " YELLOW_E "40 - 200 kg\n"\
            GREY_E "Masukkan angka saja. Contoh: 70"
        );
    }

    return Dialog_Show(playerid, DIALOG_WEIGHT, DIALOG_STYLE_INPUT, "Century: Weight", string, "Lanjut", "Kembali");
}

stock ShowHeightDialog(playerid, bool:error = false)
{
    new string[512];

    if(error)
    {
        format(string, sizeof(string),
            YELLOW_E "Character Body\n\n"\
            RED_E "Tinggi badan tidak valid.\n\n"\
            WHITE_E "Range tinggi badan: " YELLOW_E "140 - 220 cm\n"\
            GREY_E "Masukkan angka saja. Contoh: 175"
        );
    }
    else
    {
        format(string, sizeof(string),
            YELLOW_E "Character Body\n\n"\
            WHITE_E "Masukkan tinggi badan character.\n\n"\
            WHITE_E "Range: " YELLOW_E "140 - 220 cm\n"\
            GREY_E "Masukkan angka saja. Contoh: 175"
        );
    }

    return Dialog_Show(playerid, DIALOG_HEIGHT, DIALOG_STYLE_INPUT, "Century: Height", string, "Lanjut", "Kembali");
}

stock ShowGenderDialog(playerid)
{
    return Dialog_Show(playerid, DIALOG_GENDER, DIALOG_STYLE_LIST, "Century: Gender", "Male\nFemale", "Pilih", "Kembali");
}