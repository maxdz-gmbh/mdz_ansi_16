#ifndef MDZ_ATTACH_TYPE_H
#define MDZ_ATTACH_TYPE_H
/**
 * \ingroup mdz libraries
 *
 * \author maxdz Software GmbH
 *
 * \par license
 * This file is subject to the terms and conditions defined in file 'LICENSE.txt', which is part of this source code package.
 *
 * \par description
 * mdz attachment type enum for different mdz libraries
 *
 */

/**
 * Attachment type
 */
enum mdz_attach_type
{
  /**
   * Size is 0 after attachment
   */
  MDZ_ATTACH_ZEROSIZE = 0,

  /**
   * Size is predefined for attachment, 0-terminator must exist on [Size] position
   */
  MDZ_ATTACH_SIZE_TERMINATOR /* = 1 */,

  /**
   * Size is predefined for attachment, 0-terminator will be written on [Size] position
   */
  MDZ_ATTACH_SIZE_NO_TERMINATOR /* = 2*/
};

#endif
