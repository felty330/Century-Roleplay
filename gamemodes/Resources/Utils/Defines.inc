//Characters
#define MAX_CHARS 3

//Server Conf
#define TEXT_GAMEMODE		"CRP || WIP"

//SpawnPos
#define DEFAULT_SPAWN_X         (2768.3208)
#define DEFAULT_SPAWN_Y         (-2424.6943)
#define DEFAULT_SPAWN_Z         (13.6484)
#define DEFAULT_SPAWN_A         (45.5144)

//Range
#define LOCAL_CHAT_RADIUS 20.0
#define LOCAL_CHAT_RADIUS_LOW 8.0
#define LOCAL_CHAT_RADIUS_SHOUT 35.0

//Macro
#define FUNC:%0(%1) forward %0(%1); public %0(%1)
#define SendError(%1,%2) SendClientMessageEx(%1, COLOR_GREY3, ""RED_E"ERROR: "WHITE_E""%2)
#define SendSyntax(%1,%2) SendClientMessage(%1, COLOR_RED, ""ORANGE_E"USAGE: "WHITEP_E""%2)
#define SendServer(%1,%2) SendClientMessageEx(%1, COLOR_WHITE, ""ORANGE_E"SERVER: "WHITE_E""%2)
#define PermissionError(%0) SendClientMessage(%0, COLOR_RED, "ERROR: "WHITE_E"Kamu tidak ngga memiliki izin untuk menggunak perintah ini!")

#define PRESSED(%0) \
	(((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))

//Converter
#define minplus(%1) \
        (((%1) < 0) ? (-(%1)) : ((%1)))

// AntiCaps
#define UpperToLower(%1) for( new ToLowerChar; ToLowerChar < strlen( %1 ); ToLowerChar ++ ) if ( %1[ ToLowerChar ]> 64 && %1[ ToLowerChar ] < 91 ) %1[ ToLowerChar ] += 32
