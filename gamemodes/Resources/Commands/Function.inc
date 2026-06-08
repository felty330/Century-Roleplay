FUNC:ServerRestartTime(seconds)
{
	if(seconds <= 0)
	{
		GameTextForAll("~r~SERVER RESTARTING...", 2000, 3);
		SendRconCommand("gmx");
		return 1;
	}

	StartRestartCountdown(seconds);

	SetTimerEx("ServerRestartCountdown", 1000, false, "d", seconds - 1);
	return 1;
}

stock StartRestartCountdown(seconds)
{
	new text[96];

	format(text, sizeof(text), "~r~SERVER RESTART~n~~w~Saving Data...~n~~y~%d", seconds);
	foreach(new i: Player)
	{
		GameTextForPlayer(i, text, 1000, 3);
	}
}
