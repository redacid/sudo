/*
 * Copyright (c) 1999 Todd C. Miller <Todd.Miller@courtesan.com>
 * All rights reserved.
 *
 * This code is derived from software contributed by Giles Todd
 * <giles@gt.demon.co.uk>.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * 4. Products derived from this software may not be called "Sudo" nor
 *    may "Sudo" appear in their names without specific prior written
 *    permission from the author.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#include <stdio.h>
#ifdef STDC_HEADERS
#include <stdlib.h>
#endif /* STDC_HEADERS */
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */
#ifdef HAVE_STRING_H
#include <string.h>
#endif /* HAVE_STRING_H */
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif /* HAVE_STRINGS_H */
#include <sys/param.h>
#include <sys/types.h>
#include <pwd.h>

#include <sdi_athd.h>
#include <sdconf.h>
#include <sdacmvls.h>

#include "sudo.h"
#include "sudo_auth.h"

#ifndef lint
static const char rcsid[] = "$Sudo$";
#endif /* lint */

union config_record configure;

int
securid_init(pw, promptp, data)
    struct passwd *pw;
    char **promptp;
    void **data;
{

    creadcfg();					/* Only read config file once */
    return(AUTH_SUCCESS);
}

int
securid_setup(pw, promptp, data)
    struct passwd *pw;
    char **promptp;
    void **data;
{
    static SD_CLIENT sd_dat;			/* SecurID data block */

    /* Re-initialize SecurID every time. */
    *data = &sd_dat;
    if (sd_init(sd) == 0)
	return(AUTH_SUCCESS);
    else {
	(void) fprintf(stderr, "%s: Cannot contact SecurID server\n", Argv[0]);
	return(AUTH_FATAL);
    }
}

int
securid_verify(pw, pass, data)
    struct passwd *pw;
    char *pass;
    void **data;
{
    struct SD_CLIENT *sd = (struct SD_CLIENT *)(*data);

    if (sd_auth(sd) == ACM_OK)
	return(AUTH_SUCCESS);
    else
	return(AUTH_FAILURE);
}
