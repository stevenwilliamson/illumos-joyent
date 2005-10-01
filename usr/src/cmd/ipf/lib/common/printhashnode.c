/*
 * Copyright (C) 2002 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#pragma ident	"%Z%%M%	%I%	%E% SMI"

#include "ipf.h"

#define	PRINTF	(void)printf
#define	FPRINTF	(void)fprintf

iphtent_t *printhashnode(iph, ipep, copyfunc, opts)
iphtable_t *iph;
iphtent_t *ipep;
copyfunc_t copyfunc;
int opts;
{
	iphtent_t ipe;

	if ((*copyfunc)(ipep, &ipe, sizeof(ipe)))
		return NULL;

	if (ipe.ipe_family == AF_INET) {
		ipe.ipe_addr.in4_addr = htonl(ipe.ipe_addr.in4_addr);
		ipe.ipe_mask.in4_addr = htonl(ipe.ipe_mask.in4_addr);
	}

	if ((opts & OPT_DEBUG) != 0) {
#ifdef USE_INET6
		char addinfo[INET6_ADDRSTRLEN];
		PRINTF("\tAddress: %s",
			inet_ntop(ipe.ipe_family, (void *)&ipe.ipe_addr.in4, 
				  addinfo, sizeof(addinfo)));
#else
		PRINTF("\tAddress: %s",
			inet_ntoa(ipe.ipe_addr.in4));
#endif
#ifdef USE_INET6
		if (ipe.ipe_family == AF_INET6)
			printmask(6, (u_32_t *)&ipe.ipe_mask.in6);
		else
#endif
			printmask(4, (u_32_t *)&ipe.ipe_mask.in4_addr);

		PRINTF("\tRef. Count: %d\tValue: %d\n", ipe.ipe_ref,
			ipe.ipe_value);
	} else {
		putchar(' ');
#ifdef USE_INET6
		if (ipe.ipe_family == AF_INET6)
			printhostmask(6, (u_32_t *)&ipe.ipe_addr.in6, 
					 (u_32_t *)&ipe.ipe_mask.in6);
		else
#endif
		{
			printip((u_32_t *)&ipe.ipe_addr.in4_addr);
			printmask(4, (u_32_t *)&ipe.ipe_mask.in4_addr);
		}
		if (ipe.ipe_value != 0) {
			switch (iph->iph_type & ~IPHASH_ANON)
			{
			case IPHASH_GROUPMAP :
				PRINTF(", group = %s", ipe.ipe_group);
					break;
			}
		}
		putchar(';');
	}
	ipep = ipe.ipe_next;
	return ipep;
}
