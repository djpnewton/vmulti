/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

Module Name:

    testvhid.c

Environment:

    user mode only

--*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <setupapi.h>
#include <hidsdi.h>
#include <TCHAR.h>
#include "vmulticommon.h"
#include <dontuse.h>

//
// Function prototypes
//

VOID
SendHidRequests(
    HANDLE file,
    int requestType
    );

BOOL
SearchMatchingHwID (
    __in HDEVINFO            DeviceInfoSet,
    __in PSP_DEVINFO_DATA    DeviceInfoData
    );

BOOL
OpenDeviceInterface (
    __in       HDEVINFO                    HardwareDeviceInfo,
    __in       PSP_DEVICE_INTERFACE_DATA   DeviceInterfaceData,
    __in       int                         requestType
    );

BOOLEAN
CheckIfOurDevice(
    HANDLE file
    );


//
// Copied this structure from hidport.h
//

typedef struct _HID_DEVICE_ATTRIBUTES {

    ULONG           Size;
    //
    // sizeof (struct _HID_DEVICE_ATTRIBUTES)
    //
    //
    // Vendor ids of this hid device
    //
    USHORT          VendorID;
    USHORT          ProductID;
    USHORT          VersionNumber;
    USHORT          Reserved[11];

} HID_DEVICE_ATTRIBUTES, * PHID_DEVICE_ATTRIBUTES;


//
// Implementation
//

INT __cdecl
main(
    __in ULONG argc,
    __in_ecount(argc) PCHAR argv[]
    )
{
    HDEVINFO                                                hardwareDeviceInfo;
    SP_DEVICE_INTERFACE_DATA                    deviceInterfaceData;
    SP_DEVINFO_DATA                                 devInfoData;
    GUID            hidguid;
    int              i;

    UNREFERENCED_PARAMETER(argv);

    HidD_GetHidGuid(&hidguid);

    hardwareDeviceInfo =
            SetupDiGetClassDevs ((LPGUID)&hidguid,
                                            NULL,
                                            NULL, // Define no
                                            (DIGCF_PRESENT |
                                            DIGCF_INTERFACEDEVICE));

    if (INVALID_HANDLE_VALUE == hardwareDeviceInfo){
        printf("SetupDiGetClassDevs failed: %x\n", GetLastError());
        return 1;
    }

    deviceInterfaceData.cbSize = sizeof (SP_DEVICE_INTERFACE_DATA);

    devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    //
    // Enumerate devices of this interface class
    //

    printf("\n....looking for our HID device (with UP=0xFF00 "
                "and Usage=0x01)\n");

    for(i=0; SetupDiEnumDeviceInterfaces (hardwareDeviceInfo,
                            0, // No care about specific PDOs
                            (LPGUID)&hidguid,
                            i, //
                            &deviceInterfaceData);
                            i++ ){

        //
        // Open the device interface and Check if it is our device
        // by matching the Usage page and Usage from Hid_Caps.
        // If this is our device then send the hid request.
        //

        if (OpenDeviceInterface(hardwareDeviceInfo, &deviceInterfaceData,
                    argc > 1 ? REPORTID_MOUSE : REPORTID_MTOUCH)){
            //
            // device was found and the hid request sent in the above routine
            // so now cleanup
            //
            SetupDiDestroyDeviceInfoList (hardwareDeviceInfo);
            return 0;
        }

        //
        //device was not found so loop around.
        //

    }

    SetupDiDestroyDeviceInfoList (hardwareDeviceInfo);

    printf("Failure: Could not find our HID device \n");

    return 0;
}


BOOL
OpenDeviceInterface (
    __in       HDEVINFO                    hardwareDeviceInfo,
    __in       PSP_DEVICE_INTERFACE_DATA   deviceInterfaceData,
    __in       int                         requestType
    )
{
    PSP_DEVICE_INTERFACE_DETAIL_DATA    deviceInterfaceDetailData = NULL;

    DWORD        predictedLength = 0;
    DWORD        requiredLength = 0;
    HANDLE        file;
    BOOL           deviceFound = FALSE;

    SetupDiGetDeviceInterfaceDetail(
                            hardwareDeviceInfo,
                            deviceInterfaceData,
                            NULL, // probing so no output buffer yet
                            0, // probing so output buffer length of zero
                            &requiredLength,
                            NULL
                            ); // not interested in the specific dev-node

    predictedLength = requiredLength;

    deviceInterfaceDetailData =
         (PSP_DEVICE_INTERFACE_DETAIL_DATA) malloc (predictedLength);

    if (!deviceInterfaceDetailData)
    {
        printf("Error: OpenDeviceInterface: malloc failed\n");
        return FALSE;
    }

    deviceInterfaceDetailData->cbSize =
                    sizeof (SP_DEVICE_INTERFACE_DETAIL_DATA);

    if (!SetupDiGetDeviceInterfaceDetail(
                            hardwareDeviceInfo,
                            deviceInterfaceData,
                            deviceInterfaceDetailData,
                            predictedLength,
                            &requiredLength,
                            NULL))
    {
        printf("Error: SetupDiGetInterfaceDeviceDetail failed\n");
        free (deviceInterfaceDetailData);
        return FALSE;
    }

    file = CreateFile ( deviceInterfaceDetailData->DevicePath,
                            GENERIC_READ | GENERIC_WRITE,
                            0, // FILE_SHARE_READ | FILE_SHARE_READ |
                            NULL, // no SECURITY_ATTRIBUTES structure
                            OPEN_EXISTING, // No special create flags
                            0, // No special attributes
                            NULL); // No template file

    if (INVALID_HANDLE_VALUE == file) {
        printf("Error: CreateFile failed: %d\n", GetLastError());
        free (deviceInterfaceDetailData);
        return FALSE;
    }

    if (CheckIfOurDevice(file)){

        deviceFound  = TRUE;

        printf("...sending write request to our device\n");

        SendHidRequests(file, requestType);

    }

   CloseHandle(file);

    free (deviceInterfaceDetailData);

    return deviceFound;

}


BOOLEAN
CheckIfOurDevice(
    HANDLE file)
{
    PHIDP_PREPARSED_DATA Ppd = NULL; // The opaque parser info describing this device
    HIDD_ATTRIBUTES                 Attributes; // The Attributes of this hid device.
    HIDP_CAPS                       Caps; // The Capabilities of this hid device.
    USAGE                           MyUsagePage = 0xff00;
    USAGE                           MyUsage = 0x0001;
    BOOLEAN                         result = FALSE;

    if (!HidD_GetPreparsedData (file, &Ppd))
    {
        printf("Error: HidD_GetPreparsedData failed \n");
        goto cleanup;
    }

    if (!HidD_GetAttributes(file, &Attributes))
    {
        printf("Error: HidD_GetAttributes failed \n");
        goto cleanup;
    } 

    if (Attributes.VendorID == VMULTI_VID && Attributes.ProductID == VMULTI_PID)
    {
        if (!HidP_GetCaps (Ppd, &Caps))
        {
            printf("Error: HidP_GetCaps failed \n");
            goto cleanup;
        }

        if ((Caps.UsagePage == MyUsagePage) && (Caps.Usage == MyUsage))
        {
            printf("Success: Found my device.. \n");
            result = TRUE;
        }
    }

cleanup:

    if (Ppd != NULL)
    {
        HidD_FreePreparsedData (Ppd);
    }

    return result;
}


VOID
SendHidRequests(
    HANDLE file,
    int requestType
    )
{
    VMultiReportHeader* pReport = NULL;
    VMultiMultiTouchReport* pMultiReport = NULL;
    VMultiMouseReport*  pMouseReport = NULL;
    PCHAR buffer;
    ULONG bufferSize;
    BYTE inputReportSize;
    ULONG bytesWritten;

    //
    // Allocate 65 bytes of memory (the report descriptor says so).
    // We only really need 1 byte for report ID + sizeof(VMultiReportHeader) + 
    // sizeof(VMultiMouseReport/VMultiMultiTouchReport) but thats ok
    //

    bufferSize = 65;

    if (requestType == REPORTID_MTOUCH)
    {
        inputReportSize = sizeof(VMultiMultiTouchReport);
    }
    else
    {
        inputReportSize = sizeof(VMultiMouseReport);
    }

    if (bufferSize <= sizeof(VMultiReportHeader) + inputReportSize + 1)
    {
        printf("bummer, hid report size too small!\n");
        return;
    }

    buffer = (PCHAR) malloc (bufferSize);

    if (!buffer )
    {
        printf("malloc failed\n");
        return;
    }

    //
    // Set the report header
    //

    pReport = (VMultiReportHeader*)buffer;
    pReport->ReportID = REPORTID_VENDOR_01;
    pReport->ReportLength = inputReportSize;

    if (requestType == REPORTID_MTOUCH)
    {
        //
        // Set the multitouch report
        //

        printf("Sending multitouch report\n");

        pMultiReport = (VMultiMultiTouchReport*)(buffer + sizeof(VMultiReportHeader));
        pMultiReport->ReportID = REPORTID_MTOUCH;
        pMultiReport->Touch[0].Status = MULTI_CONFIDENCE_BIT | MULTI_IN_RANGE_BIT;
        pMultiReport->Touch[0].XValue = 10000;
        pMultiReport->Touch[0].YValue = 1000;
        pMultiReport->Touch[0].Width = 0;
        pMultiReport->Touch[0].Height = 0;
        pMultiReport->Touch[0].ContactID = 0;
        pMultiReport->ActualCount = 1;
    }
    else
    {
        //
        // Set the mouse report 
        //

        printf("Sending mouse report\n");

        pMouseReport = (VMultiMouseReport*)(buffer + sizeof(VMultiReportHeader));
        pMouseReport->ReportID = REPORTID_MOUSE;
        pMouseReport->Button = 0;
        pMouseReport->XValue = 1000;
        pMouseReport->YValue = 10000;
        pMouseReport->WheelPosition = 0;
    }

    //
    // Send Hid report thru HidD_SetOutputReport API
    //
    //

    if (!HidD_SetOutputReport(file, buffer, bufferSize))
    {
        printf("failed HidD_SetOutputReport %d\n", GetLastError());
    }
    else
    {
        printf("HidD_SetOutputReport succeded\n");
    }

    if (requestType == REPORTID_MTOUCH)
    {
        // non digitizer/pen devices cant hover (damn you wisptis!)
        pMultiReport->Touch[0].Status |= MULTI_TIPSWITCH_BIT;

        if (!WriteFile(file, buffer, bufferSize, &bytesWritten, NULL))
        {
            printf("failed WriteFile %d\n", GetLastError());
        }
        else
        {
            printf("WriteFile succeded, %d bytes written\n", bytesWritten);
        }
    }

    free(buffer);

    return;
}


