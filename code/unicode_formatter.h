// ================================================================================
// $Id: unicode_formatter.c 846 2009-06-10 23:41:47Z sam $
//
//! XCode variable formatter for Unicode character data.
//
//! Custom data formatters should be installed in:
//!   ~/Library/Library/Application Support/Developer/3.1/XCode/CustomDataViews/
//
//! Read DataFormatterPlugin.h for more information on custom data formatters 
//! and allocators.
//
// Copyright (c) 2009 Elegant Chaos. All Rights Reserved.
// ================================================================================

// --------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------

#include <CoreFoundation/CoreFoundation.h>

// --------------------------------------------------------------------------------
// Macros
// --------------------------------------------------------------------------------

#define DEBUG_PRINT(...) fprintf(stderr, __VA_ARGS__);

// --------------------------------------------------------------------------------
// Constants
// --------------------------------------------------------------------------------

extern char* kNullPluginError;
extern char* kNullInputError;
extern const size_t max_length;

// --------------------------------------------------------------------------------
//! Convert a CFString into a char* buffer that we can return.
// --------------------------------------------------------------------------------

char* ConvertStringToEncoding(CFStringRef string, CFStringEncoding encoding, int identifier);
