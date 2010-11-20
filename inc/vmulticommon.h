#if !defined(_VMULTI_COMMON_H_)
#define _VMULTI_COMMON_H_

//
//These are the device attributes returned by vmulti in response
// to IOCTL_HID_GET_DEVICE_ATTRIBUTES.
//

#define VMULTI_PID              0xBACC
#define VMULTI_VID              0x00FF
#define VMULTI_VERSION          0x0001

//
// These are the report ids
//

#define REPORTID_MTOUCH         0x01
#define REPORTID_FEATURE        0x02
#define REPORTID_MOUSE          0x03
#define REPORTID_DIGI           0x04
#define REPORTID_VENDOR_01      0x40

//
// Vendor defined report size
//

#define VENDOR_REPORT_SIZE      0x41

//
// Report header
//

#pragma pack(1)
typedef struct _VMULTI_REPORT_HEADER
{

    BYTE        ReportID;

    BYTE        ReportLength;

} VMultiReportHeader;
#pragma pack()

//
// Digitizer specific report infomation
//

#define DIGI_TIPSWITCH_BIT    1
#define DIGI_IN_RANGE_BIT     2

#define DIGI_MIN_COORDINATE   0x0000
#define DIGI_MAX_COORDINATE   0x7FFF

#pragma pack(1)
typedef struct _VMULTI_DIGI_REPORT
{

    BYTE      ReportID;

    BYTE      Status;

    USHORT    XValue;

    USHORT    YValue;

} VMultiDigiReport;
#pragma pack()

//
// Mouse specific report information
//

#define MOUSE_BUTTON_1     0x01
#define MOUSE_BUTTON_2     0x02

#define MOUSE_MIN_COORDINATE   0x0000
#define MOUSE_MAX_COORDINATE   0x7FFF

#define MIN_WHEEL_POS   -127
#define MAX_WHEEL_POS    127

#pragma pack(1)
typedef struct _VMULTI_MOUSE_REPORT
{

    BYTE        ReportID;

    BYTE        Button;

    USHORT      XValue;

    USHORT      YValue;

    BYTE        WheelPosition;

} VMultiMouseReport;
#pragma pack()

//
// Multitouch specific report information
//

#define MULTI_TIPSWITCH_BIT    1
#define MULTI_IN_RANGE_BIT     2
#define MULTI_CONFIDENCE_BIT   4

#define MULTI_MIN_COORDINATE   0x0000
#define MULTI_MAX_COORDINATE   0x7FFF

#define MULTI_MAX_COUNT        2

#pragma pack(1)
typedef struct
{

    BYTE      Status;

    BYTE      ContactID;

    USHORT    XValue;

    USHORT    YValue;

    USHORT    Width;

    USHORT    Height;

}
TOUCH, *PTOUCH;

typedef struct _VMULTI_MULTITOUCH_REPORT
{

    BYTE      ReportID;

    TOUCH     Touch[MULTI_MAX_COUNT];

    BYTE      ActualCount;

} VMultiMultiTouchReport;
#pragma pack()

//
// Feature report infomation
//

#define DEVICE_MODE_MOUSE        0x00
#define DEVICE_MODE_SINGLE_INPUT 0x01
#define DEVICE_MODE_MULTI_INPUT  0x02

#pragma pack(1)
typedef struct _VMULTI_FEATURE_REPORT
{

    BYTE      ReportID;

    BYTE      DeviceMode;

    BYTE      DeviceIdentifier;

} VMultiFeatureReport;

typedef struct _VMULTI_MAXCOUNT_REPORT
{

    BYTE         ReportID;

    BYTE         MaximumCount;

} VMultiMaxCountReport;
#pragma pack()

#endif
