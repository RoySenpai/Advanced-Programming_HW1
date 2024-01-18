/*
 *  Advanced Programming Course Assignment 1
 *  Shell Header File
 *  Copyright (C) 2023  Roy Simanovich and Almog Shor
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _SHELL_H
#define _SHELL_H

/********************/
/* Includes Section */
/********************/

#if !defined(_XOPEN_SOURCE) && !defined(_POSIX_C_SOURCE)
	#if __STDC_VERSION__ >= 199901L
		#define _XOPEN_SOURCE 600   /* SUS v3, POSIX 1003.1 2004 (POSIX 2001 + Corrigenda) */
	#else
		#define _XOPEN_SOURCE 500   /* SUS v2, POSIX 1003.1 1997 */
	#endif /* __STDC_VERSION__ */
#endif /* !_XOPEN_SOURCE && !_POSIX_C_SOURCE */

#include <bits/stdint-uintn.h>

#endif /* _SHELL_H */