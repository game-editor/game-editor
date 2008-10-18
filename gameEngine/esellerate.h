/*
 * esellerate.h
 *   eSellerate SDK 3.6.5
 *   Copyright 2000-2007, Digital River, Inc.
 */

#ifndef _ESELLERATE_API_H_
#define _ESELLERATE_API_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef const char* eSellerate_InputString;
typedef char* eSellerate_OutputString;
typedef char* eSellerate_FileLocation;
typedef void* eSellerate_ResultData;

typedef long  eSellerate_ErrorCode;
typedef short eSellerate_ArraySize;
typedef short eSellerate_TrueOrFalse; /* true if non-zero, false if zero */

#ifndef _VALIDATE_API_H_
typedef short eSellerate_DaysSince2000;
#endif

#define eSellerate_FALSE 0
#define eSellerate_TRUE  1
#define eSellerate_TRUE_ButNoManualActivation 2 /* additional value for use by ActivateSerialNumber */

/* eSellerate_ErrorCode return values ***************************************************************/
#define eSellerate_SUCCESS  0 /* returned if the function fulfills the action requested of it       */
#define eSellerate_FAILURE  1 /* returned if the function cannot fulfill the action requested of it */
#define eSellerate_NOENGINE 2 /* returned if the Engine cannot be found                             */
#define eSellerate_CANCELED 3 /* returned if the Engine was exited before the action was completed  */
#define eSellerate_NOUPDATE 4 /* returned by SilentCheckForUpdate if no update is available         */

/* eSellerate_ErrorCode return values specific to product activation ********************************************************/
#define eSellerate_NoSuchActivationID      -25000 /* returned if the activation ID code cannot be found on the server       */
#define eSellerate_NoSuchSerialNumber      -25001 /* returned if the serial number cannot be found on the server            */
#define eSellerate_MissingSerialNumber     -25002 /* returned if no serial number is provided for activation                */
#define eSellerate_BlacklistedSerialNumber -25003 /* returned if the serial number has been blacklisted                     */
#define eSellerate_InvalidOrder            -25004 /* returned if the associated order is invalid, for instance returned     */
#define eSellerate_ActivationLimitMet      -25005 /* returned if the serial number has used all its available activations   */
#define eSellerate_MissingActivationKey    -25006 /* returned if no activation key can be found on the client computer      */
#define eSellerate_MachineMismatch         -25007 /* returned if the activation key does not reflect the client computer    */
#define eSellerate_InvalidActivationKey    -25008 /* returned if the activation does not validate for the given parameters  */
#define eSellerate_NonUniqueSerialNumber   -25009 /* returned if the serial number is not unique to one order               */
#define eSellerate_NoSuchActivationKey     -25010 /* returned if the activation is not found on the server                  */
#define eSellerate_DeactivationLimitMet    -25011 /* returned if the serial number has used all its available deactivations */
#define eSellerate_RequestDenied           -25012 /* returned if the server has deemed to deny the action requested         */

eSellerate_ErrorCode __stdcall
eSellerate_InstallEngine ( /* install the Engine from resource to its proper system location */
);

eSellerate_ErrorCode __stdcall
eSellerate_Purchase (                  /* call on the Engine to perform a purchase ******************/
  eSellerate_InputString publisherID,  /* publisher ID code (always needed)                         */
  eSellerate_InputString esellerID,    /* eSeller ID code (always needed)                           */
  eSellerate_InputString previewID,    /* preview certificate ID code                               */
  eSellerate_InputString errorURL,     /* URL to show in case of error                              */
  eSellerate_InputString openingTitle, /* custom title of the opening page of the Wizard            */
  eSellerate_InputString openingText,  /* custom first paragraph of the opening page of the Wizard  */
  eSellerate_TrueOrFalse showFolder,   /* have the Engine open a window to display downloaded files */
  eSellerate_InputString trackingID,   /* tracking ID code                                          */
  eSellerate_InputString affiliateID,  /* affiliate ID code                                         */
  eSellerate_InputString couponID,     /* coupon ID code                                            */
  eSellerate_InputString activationID, /* activation ID code, to activate serial number             */
  eSellerate_InputString extraData,    /* extra information, for instance for custom serial numbers */
  eSellerate_ResultData* resultData    /* handle to memory to be allocated and filled by the Engine */
);
/* additional failure codes:
 * eSellerate_NoSuchActivationID
 * eSellerate_MissingSerialNumber
 * eSellerate_NonUniqueSerialNumber
 * eSellerate_RequestDenied
 */

eSellerate_ErrorCode __stdcall
eSellerate_CheckForUpdate (            /* call on the Engine to check if an update is available *****/
  eSellerate_InputString publisherID,  /* publisher ID code (always needed)                         */
  eSellerate_InputString esellerID,    /* eSeller ID code (always needed)                           */
  eSellerate_InputString previewID,    /* preview certificate ID code                               */
  eSellerate_InputString errorURL,     /* URL to show in case of error                              */
  eSellerate_InputString openingTitle, /* custom title of the opening page of the Wizard            */
  eSellerate_InputString openingText,  /* custom first paragraph of the opening page of the Wizard  */
  eSellerate_TrueOrFalse showFolder,   /* have the Engine open a window to display downloaded files */
  eSellerate_InputString trackingID,   /* tracking ID code                                          */
  eSellerate_InputString affiliateID,  /* affiliate ID code                                         */
  eSellerate_InputString couponID,     /* coupon ID code                                            */
  eSellerate_InputString activationID, /* activation ID code, to activate serial number             */
  eSellerate_InputString extraData,    /* extra information, for instance for custom serial numbers */
  eSellerate_ResultData* resultData    /* handle to memory to be allocated and filled by the Engine */
);
/* additional failure codes:
 * eSellerate_NoSuchActivationID
 * eSellerate_NonUniqueSerialNumber
 * eSellerate_RequestDenied
 */

eSellerate_ErrorCode __stdcall
eSellerate_SilentCheckForUpdate (      /* check for an update without invoking the Wizard ***/
  eSellerate_InputString publisherID,  /* publisher ID code (always needed)                 */
  eSellerate_InputString esellerID,    /* eSeller ID code (always needed)                   */
  eSellerate_InputString previewID,    /* preview certificate ID code                       */
  eSellerate_TrueOrFalse askToConnect, /* ask to connect when no active connection is found */
  eSellerate_InputString trackingID    /* tracking ID code                                  */
);

eSellerate_ErrorCode __stdcall
eSellerate_Reinstall (                 /* call on the Engine to reinstall a previous purchase *******/
  eSellerate_InputString publisherID,  /* publisher ID code (always needed)                         */
  eSellerate_InputString esellerID,    /* eSeller ID code (always needed)                           */
  eSellerate_InputString previewID,    /* preview certificate ID code                               */
  eSellerate_InputString errorURL,     /* URL to show in case of error                              */
  eSellerate_InputString openingTitle, /* custom title of the opening page of the Wizard            */
  eSellerate_InputString openingText,  /* custom first paragraph of the opening page of the Wizard  */
  eSellerate_TrueOrFalse showFolder,   /* have the Engine open a window to display downloaded files */
  eSellerate_InputString trackingID,   /* tracking ID code                                          */
  eSellerate_InputString activationID, /* activation ID code, to activate serial number             */
  eSellerate_ResultData* resultData    /* handle to memory to be allocated and filled by the Engine */
);
/* additional failure codes:
 * eSellerate_NoSuchActivationID
 * eSellerate_MissingSerialNumber
 * eSellerate_BlacklistedSerialNumber
 * eSellerate_InvalidOrder
 * eSellerate_ActivationLimitMet
 * eSellerate_NonUniqueSerialNumber
 * eSellerate_RequestDenied
 */

eSellerate_ArraySize __stdcall
eSellerate_IndexSerialNumberData (             /* build array access to serial numbers output by the Engine ****/
  eSellerate_ResultData   resultData,          /* handle to the output from call to the Engine (always needed) */
  eSellerate_OutputString productIDArray[],    /* array to be filled for access to product ID codes            */
  eSellerate_OutputString serialNumberArray[], /* array to be filled for access to associated serial numbers   */
  eSellerate_OutputString nameBasedKeyArray[], /* array to be filled for access to associated name-based keys  */
  eSellerate_ArraySize    arraySize            /* size of arrays provided                                      */
);

eSellerate_ArraySize __stdcall
eSellerate_IndexFileLocationData (             /* build array access to file locations output by the Engine ****/
  eSellerate_ResultData   resultData,          /* handle to the output from the call to Engine (always needed) */
  eSellerate_OutputString productIDArray[],    /* array to be filled for access to product ID codes            */
  eSellerate_FileLocation fileLocationArray[], /* array to be filled for access to file locations              */
  eSellerate_ArraySize    arraySize            /* size of array provided                                       */
);

/* DEPRECATED FUNCTION **************************************************************************************/
eSellerate_ArraySize __stdcall
eSellerate_IndexShoppingCartData (          /* build array access to product ID codes output by the Engine  */
  eSellerate_ResultData   resultData,       /* handle to the output from call to the Engine (always needed) */
  eSellerate_OutputString productIDArray[], /* array to be filled for access to product ID codes            */
  eSellerate_OutputString skuIDArray[],     /* array to be filled for access to SKU ID codes                */
  eSellerate_ArraySize    arraySize         /* size of arrays provided                                      */
);
/************************************************************************************** DEPRECATED FUNCTION */

void __stdcall
eSellerate_DeleteResultData (      /* free memory allocated by the Engine */
  eSellerate_ResultData resultData /* handle to the allocated memory      */
);

eSellerate_ErrorCode __stdcall
eSellerate_LaunchUpdate (              /* call on the Engine to launch an update ********/
  eSellerate_FileLocation updaterFile, /* location of file to launch (always needed)    */
  eSellerate_FileLocation fileToUpdate /* location of file to which to apply the update */
);

eSellerate_ArraySize __stdcall
eSellerate_WebStoreURL (                   /* retrieve URL stored by eSellerate SalesTrac Technology */
  eSellerate_InputString  publisherID,     /* publisher ID code (always needed)                      */
  eSellerate_InputString  skuRefNum,       /* sku RefNum code (always needed)                        */
  eSellerate_OutputString resultBuffer,    /* buffer to be filled as result                          */
  eSellerate_ArraySize    resultBufferSize /* size of buffer provided                                */
);

eSellerate_ErrorCode __stdcall
eSellerate_ActivateSerialNumber (      /* confirm origin of serial number and lock it to this machine ********/
  eSellerate_InputString publisherID,  /* publisher ID code (always needed)                                  */
  eSellerate_InputString activationID, /* activation ID code to associate with serial number (always needed) */
  eSellerate_InputString serialNumber, /* serial number (always needed)                                      */
  eSellerate_TrueOrFalse askToConnect  /* ask to connect when no active connection is found                  */
);
/* additional failure codes:
 * eSellerate_NoSuchActivationID
 * eSellerate_NoSuchSerialNumber
 * eSellerate_BlacklistedSerialNumber
 * eSellerate_InvalidOrder
 * eSellerate_ActivationLimitMet
 * eSellerate_NonUniqueSerialNumber
 * eSellerate_RequestDenied
 */

eSellerate_ErrorCode __stdcall
eSellerate_ManualActivateSerialNumber ( /* confirm origin of serial number and lock it to this machine **********/
  eSellerate_InputString publisherID,   /* publisher ID code (always needed)                                    */
  eSellerate_InputString activationID,  /* activation ID code to associate with serial number (always needed)   */
  eSellerate_InputString serialNumber,  /* serial number (always needed)                                        */
  eSellerate_InputString titlePreface,  /* custom text, to preface Wizard titles                                */
  eSellerate_InputString otherOptions   /* custom text, to include phone number, email address or other contact */
);

eSellerate_ErrorCode __stdcall
eSellerate_ValidateActivation (        /* verify that serial number is activated and locked to this machine */
  eSellerate_InputString publisherID,  /* publisher ID code (always needed)                                 */
  eSellerate_InputString activationID, /* activation ID code associated with serial number (always needed)  */
  eSellerate_InputString serialNumber  /* serial number (always needed)                                     */
);
/* additional failure codes:
 * eSellerate_MissingActivationKey
 * eSellerate_MachineMismatch
 * eSellerate_InvalidActivationKey
 */

eSellerate_DaysSince2000 __stdcall
eSellerate_DateActivation (            /* give date (days since January 1 2000) of last valid activation ***/
  eSellerate_InputString publisherID,  /* publisher ID code (always needed)                                */
  eSellerate_InputString activationID, /* activation ID code associated with serial number (always needed) */
  eSellerate_InputString serialNumber  /* serial number (always needed)                                    */
);
/*
 * return codes:
 *   if no Engine found: eSellerate_NOENGINE
 *   if valid: date (days since January 1 2000) of last valid activation
 *   if invalid or not dated: 0
 */

eSellerate_ErrorCode __stdcall
eSellerate_DeactivateSerialNumber (    /* free serial number from lock to this machine ***********************/
  eSellerate_InputString publisherID,  /* publisher ID code (always needed)                                  */
  eSellerate_InputString activationID, /* activation ID code to associate with serial number (always needed) */
  eSellerate_InputString serialNumber, /* serial number (always needed)                                      */
  eSellerate_TrueOrFalse askToConnect  /* ask to connect when no active connection is found                  */
);
/* additional failure codes:
 * eSellerate_NoSuchSerialNumber
 * eSellerate_BlacklistedSerialNumber
 * eSellerate_InvalidOrder
 * eSellerate_MissingActivationKey
 * eSellerate_MachineMismatch
 * eSellerate_InvalidActivationKey
 * eSellerate_NonUniqueSerialNumber
 * eSellerate_NoSuchActivationKey
 * eSellerate_DeactivationLimitMet
 */

eSellerate_ErrorCode __stdcall
eSellerate_ConfirmSerialNumber (       /* confirm origin of serial number *******************/
  eSellerate_InputString publisherID,  /* publisher ID code (always needed)                 */
  eSellerate_InputString serialNumber, /* serial number (always needed)                     */
  eSellerate_TrueOrFalse askToConnect  /* ask to connect when no active connection is found */
);
/* additional failure codes:
 * eSellerate_NoSuchSerialNumber
 * eSellerate_BlacklistedSerialNumber
 * eSellerate_InvalidOrder
 */

#ifndef _VALIDATE_API_H_
eSellerate_DaysSince2000 __stdcall
eSellerate_Today ( );
/*
 * return value:
 *   days from January 1 2000 to today
 */
#endif

eSellerate_ErrorCode __stdcall
eSellerate_InstallEngineFrom ( /* install the Engine from resource to its proper system location  */
  HINSTANCE ownerModule        /* handle to the executable module that has the Engine as resource */
);

eSellerate_ErrorCode __stdcall
eSellerate_ApplicationLocation (      /* determine file location of the application */
  eSellerate_FileLocation application /* file location to be filled as result       */
);

#ifdef __cplusplus
}
#endif

#endif