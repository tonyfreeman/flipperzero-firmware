#pragma once

#include <stdint.h>
#include <stdbool.h>

#include <furi_hal_version.h>

#define GAP_MAC_ADDR_SIZE (6)

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    GapEventTypeConnected,
    GapEventTypeDisconnected,
    GapEventTypeStartAdvertising,
    GapEventTypeStopAdvertising,
    GapEventTypePinCodeShow,
    GapEventTypePinCodeVerify,
    GapEventTypeUpdateMTU,
} GapEventType;

typedef union {
    uint32_t pin_code;
    uint16_t max_packet_size;
} GapEventData;

typedef struct {
    GapEventType type;
    GapEventData data;
} GapEvent;

typedef bool (*GapEventCallback)(GapEvent event, void* context);

typedef struct {
    uint8_t type;
    uint8_t mac[6];
} GapAddress;

typedef void (*GapScanCallback)(GapAddress address, void* context);

typedef enum {
    GapStateUninitialized,
    GapStateIdle,
    GapStateStartingAdv,
    GapStateAdvFast,
    GapStateAdvLowPower,
    GapStateConnected,
} GapState;

typedef enum {
    GapPairingNone,
    GapPairingPinCodeShow,
    GapPairingPinCodeVerifyYesNo,
} GapPairing;

typedef struct {
    uint16_t adv_service_uuid;
    uint16_t appearance_char;
    bool bonding_mode;
    GapPairing pairing_method;
    uint8_t mac_address[GAP_MAC_ADDR_SIZE];
    char adv_name[FURI_HAL_VERSION_DEVICE_NAME_LENGTH];
} GapConfig;

bool gap_init(GapConfig* config, GapEventCallback on_event_cb, void* context);

void gap_start_advertising();

void gap_stop_advertising();

GapState gap_get_state();

void gap_thread_stop();

void gap_start_scan(GapScanCallback callback, void* context);

void gap_stop_scan();

#ifdef __cplusplus
}
#endif
