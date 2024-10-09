/**
 * \ingroup mdz_ansi_16 library
 *
 * \author maxdz Software GmbH
 *
 * \par license
 * This file is subject to the terms and conditions defined in file 'LICENSE.txt', which is part of this source code package.
 *
 * \par description
 * mdz_ansi_16 is library for contiguous single-byte string, containing ASCII (0..127) and "ANSI" (128 - 255) characters.
 * There are no dynamic memory-allocations happen inside functions.
 *
 * String may contain 0-terminators ('\0') inside, and must end with 0-terminator.
 *
 * String should be attached to static/pre-allocated char* buffer and contains metadata (Size, Capacity).
 *
 * Capacity - how many bytes of memory is reserved for string content.
 * Size - how many characters are actually residing in a string.
 *
 * \par portability
 * Source code of library conforms to ANSI C 89/90 Standard.
 *
 * \version 0.1
 *
 * \date 2024-09
 *
 */

#ifndef MDZ_ANSI_16_H
#define MDZ_ANSI_16_H

#include <stddef.h>

#include "mdz_bool.h"
#include "mdz_ansi_compare_result.h"
#include "mdz_ansi_replace_type.h"
#include "mdz_error.h"

typedef struct mdz_Ansi16 mdz_Ansi16;

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * \defgroup Init functions
   */

  /**
   * Initializes mdz_ansi_16 library and license. This function should be called before any other function of the library.
   * \param pnFirstNameHash - user first name hash code
   * \param pnLastNameHash  - user last name hash code
   * \param pnEmailHash     - user e-mail hash code
   * \param pnLicenseHash   - license hash code
   * \return:
   * mdz_true - if the initialization succeeded, otherwise mdz_false
   */
  mdz_bool mdz_ansi_16_init(const unsigned long* pnFirstNameHash, const unsigned long* pnLastNameHash, const unsigned long* pnEmailHash, const unsigned long* pnLicenseHash);

  /**
   * Attach string to pre-allocated pcBuffer of nBufferSize bytes. If penError is not NULL, error will be written there
   * \param pcBuffer     - pointer to pre-allocated buffer to attach. Buffer has following structure: 4 bytes (reserved) + Data, ending with 0-terminator. Thus minimal pcBuffer size is 5 bytes (in this case Capacity is 0)
   * \param nBufferSize  - size of pcBuffer in bytes; should be at least 5 bytes (in this case Capacity is 0)
   * \param penError     - if not NULL, error will be written there. There are following errors possible:
   * \value:
   * MDZ_ERROR_LICENSE  - license is not initialized using mdz_ansi_16_init() or invalid
   * MDZ_ERROR_DATA     - pcBuffer is NULL
   * MDZ_ERROR_CAPACITY - nBufferSize < 5
   * MDZ_ERROR_NONE     - function succeeded
   * \return:
   * NULL   - function failed
   * Result - pointer to string for use in other mdz_ansi_16 functions
   */
  mdz_Ansi16* mdz_ansi_16_attach(char* pcBuffer, unsigned short nBufferSize, enum mdz_error* penError);

  /**
   * \defgroup Status functions
   */

  /**
   * Return Size of string Data in characters/bytes.
   * \param psAnsi - pointer to string returned by mdz_ansi_16_attach()
   * \return:
   * 0    - if psAnsi is NULL
   * Size - otherwise
   */
  unsigned short mdz_ansi_16_size(const mdz_Ansi16* psAnsi);

  /**
   * Return Capacity of string Data in characters/bytes.
   * \param psAnsi - pointer to string returned by mdz_ansi_16_attach()
   * \return:
   * 0        - if psAnsi is NULL
   * Capacity - otherwise
   */
  unsigned short mdz_ansi_16_capacity(const mdz_Ansi16* psAnsi);

  /**
   * Return pointer to string Data.
   * \param psAnsi - pointer to string returned by mdz_ansi_16_attach()
   * \return:
   * 0    - if psAnsi is NULL
   * Data - otherwise
   */
  char* mdz_ansi_16_data(mdz_Ansi16* psAnsi);

  /**
   * Return const pointer to string Data.
   * \param psAnsi - pointer to string returned by mdz_ansi_16_attach()
   * \return:
   * 0    - if psAnsi is NULL
   * Data - otherwise
   */
  const char* mdz_ansi_16_dataConst(const mdz_Ansi16* psAnsi);

  /**
   * \defgroup Insert/remove functions
   */

  /**
   * Insert pcItems from nLeftPos position. Data and pcItems cannot overlap. New Size is written in psAnsi.
   * \param psAnsi   - pointer to string returned by mdz_ansi_16_attach(). It should have enough Capacity for insertion of pcItems
   * \param nLeftPos - 0-based position to insert. If nLeftPos == Size items are appended. nLeftPos > Size is not allowed
   * \param pcItems  - items to insert. Cannot be NULL
   * \param nCount   - number of items to insert. Cannot be 0
   * \return:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_16_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is 0 or too large
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ITEMS      - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIG_LEFT   - nLeftPos > Size
   * MDZ_ERROR_BIG_COUNT  - Size + nCount > Capacity
   * MDZ_ERROR_OVERLAP    - [Data; Data + Size + nCount] area and pcItems overlap
   * MDZ_ERROR_NONE       - function succeeded
   */
  enum mdz_error mdz_ansi_16_insert(mdz_Ansi16* psAnsi, size_t nLeftPos, const char* pcItems, size_t nCount);

  /**
   * \defgroup Find functions
   */

  /**
   * Find first occurrence of cItem. Returns 0-based position of match (if found), or SIZE_MAX if not found or error happened. If penError is not NULL, error will be written there
   * \param psAnsi    - pointer to string returned by mdz_ansi_16_attach()
   * \param nLeftPos  - 0-based start position to search from left. Use 0 to search from the beginning of Data
   * \param nRightPos - 0-based end position to search up to. Use Size-1 to search till the end of Data
   * \param cItem     - character to find
   * \param penError  - if not NULL, error will be written there. There are following errors possible:
   * \value:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_16_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIG_SIZE   - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_BIG_RIGHT  - nRightPos >= Size
   * MDZ_ERROR_BIG_LEFT   - nLeftPos > nRightPos
   * MDZ_ERROR_NONE       - function succeeded
   * \return:
   * SIZE_MAX - if cItem not found or error happened
   * Result   - 0-based position of first match
   */
  size_t mdz_ansi_16_findSingle(const mdz_Ansi16* psAnsi, size_t nLeftPos, size_t nRightPos, char cItem, enum mdz_error* penError);

  /**
   * Find first occurrence of pcItems using optimized Boyer-Moore-Horspool search. Returns 0-based position of match (if found), or SIZE_MAX if not found or error happened. If penError is not NULL, error will be written there
   * \param psAnsi    - pointer to string returned by mdz_ansi_16_attach()
   * \param nLeftPos  - 0-based start position to search from left. Use 0 to search from the beginning of Data
   * \param nRightPos - 0-based end position to search up to. Use Size-1 to search till the end of Data
   * \param pcItems   - items to find. Cannot be NULL
   * \param nCount    - number of items to find. Cannot be 0
   * \param penError  - if not NULL, error will be written there. There are following errors possible:
   * \value:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_16_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIG_SIZE   - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ITEMS      - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIG_RIGHT  - nRightPos >= Size
   * MDZ_ERROR_BIG_LEFT   - nLeftPos > nRightPos
   * MDZ_ERROR_BIG_COUNT  - nCount is bigger than search area (between nLeftPos and nRightPos)
   * MDZ_ERROR_NONE       - function succeeded
   * \return:
   * SIZE_MAX - if pcItems not found or error happened
   * Result   - 0-based position of first match
   */
  size_t mdz_ansi_16_find(const mdz_Ansi16* psAnsi, size_t nLeftPos, size_t nRightPos, const char* pcItems, size_t nCount, enum mdz_error* penError);

  /**
   * Find last occurrence of cItem. Returns 0-based position of match (if found), or SIZE_MAX if not found or error happened. If penError is not NULL, error will be written there
   * \param psAnsi    - pointer to string returned by mdz_ansi_16_attach()
   * \param nLeftPos  - 0-based end position to find up to. Use 0 to search till the beginning of Data
   * \param nRightPos - 0-based start position to find from right. Use Size-1 to search from the end of Data
   * \param cItem     - character to find
   * \param penError  - if not NULL, error will be written there. There are following errors possible:
   * \value:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_16_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIG_SIZE   - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_BIG_RIGHT  - nRightPos >= Size
   * MDZ_ERROR_BIG_LEFT   - nLeftPos > nRightPos
   * MDZ_ERROR_NONE       - function succeeded
   * \return:
   * SIZE_MAX - if cItem not found or error happened
   * Result   - 0-based position of first match
   */
  size_t mdz_ansi_16_rfindSingle(const mdz_Ansi16* psAnsi, size_t nLeftPos, size_t nRightPos, char cItem, enum mdz_error* penError);

  /**
   * Find last occurrence of pcItems using optimized Boyer-Moore-Horspool search. Returns 0-based position of match (if found), or SIZE_MAX if not found or error happened. If penError is not NULL, error will be written there
   * \param psAnsi    - pointer to string returned by mdz_ansi_16_attach()
   * \param nLeftPos  - 0-based end position to find up to. Use 0 to search till the beginning of Data
   * \param nRightPos - 0-based start position to find from right. Use Size-1 to search from the end of Data
   * \param pcItems   - items to find. Cannot be NULL
   * \param nCount    - number of items to find. Cannot be 0
   * \param penError  - if not NULL, error will be written there. There are following errors possible:
   * \value:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_16_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIG_SIZE   - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ITEMS      - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIG_RIGHT  - nRightPos >= Size
   * MDZ_ERROR_BIG_LEFT   - nLeftPos > nRightPos
   * MDZ_ERROR_BIG_COUNT  - nCount is bigger than search area (between nLeftPos and nRightPos)
   * MDZ_ERROR_NONE       - function succeeded
   * \return:
   * SIZE_MAX - if pcItems not found or error happened
   * Result   - 0-based position of first match
   */
  size_t mdz_ansi_16_rfind(const mdz_Ansi16* psAnsi, size_t nLeftPos, size_t nRightPos, const char* pcItems, size_t nCount, enum mdz_error* penError);

  /**
   * Find first occurrence of any item of pcItems. Returns 0-based position of match (if found), or SIZE_MAX if not found or error happened. If penError is not NULL, error will be written there
   * \param psAnsi    - pointer to string returned by mdz_ansi_16_attach()
   * \param nLeftPos  - 0-based end position to find up to. Use 0 to search till the beginning of Data
   * \param nRightPos - 0-based start position to find from right. Use Size-1 to search till the end of Data
   * \param pcItems   - items to find. Cannot be NULL
   * \param nCount    - number of items to find. Cannot be 0
   * \param penError  - if not NULL, error will be written there. There are following errors possible:
   * \value:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_16_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIG_SIZE   - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ITEMS      - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIG_RIGHT  - nRightPos >= Size
   * MDZ_ERROR_BIG_LEFT   - nLeftPos > nRightPos
   * MDZ_ERROR_NONE       - function succeeded
   * \return:
   * SIZE_MAX - if no item of pcItems found or error happened
   * Result   - 0-based position of first match
   */
  size_t mdz_ansi_16_firstOf(const mdz_Ansi16* psAnsi, size_t nLeftPos, size_t nRightPos, const char* pcItems, size_t nCount, enum mdz_error* penError);

  /**
   * Find first non-occurrence of any item of pcItems. Returns 0-based position of match (if found), or SIZE_MAX if not found or error happened. If penError is not NULL, error will be written there
   * \param psAnsi    - pointer to string returned by mdz_ansi_16_attach()
   * \param nLeftPos  - 0-based end position to find up to. Use 0 to search till the beginning of Data
   * \param nRightPos - 0-based start position to find from right. Use Size-1 to search till the end of Data
   * \param pcItems   - items to find. Cannot be NULL
   * \param nCount    - number of items to find. Cannot be 0
   * \param penError  - if not NULL, error will be written there. There are following errors possible:
   * \value:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_16_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIG_SIZE   - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ITEMS      - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIG_RIGHT  - nRightPos >= Size
   * MDZ_ERROR_BIG_LEFT   - nLeftPos > nRightPos
   * MDZ_ERROR_NONE       - function succeeded
   * \return:
   * SIZE_MAX - if no item of pcItems found or error happened
   * Result   - 0-based position of first match
   */
  size_t mdz_ansi_16_firstNotOf(const mdz_Ansi16* psAnsi, size_t nLeftPos, size_t nRightPos, const char* pcItems, size_t nCount, enum mdz_error* penError);

  /**
   * Find last occurrence of any item of pcItems. Returns 0-based position of match (if found), or SIZE_MAX if not found or error happened. If penError is not NULL, error will be written there
   * \param psAnsi    - pointer to string returned by mdz_ansi_16_attach()
   * \param nLeftPos  - 0-based end position to find up to. Use 0 to search till the beginning of Data
   * \param nRightPos - 0-based start position to find from right. Use Size-1 to search till the end of Data
   * \param pcItems   - items to find. Cannot be NULL
   * \param nCount    - number of items to find. Cannot be 0
   * \param penError  - if not NULL, error will be written there. There are following errors possible:
   * \value:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_16_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIG_SIZE   - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ITEMS      - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIG_RIGHT  - nRightPos >= Size
   * MDZ_ERROR_BIG_LEFT   - nLeftPos > nRightPos
   * MDZ_ERROR_NONE       - function succeeded
   * \return:
   * SIZE_MAX - if no item of pcItems found or error happened
   * Result   - 0-based position of first match
   */
  size_t mdz_ansi_16_lastOf(const mdz_Ansi16* psAnsi, size_t nLeftPos, size_t nRightPos, const char* pcItems, size_t nCount, enum mdz_error* penError);

  /**
   * Find last non-occurrence of any item of pcItems. Returns 0-based position of match (if found), or SIZE_MAX if not found or error happened. If penError is not NULL, error will be written there
   * \param psAnsi    - pointer to string returned by mdz_ansi_16_attach()
   * \param nLeftPos  - 0-based end position to find up to. Use 0 to search till the beginning of Data
   * \param nRightPos - 0-based start position to find from right. Use Size-1 to search till the end of Data
   * \param pcItems   - items to find. Cannot be NULL
   * \param nCount    - number of items to find. Cannot be 0
   * \param penError  - if not NULL, error will be written there. There are following errors possible:
   * \value:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_16_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIG_SIZE   - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ITEMS      - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIG_RIGHT  - nRightPos >= Size
   * MDZ_ERROR_BIG_LEFT   - nLeftPos > nRightPos
   * MDZ_ERROR_NONE       - function succeeded
   * \return:
   * SIZE_MAX - if no item of pcItems found or error happened
   * Result   - 0-based position of first match
   */
  size_t mdz_ansi_16_lastNotOf(const mdz_Ansi16* psAnsi, size_t nLeftPos, size_t nRightPos, const char* pcItems, size_t nCount, enum mdz_error* penError);

  /**
   * \defgroup Insert/remove functions
   */

  /**
   * Remove nCount item(s) starting from 0-based nLeftPos position
   * \param psAnsi   - pointer to string returned by mdz_ansi_16_attach()
   * \param nLeftPos - 0-based start position to remove item(s) from. Use 0 to remove from the beginning of Data
   * \param nCount   - number of item(s) to remove. Cannot be 0
   * \return:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_16_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIG_SIZE   - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ZERO_SIZE  - Size is 0 (string is empty)
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIG_LEFT   - nLeftPos >= Size
   * MDZ_ERROR_BIG_COUNT  - nCount is bigger than remove area (between nLeftPos and Size)
   * MDZ_ERROR_NONE       - function succeeded
   */
  enum mdz_error mdz_ansi_16_removeFrom(mdz_Ansi16* psAnsi, size_t nLeftPos, size_t nCount);

  /**
   * Remove all ocurrences of nCount item(s) matching to pcItems, residing between nLeftPos and nRightPos
   * \param psAnsi    - pointer to string returned by mdz_ansi_16_attach()
   * \param nLeftPos  - 0-based start position to remove item(s) from. Use 0 to search from the beginning of Data
   * \param nRightPos - 0-based end position to remove item(s) up to. Use Size-1 to search till the end of Data
   * \param pcItems   - items to remove. Cannot be NULL
   * \param nCount    - number of item(s) to remove. Cannot be 0
   * \param bFromLeft - mdz_true if search for items to remove from left side, otherwise from right
   * \return:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_16_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIG_SIZE    - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ZERO_SIZE  - Size is 0 (string is empty)
   * MDZ_ERROR_ITEMS      - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIG_RIGHT  - nRightPos >= Size
   * MDZ_ERROR_BIG_LEFT   - nLeftPos > nRightPos
   * MDZ_ERROR_BIG_COUNT  - nCount is bigger than search area (between nLeftPos and nRightPos)
   * MDZ_ERROR_NONE       - function succeeded
   */
  enum mdz_error mdz_ansi_16_remove(mdz_Ansi16* psAnsi, size_t nLeftPos, size_t nRightPos, const char* pcItems, size_t nCount, mdz_bool bFromLeft);

  /**
   * Remove items which are contained in pcItems from left, until first non-contained in pcItems item is reached.
   * \param psAnsi    - pointer to string returned by mdz_ansi_16_attach()
   * \param nLeftPos  - 0-based start position to trim item(s) from left. Use 0 to trim from the beginning of Data
   * \param nRightPos - 0-based end position to trim item(s) up to. Use Size-1 to trim till the end of Data
   * \param pcItems   - items to trim. Cannot be NULL
   * \param nCount    - number of items to trim. Cannot be 0
   * \return:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_16_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIG_SIZE   - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ZERO_SIZE  - Size is 0 (string is empty)
   * MDZ_ERROR_ITEMS      - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIG_RIGHT  - nRightPos >= Size
   * MDZ_ERROR_BIG_LEFT   - nLeftPos > nRightPos
   * MDZ_ERROR_NONE       - function succeeded
   */
  enum mdz_error mdz_ansi_16_trimLeft(mdz_Ansi16* psAnsi, size_t nLeftPos, size_t nRightPos, const char* pcItems, size_t nCount);

  /**
   * Remove items which are contained in pcItems from right, until first non-contained in pcItems item is reached.
   * \param psAnsi    - pointer to string returned by mdz_ansi_16_attach()
   * \param nLeftPos  - 0-based end position to trim item(s) up to. Use 0 to trim till the beginning of Data
   * \param nRightPos - 0-based start position to trim item(s) from right. Use Size-1 to trim from the end of Data
   * \param pcItems   - items to trim. Cannot be NULL
   * \param nCount    - number of items to trim. Cannot be 0
   * \return:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_16_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIG_SIZE   - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ZERO_SIZE  - Size is 0 (string is empty)
   * MDZ_ERROR_ITEMS      - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIG_RIGHT  - nRightPos >= Size
   * MDZ_ERROR_BIG_LEFT   - nLeftPos > nRightPos
   * MDZ_ERROR_NONE       - function succeeded
   */
  enum mdz_error mdz_ansi_16_trimRight(mdz_Ansi16* psAnsi, size_t nLeftPos, size_t nRightPos, const char* pcItems, size_t nCount);

  /**
   * Remove items which are contained in pcItems from left and from right, until first non-contained in pcItems item is reached.
   * \param psAnsi    - pointer to string returned by mdz_ansi_16_attach()
   * \param nLeftPos  - 0-based start position to trim item(s) from left. Use 0 to trim from the beginning of Data
   * \param nRightPos - 0-based start position to trim item(s) from right. Use Size-1 to trim from the end of Data
   * \param pcItems   - items to trim. Cannot be NULL
   * \param nCount    - number of items to trim. Cannot be 0
   * \return:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_16_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIG_SIZE   - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ZERO_SIZE  - Size is 0 (string is empty)
   * MDZ_ERROR_ITEMS      - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIG_RIGHT  - nRightPos >= Size
   * MDZ_ERROR_BIG_LEFT   - nLeftPos > nRightPos
   * MDZ_ERROR_NONE       - function succeeded
   */
  enum mdz_error mdz_ansi_16_trim(mdz_Ansi16* psAnsi, size_t nLeftPos, size_t nRightPos, const char* pcItems, size_t nCount);

  /**
   * \defgroup Miscellaneous functions
   */

  /**
   * Compare content of Data with pcItems. If penError is not NULL, error will be written there
   * \param psAnsi          - pointer to string returned by mdz_ansi_16_attach()
   * \param nLeftPos        - 0-based start position to compare from left. Use 0 to compare from the beginning of Data
   * \param pcItems         - items to compare. Cannot be NULL
   * \param nCount          - number of items to compare. Cannot be 0
   * \param bPartialCompare - if mdz_true compare only nCount items, otherwise compare full strings
   * \param penError        - if not NULL, error will be written there. There are following errors possible:
   * \value:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_16_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIG_SIZE   - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ITEMS      - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIG_LEFT   - nLeftPos >= Size
   * MDZ_ERROR_BIG_COUNT  - nCount is bigger than compare area (between nLeftPos and Size)
   * MDZ_ERROR_NONE       - function succeeded
   * \return:
   * MDZ_COMPARE_EQUAL or MDZ_COMPARE_NONEQUAL - Result of comparison
   */
  enum mdz_ansi_compare_result mdz_ansi_16_compare(const mdz_Ansi16* psAnsi, size_t nLeftPos, const char* pcItems, size_t nCount, mdz_bool bPartialCompare, enum mdz_error* penError);

  /**
   * Counts number of pcItems substring occurences in Data. If penError is not NULL, error will be written there
   * \param psAnsi           - pointer to string returned by mdz_ansi_16_attach()
   * \param nLeftPos         - 0-based start position to search from left. Use 0 to search from the beginning of Data
   * \param nRightPos        - 0-based end position to search up to. Use Size-1 to search till the end of Data
   * \param pcItems          - items to find. Cannot be NULL
   * \param nCount           - number of items to find. Cannot be 0
   * \param bAllowOverlapped - mdz_true if overlapped substrings should be counted, otherwise mdz_false
   * \param bFromLeft        - mdz_true if search for items to count from left side, otherwise from right
   * \param penError         - if not NULL, error will be written there. There are following errors possible:
   * \value:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_16_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIG_SIZE   - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ITEMS      - pcItems is NULL
   * MDZ_ERROR_ZERO_COUNT - nCount is 0
   * MDZ_ERROR_BIG_RIGHT  - nRightPos >= Size
   * MDZ_ERROR_BIG_LEFT   - nLeftPos > nRightPos
   * MDZ_ERROR_BIG_COUNT  - nCount is bigger than search area (between nLeftPos and nRightPos)
   * MDZ_ERROR_NONE       - function succeeded
   * \return:
   * SIZE_MAX - if error happened
   * Result   - count of substring occurences. 0 if not found
   */
  size_t mdz_ansi_16_count(const mdz_Ansi16* psAnsi, size_t nLeftPos, size_t nRightPos, const char* pcItems, size_t nCount, mdz_bool bAllowOverlapped, mdz_bool bFromLeft, enum mdz_error* penError);

  /**
   * Replace every occurence of pcItemsBefore with pcItemsAfter in Data. There should be enough Capacity for replacing data.
   * \param psAnsi            - pointer to string returned by mdz_ansi_16_attach()
   * \param nLeftPos          - 0-based start position to search from left. Use 0 to search from the beginning of Data
   * \param nRightPos         - 0-based end position to search up to. Use Size-1 to search till the end of Data
   * \param pcItemsBefore     - items to find. Cannot be NULL
   * \param nCountBefore      - number of items to find. Cannot be 0
   * \param pcItemsAfter      - pointer to items to replace with. Can be NULL
   * \param nCountAfter       - number of items to replace. Can be 0
   * \param bFromLeft         - mdz_true if search for items to replace from left side, otherwise from right
   * \param enReplacementType - type of replacement when nCountAfter > nCountBefore (thus Size is growing). For now only MDZ_ANSI_REPLACE_DUAL is supported (please refer to description of mdz_ansi_replace_type enum)
   * \return:
   * MDZ_ERROR_LICENSE         - license is not initialized using mdz_ansi_16_init() or invalid
   * MDZ_ERROR_DATA            - psAnsi is NULL
   * MDZ_ERROR_CAPACITY        - Capacity is 0 or too large
   * MDZ_ERROR_BIG_SIZE        - Size > Capacity
   * MDZ_ERROR_ZERO_SIZE       - Size is 0 (string is empty)
   * MDZ_ERROR_TERMINATOR      - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_ITEMS           - pcItemsBefore is NULL
   * MDZ_ERROR_ZERO_COUNT      - nCountBefore is 0
   * MDZ_ERROR_BIG_RIGHT       - nRightPos >= Size
   * MDZ_ERROR_BIG_LEFT        - nLeftPos > nRightPos
   * MDZ_ERROR_BIG_COUNT       - nCount is bigger than search area (between nLeftPos and nRightPos)
   * MDZ_ERROR_OVERLAP         - Data overlaps with pcItemsBefore, or Data overlaps with pcItemsAfter
   * MDZ_ERROR_BIG_REPLACE     - new Size after replacement > Capacity
   * MDZ_ERROR_OVERLAP_REPLACE - Data after replacement - overlaps with pcItemsBefore, or Data after replacement - overlaps with pcItemsAfter
   * MDZ_ERROR_NONE            - function succeeded
   */
  enum mdz_error mdz_ansi_16_replace(mdz_Ansi16* psAnsi, size_t nLeftPos, size_t nRightPos, const char* pcItemsBefore, size_t nCountBefore, const char* pcItemsAfter, size_t nCountAfter, mdz_bool bFromLeft, enum mdz_ansi_replace_type enReplacementType);

  /**
   * Reverses characters in string, like "1234" into "4321".
   * \param psAnsi    - pointer to string returned by mdz_ansi_16_attach()
   * \param nLeftPos  - 0-based start position to search from left. Use 0 to search from the beginning of Data
   * \param nRightPos - 0-based end position to search up to. Use Size-1 to search till the end of Data
   * \return:
   * MDZ_ERROR_LICENSE    - license is not initialized using mdz_ansi_16_init() or invalid
   * MDZ_ERROR_DATA       - psAnsi is NULL
   * MDZ_ERROR_CAPACITY   - Capacity is too large
   * MDZ_ERROR_BIG_SIZE   - Size > Capacity
   * MDZ_ERROR_TERMINATOR - there is no 0-terminator on Data[Size] position
   * MDZ_ERROR_BIG_RIGHT  - nRightPos >= Size
   * MDZ_ERROR_BIG_LEFT   - nLeftPos >= nRightPos
   * MDZ_ERROR_NONE       - function succeeded
   */
  enum mdz_error mdz_ansi_16_reverse(mdz_Ansi16* psAnsi, size_t nLeftPos, size_t nRightPos);

#ifdef __cplusplus
}
#endif

#endif
