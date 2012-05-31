#if !defined(_VMULTI_H_)
#define _VMULTI_H_

#pragma warning(disable:4200)  // suppress nameless struct/union warning
#pragma warning(disable:4201)  // suppress nameless struct/union warning
#pragma warning(disable:4214)  // suppress bit field types other than int warning
#include <initguid.h>
#include <wdm.h>

#pragma warning(default:4200)
#pragma warning(default:4201)
#pragma warning(default:4214)
#include <wdf.h>

#pragma warning(disable:4201)  // suppress nameless struct/union warning
#pragma warning(disable:4214)  // suppress bit field types other than int warning
#include <hidport.h>

#include "vmulticommon.h"

//
// String definitions
//

#define DRIVERNAME                 "vmulti.sys: "

#define VMULTI_POOL_TAG            (ULONG) 'luMV'
#define VMULTI_HARDWARE_IDS        L"djpnewton\\VMulti\0\0"
#define VMULTI_HARDWARE_IDS_LENGTH sizeof(VMULTI_HARDWARE_IDS)

#define NTDEVICE_NAME_STRING       L"\\Device\\VMulti"
#define SYMBOLIC_NAME_STRING       L"\\DosDevices\\VMulti"

#define MT_TOUCH_COLLECTION                                                    \
    0xa1, 0x02,                         /*     COLLECTION (Logical)         */ \
    0x09, 0x42,                         /*       USAGE (Tip Switch)         */ \
    0x15, 0x00,                         /*       LOGICAL_MINIMUM (0)        */ \
    0x25, 0x01,                         /*       LOGICAL_MAXIMUM (1)        */ \
    0x75, 0x01,                         /*       REPORT_SIZE (1)            */ \
    0x95, 0x01,                         /*       REPORT_COUNT (1)           */ \
    0x81, 0x02,                         /*       INPUT (Data,Var,Abs)       */ \
    0x09, 0x32,                         /*       USAGE (In Range)           */ \
    0x81, 0x02,                         /*       INPUT (Data,Var,Abs)       */ \
    0x09, 0x47,                         /*       USAGE (Confidence)         */ \
    0x81, 0x02,                         /*       INPUT (Data,Var,Abs)       */ \
    0x95, 0x05,                         /*       REPORT_COUNT (5)           */ \
    0x81, 0x03,                         /*       INPUT (Cnst,Ary,Abs)       */ \
    0x75, 0x08,                         /*       REPORT_SIZE (8)            */ \
    0x09, 0x51,                         /*       USAGE (Contact Identifier) */ \
    0x95, 0x01,                         /*       REPORT_COUNT (1)           */ \
    0x81, 0x02,                         /*       INPUT (Data,Var,Abs)       */ \
    0x05, 0x01,                         /*       USAGE_PAGE (Generic Desk.. */ \
    0x26, 0xff, 0x7f,                   /*       LOGICAL_MAXIMUM (32767)    */ \
    0x75, 0x10,                         /*       REPORT_SIZE (16)           */ \
    0x55, 0x00,                         /*       UNIT_EXPONENT (0)          */ \
    0x65, 0x00,                         /*       UNIT (None)                */ \
    0x35, 0x00,                         /*       PHYSICAL_MINIMUM (0)       */ \
    0x46, 0x00, 0x00,                   /*       PHYSICAL_MAXIMUM (0)       */ \
    0x09, 0x30,                         /*       USAGE (X)                  */ \
    0x81, 0x02,                         /*       INPUT (Data,Var,Abs)       */ \
    0x09, 0x31,                         /*       USAGE (Y)                  */ \
    0x81, 0x02,                         /*       INPUT (Data,Var,Abs)       */ \
    0x05, 0x0d,                         /*       USAGE PAGE (Digitizers)    */ \
    0x09, 0x48,                         /*       USAGE (Width)              */ \
    0x81, 0x02,                         /*       INPUT (Data,Var,Abs)       */ \
    0x09, 0x49,                         /*       USAGE (Height)             */ \
    0x81, 0x02,                         /*       INPUT (Data,Var,Abs)       */ \
    0xc0,                               /*    END_COLLECTION                */

//
// This is the default report descriptor for the Hid device provided
// by the mini driver in response to IOCTL_HID_GET_REPORT_DESCRIPTOR.
// 

typedef UCHAR HID_REPORT_DESCRIPTOR, *PHID_REPORT_DESCRIPTOR;

HID_REPORT_DESCRIPTOR DefaultReportDescriptor[] = {
//
// Multitouch report starts here
//
    0x05, 0x0d,                         // USAGE_PAGE (Digitizers)
    0x09, 0x04,                         // USAGE (Touch Screen)
    0xa1, 0x01,                         // COLLECTION (Application)
    0x85, REPORTID_MTOUCH,              //   REPORT_ID (Touch)
    0x09, 0x22,                         //   USAGE (Finger)
    MT_TOUCH_COLLECTION
    MT_TOUCH_COLLECTION
    0x05, 0x0d,                         //    USAGE_PAGE (Digitizers)
    0x09, 0x54,                         //    USAGE (Contact Count)
    0x95, 0x01,                         //    REPORT_COUNT (1)
    0x75, 0x08,                         //    REPORT_SIZE (8)
    0x15, 0x00,                         //    LOGICAL_MINIMUM (0)
    0x25, 0x08,                         //    LOGICAL_MAXIMUM (8)
    0x81, 0x02,                         //    INPUT (Data,Var,Abs)
    0x09, 0x55,                         //    USAGE(Contact Count Maximum)
    0xb1, 0x02,                         //    FEATURE (Data,Var,Abs)
    0xc0,                               // END_COLLECTION

//
// Feature report starts here
//
    0x09, 0x0E,                         // USAGE (Device Configuration)
    0xa1, 0x01,                         // COLLECTION (Application)
    0x85, REPORTID_FEATURE,             //   REPORT_ID (Configuration)              
    0x09, 0x23,                         //   USAGE (Device Settings)              
    0xa1, 0x02,                         //   COLLECTION (logical)    
    0x09, 0x52,                         //    USAGE (Device Mode)         
    0x09, 0x53,                         //    USAGE (Device Identifier)
    0x15, 0x00,                         //    LOGICAL_MINIMUM (0)      
    0x25, 0x0a,                         //    LOGICAL_MAXIMUM (10)
    0x75, 0x08,                         //    REPORT_SIZE (8)         
    0x95, 0x02,                         //    REPORT_COUNT (2)         
    0xb1, 0x02,                         //   FEATURE (Data,Var,Abs)    
    0xc0,                               //   END_COLLECTION
    0xc0,                               // END_COLLECTION

//
// Mouse report starts here
//
    0x05, 0x01,                         // USAGE_PAGE (Generic Desktop) 
    0x09, 0x02,                         // USAGE (Mouse)               
    0xa1, 0x01,                         // COLLECTION (Application)   
    0x85, REPORTID_MOUSE,               //   REPORT_ID (Mouse)       
    0x09, 0x01,                         //   USAGE (Pointer)        
    0xa1, 0x00,                         //   COLLECTION (Physical) 
    0x05, 0x09,                         //     USAGE_PAGE (Button)
    0x19, 0x01,                         //     USAGE_MINIMUM (Button 1) 
    0x29, 0x05,                         //     USAGE_MAXIMUM (Button 5)
    0x15, 0x00,                         //     LOGICAL_MINIMUM (0)    
    0x25, 0x01,                         //     LOGICAL_MAXIMUM (1)   
    0x75, 0x01,                         //     REPORT_SIZE (1)      
    0x95, 0x05,                         //     REPORT_COUNT (5)    
    0x81, 0x02,                         //     INPUT (Data,Var,Abs)
    0x95, 0x03,                         //     REPORT_COUNT (3)   
    0x81, 0x03,                         //     INPUT (Cnst,Var,Abs)    
    0x05, 0x01,                         //     USAGE_PAGE (Generic Desktop)
    0x26, 0xff, 0x7f,                   //     LOGICAL_MAXIMUM (32767)    
    0x75, 0x10,                         //     REPORT_SIZE (16)
    0x95, 0x01,                         //     REPORT_COUNT (1)
    0x55, 0x0F,                         //     UNIT_EXPONENT (-1)
    0x65, 0x11,                         //     UNIT (cm,SI Linear)
    0x35, 0x00,                         //     PHYSICAL_MINIMUM (0)
    0x45, 0x00,                         //     PHYSICAL_MAXIMUM (0)
    0x09, 0x30,                         //     USAGE (X)
    0x81, 0x02,                         //     INPUT (Data,Var,Abs)
    0x09, 0x31,                         //     USAGE (Y)
    0x81, 0x02,                         //     INPUT (Data,Var,Abs)
    0x05, 0x01,                         //     Usage Page (Generic Desktop)
    0x09, 0x38,                         //     Usage (Wheel)
    0x15, 0x81,                         //     Logical Minimum (-127)
    0x25, 0x7F,                         //     Logical Maximum (127)
    0x75, 0x08,                         //     Report Size (8)
    0x95, 0x01,                         //     Report Count (1)
    0x81, 0x06,                         //     Input (Data, Variable, Relative)
    0xc0,                               //   END_COLLECTION              
    0xc0,                               // END_COLLECTION     

//
// Relative mouse report starts here
//
    0x05, 0x01,                         // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                         // USAGE (Mouse)
    0xa1, 0x01,                         // COLLECTION (Application)
    0x85, REPORTID_RELATIVE_MOUSE,      //   REPORT_ID (Mouse)
    0x09, 0x01,                         //   USAGE (Pointer)
    0xa1, 0x00,                         //   COLLECTION (Physical)
    0x05, 0x09,                         //     USAGE_PAGE (Button)
    0x19, 0x01,                         //     USAGE_MINIMUM (Button 1)
    0x29, 0x05,                         //     USAGE_MAXIMUM (Button 5)
    0x15, 0x00,                         //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                         //     LOGICAL_MAXIMUM (1)
    0x75, 0x01,                         //     REPORT_SIZE (1)
    0x95, 0x05,                         //     REPORT_COUNT (5)
    0x81, 0x02,                         //     INPUT (Data,Var,Abs)
    0x95, 0x03,                         //     REPORT_COUNT (3)
    0x81, 0x03,                         //     INPUT (Cnst,Var,Abs)
    0x05, 0x01,                         //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                         //     USAGE (X)
    0x09, 0x31,                         //     USAGE (Y)
    0x15, 0x81,                         //     Logical Minimum (-127)
    0x25, 0x7F,                         //     Logical Maximum (127)
    0x75, 0x08,                         //     REPORT_SIZE (8)
    0x95, 0x02,                         //     REPORT_COUNT (2)
    0x81, 0x06,                         //     INPUT (Data,Var,Rel)
    0x05, 0x01,                         //     Usage Page (Generic Desktop)
    0x09, 0x38,                         //     Usage (Wheel)
    0x15, 0x81,                         //     Logical Minimum (-127)
    0x25, 0x7F,                         //     Logical Maximum (127)
    0x75, 0x08,                         //     Report Size (8)
    0x95, 0x01,                         //     Report Count (1)
    0x81, 0x06,                         //     Input (Data, Variable, Relative)
    0xc0,                               //   END_COLLECTION
    0xc0,                               // END_COLLECTION

//
// Digitizer report starts here
//
    0x05, 0x0d,                         // USAGE_PAGE (Digitizers)
    0x09, 0x02,                         // USAGE (Pen digitizer)
    0xa1, 0x01,                         // COLLECTION (Application)
    0x85, REPORTID_DIGI,                //   REPORT_ID (Digi)
    0x05, 0x0d,                         //   USAGE_PAGE (Digitizers)
    0x09, 0x20,                         //   USAGE (Stylus)
    0xa1, 0x00,                         //   COLLECTION (Physical)
    0x09, 0x42,                         //     USAGE (Tip Switch)
    0x09, 0x32,                         //     USAGE (In Range)
    0x15, 0x00,                         //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                         //     LOGICAL_MAXIMUM (1)
    0x75, 0x01,                         //     REPORT_SIZE (1)
    0x95, 0x02,                         //     REPORT_COUNT (2)
    0x81, 0x02,                         //     INPUT (Data,Var,Abs)
    0x75, 0x01,                         //     REPORT_SIZE (1)
    0x95, 0x06,                         //     REPORT_COUNT (6)
    0x81, 0x01,                         //     INPUT (Cnst,Ary,Abs)
    0x05, 0x01,                         //     USAGE_PAGE (Generic Desktop)
    0x26, 0xff, 0x7f,                   //     LOGICAL_MAXIMUM (32767)       
    0x75, 0x10,                         //     REPORT_SIZE (16) 
    0x95, 0x01,                         //     REPORT_COUNT (1)            
    0x55, 0x0F,                         //     UNIT_EXPONENT (-1)           
    0x65, 0x11,                         //     UNIT (cm,SI Linear)                  
    0x35, 0x00,                         //     PHYSICAL_MINIMUM (0)         
    0x45, 0x00,                         //     PHYSICAL_MAXIMUM (0)
    0x09, 0x30,                         //     USAGE (X)                    
    0x81, 0x02,                         //     INPUT (Data,Var,Abs)         
    0x09, 0x31,                         //     USAGE (Y)                    
    0x81, 0x02,                         //     INPUT (Data,Var,Abs)
    0xc0,                               //   END_COLLECTION
    0xc0,                               // END_COLLECTION

//
// Joystick report starts here
//
    0x05, 0x01,                         // USAGE_PAGE (Generic Desktop)
    0x15, 0x00,                         // LOGICAL_MINIMUM (0)
    0x09, 0x04,                         // USAGE (Joystick)
    0xa1, 0x01,                         // COLLECTION (Application)
    0x85, REPORTID_JOYSTICK,            //   REPORT_ID (Joystick)
    
    0x05, 0x02,                         //   USAGE_PAGE (Simulation Controls)
    0x09, 0xbb,                         //   USAGE (Throttle)
    0x15, 0x81,                         //   LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                         //   LOGICAL_MAXIMUM (127)
    0x75, 0x08,                         //   REPORT_SIZE (8)
    0x95, 0x01,                         //   REPORT_COUNT (1)
    0x81, 0x02,                         //   INPUT (Data,Var,Abs)

    0x05, 0x01,                         //   USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                         //   USAGE (Pointer)
    0xa1, 0x00,                         //   COLLECTION (Physical)
    0x09, 0x30,                         //     USAGE (X)
    0x09, 0x31,                         //     USAGE (Y)
    0x95, 0x02,                         //     REPORT_COUNT (2)
    0x81, 0x02,                         //     INPUT (Data,Var,Abs)
    0xc0,                               //   END_COLLECTION

    0xa1, 0x00,                         //   COLLECTION (Physical)
    0x09, 0x39,                         //     USAGE (Hat switch)
    0x15, 0x01,                         //     LOGICAL_MINIMUM (1)
    0x26, 0xff, 0x00,                   //     LOGICAL_MAXIMUM (255)
    0x35, 0x00,                         //     PHYSICAL_MINIMUM (0)
    0x46, 0x0e, 0x01,                   //     PHYSICAL_MAXIMUM (270)
    0x65, 0x14,                         //     UNIT (Eng Rot:Angular Pos)
    0x75, 0x08,                         //     REPORT_SIZE (8)
    0x95, 0x01,                         //     REPORT_COUNT (1)
    0x81, 0x02,                         //     INPUT (Data,Var,Abs)

    0x09, 0x33,                         //     USAGE (Rx)
    0x95, 0x01,                         //     REPORT_COUNT (1)
    0x81, 0x02,                         //     INPUT (Data,Var,Abs)
 
    0x09, 0x34,                         //     USAGE (Ry)
    0x95, 0x01,                         //     REPORT_COUNT (1)
    0x81, 0x02,                         //     INPUT (Data,Var,Abs)
    0xc0,                               //   END_COLLECTION

    0x05, 0x09,                         //   USAGE_PAGE (Button)
    0x19, 0x01,                         //   USAGE_MINIMUM (Button 1)
    0x29, 0x10,                         //   USAGE_MAXIMUM (Button 16)
    0x15, 0x00,                         //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                         //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                         //   REPORT_SIZE (1)
    0x95, 0x10,                         //   REPORT_COUNT (16)
    0x55, 0x00,                         //   UNIT_EXPONENT (0)
    0x65, 0x00,                         //   UNIT (None)
    0x81, 0x02,                         //   INPUT (Data,Var,Abs)
    0xc0,                               // END_COLLECTION

//
// Keyboard report starts here
//    
    0x05, 0x01,                         // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                         // USAGE (Keyboard)
    0xa1, 0x01,                         // COLLECTION (Application)
    0x85, REPORTID_KEYBOARD,            //   REPORT_ID (Keyboard)    
    0x05, 0x07,                         //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                         //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                         //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                         //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                         //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                         //   REPORT_SIZE (1)
    0x95, 0x08,                         //   REPORT_COUNT (8)
    0x81, 0x02,                         //   INPUT (Data,Var,Abs)
    0x95, 0x01,                         //   REPORT_COUNT (1)
    0x75, 0x08,                         //   REPORT_SIZE (8)
    0x81, 0x03,                         //   INPUT (Cnst,Var,Abs)
    0x95, 0x05,                         //   REPORT_COUNT (5)
    0x75, 0x01,                         //   REPORT_SIZE (1)
    0x05, 0x08,                         //   USAGE_PAGE (LEDs)
    0x19, 0x01,                         //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                         //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,                         //   OUTPUT (Data,Var,Abs)
    0x95, 0x01,                         //   REPORT_COUNT (1)
    0x75, 0x03,                         //   REPORT_SIZE (3)
    0x91, 0x03,                         //   OUTPUT (Cnst,Var,Abs)
    0x95, 0x06,                         //   REPORT_COUNT (6)
    0x75, 0x08,                         //   REPORT_SIZE (8)
    0x15, 0x00,                         //   LOGICAL_MINIMUM (0)
    0x25, 0x65,                         //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,                         //   USAGE_PAGE (Keyboard)
    0x19, 0x00,                         //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,                         //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                         //   INPUT (Data,Ary,Abs)
    0xc0,                               // END_COLLECTION

//
// Vendor defined control report starts here
//
    0x06, 0x00, 0xff,                    // USAGE_PAGE (Vendor Defined Page 1)
    0x09, 0x01,                          // USAGE (Vendor Usage 1)
    0xa1, 0x01,                          // COLLECTION (Application)
    0x85, REPORTID_CONTROL,              //   REPORT_ID (1)  
    0x15, 0x00,                          //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,                    //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                          //   REPORT_SIZE  (8)   - bits
    0x95, 0x40,                          //   REPORT_COUNT (64)  - Bytes
    0x09, 0x02,                          //   USAGE (Vendor Usage 1)
    0x81, 0x02,                          //   INPUT (Data,Var,Abs)
    0x95, 0x40,                          //   REPORT_COUNT (64)  - Bytes
    0x09, 0x02,                          //   USAGE (Vendor Usage 1)
    0x91, 0x02,                          //   OUTPUT (Data,Var,Abs)
    0xc0,                                // END_COLLECTION

//
// Vendor defined message report starts here
//
    0x06, 0x00, 0xff,                    // USAGE_PAGE (Vendor Defined Page 1)
    0x09, 0x02,                          // USAGE (Vendor Usage 2)
    0xa1, 0x01,                          // COLLECTION (Application)
    0x85, REPORTID_MESSAGE,              //   REPORT_ID (1)  
    0x15, 0x00,                          //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,                    //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                          //   REPORT_SIZE  (8)   - bits
    0x95, 0x20,                          //   REPORT_COUNT (32)  - Bytes
    0x09, 0x02,                          //   USAGE (Vendor Usage 1)
    0x81, 0x02,                          //   INPUT (Data,Var,Abs)
    0x95, 0x40,                          //   REPORT_COUNT (64)  - Bytes
    0x09, 0x02,                          //   USAGE (Vendor Usage 1)
    0x91, 0x02,                          //   OUTPUT (Data,Var,Abs)
    0xc0,                                // END_COLLECTION
};


//
// This is the default HID descriptor returned by the mini driver
// in response to IOCTL_HID_GET_DEVICE_DESCRIPTOR. The size
// of report descriptor is currently the size of DefaultReportDescriptor.
//

CONST HID_DESCRIPTOR DefaultHidDescriptor = {
    0x09,   // length of HID descriptor
    0x21,   // descriptor type == HID  0x21
    0x0100, // hid spec release
    0x00,   // country code == Not Specified
    0x01,   // number of HID class descriptors
    { 0x22,   // descriptor type 
    sizeof(DefaultReportDescriptor) }  // total length of report descriptor
};


typedef struct _VMULTI_CONTEXT 
{

    WDFQUEUE ReportQueue;

    BYTE DeviceMode;

} VMULTI_CONTEXT, *PVMULTI_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(VMULTI_CONTEXT, VMultiGetDeviceContext)

//
// Function definitions
//

DRIVER_INITIALIZE DriverEntry;

EVT_WDF_DRIVER_UNLOAD VMultiDriverUnload;

EVT_WDF_DRIVER_DEVICE_ADD VMultiEvtDeviceAdd;

EVT_WDFDEVICE_WDM_IRP_PREPROCESS VMultiEvtWdmPreprocessMnQueryId;

EVT_WDF_IO_QUEUE_IO_INTERNAL_DEVICE_CONTROL VMultiEvtInternalDeviceControl;

NTSTATUS
VMultiGetHidDescriptor(
    IN WDFDEVICE Device,
    IN WDFREQUEST Request
    );

NTSTATUS
VMultiGetReportDescriptor(
    IN WDFDEVICE Device,
    IN WDFREQUEST Request
    );

NTSTATUS
VMultiGetDeviceAttributes(
    IN WDFREQUEST Request
    );

NTSTATUS
VMultiGetString(
    IN WDFREQUEST Request
    );

NTSTATUS
VMultiWriteReport(
    IN PVMULTI_CONTEXT DevContext,
    IN WDFREQUEST Request
    );

NTSTATUS
VMultiProcessVendorReport(
    IN PVMULTI_CONTEXT DevContext,
    IN PVOID ReportBuffer,
    IN ULONG ReportBufferLen,
    OUT size_t* BytesWritten
    );

NTSTATUS
VMultiReadReport(
    IN PVMULTI_CONTEXT DevContext,
    IN WDFREQUEST Request,
    OUT BOOLEAN* CompleteRequest
    );

NTSTATUS
VMultiSetFeature(
    IN PVMULTI_CONTEXT DevContext,
    IN WDFREQUEST Request,
    OUT BOOLEAN* CompleteRequest
    );

NTSTATUS
VMultiGetFeature(
    IN PVMULTI_CONTEXT DevContext,
    IN WDFREQUEST Request,
    OUT BOOLEAN* CompleteRequest
    );

PCHAR
DbgHidInternalIoctlString(
    IN ULONG        IoControlCode
    );

//
// Helper macros
//

#define DEBUG_LEVEL_ERROR   1
#define DEBUG_LEVEL_INFO    2
#define DEBUG_LEVEL_VERBOSE 3

#define DBG_INIT  1
#define DBG_PNP   2
#define DBG_IOCTL 4

#if DBG
#define VMultiPrint(dbglevel, dbgcatagory, fmt, ...) {          \
    if (VMultiDebugLevel >= dbglevel &&                         \
        (VMultiDebugCatagories && dbgcatagory))                 \
    {                                                           \
        DbgPrint(DRIVERNAME);                                   \
        DbgPrint(fmt, __VA_ARGS__);                             \
    }                                                           \
}
#else
#define VMultiPrint(dbglevel, fmt, ...) {                       \
}
#endif

#endif
