#pragma once

#ifndef TIME_H
#define TIME_H
#include<time.h>
#endif

#define LOG_DEVICECONNECT 1
#define LOG_PASSWORD 2
#define LOG_CMD 3
#define LOG_BREAK 4


FILE *NewLog(void)
{
	FILE *fplog;
	struct tm *tmnow;
	time_t ttnow;
	//获取系统时间
	time(&ttnow);
	tmnow = localtime(&ttnow);
	//创建文件
	char fname[1000];
	sprintf(fname, "%d%d%d.log", tmnow->tm_year + 1900, tmnow->tm_mon + 1, tmnow->tm_mday);
	fplog = fopen(fname, "w");
	return fplog;
}

bool AddLog(FILE *fplog,char *s,int flag)
{
	char buf[1000];
	switch (flag)
	{
	case LOG_DEVICECONNECT:
		struct tm *tmnow;
		time_t ttnow;
		time(&ttnow);
		tmnow = localtime(&ttnow);
		sprintf(buf, "[%d-%d-%d]\nDevice %s connect.\n", tmnow->tm_hour, tmnow->tm_min, tmnow->tm_sec, s);
		break;
	case LOG_PASSWORD:
		sprintf(buf, "Password %s.\n", s);
		break;
	case LOG_CMD:
		sprintf(buf, "Msg:%s\n", s);
		break;
	case LOG_BREAK:
		sprintf(buf, "Connection break.\n\n");
		break;
	default:
	
		break;
		//return false;
	}
	if (fprintf(fplog, "%s", buf) == 0)
		perror("Add log error:");
	fflush(fplog);
	return true;
}