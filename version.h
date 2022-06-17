/*//====================================================================================
// Revision History :
//---------------------------------------------------------------
// 2013.11.13 by Weichun.Pan
//	 First version
// 
//---------------------------------------------------------------
// Copyright (c) Advantech Co., Ltd. All Rights Reserved
//====================================================================================*/
#ifndef _DEMO_VERSION_H_
#define _DEMO_VERSION_H_



#define VER_MAJOR	4
#define VER_MINOR	2
#define VER_BUILD	0
#define VER_FIX		0

#define U32VER		(VER_MAJOR << 24 | VER_MINOR << 16 | VER_BUILD)

#define CREATE_XVER(maj, min, build, fix) 		maj ##, ## min ##, ## build ##, ## fix

#endif /* _DEMO_VERSION_H_ */
