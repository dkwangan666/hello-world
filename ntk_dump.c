/****************************************************************************
 * @_file:
 * 								ntk_dump.c
 * 
 *---------------------------------------------------------------------------
 * DESCRIPTION: @{HDES}
 *-------------
 * 封装调试输出相关函数
 *---------------------------------------------------------------------------
 * REQUIREMENT: @{HREQ}
 * -------------------------------------
 * $ Macros $ :  
 * $ Headers $ :  
 * $ Libraries $ :  
 *---------------------------------------------------------------------------
 * CHANGES LOG: @{HREV}
 * -------------------------------------
 * Version : 1.0.0.0
 * Author  : jessetang
 * Date    : 2018-12-5 17:24:38
 * Comments: First Issue
 * -------------------------------------
 * changes :
 */


/**************************************************************************/
#include "../include/ntk_list.h"
#include "../include/ntk_common.h"

//获取线程ID
#ifndef csa_thread_id
#define csa_thread_id() syscall(__NR_gettid)
#define _syscall0(pid_t, csa_thread_id)
#endif

int get_current_thread_id()
{
	return (csa_thread_id());
}

struct   timeval * get_timeval(struct   timeval *ptv)
{
	if( NULL == ptv )
	{
		return NULL;
	}

	gettimeofday(ptv, 0);
	return ptv;
}

float span_timeval(struct   timeval *p_old, struct   timeval *p_new )
{
	struct   timeval tmp;
	struct   timeval * ptv = NULL;
	float	 f_span = 0.000000;
	float	 f_urat = 1000000.000000;
	float	 f_tmp = 0.000000;

	if (NULL == p_old)
	{
		return (float)(0.000000);
	}

	if( NULL == p_new )
	{
		ptv = get_timeval(&tmp);
	}
	else 
	{
		ptv = p_new;
	}

	if( ptv->tv_sec < p_old->tv_sec )
	{
		return (-1.000000);
	}
	else if (ptv->tv_sec == p_old->tv_sec)
	{
		if( ptv->tv_usec < p_old->tv_usec )
		{
			return (-1.000000);
		}

		f_span = (float)(ptv->tv_usec) / f_urat - (float)(p_old->tv_usec) / f_urat;
		return (f_span);
	}		

	f_span = (float)(ptv->tv_sec - p_old->tv_sec);
	if( ptv->tv_usec < p_old->tv_usec )
	{
		f_tmp = (float)(p_old->tv_usec - ptv->tv_usec);
		f_tmp = f_tmp / f_urat;
		f_span = f_span - f_tmp;
		return f_span;
	}

	f_tmp = (float)(ptv->tv_usec - p_old->tv_usec);
	f_tmp = f_tmp / f_urat;
	f_span = f_span + f_tmp;
	return f_span;
}

void dump_spec_timeval(
		const char * tag,
		struct   timeval *p_old, struct   timeval *p_new)
{
	float f_span = 0.000000;

	f_span = span_timeval(p_old, p_new);
	
	if( f_span < 0.010000 )
	{
		ntd_info("----[%s]----spend timval : %0.3f ms", TNuVa(tag), f_span*1000.000 );
	}
	else
	{
		ntd_info("----[%s]----spend timval : %0.6f sec", TNuVa(tag), f_span);
	}	
}

float get_utick_nowtime()
{
	struct   timeval   ust;
	
	long    ltime = (long)time(NULL);
	long	lmsec = 0;

	float   rat = 1000000.000000;

	gettimeofday(&ust, 0);

	ltime = (float)ust.tv_sec;
	ltime += (float)(ust.tv_usec) / rat;

	return (float)(ltime);	
}

int FormatTimeStrEx(struct tm *pTmNow, char *cpTimeStr, int iBufSize, int nFmt, bool bAlignZero /*= true*/)
{
	char		cpFmtStr[65] = { 0 };
	char        cpTime[65] = { 0 };
	int         iRet = 0;

	struct   timeval   ust;

	gettimeofday(&ust, 0);

	
	// show date and time
	//--------------------------------
	//nFmt = 0  : YYYYMMDDHHMMSS
	//nFmt = 1  : YYYY_MM_DD_HH_MM_SS
	//nFmt = 2  : YYYY-MM-DD HH:MM:SS
	// 
	// show time only
	//--------------------------------
	//nFmt = 10 : HHMMSS
	//nFmt = 11 : HH_MM_SS
	//nFmt = 12 : HH:MM:SS
	//
	// show date only
	//--------------------------------
	//nFmt = 20 : YYYYMMDD
	//nFmt = 21 : YYYY_MM_DD
	//nFmt = 22 : YYYY-MM-DD
	switch (nFmt)
	{
	case 0: //YYYYMMDDHHMMSS
		snprintf(cpFmtStr, sizeof(cpFmtStr)-1, "%s", bAlignZero ? "%04d%02d%02d%02d%02d%02d" : "%d%d%d%d%d%d");
		break;
	case 1: //YYYY_MM_DD_HH_MM_SS
		snprintf(cpFmtStr, sizeof(cpFmtStr)-1, "%s", bAlignZero ? "%04d_%02d_%02d_%02d_%02d_%02d" : "%d_%d_%d_%d_%d_%d");
		break;
	case 2://YYYY-MM-DD HH:MM:SS
		snprintf(cpFmtStr, sizeof(cpFmtStr)-1, "%s", bAlignZero ? "%04d-%02d-%02d %02d:%02d:%02d" : "%d-%d-%d %d:%d:%d");
		break;
	case 3://YYYY-MM-DD HH:MM:SS.ms
		snprintf(cpFmtStr, sizeof(cpFmtStr)-1, "%s", bAlignZero ? "%04d-%02d-%02d %02d:%02d:%02d.%03d" : "%d-%d-%d %d:%d:%d.%d");
		break;
	case 10: //HHMMSS
		snprintf(cpFmtStr, sizeof(cpFmtStr)-1, "%s", bAlignZero ? "%02d%02d%02d" : "%d%d%d");
		break;
	case 11: //HH_MM_SS
		snprintf(cpFmtStr, sizeof(cpFmtStr)-1, "%s", bAlignZero ? "%02d_%02d_%02d" : "%d_%d_%d");
		break;
	case 12: //HH:MM:SS
		snprintf(cpFmtStr, sizeof(cpFmtStr)-1, "%s", bAlignZero ? "%02d:%02d:%02d" : "%d:%d:%d");
		break;
	case 13: //HH:MM:SS.ms
		snprintf(cpFmtStr, sizeof(cpFmtStr)-1, "%s", bAlignZero ? "%02d:%02d:%02d.%03d" : "%d:%d:%d.%d");
		break;
	case 20: //YYYYMMDD
		snprintf(cpFmtStr, sizeof(cpFmtStr)-1, "%s", bAlignZero ? "%04d%02d%02d" : "%d%d%d");
		break;
	case 21: //YYYY_MM_DD
		snprintf(cpFmtStr, sizeof(cpFmtStr)-1, "%s", bAlignZero ? "%04d_%02d_%02d" : "%d_%d_%d");
		break;
	case 22: //YYYY-MM-DD
		snprintf(cpFmtStr, sizeof(cpFmtStr)-1, "%s", bAlignZero ? "%04d-%02d-%02d" : "%d-%d-%d");
		break;
	default:
		return 0;
	}
	if (nFmt == 3)
	{
		iRet = snprintf(cpTime, sizeof(cpTime)-1, cpFmtStr,
			(1900 + pTmNow->tm_year),
			(1 + pTmNow->tm_mon),
			pTmNow->tm_mday,
			pTmNow->tm_hour,
			pTmNow->tm_min,
			pTmNow->tm_sec,
			(int)(ust.tv_usec / 1000));
	}
	else if (nFmt < 10) //date + time
	{
		iRet = snprintf(cpTime, sizeof(cpTime)-1, cpFmtStr,
			(1900 + pTmNow->tm_year),
			(1 + pTmNow->tm_mon),
			pTmNow->tm_mday,
			pTmNow->tm_hour,
			pTmNow->tm_min,
			pTmNow->tm_sec);
	}
	else if (nFmt == 13)
	{
		iRet = snprintf(cpTime, sizeof(cpTime)-1, cpFmtStr,
			pTmNow->tm_hour,
			pTmNow->tm_min,
			pTmNow->tm_sec,
			(int)(ust.tv_usec / 1000));
	}
	else if (nFmt < 20)//time only
	{
		iRet = snprintf(cpTime, sizeof(cpTime)-1, cpFmtStr,
			pTmNow->tm_hour,
			pTmNow->tm_min,
			pTmNow->tm_sec);
	}
	else if (nFmt < 30)//date only
	{
		iRet = snprintf(cpTime, sizeof(cpTime)-1, cpFmtStr,
			(1900 + pTmNow->tm_year),
			(1 + pTmNow->tm_mon),
			pTmNow->tm_mday
			);

	}
	else
	{
		return 0;
	}

	if (iBufSize > iRet)
	{
		memcpy(cpTimeStr, cpTime, iRet);
		cpTimeStr[iRet] = 0;
	}
	else
	{
		iRet = 0;
	}

	return iRet;
}

int FormatTimeStr(	time_t *tpRcd, char *cpTimeStr, int iBufSize, 
					int nFmt, bool bAlignZero /*= true*/)
{
	struct tm   pTmNow;
	struct   timeval   ust;


	localtime_r(tpRcd, &pTmNow);

	if (nFmt == 32)
	{
		return TSPrintf(cpTimeStr, iBufSize, "%ld", *tpRcd);
	}
	else if (nFmt == 33)
	{
		gettimeofday(&ust, 0);


		return TSPrintf(cpTimeStr, iBufSize, "%ld.%lu", *tpRcd, ust.tv_usec / 1000);
	}

	return FormatTimeStrEx(&pTmNow, cpTimeStr, iBufSize, nFmt, bAlignZero);
}

int GetTimeStrNow(char *cpTimeStr, int iBufSize, int nFmt, bool bAlignZero /*= true*/)
{
	time_t		timeNow;     //实例化time_t结构

	time(&timeNow);

	return FormatTimeStr(&timeNow, cpTimeStr, iBufSize, nFmt, bAlignZero);
}

void ntw_dump_tcp_iph(const char * tag,
	unsigned int uiSrcIP, unsigned int uiSrcPort,
	unsigned int uiDesIP, unsigned int uiDestPort,
	BYTE* smac, BYTE* dmac)
{
	char cDestIP[32] = { 0 };
	char cSrcIP[32] = { 0 };
	char cDestMAC[32] = { 0 };
	char cSrcMAC[32] = { 0 };
	int  iDstPort = 0, iSrcPort = 0;

	iSrcPort = ntohs(uiSrcPort);
	iDstPort = ntohs(uiDestPort);
	ntw_ipv4_to_host(cDestIP, sizeof(cDestIP)-1, uiDesIP);
	ntw_ipv4_to_host(cSrcIP, sizeof(cSrcIP)-1, uiSrcIP);

	ntw_mac_bytes_2_string(cDestMAC, sizeof(cDestMAC)-1, dmac);
	ntw_mac_bytes_2_string(cSrcMAC, sizeof(cSrcMAC)-1, smac);

	if (NULL != tag)
	{
		ntd_info("~~[%s]~~[ %s|%s:%d ]-->[ %s|%s:%d ]",
			tag,
			cSrcMAC, cSrcIP, iSrcPort, cDestMAC, cDestIP, iDstPort);
	}
	else
	{
		ntd_info("[ %s|%s:%d ]-->[ %s|%s:%d ]",
			cSrcMAC, cSrcIP, iSrcPort, cDestMAC, cDestIP, iDstPort);
	}
}

void safe_dump_buffer(const char *buf, int buf_len, const char* tag)
{
	char * p = NULL;
	char temp[4096] = {0};

	if( NULL == buf || buf_len <= 0 )
	{
		return;
	}

	p = strchr(buf, '%');
	if( NULL == p )
	{
		ntd_info("[%s] (%d)-%s", TNuVa(tag), buf_len, buf);
	}
	else
	{
		TSafeNMemcpy(temp, buf, p - buf, sizeof(temp));
		ntd_info("[%s] (%d)-%s***", TNuVa(tag), buf_len, temp );
	}
}

void dump_white_list(NTK_LIST_HEAD *plist, int type, const char * tag)
{
	char szType[2][5] = {"host","type"};
	int	 index = 0;
	char * key = NULL;
	NTK_LIST_ITERATOR	it;

	TVOIDRET_IF(NULL == plist);
	TVOIDRET_IF(type<0 || type>1);

	ntd_info("------ dump [%s] white_list(%s) ------", szType[type], TNuVa(tag));
	ntd_info("list.size	 : %ld", plist->size);
	it = plist->head;
	while ( it )
	{
		key = (char *)it->data;
		ntd_info("node[%03d]  : %s", TNuVa(key) );

		it = it->next;
	}
	ntd_info("-------------- (over) --------------");
}

void dump_target(NTK_TARGET *ptarget, const char * tag)
{
	TVOIDRET_IF(NULL == ptarget);

	ntd_info("------ dump nt_target(%s) ------", TNuVa(tag));
	ntd_info("id         : %s", ptarget->id);
	ntd_info("tagt_type  : %d", ptarget->tagt_type);
	ntd_info("is_enable  : %d", ptarget->is_enable);
	ntd_info("target     : %s", ptarget->target);
	ntd_info("name       : %s", ptarget->name);
	ntd_info("rule_list  : %s", ptarget->rule_list);
	ntd_info("-------------- (over) --------------");
}

void dump_rule(NTK_RULE *prule, const char * tag )
{
	TVOIDRET_IF(NULL == prule);

	ntd_info("------ dump nt_rule(%s) ------", TNuVa(tag));
	ntd_info("ruleid      : %d", prule->ruleid);
	ntd_info("match_type  : %d", prule->match_type);
	ntd_info("cmd_type    : %d", prule->cmd_type);
	ntd_info("action_type : %d", prule->action_type);
	ntd_info("dir_index   : %d", prule->dir_index);
	ntd_info("period      : %d", prule->period);
	ntd_info("replay      : %d", prule->replay);
	ntd_info("is_enable   : %d", prule->is_enable);
	ntd_info("url1        : %s", prule->url1);
	ntd_info("url2        : %s", prule->url2);
	ntd_info("url3        : %s", prule->url3);
	ntd_info("ua          : %s", prule->ua);
	ntd_info("method      : %s", prule->method);
	ntd_info("file_name   : %s", prule->file_name);
	ntd_info("notes       : %s", prule->notes);
	ntd_info("-------------- (over) --------------");
}

void _ntk_dump(int dlv, const char* fun, int nline, const char * fmt, ...)
{
	char	buff[MAX_DBG_BUFFER] = { 0 };
	char	line[MAX_DBG_BUFFER] = { 0 };
	va_list va;
	int     nLen = 0, nOff=0, nPos=0;
	int		nHead = 0,nMsg=0;
	int		thrd_id = 0;
	char *	p = NULL;
	
	struct stat st;
	if (!stat(NTK_DUMP_FROBIDDEN_FFLG, &st) )
	{
		return;
	}
	
	thrd_id = get_current_thread_id();

	//格式化输出头部：[时间戳][调试级别][函数位置]
	line[0] = '[';
	nHead = GetTimeStrNow(line + 1, sizeof(line)-2, 13, true);
	strcat(line, "]");
	nHead += 2;
	if (dlv == dl_error)
	{
		strcat(line, "[ error]");
	}
	else if( dlv == dl_warning)
	{
		strcat(line, "[ warn ]");
	}
	else 
	{
		strcat(line, "[ info ]");
	} 	
	nHead = strlen(line);
	nMsg = sizeof(line)-nHead - 2;
	nHead += TSPrintf(line + nHead, nMsg, "[%04d][%s:%04d]", 
		thrd_id, TNuVa(fun), nline);
	nMsg = sizeof(line)-nHead - 2;
	

	nLen = sizeof(buff)-2;
	va_start(va, fmt);
	nLen = TFmtBuffer(buff, nLen, fmt, va);
	va_end(va);
	TVOIDRET_IF(nLen <= 0);

	if (buff[nLen - 1] != '\n')
	{
		buff[nLen++] = '\n';
	}

	do 
	{
		p = strchr(buff+nOff, '\n');
		TVOIDRET_IF(p == NULL);

		nPos = p - buff - nOff + 1; //包括换行符
		TSafeNMemcpy(line + nHead, buff + nOff, nMsg, nPos);

		//stdout
		printf(line);	

		//next line
		p++;
		nOff = p - buff;

	} while (nOff<nLen);
	
}


