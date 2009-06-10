// ================================================================================
// $Id$
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

#import "/Developer/Applications/Xcode.app/Contents/PlugIns/GDBMIDebugging.xcplugin/Contents/Headers/DataFormatterPlugin.h"

#include <CoreFoundation/CoreFoundation.h>

// --------------------------------------------------------------------------------
// Macros
// --------------------------------------------------------------------------------

#define DEBUG_PRINT(...) fprintf(stderr, __VA_ARGS__);

// --------------------------------------------------------------------------------
// Globals
// --------------------------------------------------------------------------------

//  A bundle to support custom data formatting must define the following symbol:
_pbxgdb_plugin_function_list *_pbxgdb_plugin_functions;

// --------------------------------------------------------------------------------
// Constants
// --------------------------------------------------------------------------------

static char* kNullPluginError = "CFDataFormatter plugin error: _pbxgdb_plugin_functions not set!";
static char* kNullInputError = "<null string>";
static const size_t max_length = 1024;

// --------------------------------------------------------------------------------
//! Convert a CFString into a char* buffer that we can return.
// --------------------------------------------------------------------------------

char* ConvertStringToEncoding(CFStringRef string, CFStringEncoding encoding, int identifier)
{
	char* output = NULL;
    if (_pbxgdb_plugin_functions)
	{
		// we want to convert the whole string
		CFRange range;
		range.location = 0;
		range.length = CFStringGetLength(string);
		
		// find out how big our utf-8 buffer needs to be
		CFIndex length_needed;
		CFStringGetBytes(string, range, encoding, '?', false, NULL, 0, &length_needed);
		
		// make the output buffer
		// just in case the convert call fails completely, we zero terminate it
        output = (char*)(_pbxgdb_plugin_functions->allocate(identifier, length_needed + 1));
		if (output)
		{
			output[0] = 0; 
			
			// try to get the actual string - we terminate it for safety
			CFStringGetBytes(string, range, encoding, '?', false, (UInt8*) output, length_needed, &length_needed);
			output[length_needed] = 0;
		}
    }
    
	if (output) DEBUG_PRINT("converted: %s\n", output);
		
	
	return output ? output : kNullPluginError;    
}

// --------------------------------------------------------------------------------
//! Takes a unicode character and returns it as a C string.
// --------------------------------------------------------------------------------

char* formatUnicodeCharacter(long character, int identifier)
{
	CFStringRef string = CFStringCreateWithBytes(NULL, (UInt8*) &character, sizeof(character), kCFStringEncodingUTF32LE, false);
//	CFStringRef string = CFStringCreateWithCString(NULL, "character test", kCFStringEncodingUTF8);												   ;
	char* result = ConvertStringToEncoding(string, kCFStringEncodingUTF8, identifier);
	CFRelease(string);
	
	return result;
}

// --------------------------------------------------------------------------------
//! Takes a pointer to a unicode string, and returns it as a C string.
//! The size of one character is passed in, so that we can tell if we're dealing
//! with 16 or 32 bit characters.
// --------------------------------------------------------------------------------

char* formatUnicodeString(long* input, size_t size_of_one_char, int identifier)
{
	if (!input)
		return kNullInputError;
	
	size_t length = 0;
	CFStringEncoding encoding;
	
	if (size_of_one_char == 2)
	{
		short* temp = (short*) input;
		while (*temp++ && (length < max_length)) length++;
		encoding = kCFStringEncodingUTF16LE;
	}
	else
	{
		long* temp = (long*) input;
		while (*temp++ && (length < max_length)) length++;
		encoding = kCFStringEncodingUTF32LE;
	}
	
	CFStringRef string = CFStringCreateWithBytes(NULL, (UInt8*) input, length * size_of_one_char, encoding, false);
	char* result = ConvertStringToEncoding(string, kCFStringEncodingUTF8, identifier);
	CFRelease(string);

	return result;
}

// --------------------------------------------------------------------------------
//! Takes a pointer to a SICORE string
// --------------------------------------------------------------------------------

char* formatCoreString(long* input, int identifier)
{
	if (!input)
		return kNullInputError;
	
	if (((char*) input)[0] != '\1')
		return (char*) input;

	size_t length = 0;
	short* actual_buffer = (short*) (input + 1);
	short* temp = actual_buffer;
	while (*temp++ && (length < max_length)) length++;
	
	CFStringRef string = CFStringCreateWithBytes(NULL, (UInt8*) actual_buffer, length * 2, kCFStringEncodingUTF16LE, false);
	char* result = ConvertStringToEncoding(string, kCFStringEncodingUTF8, identifier);
	CFRelease(string);
	
	return result;
}
