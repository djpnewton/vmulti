#if !defined(_VMULTI_CLIENT_H_)
#define _VMULTI_CLIENT_H_

#include "vmulticommon.h"

typedef struct _vmulti_client_t* pvmulti_client;

pvmulti_client vmulti_alloc(void);

void vmulti_free(pvmulti_client vmulti);

BOOL vmulti_connect(pvmulti_client vmulti);

void vmulti_disconnect(pvmulti_client vmulti);

BOOL vmulti_update_mouse(pvmulti_client vmulti, BYTE button, USHORT x, USHORT y, BYTE wheelPosition);

BOOL vmulti_update_relative_mouse(pvmulti_client vmulti, BYTE button, BYTE x, BYTE y, BYTE wheelPosition);

BOOL vmulti_update_digi(pvmulti_client vmulti, BYTE status, USHORT x, USHORT y);

BOOL vmulti_update_multitouch(pvmulti_client vmulti, PTOUCH pTouch, BYTE actualCount);

BOOL vmulti_update_joystick(pvmulti_client vmulti, USHORT buttons, BYTE hat, BYTE x, BYTE y, BYTE rx, BYTE ry, BYTE throttle);

BOOL vmulti_update_keyboard(pvmulti_client vmulti, BYTE shiftKeyFlags, BYTE keyCodes[KBD_KEY_CODES]);

BOOL vmulti_write_message(pvmulti_client vmulti, VMultiMessageReport* pReport);

BOOL vmulti_read_message(pvmulti_client vmulti, VMultiMessageReport* pReport);

#endif
