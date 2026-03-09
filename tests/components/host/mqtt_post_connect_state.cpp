#include <gtest/gtest.h>

#define USE_MQTT
#include "esphome/core/mqtt_post_connect_state.h"

namespace esphome::host::testing {

TEST(MQTTPostConnectStateTest, BeginsWaitingForSubscriptions) {
  ::esphome::mqtt::MQTTPostConnectState state;

  state.begin(true);

  EXPECT_EQ(state.step(), ::esphome::mqtt::MQTTPostConnectStep::WAIT_FOR_SUBSCRIPTIONS);
}

TEST(MQTTPostConnectStateTest, AdvancesToDeviceInfoWhenDiscoveryIsEnabled) {
  ::esphome::mqtt::MQTTPostConnectState state;

  state.begin(true);
  state.mark_subscriptions_ready();

  EXPECT_EQ(state.step(), ::esphome::mqtt::MQTTPostConnectStep::SEND_DEVICE_INFO);
}

TEST(MQTTPostConnectStateTest, SkipsDeviceInfoWhenDiscoveryIsDisabled) {
  ::esphome::mqtt::MQTTPostConnectState state;

  state.begin(false);
  state.mark_subscriptions_ready();

  EXPECT_EQ(state.step(), ::esphome::mqtt::MQTTPostConnectStep::SCHEDULE_RESENDS);
}

TEST(MQTTPostConnectStateTest, AdvancesToResendsAfterDeviceInfo) {
  ::esphome::mqtt::MQTTPostConnectState state;

  state.begin(true);
  state.mark_subscriptions_ready();
  state.mark_device_info_sent();

  EXPECT_EQ(state.step(), ::esphome::mqtt::MQTTPostConnectStep::SCHEDULE_RESENDS);
}

TEST(MQTTPostConnectStateTest, ResetsToIdleAfterSchedulingResends) {
  ::esphome::mqtt::MQTTPostConnectState state;

  state.begin(true);
  state.mark_subscriptions_ready();
  state.mark_device_info_sent();
  state.mark_resends_scheduled();

  EXPECT_EQ(state.step(), ::esphome::mqtt::MQTTPostConnectStep::IDLE);
}

TEST(MQTTPostConnectStateTest, ResetClearsPendingWork) {
  ::esphome::mqtt::MQTTPostConnectState state;

  state.begin(true);
  state.reset();

  EXPECT_EQ(state.step(), ::esphome::mqtt::MQTTPostConnectStep::IDLE);
}

}  // namespace esphome::host::testing
