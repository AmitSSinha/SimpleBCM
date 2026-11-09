//
//  Bridge.hpp
//  SimpleBCM
//
//  Created by laobamac on 2026/9/19.
//  Copyright © 2026 laobamac. All rights reserved.
//

#ifndef SIMPLEBCM_BRIDGE_HPP
#define SIMPLEBCM_BRIDGE_HPP

#include "WirelessABI.hpp"
#include "Patcher.hpp"
#include <IOKit/IOService.h>

class BCM80211UserClient;

namespace bcm {

enum class InterfaceKind : uint8_t { Classic, Virtual, Skywalk };

struct Endpoint {
    IOService *interface;
    IOService *controller;
    InterfaceKind kind;
    char name[16];
};

struct AvailabilityEvent {
    uint64_t seconds;
    uint64_t microseconds;
    uint32_t length;
    uint8_t payload[NativeDriverAvailableSize];
};

struct Counters {
    uint64_t opens;
    uint64_t denied;
    uint64_t commands;
    uint64_t errors;
    uint64_t events;
    uint64_t dropped;
    uint64_t notifications;
    uint64_t notificationErrors;
    uint64_t scanRecords;
    uint64_t scanDoneEvents;
    uint64_t scanDoneRecords;
    uint64_t associationRequests;
    uint64_t associationAccepted;
    uint64_t availabilityEvents;
    uint64_t availabilityReplays;
    uint64_t associationEvents;
    uint64_t associationEventConversions;
    uint64_t associationEventReadErrors;
    uint64_t powerAvailabilityEvents;
    uint32_t lastAssociationEventLength;
    uint32_t lastAssociationEventReadResult;
    uint32_t lastAvailabilityReadResult;
    uint64_t selectors[9];
    uint32_t lastRequest;
    uint32_t lastResult;
    uint32_t lastAssociationResult;
};

extern Counters counters;

// DIAGNOSTIC (Wi-Fi off/on with sharing): ring of recent host-AP/power/failed-set requests.
// Holds request metadata only; key material is never copied.
struct DiagEvent {
    uint32_t sequence;
    uint32_t uptimeMs;
    uint32_t request;
    uint32_t command;
    int32_t value;
    uint32_t requestedLength;
    uint32_t passedLength;
    uint32_t result;
    uint32_t authUpper;
    uint32_t channel;
    uint32_t ssidLength;
    uint32_t keyLength;
    uint32_t cipher;
};

void recordDiagEvent(const DiagEvent &event);

class Bridge {
public:
    static void initialize();
    static void trackController(IOService *controller);
    static void enableNativeSkywalk();
    static bool ready();
    static const UserABI &abi();
    static uint64_t copyAvailability(const Endpoint &endpoint, AvailabilityEvent &event);
    static int refreshAvailability(const Endpoint &endpoint);
    static bool isScalarGet(uint32_t request);
    static bool identify(IOService *interface, InterfaceKind kind, Endpoint &endpoint);
    static bool addClient(BCM80211UserClient *client);
    static void removeClient(BCM80211UserClient *client);
    static int command(const Endpoint &endpoint, bool set, LegacyRequest &request, void *shadow, size_t size);
    static void publish(IOService *service);
    static bool prepareImage(LinkedImage &image, size_t index);
    static void disable(const char *reason);
private:
    static void patchImage(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size);
};

}

#endif
