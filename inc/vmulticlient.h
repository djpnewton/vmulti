#if !defined(_VMULTI_CLIENT_H_)
#define _VMULTI_CLIENT_H_

typedef struct _vmulti_client_t* pvmulti_client;

pvmulti_client vmulti_alloc(void);

void vmulti_free(pvmulti_client vmulti);

BOOL vmulti_connect(pvmulti_client vmulti);

void vmulti_disconnect(pvmulti_client vmulti);

BOOL vmulti_update_mouse(pvmulti_client vmulti, BYTE button, USHORT x, USHORT y, BYTE wheelPosition);

BOOL vmulti_update_digi(pvmulti_client vmulti, BYTE status, USHORT x, USHORT y);

BOOL vmulti_update_multitouch(pvmulti_client vmulti, BYTE actualCount, BYTE status1, USHORT x1, USHORT y1, BYTE contactId1, BYTE status2, USHORT x2, USHORT y2, BYTE contactId2);

BOOL vmulti_update_joystick(pvmulti_client vmulti, BYTE buttonsAndHat, BYTE x, BYTE y, BYTE throttle);

#endif