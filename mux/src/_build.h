// _build.h
//
// $Id: _build.h,v 1.61 2003-03-08 21:34:26 sdennis Exp $
//
// MUX 2.2
// Copyright (C) 1998 through 2003 Solid Vertical Domains, Ltd. All
// rights not explicitly given are reserved.  
//
#ifndef MUX_BUILD_NUM
extern char szBuildNum[];
#define MUX_BUILD_NUM szBuildNum
#endif // MUX_BUILD_NUM

#ifndef MUX_BUILD_DATE
extern char szBuildDate[];
#define MUX_BUILD_DATE szBuildDate
#endif // MUX_BUILD_DATE

#define MUX_VERSION       "2.2.7.59"     // Version number
#define MUX_RELEASE_DATE  "2003-MAR-08"  // Source release date

// Define if this release is qualified as ALPHA or BETA.
//
//#define ALPHA
#define BETA
