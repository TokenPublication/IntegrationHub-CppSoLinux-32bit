/**
 * @file test.cpp
 * @brief This is a demo application for the IntegrationHub library.
 *
 * This project demonstrates how to use the IntegrationHub library (.so) to integrate
 * with Beko fiscal devices (X30TR and 300TR) on a 32-bit Linux system.
 * It provides a simple command-line interface to test various functionalities
 * like sending baskets, processing payments, and retrieving fiscal information.
 */

#include <string>
#include <iostream>
#include <vector>
#include "IntegrationHubWrapper.h"

// these 2 are just to keep app alive
#include <chrono>
#include <thread>


/**
 * @brief Callback function to handle serial data received from the IntegrationHub.
 * This function is registered with the library and called asynchronously.
 * @param tag An integer tag identifying the data source.
 * @param data The serial data received.
 */
void setSerialInCallbackTest(int tag, std::string data) {
    std::cout << "Serial In Callback Result In Test: \n" << data << std::endl;
}

/**
 * @brief Callback function to handle device state changes.
 * This function is registered with the library and called when a device is connected or disconnected.
 * @param state The new state of the device (true for connected, false for disconnected).
 * @param deviceId A string identifying the device.
 */
void setDeviceStateCallbackTest(bool state, std::string deviceId) {
    std::cout << "Device State Callback Result In Test: \n" << deviceId << std::endl;
}


/**
 * @brief Sends a sample basket to the active fiscal device.
 * The basket data is different for X30TR and 300TR devices.
 * @param communication A pointer to the ConnectionWrapper object.
 * @param activeDevice The index of the active device (0 for X30TR, 1 for 300TR).
 */
void sendBasketTest(ConnectionWrapper* communication, int activeDevice) {
    if (activeDevice == 0) { // X30TR
        int basketResult = sendBasket(communication , "{\n  \"basketID\": \"a123ca24-ca2c-401c-8134-f0de2ec25c25\",\n  \"documentType\": 9008,\n  \"customerInfo\": {\n    \"taxID\": \"11111111111\"\n  },\n  \"items\": [\n    {\n      \"name\": \"�LA�\",\n      \"price\": 1000,\n      \"quantity\": 1000,\n      \"sectionNo\": 2,\n      \"taxPercent\": 2000,\n      \"type\": 0\n    }\n  ],\n  \"taxFreeAmount\": 5000,\n  \"paymentItems\": [\n    {\n      \"amount\": 6000,\n      \"description\": \"Cash\",\n      \"type\": 1\n    }\n  ]\n}");
        std::cout << "basketResult: " << basketResult << std::endl;;
    } else if (activeDevice == 1) { // 300TR
        int basketResult = sendBasket(communication , "{\n  \"basketID\": \"a123ca24-ca2c-401c-8134-f0de2ec25c25\",\n  \"documentType\": 0,\n  \"customerInfo\": {\n    \"taxID\": \"11111111111\"\n  },\n  \"items\": [\n    {\n      \"name\": \"�LA�\",\n      \"price\": 1000,\n      \"quantity\": 1000,\n      \"sectionNo\": 1,\n      \"taxPercent\": 1000,\n      \"type\": 0\n    }\n  ],\n  \"taxFreeAmount\": 5000\n}");
        std::cout << "basketResult: " << basketResult << std::endl;;
    }
}

/**
 * @brief Sends a sample payment to the 300TR device.
 * This function is specific to the 300TR model.
 * @param communication A pointer to the ConnectionWrapper object.
 * @param activeDevice The index of the active device.
 */
void sendPaymentTest(ConnectionWrapper* communication, int activeDevice) {
    if (activeDevice == 1) { // 300TR
        int paymentResult = sendPayment(communication, "{\"amount\":6000,\"description\":\"Nakit\",\"type\":1}");
        std::cout << "paymentResult: " << paymentResult << std::endl;;
    }
}

/**
 * @brief Retrieves fiscal information from the connected device.
 * @param communication A pointer to the ConnectionWrapper object.
 */
void getFiscalInfoTest(ConnectionWrapper* communication) {
    std::string fiscalInfo = getFiscalInfo(communication);
    std::cout << "Get Fiscal Info Result In Test: \n" << fiscalInfo << std::endl;
}


/**
 * @brief Main handler for the test application logic.
 * This function runs in a separate thread and provides a command-line menu
 * to interact with the fiscal device.
 */
void threadHandle() {
    const std::string companyName = "TokenLinuxTest";
    ConnectionWrapper* communication = createCommunication(companyName);
    setSerialInCallback(communication, setSerialInCallbackTest);
    setDeviceStateCallback(communication, setDeviceStateCallbackTest);

    std::this_thread::sleep_for(std::chrono::seconds(3));

    while (true) {
        std::cout << "Press [0-3] to execute the actions below" << std::endl;
        std::cout << "0: Get Active Device" << std::endl;
        std::cout << "1: Send Example Basket" << std::endl;
        std::cout << "2: Send Example Payment" << std::endl;
        std::cout << "3: Get Fiscal Info" << std::endl;

        int input;
        std::cin >> input;
        int activeDevice = getActiveDeviceIndex(communication);
        switch (input)
        {
            case 0: // Get Active Device
                std::cout<< "Active Device Index " << activeDevice << std::endl;
                break;
            case 1: // Send Example Basket
                sendBasketTest(communication, activeDevice);
                break;
            case 2: // Send Example Payment (300TR Only)
                sendPaymentTest(communication, activeDevice);
                break;
            case 3: // Get Fiscal Info
                getFiscalInfoTest(communication);
                break;
            default:
                break;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

/**
 * @brief The main entry point of the application.
 * It creates a new thread to run the test application logic.
 * @return 0 on successful execution.
 */
int main() {
    std::thread _thread(threadHandle);
    _thread.join();
}