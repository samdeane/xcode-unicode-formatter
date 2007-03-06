// ================================================================================
// $Id: $
//
//! XCode variable formatter for Unicode character data.
//
//! Custom data formatters should be installed in:
//!    /Library/Application Support/Apple/Developer\ Tools/CustomDataViews
//
//! Read DataFormatterPlugin.h for more information on custom data formatters 
//! and allocators.
//
// Copyright (c) 2007 Elegant Chaos. All Rights Reserved.
// ================================================================================

// --------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------

#import "/Developer/Applications/Xcode.app/Contents/PlugIns/GDBMIDebugging.xcplugin/Contents/Headers/DataFormatterPlugin.h"

#include <stdlib.h>
#include <wchar.h>
#include <string.h>

// --------------------------------------------------------------------------------
// Macros
// --------------------------------------------------------------------------------

#define DEBUG_PRINT(...)

// --------------------------------------------------------------------------------
// Globals
// --------------------------------------------------------------------------------

//  A bundle to support custom data formatting must define the following symbol:
_pbxgdb_plugin_function_list *_pbxgdb_plugin_functions;

// --------------------------------------------------------------------------------
// Constants
// --------------------------------------------------------------------------------

static char* kNullPluginError = "CFDataFormatter plugin error: _pbxgdb_plugin_functions not set!";

// --------------------------------------------------------------------------------
//! Convert wchar_t or UTF16 string data into UTF8 / ASCII.
//! All data formatters plugins can currently only return a 'char *' to be displayed
//! in the Summary column, so this can be a lossy conversion.
// --------------------------------------------------------------------------------

char * dataformatter_char_for_wchar(wchar_t *wcharData, int identifier, int bufLen, int convertFromUTF16)
{
    size_t maxChars = bufLen - 1;
	
	wchar_t* inputBuffer = wcharData;
    char* outputBuffer;

    if (_pbxgdb_plugin_functions)
	{
		if (convertFromUTF16)
		{
			// copy from UTF16 buffer into UTF32 buffer
			inputBuffer = (wchar_t *) malloc(bufLen * sizeof(wchar_t));
			unsigned short* copyFrom = (unsigned short*) wcharData;
			unsigned long* copyTo = (unsigned long*) inputBuffer;
			size_t copyCount = maxChars;
			while (copyCount-- && (*(copyTo++) = (unsigned long) (*(copyFrom++))))/* do nothing */;
			*copyTo = 0;
		}
		
        outputBuffer = (char *)(_pbxgdb_plugin_functions->allocate(identifier, bufLen));
        outputBuffer[0] = 0;
		outputBuffer[maxChars] = 0;
        wcstombs( outputBuffer, inputBuffer, maxChars);

        // Uncomment if you want this printed in the console every time the formatter is evaluated. This is good for Debugging.
		int n;
		DEBUG_PRINT("original:");
		for (n = 0; n < bufLen; ++n) { DEBUG_PRINT("%08x", wcharData[n]); }
		DEBUG_PRINT("\ninput:");
		for (n = 0; n < bufLen; ++n) { DEBUG_PRINT("%08x", inputBuffer[n]); }
        DEBUG_PRINT("\nwchar: %s, convert:%d, bufLen:%d, wchar size:%d\n", outputBuffer, convertFromUTF16, (int) bufLen, (int) sizeof(wchar_t));		
		
		if (convertFromUTF16)
			free(inputBuffer);
    }
    else
	{
        outputBuffer = kNullPluginError;
    }
    
	return outputBuffer;    
}

// --------------------------------------------------------------------------------
//! Test Entrypoint.
// --------------------------------------------------------------------------------

char * formatUTFTest(void)
{
	return "this is a test";
}

// --------------------------------------------------------------------------------
//! Entrypoint for a UTF32 character.
//!
//! Will also do 16-bit characters since it's easy enough to convert it up to a
//! wchar_t.
// --------------------------------------------------------------------------------

char * formatUTF32(wchar_t wcharData, int identifier)
{
    size_t bufLen = 2;
    return dataformatter_char_for_wchar(&wcharData, identifier, bufLen, 0);
}

// --------------------------------------------------------------------------------
// Entrypoint for a wchar_t/UTF32 string
// --------------------------------------------------------------------------------

char * formatUTF32Pointer(wchar_t *wcharData, int identifier)
{
    size_t bufLen = 255; 
    return dataformatter_char_for_wchar(wcharData, identifier, bufLen, 0);
}

// --------------------------------------------------------------------------------
// Entrypoint for a UTF16 string
// --------------------------------------------------------------------------------

char * formatUTF16Pointer(unsigned short *wcharData, int identifier)
{
    size_t bufLen = 255; 
    return dataformatter_char_for_wchar((wchar_t*) wcharData, identifier, bufLen, 1);
}
