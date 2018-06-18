#pragma once

int DoMsg(char *msg) {
	printf("Msg : %s\n", msg);
	if (strcmp(msg, "shutdown") == 0) {
		ShutdownHost();
	}
	else if (strcmp(msg, "closemusic") == 0) {
		KillProcessbyName("cloudmusic.exe");
		KillProcessbyName("QQMusic.exe");
		KillProcessbyName("QQMusicExternal.exe");
	}
	else
		return -1;
	return 0;
}

bool GetPassword(SOCKET *s) {
	char buf[100] = { 0 };
	//do{
		int len = recv(*s, buf, sizeof(buf), 0);
		//printf("Password:%s\n", buf);
		if (len > 0 && strcmp(buf, PASSWORD) == 0)return true;
			//break;
		
			//buf[0] = '\0';
	//} while (1);
	//printf("%s\n", buf);

	return false;
}