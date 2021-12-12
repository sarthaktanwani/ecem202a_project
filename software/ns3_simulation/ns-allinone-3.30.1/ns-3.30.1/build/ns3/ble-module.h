
#ifdef NS3_MODULE_COMPILATION
# error "Do not include ns3 module aggregator headers from other modules; these are meant only for end user scripts."
#endif

#ifndef NS3_MODULE_BLE
    

// Module headers:
#include "AES.h"
#include "AES_Const.h"
#include "SymAlg.h"
#include "ble-application.h"
#include "ble-bb-manager.h"
#include "ble-error-model.h"
#include "ble-helper.h"
#include "ble-link-controller.h"
#include "ble-link-manager.h"
#include "ble-link.h"
#include "ble-mac-header.h"
#include "ble-net-device.h"
#include "ble-phy.h"
#include "ble-privacy-manager.h"
#include "ble-spectrum-signal-parameters.h"
#include "constants.h"
#include "cryptomath.h"
#include "includes.h"
#include "ns-tag.h"
#endif
