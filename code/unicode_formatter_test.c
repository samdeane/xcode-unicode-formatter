// ================================================================================
// $Id: $
//
//! To use this sample, build the target in the <Debug> configuration and set a
//! breakpoint at the start of the program.  Build and debug the example, and step
//! through the code.  Inspect the values of the d and b variables after the last 
//! operation - they will be type wchar_t and displaying correctly in the debugger.
//
//! See http://www.elegantchaos.com/projects/unicode-formatter for more info.
//
// Copyright (c) 2009 Elegant Chaos. Based on Apple sample code.
// ================================================================================

#include <stdlib.h>
#include <wchar.h>
#include <string.h>

int main(int argc, char *argv[])
{
	const wchar_t* null_string = NULL;
    wchar_t c = 'A';
    wchar_t b[255];
    wchar_t *d = &b[0];
    char *a = "a b c d e f g";
	wchar_t* test = L"this is a test";
    mbstowcs(d, a , strlen(a));
	return 0;

}