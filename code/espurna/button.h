/*

BUTTON MODULE

Copyright (C) 2016-2019 by Xose Pérez <xose dot perez at gmail dot com>

*/

#pragma once

#include "espurna.h"

#include "broker.h"

#include "libs/BasePin.h"
#include "libs/DebounceEvent.h"

#include <memory>

constexpr size_t ButtonsActionMax { 255ul };

constexpr size_t ButtonsPresetMax { 8ul };
constexpr size_t ButtonsMax { 32ul };

enum class ButtonProvider : int {
    None,
    Gpio,
    Analog
};

enum class button_event_t {
    None,
    Pressed,
    Released,
    Click,
    DoubleClick,
    LongClick,
    LongLongClick,
    TripleClick
};

// button actions, limited to 8-bit number (0b11111111 / 0xff / 255)

enum class ButtonAction : uint8_t  {
    None,
    Toggle,
    On,
    Off,
    AccessPoint,
    Reset,
    Pulse,
    FactoryReset,
    Wps,
    SmartConfig,
    BrightnessIncrease,
    BrightnessDecrease,
    DisplayOn,
    Custom,
    FanLow,
    FanMedium,
    FanHigh
};

struct ButtonActions {
    ButtonAction pressed;
    ButtonAction released;
    ButtonAction click;
    ButtonAction dblclick;
    ButtonAction lngclick;
    ButtonAction lnglngclick;
    ButtonAction trplclick;
};

struct button_event_delays_t {
    button_event_delays_t();
    button_event_delays_t(unsigned long debounce, unsigned long repeat, unsigned long lngclick, unsigned long lnglngclick);

    unsigned long debounce;
    unsigned long repeat;
    unsigned long lngclick;
    unsigned long lnglngclick;
};

struct button_t {
    button_t(ButtonActions&& actions, button_event_delays_t&& delays);
    button_t(BasePinPtr&& pin, const debounce_event::types::Config& config,
        ButtonActions&& actions, button_event_delays_t&& delays);

    bool state();
    button_event_t loop();

    std::unique_ptr<debounce_event::EventEmitter> event_emitter;

    ButtonActions actions;
    button_event_delays_t event_delays;
};

BrokerDeclare(ButtonBroker, void(unsigned char id, button_event_t event));

using ButtonCustomAction = void(*)(unsigned char id, button_event_t event);
void buttonSetCustomAction(ButtonCustomAction);

bool buttonState(unsigned char id);
ButtonAction buttonAction(unsigned char id, const button_event_t event);

void buttonEvent(unsigned char id, button_event_t event);

unsigned char buttonCount();
void buttonSetup();
