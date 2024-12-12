#ifndef DM9051_ETH_DEBUG_H
#define DM9051_ETH_DEBUG_H

#include "dm9051_debug_common.h"

#define DM9051_ETH_DEBUG_LEVEL 2

#define DM9051_ETH_DEBUG_HANDLER dm9051_eth_debug_handler
void dm9051_eth_debug_handler(debug_level_t level, const char *message);

// Debug macros
#if DM9051_ETH_DEBUG_LEVEL > 0
    #define DM9051_ETH_LOG(level, fmt, ...) \
        do { \
            char debug_msg[256]; \
            snprintf(debug_msg, sizeof(debug_msg), fmt, ##__VA_ARGS__); \
            DM9051_ETH_DEBUG_HANDLER(level, debug_msg); \
        } while (0)
#else
    #define DM9051_ETH_LOG(level, fmt, ...)
#endif

// Debug levels
typedef enum {
    DM9051_ETH_DEBUG_LEVEL_ERROR = 0,
    DM9051_ETH_DEBUG_LEVEL_WARN,
    DM9051_ETH_DEBUG_LEVEL_INFO,
    DM9051_ETH_DEBUG_LEVEL_DEBUG
} dm9051_eth_debug_level_t;

#endif // DM9051_ETH_DEBUG_H




#include "dm9051_eth.h"
#include "dm9051_eth_debug.h"
#include "dm9051_registers.h"

// Example function to initialize the Ethernet interface
dm9051_error_t dm9051_eth_init(const uint8_t *mac_addr) {
    DM9051_ETH_LOG(DM9051_ETH_DEBUG_LEVEL_INFO, "Initializing DM9051 Ethernet interface");

    // Read chip ID to verify communication
    uint8_t chip_id = dm9051_read_reg(DM9051_PIDL);
    if (chip_id != DM9051_ID) {
        DM9051_ETH_LOG(DM9051_ETH_DEBUG_LEVEL_ERROR, "Invalid chip ID: 0x%02X", chip_id);
        return DM9051_ERROR_INIT;
    }

    // Set MAC address
    for (int i = 0; i < 6; i++) {
        dm9051_write_reg(DM9051_PAR + i, mac_addr[i]);
    }
    DM9051_ETH_LOG(DM9051_ETH_DEBUG_LEVEL_DEBUG, "MAC address set: %02X:%02X:%02X:%02X:%02X:%02X",
                   mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);

    // Enable receiver
    dm9051_write_reg(DM9051_RCR, RCR_DIS_LONG | RCR_DIS_CRC | RCR_RXEN);
    DM9051_ETH_LOG(DM9051_ETH_DEBUG_LEVEL_INFO, "Receiver enabled");

    return DM9051_OK;
}

// Example function to handle received Ethernet frames
dm9051_error_t dm9051_eth_receive_frame(uint8_t *buffer, uint16_t *length) {
    uint8_t status = dm9051_read_reg(DM9051_RSR);
    
    if (!(status & RSR_RECV)) {
        DM9051_ETH_LOG(DM9051_ETH_DEBUG_LEVEL_DEBUG, "No frame received");
        return DM9051_OK;
    }

    // Read frame length
    *length = dm9051_read_reg(DM9051_RXCNTL) | (dm9051_read_reg(DM9051_RXCNTH) << 8);

    if (*length > DM9051_MAX_PACKET) {
        DM9051_ETH_LOG(DM9051_ETH_DEBUG_LEVEL_WARN, "Received oversized frame: %d bytes", *length);
        // Handle oversized frame...
        return DM9051_ERROR_RX;
    }

    // Read frame data
    dm9051_read_mem(buffer, *length);

    DM9051_ETH_LOG(DM9051_ETH_DEBUG_LEVEL_INFO, "Received frame: %d bytes", *length);
    
    return DM9051_OK;
}

// Implementation of the debug handler
void dm9051_eth_debug_handler(debug_level_t level, const char *message) {
    const char *level_str;
    switch (level) {
        case DM9051_ETH_DEBUG_LEVEL_ERROR: level_str = "ERROR"; break;
        case DM9051_ETH_DEBUG_LEVEL_WARN:  level_str = "WARN";  break;
        case DM9051_ETH_DEBUG_LEVEL_INFO:  level_str = "INFO";  break;
        case DM9051_ETH_DEBUG_LEVEL_DEBUG: level_str = "DEBUG"; break;
        default:                           level_str = "UNKNOWN";
    }

    // This could be replaced with a platform-specific logging function
    printf("[DM9051_ETH][%s] %s\n", level_str, message);
}
