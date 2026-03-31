#include "../include/mqtt.h"
#include <chrono>
#include <fmt/core.h>
#include <thread>

#ifdef _WIN32
namespace mqtt {
const std::string message::EMPTY_STR{};
}
#endif // _WIN32

class Callback : public virtual mqtt::callback {
  public:
    void message_arrived(mqtt::const_message_ptr msg) override {
        fmt::print("\nReceived message:\n\tTopic: {}\n\tPayload: {}\n", msg->get_topic(), msg->get_payload_str());
    }

    void connection_lost(const std::string &cause) override {
        fmt::print("\nConnection lost: {}\n", cause);
    }
};

void run_mqtt() {
    constexpr std::string_view server_address = "test.mosquitto.org";
    constexpr std::string_view client_id = "my_mqtt_tester";
    constexpr std::string_view pub_topic = "royyan_topic/test";
    constexpr std::string_view sub_topic = "royyan_topic/test_sub";

    // Create MQTT client
    mqtt::async_client mqtt_client{std::string{server_address}, std::string{client_id}};
    fmt::print("MQTT client instance created: Address={}, ClientID={}\n", server_address, client_id);

    // Set callback
    Callback cb;
    mqtt_client.set_callback(cb);

    // Create connection options
    mqtt::connect_options conn_opts;
    conn_opts.set_keep_alive_interval(20);
    conn_opts.set_clean_session(true);

    try {
        // Connect to the broker
        fmt::print("Connecting to the MQTT broker...\n");
        mqtt_client.connect(conn_opts)->wait();
        fmt::print("Connected to the MQTT broker\n");

        // Subscribe to topic
        fmt::print("Subscribing to topic: {}\n", sub_topic);
        mqtt_client.subscribe(std::string{sub_topic}, 1)->wait();
        fmt::print("Subscribed to topic: {}\n", sub_topic);

        // Create and publish message
        auto msg = mqtt::make_message(std::string{pub_topic}, "Hello MQTT linker!");
        fmt::print("Publishing message:\n\tTopic: {}\n\tPayload: {}\n", msg->get_topic(), msg->get_payload_str());
        mqtt_client.publish(msg)->wait();
        fmt::print("Message published\n");

        // Set up timer for 3 seconds
        const auto start = std::chrono::steady_clock::now();
        fmt::print("\nWaiting for incoming messages for 3 seconds...\n");

        // Keep running for 3 seconds
        while (std::chrono::steady_clock::now() - start < std::chrono::seconds(3)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // Disconnect
        fmt::print("\n3 seconds elapsed. Disconnecting from the MQTT broker...\n");
        mqtt_client.disconnect()->wait();
        fmt::print("Disconnected\n");
    } catch (const mqtt::exception &exc) {
        fmt::println(stderr, "MQTT error: {}", exc.what());
    }
}