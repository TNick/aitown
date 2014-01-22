/* pivotal_gmtime_r - a replacement for gmtime/localtime/mktime
                      that works around the 2038 bug on 32-bit
                      systems. (Version 4)

   Copyright (C) 2009  Paul Sheer

   Redistribution and use in source form, with or without modification,
   is permitted provided that the above copyright notice, this list of
   conditions, the following disclaimer, and the following char array
   are retained.

   Redistribution and use in binary form must reproduce an
   acknowledgment: 'With software provided by http://2038bug.com/' in
   the documentation and/or other materials provided with the
   distribution, and wherever such acknowledgments are usually
   accessible in Your program.

   This software is provided "AS IS" and WITHOUT WARRANTY, either
   express or implied, including, without limitation, the warranties of
   NON-INFRINGEMENT, MERCHANTABILITY or FITNESS FOR A PARTICULAR
   PURPOSE. THE ENTIRE RISK AS TO THE QUALITY OF THIS SOFTWARE IS WITH
   YOU. Under no circumstances and under no legal theory, whether in
   tort (including negligence), contract, or otherwise, shall the
   copyright owners be liable for any direct, indirect, special,
   incidental, or consequential damages of any character arising as a
   result of the use of this software including, without limitation,
   damages for loss of goodwill, work stoppage, computer failure or
   malfunction, or any and all other commercial damages or losses. This
   limitation of liability shall not apply to liability for death or
   personal injury resulting from copyright owners' negligence to the
   extent applicable law prohibits such limitation. Some jurisdictions
   do not allow the exclusion or limitation of incidental or
   consequential damages, so this exclusion and limitation may not apply
   to You.

*/

#ifndef linked_list_str_h_INCLUDE
#define linked_list_str_h_INCLUDE

#include <aitown/aitown_global.h>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef long long time64_t;

AITOWN_EXPORT time64_t
pivot_time_t (const time_t * now, time64_t * _t);

AITOWN_EXPORT struct tm *
gmtime64_r (const time64_t * _t, struct tm *p);

AITOWN_EXPORT struct tm *
pivotal_gmtime_r (const time_t * now, const time_t * _t, struct tm *p);

AITOWN_EXPORT time64_t
mktime64 (struct tm * t);

AITOWN_EXPORT struct tm *
pivotal_localtime_r (const time_t * now, const time_t * _t, struct tm *p);

AITOWN_EXPORT struct tm *
localtime64_r (const time64_t * _t, struct tm *p);

AITOWN_EXPORT time64_t
pivotal_timestamp ();

#ifdef __cplusplus
}
#endif
#endif // linked_list_str_h_INCLUDE
