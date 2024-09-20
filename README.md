**NOTE:** All 0.x releases are kind of "alpha-versions" without expectations of API backward-compatibility.

## Table of Contents
[mdz_ansi_16 Overview](#mdz_ansi-Overview)<br>
[mdz_ansi_16 Advantages](#mdz_ansi-Advantages)<br>
[mdz_ansi_16 Usage](#mdz_ansi_16-Usage)<br>
[Licensing info](#Licensing-info)<br>

## mdz_ansi_16 Overview
Please take a look at *"mdz_ansi_16.h"* C header file or [mdz_ansi_16 Wiki] site for detailed functions descriptions.

[mdz_ansi_16 Wiki]: https://github.com/maxdz-gmbh/mdz_ansi_16/wiki

[mdz_ansi_16] - is a very lightweight, versatile and speedy C  library for handling single-byte (ASCII/ANSI) strings, developed by [maxdz Software GmbH]. Source code of library is highly-portable, conforms to ANSI C 89/90 Standard.

Only shared/dynamically loaded libraries (*.so* and *.dll* files with import libraries) are available for evaluation testing purposes. Static libraries are covered by our commercial licenses.

**Win32** binaries are built using Visual Studio Platform Toolset "v90", thus are compatible with Windows versions from Windows 2000.<br>
**Win64** binaries are built using Visual Studio Platform Toolset "v100", thus are compatible with Windows versions from Windows XP (64-bit).<br>

**Linux** binaries are built against Linux Kernel 2.6.17 - and thus should be compatible with Debian (from ver. 4), Ubuntu (from ver. 6.10), Fedora (from ver. 9), Red Hat/CentOS (from ver. 5)

**FreeBSD** binaries - may be used from FreeBSD ver. 7.0 (for x86) and ver. 8.0 (for x64)

~~**Android** x86/armeabi-v7a binaries - may be used from Android API level 16 ("Jelly Bean" ver. 4.1.x)<br>~~
~~**Android** x86_64/arm64-v8a binaries - may be used from Android API level 21 ("Lollipop" ver. 5.0)~~

~~**macOS** binaries - x86_64, from *MacOS X v10.6.0*~~

[mdz_ansi_16]: https://github.com/maxdz-gmbh/mdz_ansi_16
[maxdz Software GmbH]: https://maxdz.com/

## mdz_ansi_16 Advantages

**1. High Portability:** The entire codebase conforms to the ANSI C 89/90 standard.

**2. Minimal Dependencies:** *mdz_ansi_16* functions solely depend on standard C-library functions. This means you can integrate the library into your code without any additional dependencies beyond the standard platform libraries/APIs.

**3. Performance:** The library functions are highly optimized for speed, particularly for operations like searching. Performance is very similar to [mdz_ansi_alg] functions.

**4. Flexibilty:** Most library functions offer both "left position" and "right position" parameters, allowing you to limit the processing area. Additionally, the library provides more string functions than comparable libraries such as STL, Boost, or GLib.

**5. Extended Error-Checking:** All functions use strict error-checking mechanisms and return specific error codes pointing the problem precisely.

**6. Extended Control:** The functions perform only explicit operations. For example, when an "insert" function is called, it will return an error if there is not enough capacity in string - no implicit memory allocations/reservations will be made.

**7. No Memory Overhead:** The functions are designed to process input data without making internal memory allocations or using dynamic memory.

**8. Metadata encapsulated:** String controls its Size and Capacity, thus making automatical checking and if necessary adjustment during each string function call.

[mdz_ansi_alg]: https://github.com/maxdz-gmbh/mdz_ansi_alg

## mdz_ansi_16 Usage

**Test license generation:** - in order to get free test-license, please proceed to our Shop page [maxdz Shop] and register an account. After registration you will be able to obtain free 30-days test-licenses for our products using "Obtain for free" button. 
Test license data should be used in *mdz_ansi_16_init()* call for library initialization.

**NOTE:** All 0.x releases are kind of "beta-versions" and can be used 1) only with test-license (during test period of 30 days, with necessity to re-generate license for the next 30 days test period) and 2) without expectations of interface backward-compatibility.

[mdz_ansi_16 Wiki]: https://github.com/maxdz-gmbh/mdz_ansi_16/wiki
[maxdz Shop]: https://maxdz.com/shop.php

#### Code Example

file "test.c"

```
#include <mdz_ansi_16.h>

#include <string.h>
#include <assert.h>

int main(void)
{
  char pcBuffer[100];
  unsigned short nBuffSize = sizeof(pcBuffer);
  mdz_Ansi16* psAnsi16;

  size_t nPosition, nCount;
  enum mdz_error enError;
  enum mdz_ansi_compare_result enCompareResult;

  unsigned long pnFirstNameHash[] = { /* your personal first name hash */ };
  unsigned long pnLastNameHash[] = { /* your personal last name hash */ };
  unsigned long pnEmailHash[] = { /* your personal last email hash */ };
  unsigned long pnLicenseHash[] = { /* your personal license hash */ };

  /* library/license initialization, must be called before any other library function */
  if (mdz_false == mdz_ansi_16_init(pnFirstNameHash, pnLastNameHash, pnEmailHash, pnLicenseHash))
  {
    return 0;
  }

  if (NULL == (psAnsi16 = mdz_ansi_16_attach(pcBuffer, 0, nBuffSize, MDZ_ATTACH_ZEROSIZE, &enError)))
  {
    return 0;
  }
  assert(0 == mdz_ansi_16_size(psAnsi16));
  assert(95 == mdz_ansi_16_capacity(psAnsi16));
  assert(0 == strcmp(mdz_ansi_16_data(psAnsi16), ""));

  if (MDZ_ERROR_NONE != mdz_ansi_16_insert(psAnsi16, 0, "0123456789", 10))
  {
    return 0;
  }
  assert(10 == mdz_ansi_16_size(psAnsi16));
  assert(95 == mdz_ansi_16_capacity(psAnsi16));
  assert(0 == strcmp(mdz_ansi_16_data(psAnsi16), "0123456789"));

  if (MDZ_ERROR_NONE != mdz_ansi_16_insert(psAnsi16, mdz_ansi_16_size(psAnsi16), "9876543210", 10))
  {
    return 0;
  }
  assert(20 == mdz_ansi_16_size(psAnsi16));
  assert(95 == mdz_ansi_16_capacity(psAnsi16));
  assert(0 == strcmp(mdz_ansi_16_data(psAnsi16), "01234567899876543210"));

  nPosition = mdz_ansi_16_findSingle(psAnsi16, 0, 10, '4', &enError);
  assert(4 == nPosition);
  assert(MDZ_ERROR_NONE == enError);

  nPosition = mdz_ansi_16_find(psAnsi16, 0, mdz_ansi_16_size(psAnsi16) - 1, "99", 2, &enError);
  assert(9 == nPosition);
  assert(MDZ_ERROR_NONE == enError);

  nPosition = mdz_ansi_16_rfindSingle(psAnsi16, 0, mdz_ansi_16_size(psAnsi16) - 1, '4', &enError);
  assert(15 == nPosition);
  assert(MDZ_ERROR_NONE == enError);

  nPosition = mdz_ansi_16_rfind(psAnsi16, 0, mdz_ansi_16_size(psAnsi16) - 1, "9", 1, &enError);
  assert(10 == nPosition);
  assert(MDZ_ERROR_NONE == enError);

  nPosition = mdz_ansi_16_firstOf(psAnsi16, 0, mdz_ansi_16_size(psAnsi16) - 1, "43", 2, &enError);
  assert(3 == nPosition);
  assert(MDZ_ERROR_NONE == enError);

  nPosition = mdz_ansi_16_firstNotOf(psAnsi16, 0, mdz_ansi_16_size(psAnsi16) - 1, "3210", 4, &enError);
  assert(4 == nPosition);
  assert(MDZ_ERROR_NONE == enError);

  nPosition = mdz_ansi_16_lastOf(psAnsi16, 0, mdz_ansi_16_size(psAnsi16) - 1, "34", 4, &enError);
  assert(16 == nPosition);
  assert(MDZ_ERROR_NONE == enError);

  nPosition = mdz_ansi_16_lastNotOf(psAnsi16, 0, mdz_ansi_16_size(psAnsi16) - 1, "0123", 4, &enError);
  assert(15 == nPosition);
  assert(MDZ_ERROR_NONE == enError);

  if (MDZ_ERROR_NONE != mdz_ansi_16_removeFrom(psAnsi16, 2, 3))
  {
    return 0;
  }
  assert(17 == mdz_ansi_16_size(psAnsi16));
  assert(95 == mdz_ansi_16_capacity(psAnsi16));
  assert(0 == strcmp(mdz_ansi_16_data(psAnsi16), "01567899876543210"));

  if (MDZ_ERROR_NONE != mdz_ansi_16_remove(psAnsi16, 0, mdz_ansi_16_size(psAnsi16) - 1, "99", 2))
  {
    return 0;
  }
  assert(15 == mdz_ansi_16_size(psAnsi16));
  assert(95 == mdz_ansi_16_capacity(psAnsi16));
  assert(0 == strcmp(mdz_ansi_16_data(psAnsi16), "015678876543210"));

  if (MDZ_ERROR_NONE != mdz_ansi_16_trimLeft(psAnsi16, 0, mdz_ansi_16_size(psAnsi16) - 1, "012", 3))
  {
    return 0;
  }
  assert(13 == mdz_ansi_16_size(psAnsi16));
  assert(95 == mdz_ansi_16_capacity(psAnsi16));
  assert(0 == strcmp(mdz_ansi_16_data(psAnsi16), "5678876543210"));

  if (MDZ_ERROR_NONE != mdz_ansi_16_trimRight(psAnsi16, 0, mdz_ansi_16_size(psAnsi16) - 1, "012", 3))
  {
    return 0;
  }
  assert(10 == mdz_ansi_16_size(psAnsi16));
  assert(95 == mdz_ansi_16_capacity(psAnsi16));
  assert(0 == strcmp(mdz_ansi_16_data(psAnsi16), "5678876543"));

  if (MDZ_ERROR_NONE != mdz_ansi_16_trim(psAnsi16, 0, mdz_ansi_16_size(psAnsi16) - 1, "012345", 6))
  {
    return 0;
  }
  assert(6 == mdz_ansi_16_size(psAnsi16));
  assert(95 == mdz_ansi_16_capacity(psAnsi16));
  assert(0 == strcmp(mdz_ansi_16_data(psAnsi16), "678876"));

  enCompareResult = mdz_ansi_16_compare(psAnsi16, 0, "678", 3, mdz_false, &enError);
  assert(MDZ_ANSI_COMPARE_NONEQUAL == enCompareResult);
  assert(MDZ_ERROR_NONE == enError);

  enCompareResult = mdz_ansi_16_compare(psAnsi16, 0, "678", 3, mdz_true, &enError);
  assert(MDZ_ANSI_COMPARE_EQUAL == enCompareResult);
  assert(MDZ_ERROR_NONE == enError);

  nCount = mdz_ansi_16_count(psAnsi16, 0, mdz_ansi_16_size(psAnsi16) - 1, "6", 1, mdz_true, &enError);
  assert(2 == nCount);
  assert(MDZ_ERROR_NONE == enError);

  return 0;
}
```

## Licensing info

Use of **mdz_ansi_alg** library is regulated by license agreement in *LICENSE.txt*

Basically private non-commercial "test" usage is unrestricted. Commercial usage of library (incl. its source code) will be regulated by according license agreement.
