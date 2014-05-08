/*------------------------------------------------------------------------------

CheckRegion.h - Reports region info on your Wii
Copyright (C) 2013 JoostinOnline

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

------------------------------------------------------------------------------*/

#ifndef __CHECKREGION_H__
#define __CHECKREGION_H__

/**
 * Version information for CheckRegion.
 */
#define CHECKREGION_VERSION "1.7"

//==============================================================================
// Includes
//==============================================================================
#include <gccore.h>
//==============================================================================

//==============================================================================
// HW_RVL header
//==============================================================================
#if defined(HW_RVL) /* defined(HW_RVL) */

//==============================================================================
// Error code definitions
//==============================================================================
#define CONF_EBADFILE			-0x6001
#define CONF_ENOENT 			-0x6002
#define CONF_ETOOBIG			-0x6003
#define CONF_ENOTINIT			-0x6004
#define CONF_ENOTIMPL			-0x6005
#define CONF_EBADVALUE			-0x6006
#define CONF_ENOMEM 			-0x6007
#define	CONF_ERR_OK 			 0

//==============================================================================
// Variable size definitions
//==============================================================================
#define SERIALCODE_SIZE          4
#define SERIALNUMBER_SIZE        10
#define MODEL_SIZE               13
#define COUNTRY_SIZE             44

//==============================================================================
// C++ header
//==============================================================================
#ifdef __cplusplus
extern "C" {
#endif
/* __cplusplus */

//==============================================================================
// Extra standard declarations
//==============================================================================
typedef signed int s32;
//==============================================================================


/**
 * Multi-dimensional array of country names.  Their location corresponds with the shop
 * code given by CONF_GetShopCode(u8 *shopcode)
 */
extern const char *CONF_CountryCodes[];


/**
 * CheckRegion CONF codes
 */
enum {
    /*------------------------Wii Codes------------------------*/
    CONF_CODE_JPN = 0,      /**< CODE "LJF" (Original model)   */
    CONF_CODE_USA,          /**< CODE "LU"  (Original model)   */
    CONF_CODE_EURH,         /**< CODE "LEH" (Original model)   */
    CONF_CODE_EURM,         /**< CODE "LEM" (Original model)   */
    CONF_CODE_EURF,         /**< CODE "LEF" (Original model)   */
    CONF_CODE_KOR,          /**< CODE "LKM" (Original model)   */
    CONF_CODE_AUS,          /**< CODE "LAH" (Original model)   */
    CONF_CODE_USAK,         /**< CODE "KU"  (New model)        */
    CONF_CODE_EURHK,        /**< CODE "KEH" (New model)        */
    CONF_CODE_EURMK,        /**< CODE "KEM" (New model)        */
    CONF_CODE_EURFK,        /**< CODE "KEF" (New model)        */
    CONF_CODE_AUSK,         /**< CODE "KAM" (New model)        */
    CONF_CODE_DLPH,         /**< CODE "LE"  (Dolphin emulator) */
    /*-----------------------Wii U Codes-----------------------*/
    CONF_CODE_VJPNI,        /**< CODE "IJF" (8GB model)        */
    CONF_CODE_VJPNO,        /**< CODE "OJF" (32GB model)       */ //Unconfirmed
    CONF_CODE_VUSAI,        /**< CODE "IW"  (8GB model)        */
    CONF_CODE_VUSAO,        /**< CODE "OW"  (32GB model)       */
    CONF_CODE_VEURHI,       /**< CODE "IEH" (8GB model)        */
    CONF_CODE_VEURHO,       /**< CODE "OEH" (32GB model)       */
    CONF_CODE_VEURMI,       /**< CODE "IEM" (8GB model)        */ //Unconfirmed
    CONF_CODE_VEURMO,       /**< CODE "OEM" (32GB model)       */
    CONF_CODE_VEURFI,       /**< CODE "IEF" (8GB model)        */
    CONF_CODE_VEURFO,       /**< CODE "OEF" (32GB model)       */
    CONF_CODE_VKORI,        /**< CODE "I??" (8GB model)        */ //Not released
    CONF_CODE_VKORO,        /**< CODE "O??" (32GB model)       */ //Not released
    CONF_CODE_VAUSI,        /**< CODE "IAH" (8GB model)        */ //Unconfirmed
    CONF_CODE_VAUSO         /**< CODE "OAH" (32GB model)       */
};


/**
 * Initializes everything necessary for CONF functions.  Must be run
 * before any other CONF function, including those in ogc\conf.h
 * @return 0 for success, anything else means init failed
 *      CONF_ERR_OK     : Success   - Initialization was successful, or was already complete
 *      CONF_EBADFILE   : Error     - setting.txt or SYSCONF is corrupt
 *      Anything else   : Error     - Not enough free memory
 */
s32 CONF_Init(void);


/**
 * Gets value of CODE in setting.txt
 * @param serialcode Pointer to a 4 character array.  Can be NULL.
 * @example char serialcode[SERIALCODE_SIZE] = {0}; CONF_GetSerialCode(serialcode);
 * @return Integer representing code
 *      CONF_CODE_*     : Success   - serialcode was retrieved and is a known code
 *      CONF_ENOENT     : Error     - Could not find serialcode in setting.txt
 *      CONF_ENOTINIT   : Error     - CONF_Init() was not run before function
 *      CONF_EBADVALUE  : Error     - Unknown serial code
 *      CONF_ENOMEM     : Error     - Not enough free memory
 */
s32 CONF_GetSerialCode(char *serialcode);


/**
 * Gets value of SERNO in setting.txt
 * @param serialnumber Pointer to a 12 character array.  Can be NULL.
 * @example char serialnumber[SERIALNUMBER_SIZE] = {0}; CONF_GetSerialNumber(serialnumber);
 * @return Integer representing code
 *      CONF_ERR_OK     : Success   - Retrieved serialnumber
 *      CONF_ENOENT     : Error     - Could not find serialnumber in setting.txt
 *      CONF_ENOTINIT   : Error     - CONF_Init() was not run before function
 *      CONF_ENOMEM     : Error     - Not enough free memory
 */
s32 CONF_GetSerialNumber(char *serialnumber);


/**
 * Gets value of MODEL in setting.txt
 * @param model Pointer to a 13 character array.  Can be NULL.
 * @example char model[MODEL_SIZE] = {0}; CONF_GetModel(model);
 * @return Integer representing code
 *      CONF_ERR_OK     : Success   - Retrieved model
 *      CONF_ENOENT     : Error     - Could not find model in setting.txt
 *      CONF_ENOTINIT   : Error     - CONF_Init() was not run before function
 *      CONF_ENOMEM     : Error     - Not enough free memory
 */
s32 CONF_GetModel(char *model);


/**
 * Gets the Shop Channel country code in SYSCONF
 * @see http://wiibrew.org/wiki/Country_Codes
 * @param shopcode Pointer to an unsigned 8bit integer. Can be NULL.
 * @example u8 shopcode = 0; 
 *          CONF_GetShopCode(&shopcode);
 *          printf("Shop Channel Country is %s", CONF_CountryCodes[shopcode]);
 * @return Integer representing code
 *      CONF_ERR_OK     : Success   - Retrieved shopcode
 *      CONF_ENOENT     : Error     - Could not find shopcode in SYSCONF
 *      CONF_ENOTINIT   : Error     - CONF_Init() was not run before function
 *      CONF_ENOMEM     : Error     - Not enough free memory
 */
s32 CONF_GetShopCode(u8 *shopcode);


//==============================================================================
// C++ footer
//==============================================================================
#ifdef __cplusplus
}
#endif /* __cplusplus */

//==============================================================================
// HW_RVL footer
//==============================================================================
#endif /* defined(HW_RVL) */

#endif // __CHECKREGION_H__

/**
 * @section Introduction
 * CheckRegion provides region information about your Wii.
 * 
 * While AnyRegionChanger (and its mods) can modify all the region
 * settings that are needed for the Wii to work, it does not change
 * the value of "CODE", which are the letters that precede the serial
 * number.  From that, you can determine the original region.
 * 
 * 
 *                        ***IMPORTANT***
 * If your Wii was purchased before May 2011 or does not have GC ports,
 * the report should be 100% accurate.
 * 
 * If that's not the case, there is a chance someone used GiantPune's
 * Ohneschwanzenegger and a GC controller to create a new NAND, which
 * allows a custom serial number, thus fooling CheckRegion.  You can
 * only then assume it's right if it reports a different region than
 * your Wii's current one.
 * 
 * @section Links
 * Forum : http://HacksDen.com
 * Code  : http://tinyurl.com/CheckRegion
 * 
 * @section Credits
 * God for always being there.
 * LiNkZoR, JohnyNyga, VampireLordAlucard, sureiya, Drexyl, and fudgenuts64 for information on the new Wii models.
 * marcan for conf.c/conf.h, which this is heavily based on.
 * megazig for several tips and suggestions.
 * 
 * @section Licence
 * Copyright (C) 2013 JoostinOnline
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
