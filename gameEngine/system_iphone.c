/*
 *  system_iphone.c
 *  DemosiPhoneOS
 *
 *  Created by Andreas on 2/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "system_iphone.h"

/*******************************************************************************
 system_iphone.c: Implementation of system.c for Apple iPhone
 Author: Andreas Kindlbacher
 Last modified: 02/15/10
 Note: Do note compile this file directly. Compile system.c instead.
 *******************************************************************************/
char *ged_getcwd( char *buffer, int maxlen ) {return getcwd(buffer, maxlen);}
