// ================================================================================
// $Id$
//
//! To use this sample, build the target in the <Debug> configuration and set a
//! breakpoint at the start of the program.  Build and debug the example, and step
//! through the code.  Inspect the values of the d and b variables after the last 
//! operation - they will be type wchar_t and displaying correctly in the debugger.
//
// Copyright (c) 2007 Elegant Chaos. All Rights Reserved.
// ================================================================================

#include <stdlib.h>
#include <wchar.h>

	typedef wchar_t C16;

int main(int argc, char *argv[])
{
	const wchar_t* null_string = NULL;
    C16 c = 'A';
    C16 b[255];
    C16 *d = &b[0];
    char *a = "a b c d e f g";
	C16* test = L"this is a test";
    mbstowcs(d, a , strlen(a));
	return 0;

}