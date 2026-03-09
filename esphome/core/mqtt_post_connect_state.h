#pragma once

#include "esphome/core/defines.h"

#ifdef USE_MQTT

#include <cstdint>

namespace esphome::mqtt {

enum class MQTTPostConnectStep : uint8_t {
  IDLE = 0,
  WAIT_FOR_SUBSCRIPTIONS,
  SEND_DEVICE_INFO,
  SCHEDULE_RESENDS,
};

class MQTTPostConnectState {
 public:
  void begin(bool discovery_ip_enabled) {
    this->step_ = MQTTPostConnectStep::WAIT_FOR_SUBSCRIPTIONS;
    this->discovery_ip_enabled_ = discovery_ip_enabled;
  }

  void reset() {
    this->step_ = MQTTPostConnectStep::IDLE;
    this->discovery_ip_enabled_ = false;
  }

  MQTTPostConnectStep step() const { return this->step_; }

  void mark_subscriptions_ready() {
    if (this->step_ != MQTTPostConnectStep::WAIT_FOR_SUBSCRIPTIONS) {
      return;
    }
    this->step_ =
        this->discovery_ip_enabled_ ? MQTTPostConnectStep::SEND_DEVICE_INFO : MQTTPostConnectStep::SCHEDULE_RESENDS;
  }

  void mark_device_info_sent() {
    if (this->step_ != MQTTPostConnectStep::SEND_DEVICE_INFO) {
      return;
    }
    this->step_ = MQTTPostConnectStep::SCHEDULE_RESENDS;
  }

  void mark_resends_scheduled() { this->reset(); }

 protected:
  MQTTPostConnectStep step_{MQTTPostConnectStep::IDLE};
  bool discovery_ip_enabled_{false};
};

}  // namespace esphome::mqtt

#endif
