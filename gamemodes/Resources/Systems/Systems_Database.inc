/*==================================================

                    Database                 

==================================================*/

#define MYSQL_CONFIG_FILE "mysql_settings.cfg"
#define MYSQL_CONFIG_LINE_SIZE 144
#define ERROR_DUPLICATE 1062
enum E_MYSQL_SETTINGS
{
    MYSQL_HOST[64],
    MYSQL_USER[64],
    MYSQL_PASS[64],
    MYSQL_DATABASE[64],
    MYSQL_OPTION_ID
};
new MySQLSettings[E_MYSQL_SETTINGS];
new MySQL: c_SQL;

stock TrimRightString(string[]) {
    new len = strlen(string);

    while(len > 0 && (string[len - 1] == '\n' || string[len - 1] == '\r' || string[len - 1] == ' ' || string[len - 1] == '\t'))
    {
        string[len - 1] = EOS;
        len--;
    }

    return 1;
}

GetConfigValue(const line[], key[], value[], valueSize) {
    new pos = strfind(line, "=", false);

    if(pos == -1)
        return 0;

    strmid(key, line, 0, pos, 32);
    strmid(value, line, pos + 1, strlen(line), valueSize);

    TrimRightString(key);
    TrimRightString(value);

    return 1;
}

LoadMySQLSettings() {
    new File:file = fopen(MYSQL_CONFIG_FILE, io_read);

    if(!file)
    {
        print("[MYSQL] mysql_settings.cfg tidak ditemukan di folder scriptfiles.");
        return 0;
    }

    new line[MYSQL_CONFIG_LINE_SIZE];
    new key[32];
    new value[96];

    while(fread(file, line))
    {
        TrimRightString(line);

        if(!strlen(line))
            continue;

        if(line[0] == '#')
            continue;

        if(!GetConfigValue(line, key, value, sizeof(value)))
            continue;

        if(!strcmp(key, "host", true))
        {
            format(MySQLSettings[MYSQL_HOST], 64, "%s", value);
        }
        else if(!strcmp(key, "username", true))
        {
            format(MySQLSettings[MYSQL_USER], 32, "%s", value);
        }
        else if(!strcmp(key, "password", true))
        {
            format(MySQLSettings[MYSQL_PASS], 64, "%s", value);
        }
        else if(!strcmp(key, "database", true))
        {
            format(MySQLSettings[MYSQL_DATABASE], 64, "%s", value);
        }
    }

    fclose(file);

    if(!strlen(MySQLSettings[MYSQL_HOST]))
    {
        print("[MYSQL] host kosong di mysql_settings.cfg.");
        return 0;
    }

    if(!strlen(MySQLSettings[MYSQL_USER]))
    {
        print("[MYSQL] username kosong di mysql_settings.cfg.");
        return 0;
    }

    if(!strlen(MySQLSettings[MYSQL_DATABASE]))
    {
        print("[MYSQL] database kosong di mysql_settings.cfg.");
        return 0;
    }

    print("[MYSQL] Konfigurasi MySQL berhasil dimuat.");
    return 1;
}

DatabaseConnect() {
    if(!LoadMySQLSettings()) {
        print("[MySQL] Server dimatikan karena konfiguarsi database gagal dimuat.");
        SendRconCommand("exit");
        return 1;
    }
    c_SQL = mysql_connect(
        MySQLSettings[MYSQL_HOST],
        MySQLSettings[MYSQL_USER],
        MySQLSettings[MYSQL_PASS],
        MySQLSettings[MYSQL_DATABASE]
    );
    if(c_SQL == MYSQL_INVALID_HANDLE || mysql_errno(c_SQL) != 0) {
        printf("[MYSQL ERROR] Gagal terhubung ke database. Error code: %d", mysql_errno(c_SQL));
        SendRconCommand("exit");
        return 1;
    }
    print("[MySQL] Berhasil terhubung ke database");
    return 1;
}