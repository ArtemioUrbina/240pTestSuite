/*------------------------------------------------------------------------------

CheckRegion.c - Reports region info on your Wii
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

//==============================================================================
// Includes
//==============================================================================
#include <string.h>
#include "CheckRegion.h"
//==============================================================================

//==============================================================================
// Defines
//==============================================================================
#define SADR_LENGTH 0x1007+1
//==============================================================================

//==============================================================================
// External functions
//==============================================================================
extern int __CONF_GetTxt(const char *name, char *buf, int length);
//==============================================================================

const char *CONF_CountryCodes[] = {
	"",
	"Japan",
	"",
	"",
	"",
	"",
	"",
	"",
	"Anguila",
	"Antigua and Barbuda",
	"Argentina",
	"Aruba",
	"Bahamas",
	"Barbados",
	"Belize",
	"Bolivia",
	"Brazil",
	"British Virgin Islands",
	"Canada",
	"Cayman Islands",
	"Chile",
	"Colombia",
	"Costa Rica",
	"Dominica",
	"Dominican Republic",
	"Ecuador",
	"El Salvador",
	"French Guiana",
	"Grenada",
	"Guadeloupe",
	"Guatemala",
	"Guyana",
	"Haiti",
	"Honduras",
	"Jamaica",
	"Martinique",
	"Mexico",
	"Monsterrat",
	"Netherlands Antilles",
	"Nicaragua",
	"Panama",
	"Paraguay",
	"Peru",
	"St. Kitts and Nevis",
	"St. Lucia",
	"St. Vincent and the Grenadines",
	"Suriname",
	"Trinidad and Tobago",
	"Turks and Caicos Islands",
	"United States",
	"Uruguay",
	"US Virgin Islands",
	"Venezuela",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"Albania",
	"Australia",
	"Austria",
	"Belgium",
	"Bosnia and Herzegovina",
	"Botswana",
	"Bulgaria",
	"Croatia",
	"Cyprus",
	"Czech Republic",
	"Denmark",
	"Estonia",
	"Finland",
	"France",
	"Germany",
	"Greece",
	"Hungary",
	"Iceland",
	"Ireland",
	"Italy",
	"Latvia",
	"Lesotho",
	"Lichtenstein",
	"Lithuania",
	"Luxembourg",
	"F.Y.R of Macedonia",
	"Malta",
	"Montenegro",
	"Mozambique",
	"Namibia",
	"Netherlands",
	"New Zealand",
	"Norway",
	"Poland",
	"Portugal",
	"Romania",
	"Russia",
	"Serbia",
	"Slovakia",
	"Slovenia",
	"South Africa",
	"Spain",
	"Swaziland",
	"Sweden",
	"Switzerland",
	"Turkey",
	"United Kingdom",
	"Zambia",
	"Zimbabwe",
	"Azerbaijan",
	"Mauritania (Islamic Republic of Mauritania)",
	"Mali (Republic of Mali)",
	"Niger (Republic of Niger)",
	"Chad (Republic of Chad)",
	"Sudan (Republic of the Sudan)",
	"Eritrea (State of Eritrea)",
	"Dijibouti (Republic of Djibouti)",
	"Somalia (Somali Republic)",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"Korea",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"Taiwan",
	"",
	"Hong Kong",
	"MO",
	"Indonesia",
	"Singapore",
	"Thailand",
	"PH",
	"MY",
	"China",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"U.A.E.",
	"India",
	"EG",
	"Omar",
	"Qatar",
	"Kuwait",
	"Saudi Arabia",
	"Syria",
	"BH",
	"Jordan"
};


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
s32 CONF_GetSerialCode(char *serialcode) {
    int res;
    char buf[SERIALCODE_SIZE];
    res = __CONF_GetTxt("CODE", buf, SERIALCODE_SIZE);
    if(res < 0) {
        if (serialcode) strcpy(serialcode, "??");
        return res;
    }
    if (serialcode) strcpy(serialcode, buf);
    if(!strcmp(buf, "LJF")) return CONF_CODE_JPN;
    if(!strcmp(buf, "LU")) return CONF_CODE_USA;
    if(!strcmp(buf, "LEH")) return CONF_CODE_EURH;
    if(!strcmp(buf, "LEM")) return CONF_CODE_EURM;
    if(!strcmp(buf, "LEF")) return CONF_CODE_EURF;
    if(!strcmp(buf, "LKM")) return CONF_CODE_KOR;
    if(!strcmp(buf, "LAH")) return CONF_CODE_AUS;
    if(!strcmp(buf, "KU")) return CONF_CODE_USAK;
    if(!strcmp(buf, "KEH")) return CONF_CODE_EURHK;
    if(!strcmp(buf, "KEM")) return CONF_CODE_EURMK;
    if(!strcmp(buf, "KEF")) return CONF_CODE_EURFK;
    if(!strcmp(buf, "KAM")) return CONF_CODE_AUSK;
    if(!strcmp(buf, "LE")) return CONF_CODE_DLPH;
    /*-----------------Wii U Codes-----------------*/
    if(!strcmp(buf, "IJF")) return CONF_CODE_VJPNI;
    if(!strcmp(buf, "OJF")) return CONF_CODE_VJPNO; //Unconfirmed
    if(!strcmp(buf, "IW")) return CONF_CODE_VUSAI;
    if(!strcmp(buf, "OW")) return CONF_CODE_VUSAO;
    if(!strcmp(buf, "IEH")) return CONF_CODE_VEURHI;
    if(!strcmp(buf, "OEH")) return CONF_CODE_VEURHO;
    if(!strcmp(buf, "IEM")) return CONF_CODE_VEURMI; //Unconfirmed
    if(!strcmp(buf, "OEM")) return CONF_CODE_VEURMO;
    if(!strcmp(buf, "IEF")) return CONF_CODE_VEURFI;
    if(!strcmp(buf, "OEF")) return CONF_CODE_VEURFO;
    //if(!strcmp(buf, "")) return CONF_CODE_VKORI;
    //if(!strcmp(buf, "")) return CONF_CODE_VKORO;
    if(!strcmp(buf, "IAH")) return CONF_CODE_VAUSI; //Unconfirmed
    if(!strcmp(buf, "OAH")) return CONF_CODE_VAUSO;
    return CONF_EBADVALUE;
}


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
s32 CONF_GetSerialNumber(char *serialnumber) {
    int res;
    char buf[SERIALNUMBER_SIZE];
    res = __CONF_GetTxt("SERNO", buf, SERIALNUMBER_SIZE);
    if(res < 0) {
        if (serialnumber) strcpy(serialnumber, "?");
        return res;
    }
    if (serialnumber) strcpy(serialnumber, buf);
    return CONF_ERR_OK;
}


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
s32 CONF_GetModel(char *model) {
    int res;
    char buf[MODEL_SIZE];
    res = __CONF_GetTxt("MODEL", buf, MODEL_SIZE);
    if(res < 0) {
        if (model) strcpy(model, "?");
        return res;
    }
    if (model) strcpy(model, buf);
    return CONF_ERR_OK;
}


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
s32 CONF_GetShopCode(u8 *shopcode) {
    int res;
    u8 sadr[SADR_LENGTH];
    memset(sadr, 0, SADR_LENGTH);
    res = CONF_Get("IPL.SADR", sadr, SADR_LENGTH);
    if (res < 0) {
        if (shopcode) *shopcode = 0;
        return res;
    }
    if (shopcode) *shopcode = sadr[0];
    return CONF_ERR_OK;
}

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
 *                          ***IMPORTANT***
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
