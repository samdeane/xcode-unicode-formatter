/*

File: myCustomFormatter.c

Abstract: wchardataformatter - a custom data formatter example for displaying content in 
the built-in Xcode Debugger.  The example uses wchar data.

Version: <1.0>

¬© Copyright 2006 Apple Computer, Inc. All rights reserved.

IMPORTANT:  This Apple software is supplied to 
you by Apple Computer, Inc. ("Apple") in 
consideration of your agreement to the following 
terms, and your use, installation, modification 
or redistribution of this Apple software 
constitutes acceptance of these terms.  If you do 
not agree with these terms, please do not use, 
install, modify or redistribute this Apple 
software.

In consideration of your agreement to abide by 
the following terms, and subject to these terms, 
Apple grants you a personal, non-exclusive 
license, under Apple's copyrights in this 
original Apple software (the "Apple Software"), 
to use, reproduce, modify and redistribute the 
Apple Software, with or without modifications, in 
source and/or binary forms; provided that if you 
redistribute the Apple Software in its entirety 
and without modifications, you must retain this 
notice and the following text and disclaimers in 
all such redistributions of the Apple Software. 
Neither the name, trademarks, service marks or 
logos of Apple Computer, Inc. may be used to 
endorse or promote products derived from the 
Apple Software without specific prior written 
permission from Apple.  Except as expressly 
stated in this notice, no other rights or 
licenses, express or implied, are granted by 
Apple herein, including but not limited to any 
patent rights that may be infringed by your 
derivative works or by other works in which the 
Apple Software may be incorporated.

The Apple Software is provided by Apple on an "AS 
IS" basis.  APPLE MAKES NO WARRANTIES, EXPRESS OR 
IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED 
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY 
AND FITNESS FOR A PARTICULAR PURPOSE, REGARDING 
THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE 
OR IN COMBINATION WITH YOUR PRODUCTS.

IN NO EVENT SHALL APPLE BE LIABLE FOR ANY 
SPECIAL, INDIRECT, INCIDENTAL OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, 
REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION OF 
THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER 
UNDER THEORY OF CONTRACT, TORT (INCLUDING 
NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN 
IF APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF 
SUCH DAMAGE.
*/



// Read this header for more information on custom data formatters and allocators
#import "/Developer/Applications/Xcode.app/Contents/PlugIns/GDBMIDebugging.xcplugin/Contents/Headers/DataFormatterPlugin.h"

#include <stdlib.h>
#include <wchar.h>
#include <string.h>

/* Custom data formatters should be installed in:

/Library/Application Support/Apple/Developer\ Tools/CustomDataViews

*/

//  A bundle to support custom data formatting must define the following symbol:
_pbxgdb_plugin_function_list *_pbxgdb_plugin_functions;

static char *nullPluginFunctions = "CFDataFormatter plugin error: _pbxgdb_plugin_functions not set!";

// All data formatters plugins can currently only return a 'char *' to be displayed
// in the Summary column, so this can be a lossy conversion.

// Read the DataFormatterPlugin.h header for more information on custom data formatters and allocators requirements
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
		/*int n;
		printf("original:");
		for (n = 0; n < bufLen; ++n) { printf("%08x", wcharData[n]); }
		printf("\ninput:");
		for (n = 0; n < bufLen; ++n) { printf("%08x", inputBuffer[n]); }
        printf("\nwchar: %s, convert:%d, bufLen:%d, wchar size:%d\n", outputBuffer, convertFromUTF16, (int) bufLen, (int) sizeof(wchar_t));
		*/
		
		if (convertFromUTF16)
			free(inputBuffer);
    }
    else
	{
        outputBuffer = nullPluginFunctions;
    }
    
	return outputBuffer;    
}

char * formatUTF32(wchar_t wcharData, int identifier)
{
    size_t bufLen = 2;
    return dataformatter_char_for_wchar(&wcharData, identifier, bufLen, 0);
}

char * formatUTF32Pointer(wchar_t *wcharData, int identifier)
{
    size_t bufLen = 255; 
    return dataformatter_char_for_wchar(wcharData, identifier, bufLen, 0);
}

char * formatUTF16Pointer(unsigned short *wcharData, int identifier)
{
    size_t bufLen = 255; 
    return dataformatter_char_for_wchar((wchar_t*) wcharData, identifier, bufLen, 1);
}
