/************************************************
 *  Author        :mathon
 *  Email         :luoxinchen96@gmail.com
 *  Time          :2017年 4月13日 星期四
*************************************************/

#ifndef _SCAN_H_
#define _SCAN_H_

#define MAXTOKENLEN 40

extern char tokenString[MAXTOKENLEN+1];

TokenType getToken(void);

#endif
