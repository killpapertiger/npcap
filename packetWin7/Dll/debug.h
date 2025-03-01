/***********************IMPORTANT NPCAP LICENSE TERMS***********************
 *                                                                         *
 * Npcap (https://npcap.com) is the Nmap Project's packet capture (and     *
 * transmission) library for Microsoft Windows. It is (c) 2013-2022 by     *
 * Nmap Software LLC ("The Nmap Project"). All rights reserved.            *
 *                                                                         *
 * Even though Npcap source code is publicly available for review, it is   *
 * not open source software and the free version may not be redistributed  *
 * or incorporated into other software without special permission from     *
 * the Nmap Project. It also has certain usage limitations described in    *
 * the LICENSE file included with Npcap and also available at              *
 * https://github.com/nmap/npcap/blob/master/LICENSE. This header          *
 * summarizes a few important aspects of the Npcap license, but is not a   *
 * substitute for that full Npcap license agreement.                       *
 *                                                                         *
 * We fund the Npcap project by selling two commercial licenses:           *
 *                                                                         *
 * The Npcap OEM Redistribution License allows companies distribute Npcap  *
 * OEM within their products. Licensees generally use the Npcap OEM        *
 * silent installer, ensuring a seamless experience for end                *
 * users. Licensees may choose between a perpetual unlimited license or    *
 * an annual term license, along with options for commercial support and   *
 * updates. Prices and details: https://npcap.com/oem/redist.html          *
 *                                                                         *
 * The Npcap OEM Internal-Use License is for organizations that wish to    *
 * use Npcap OEM internally, without redistribution outside their          *
 * organization. This allows them to bypass the 5-system usage cap of the  *
 * Npcap free edition. It includes commercial support and update options,  *
 * and provides the extra Npcap OEM features such as the silent installer  *
 * for automated deployment. Prices and details:                           *
 * https://npcap.com/oem/internal.html                                     *
 *                                                                         *
 * Free and open source software producers are also welcome to contact us  *
 * for redistribution requests, but we normally recommend that such        *
 * authors instead ask their users to download and install Npcap           *
 * themselves.                                                             *
 *                                                                         *
 * Since the Npcap source code is available for download and review,       *
 * users sometimes contribute code patches to fix bugs or add new          *
 * features.  You are encouraged to submit such patches as Github pull     *
 * requests or by email to fyodor@nmap.org.  If you wish to specify        *
 * special license conditions or restrictions on your contributions, just  *
 * say so when you send them. Otherwise, it is understood that you are     *
 * offering the Nmap Project the unlimited, non-exclusive right to reuse,  *
 * modify, and relicense your code contributions so that we may (but are   *
 * not obligated to) incorporate them into Npcap.                          *
 *                                                                         *
 * This software is distributed in the hope that it will be useful, but    *
 * WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. Warranty rights    *
 * and commercial support are available for the OEM Edition described      *
 * above.                                                                  *
 *                                                                         *
 * Other copyright notices and attribution may appear below this license   *
 * header. We have kept those for attribution purposes, but any license    *
 * terms granted by those notices apply only to their original work, and   *
 * not to any changes made by the Nmap Project or to this entire file.     *
 *                                                                         *
 ***************************************************************************/
/*
 * Copyright (c) 2005 - 2006
 * CACE Technologies LLC, Davis (CA)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the company (CACE Technologies LLC) nor the 
 * names of its contributors may be used to endorse or promote products 
 * derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef __PACKET_DEBUG_393073863432093179878957
#define __PACKET_DEBUG_393073863432093179878957

#if defined(_DBG) || defined(_DEBUG_TO_FILE)

#include <stdio.h>
#include <windows.h>

#include <tchar.h>

static VOID OutputDebugStringV(LPCTSTR Format, ...)
{
	FILE *f;
	SYSTEMTIME LocalTime;
	va_list Marker;
	DWORD dwThreadId;
	int loops = 0;
	const DWORD dwLastError = GetLastError();

	dwThreadId = GetCurrentThreadId();

	va_start(Marker, Format); /* Initialize variable arguments. */

	GetLocalTime(&LocalTime);

	do
	{

#ifdef _CONSOLE
		if (_tfopen_s(&f, _T("C:\\Program Files\\Npcap\\NPFInstall.log"), _T("a,ccs=UTF-8")) == 0)
#else
		if (_tfopen_s(&f, _T("C:\\Program Files\\Npcap\\Packet.log"), _T("a,ccs=UTF-8")) == 0)
#endif
			break;

		Sleep(0);
		loops++;
	} while (loops <= 10);

	if (loops > 10 || !f)
	{
		SetLastError(dwLastError);
		return;
	}

	_ftprintf(f, _T("[%.08X] %.04u-%.02u-%.02u %.02u:%02u:%02u "),
			dwThreadId,
			LocalTime.wYear,							
			LocalTime.wMonth,							
			LocalTime.wDay,								
			LocalTime.wHour,							
			LocalTime.wMinute,							
			LocalTime.wSecond);										
	_vftprintf(f, Format, Marker);
	
	fclose(f);											


	SetLastError(dwLastError);
}

#elif defined (_DBG)

#include <strsafe.h>

static VOID OutputDebugStringV(LPCTSTR Format, ...)
{
	va_list Marker;
	TCHAR string[1024];
	DWORD dwLastError = GetLastError();

	va_start(Marker, Format); /* Initialize variable arguments. */

	StringCchVPrintf(string, sizeof(string), Format, Marker);

	OutputDebugString(string);

	va_end(Marker);

	SetLastError(dwLastError);
}
#endif


#if defined(_DBG) || defined(_DEBUG_TO_FILE)

#define TRACE_ENTER()									OutputDebugStringV(_T("--> ") _T(__FUNCTION__) _T("\n"))
#define TRACE_EXIT()									OutputDebugStringV(_T("<-- ") _T(__FUNCTION__) _T("\n"))
#define TRACE_PRINT(_x)									OutputDebugStringV(_T("    ") _T(_x) _T("\n"))
#define TRACE_PRINT1(_x, _p1)							OutputDebugStringV(_T("    ") _T(_x) _T("\n"), _p1)
#define TRACE_PRINT2(_x, _p1, _p2)						OutputDebugStringV(_T("    ") _T(_x) _T("\n"), _p1, _p2)
#define TRACE_PRINT3(_x, _p1, _p2, _p3)					OutputDebugStringV(_T("    ") _T(_x) _T("\n"), _p1, _p2, _p3)
#define TRACE_PRINT4(_x, _p1, _p2, _p3, _p4)			OutputDebugStringV(_T("    ") _T(_x) _T("\n"), _p1, _p2, _p3, _p4)
#define TRACE_PRINT5(_x, _p1, _p2, _p3, _p4, _p5)		OutputDebugStringV(_T("    ") _T(_x) _T("\n"), _p1, _p2, _p3, _p4, _p5)
#define TRACE_PRINT6(_x, _p1, _p2, _p3, _p4, _p5, _p6)	OutputDebugStringV(_T("    ") _T(_x) _T("\n"), _p1, _p2, _p3, _p4, _p5, _p6)

static __forceinline void TRACE_PRINT_OS_INFO()
{
	HKEY	hKey;
	CHAR buffer[1024];
	DWORD size = sizeof(buffer);
	DWORD type;
	DWORD dwLastError;

	dwLastError = GetLastError();

	TRACE_PRINT("********************* OS info.*********************");
	buffer[size-1] = 0;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		if (RegQueryValueEx(hKey, _T("PROCESSOR_ARCHITECTURE"), 0, &type, (LPBYTE)buffer, &size) == ERROR_SUCCESS && type == REG_SZ)
		{
			OutputDebugStringV(_T("Architecture = %s\n"), buffer);
		}
		else
		{
			OutputDebugStringV(_T("Architecture = <UNKNOWN>\n"));
		}
		
		RegCloseKey(hKey);
	}
	else
	{
		OutputDebugStringV(_T("Architecture = <UNKNOWN>\n"));
	}

	size = sizeof(buffer);
	buffer[size-1] = 0;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		if (RegQueryValueEx(hKey, _T("CurrentVersion"), 0, &type,  (LPBYTE)buffer, &size) == ERROR_SUCCESS && type == REG_SZ)
		{
			OutputDebugStringV(_T("Windows version = %s\n"), buffer);
		}
		else
		{
			OutputDebugStringV(_T("Windows version = <UNKNOWN>\n"));
		}
		
		RegCloseKey(hKey);
	}
	else
	{
		OutputDebugStringV(_T("Windows version = <UNKNOWN>\n"));
	}

	size = sizeof(buffer);
	buffer[size-1] = 0;
	if(	RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		if (RegQueryValueEx(hKey, _T("CurrentType"), 0, &type,  (LPBYTE)buffer, &size) == ERROR_SUCCESS && type == REG_SZ)
		{
			OutputDebugStringV(_T("Windows CurrentType = %s\n"), buffer);
		}
		else
		{
			OutputDebugStringV(_T("Windows CurrentType = <UNKNOWN>\n"));
		}
		
		RegCloseKey(hKey);
	}
	else
	{
		OutputDebugStringV(_T("Windows CurrentType = <UNKNOWN>\n"));
	}

	OutputDebugStringV(_T("*************************************************** \n"));

	SetLastError(dwLastError);
}
#else

#define TRACE_ENTER()
#define TRACE_EXIT()
#define TRACE_PRINT(_x)
#define TRACE_PRINT1(_x, _p1)
#define TRACE_PRINT2(_x, _p1, _p2)
#define TRACE_PRINT3(_x, _p1, _p2, _p3)
#define TRACE_PRINT4(_x, _p1, _p2, _p3, _p4)
#define TRACE_PRINT5(_x, _p1, _p2, _p3, _p4, _p5)
#define TRACE_PRINT6(_x, _p1, _p2, _p3, _p4, _p5, _p6)
#define TRACE_PRINT_WIDECHAR(_x)
#define TRACE_PRINT_OS_INFO()

#endif



#endif //__PACKET_DEBUG_393073863432093179878957
