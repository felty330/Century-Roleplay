#define LOGIN_MAX_ATTEMPTS 3
#define PASSWORD_MIN_LENGTH             (8)
#define PASSWORD_MAX_LENGTH             (64)
#define NAME_MIN_PART_LENGTH            (3)
#define NAME_MAX_PART_LENGTH            (12)

enum
{
    REGISTER_ERROR_NONE,
    REGISTER_ERROR_EMPTY_PASSWORD,
    REGISTER_ERROR_SHORT_PASSWORD,
    REGISTER_ERROR_LONG_PASSWORD,
    REGISTER_ERROR_NO_LETTER,
    REGISTER_ERROR_NO_NUMBER,
    
    //Characters Name Validasi
    CHARSNAME_PASS,
    CHARSNAME_BADWORD,
    CHARSNAME_SYMBOL,
    CHARSNAME_SHORT,
    CHARSNAME_LONG,
    CHARSNAME_NO_UNDERSCORE,
    CHARSNAME_MULTI_UNDERSCORE,
    CHARSNAME_FIRST_SHORT,
    CHARSNAME_FIRST_LONG,
    CHARSNAME_LAST_SHORT,
    CHARSNAME_LAST_LONG,

    //UCP Name Validasi
    UCPNAME_PASS,
    UCPNAME_BADWORD,
    UCPNAME_SYMBOL,
    UCPNAME_SHORT,
    UCPNAME_LONG,
    UCPNAME_HAS_UNDERSCORE,

    //Validasi
    CHAR_NONE,
    CHAR_NAME,
    CHAR_BIRTHDATE,
    CHAR_GENDER,
    CHAR_ORIGIN,
    CHAR_WEIGHT,
    CHAR_HEIGHT,
    CHAR_FINISHED
}

stock IsNameBadWord(const name[])
{
    static const blocked[][] =
    {
        "admin", "administrator", "owner", "founder", "developer", "dev",
        "staff", "helper", "moderator", "support", "management",
        "server", "system", "console", "security", "anticheat", "ccr",

        "hacker", "cheater", "cheat", "aimbot", "wallhack",
        "injector", "inject", "sobeit", "modmenu", "exploit",

        "kontol", "kontl", "kntl", "memek", "mmk",
        "ngentot", "entot", "ngocok", "coli",
        "anjing", "ajg", "babi", "bangsat", "bajingan",
        "goblok", "tolol", "bego", "idiot",
        "jancok", "jancuk", "asu", "kampret", "bacot",
        "lonte", "pecun", "pelacur", "perek",

        "nigga", "nigger", "hitler", "nazi", "isis",
        "terrorist", "teroris",

        "loli", "pedo", "pedofil",

        "police", "polisi", "fbi", "cia",
        "army", "military", "president",
        "governor", "walikota", "bupati",

        "test", "tester", "unknown",
        "anonymous", "noname", "guest"
    };

    for(new i; i < sizeof(blocked); i++)
    {
        if(strfind(name, blocked[i], true) != -1)
            return 1;
    }

    return 0;
}

stock CheckUCPName(const name[])
{
    new len = strlen(name);

    if(!len)
        return UCPNAME_SHORT;

    if(len < 3)
        return UCPNAME_SHORT;

    if(len > 24)
        return UCPNAME_LONG;

    if(IsNameBadWord(name))
        return UCPNAME_BADWORD;

    for(new i; i < len; i++)
    {
        if(name[i] == '_')
            return UCPNAME_HAS_UNDERSCORE;

        if((name[i] >= 'A' && name[i] <= 'Z') ||
           (name[i] >= 'a' && name[i] <= 'z') ||
           (name[i] >= '0' && name[i] <= '9'))
        {
            continue;
        }

        return UCPNAME_SYMBOL;
    }

    return UCPNAME_PASS;
}

stock CheckCharacterName(const name[])
{
    new len = strlen(name);
    new underscoreCount, underscorePos = -1;

    if(!len)
        return CHARSNAME_SHORT;

    if(len < 5)
        return CHARSNAME_SHORT;

    if(len > 24)
        return CHARSNAME_LONG;

    if(IsNameBadWord(name))
        return CHARSNAME_BADWORD;

    for(new i; i < len; i++)
    {
        if(name[i] == '_')
        {
            underscoreCount++;
            underscorePos = i;

            if(underscoreCount > 1)
                return CHARSNAME_MULTI_UNDERSCORE;

            continue;
        }

        if(!((name[i] >= 'A' && name[i] <= 'Z') ||
             (name[i] >= 'a' && name[i] <= 'z')))
        {
            return CHARSNAME_SYMBOL;
        }
    }

    if(!underscoreCount)
        return CHARSNAME_NO_UNDERSCORE;

    if(underscorePos < 2 || (len - underscorePos - 1) < 2)
        return CHARSNAME_NO_UNDERSCORE;

    new firstNameLength = underscorePos;
    new lastNameLength = len - underscorePos - 1;

    if(firstNameLength < 3)
        return CHARSNAME_FIRST_SHORT;
    if(firstNameLength > 12)
        return CHARSNAME_FIRST_LONG;

    if(lastNameLength < 3)
        return CHARSNAME_LAST_SHORT;
    if(lastNameLength > 12)
        return CHARSNAME_LAST_LONG;

    return CHARSNAME_PASS;
}

stock GetUCPNameWarningText(reason, output[], size = sizeof(output))
{
    switch(reason)
    {
        case UCPNAME_BADWORD:
            format(output, size, "Nama mengandung kata yang tidak diperbolehkan.");

        case UCPNAME_SYMBOL:
            format(output, size, "Nama hanya boleh menggunakan huruf yang valid.");

        case UCPNAME_SHORT:
            format(output, size, "Nama terlalu pendek.");

        case UCPNAME_LONG:
            format(output, size, "Nama terlalu panjang.");

        case UCPNAME_HAS_UNDERSCORE:
            format(output, size, "Nama UCP tidak boleh menggunakan underscore (_).");

        default:
            format(output, size, "Nama tidak valid.");
    }

    return 1;
}

stock GetCharsNameWarningText(reason, output[], size = sizeof(output))
{
    switch(reason)
    {
        case CHARSNAME_BADWORD:
            format(output, size, "Nama mengandung kata yang tidak diperbolehkan.");

        case CHARSNAME_SYMBOL:
            format(output, size, "Nama hanya boleh menggunakan huruf yang valid.");

        case CHARSNAME_SHORT:
            format(output, size, "Nama terlalu pendek.");

        case CHARSNAME_LONG:
            format(output, size, "Nama terlalu panjang.");

        case CHARSNAME_NO_UNDERSCORE:
            format(output, size, "Gunakan format Firstname_Lastname.");

        case CHARSNAME_MULTI_UNDERSCORE:
            format(output, size, "Nama hanya boleh memiliki satu underscore.");

        case CHARSNAME_FIRST_SHORT:
            format(output, size, "Nama depan terlalu pendek.");

        case CHARSNAME_FIRST_LONG:
            format(output, size, "Nama depan terlalu panjang.");

        case CHARSNAME_LAST_SHORT:
            format(output, size, "Nama belakang terlalu pendek.");

        case CHARSNAME_LAST_LONG:
            format(output, size, "Nama belakang terlalu panjang.");

        default:
            format(output, size, "Nama tidak valid.");
    }

    return 1;
}


stock ShowCharacterNameWarning(playerid, reason)
{
    new text[144];

    GetCharsNameWarningText(reason, text);
    return ShowCreateCharacterDialog(playerid, true, text);
}

stock ShowUCPNameWarning(playerid, reason)
{
    new text[512], reasonText[144];

    GetUCPNameWarningText(reason, reasonText);

    format(text, sizeof(text),
        "{FFFFFF}Nama akun UCP kamu tidak dapat digunakan di server ini.\n\n"\
        "{FFCC00}Alasan:\n"\
        "{FFFFFF}%s\n\n"\
        "{AAAAAA}Gunakan nama akun yang sopan, normal, dan tanpa underscore.\n"\
        "Silakan ganti nama UCP terlebih dahulu sebelum masuk kembali.",
        reasonText
    );

    Dialog_Show(playerid, DIALOG_WARNING_UCP, DIALOG_STYLE_MSGBOX,
        "{FF3333}Peringatan - UCP Ditolak",
        text,
        "Keluar", "");

    return 1;
}

GetRegisterPasswordErr(const password[])
{
    new len = strlen(password);

    if(len == 0)
        return REGISTER_ERROR_EMPTY_PASSWORD;

    if(len < PASSWORD_MIN_LENGTH)
        return REGISTER_ERROR_SHORT_PASSWORD;

    if(len > PASSWORD_MAX_LENGTH)
        return REGISTER_ERROR_LONG_PASSWORD;

    new bool:hasLetter = false;
    new bool:hasNumber = false;

    for(new i = 0; i < len; i++)
    {
        if(('a' <= password[i] <= 'z') || ('A' <= password[i] <= 'Z'))
        {
            hasLetter = true;
        }

        if('0' <= password[i] <= '9')
        {
            hasNumber = true;
        }
    }

    if(!hasLetter)
        return REGISTER_ERROR_NO_LETTER;

    if(!hasNumber)
        return REGISTER_ERROR_NO_NUMBER;

    return REGISTER_ERROR_NONE;
}

ParseBirthDate(playerid, const date[])
{
    new day, month, year;

    if(sscanf(date, "p</>ddd", day, month, year))
    {
        ShowBirthDateDialog(playerid, true);
        return 0;
    }

    if(!IsValidDate(day, month, year))
    {
        ShowBirthDateDialog(playerid, true);
        return 0;
    }

    PlayerData[playerid][pBirthDay] = day;
    PlayerData[playerid][pBirthMonth] = month;
    PlayerData[playerid][pBirthYear] = year;

    PlayerData[playerid][pCharSetup] = CHAR_ORIGIN;
    ShowOriginDialog(playerid);
    return 1;
}

IsValidDate(day, month, year)
{
    if(year < 1900 || year > 2100) return 0;
    if(month < 1 || month > 12) return 0;
    if(day < 1) return 0;

    new daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};

    if(month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)))
    {
        return day <= 29;
    }
    return day <= daysInMonth[month - 1];
}