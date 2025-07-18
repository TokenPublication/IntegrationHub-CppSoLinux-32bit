/**
 * @file IntegrationHubWrapper.h
 * @brief This header file defines the C-style wrapper for the IntegrationHub library.
 *
 * The IntegrationHub library is used to integrate desktop applications with
 * Beko fiscal devices (X30TR and 300TR). This wrapper provides a set of
 * C-compatible functions to communicate with the underlying shared library (.so).
 * It is designed for 32-bit Linux systems.
 */

#pragma once
#include <string>

/**
 * @brief An opaque pointer representing a connection to the IntegrationHub.
 * This handle is used in all subsequent calls to the library.
 */
typedef void ConnectionWrapper;

/**
 * @brief Callback function type for receiving serial data.
 * @param tag An integer tag identifying the data source.
 * @param data The serial data received, typically as a string.
 */
typedef void(*SerialInCallback)(int, std::string);

/**
 * @brief Callback function type for device state changes.
 * @param state The new state of the device (true for connected, false for disconnected).
 * @param deviceId A string identifying the device.
 */
typedef void(*DeviceStateCallback)(bool, std::string);

/**
 * @brief Creates a communication channel with the IntegrationHub.
 * @param companyName A string identifying the client application or company.
 * @return A pointer to a ConnectionWrapper object, or nullptr on failure.
 */
extern "C" ConnectionWrapper* createCommunication(std::string companyName);

/**
 * @brief Deletes the communication channel and releases all associated resources.
 * @param ptr A pointer to the ConnectionWrapper object.
 */
extern "C" void deleteCommunication(ConnectionWrapper* ptr);

/**
 * @brief Attempts to reconnect to the IntegrationHub.
 * @param ptr A pointer to the ConnectionWrapper object.
 */
extern "C" void reconnect(ConnectionWrapper* ptr);

/**
 * @brief Gets the index of the currently active fiscal device.
 * The index typically corresponds to a specific device model (e.g., 0 for X30TR, 1 for 300TR).
 * @param ptr A pointer to the ConnectionWrapper object.
 * @return The index of the active device as an integer.
 */
extern "C" int getActiveDeviceIndex(ConnectionWrapper* ptr);

/**
 * @brief Sends a basket of items to the fiscal device for processing.
 * @param ptr A pointer to the ConnectionWrapper object.
 * @param jsonData A JSON string representing the basket data.
 * @return An integer status code indicating the result of the operation.
 */
extern "C" int sendBasket(ConnectionWrapper* ptr, std::string jsonData);

/**
 * @brief Sends payment information to the fiscal device.
 * @param ptr A pointer to the ConnectionWrapper object.
 * @param jsonData A JSON string representing the payment data.
 * @return An integer status code indicating the result of the operation.
 */
extern "C" int sendPayment(ConnectionWrapper* ptr, std::string jsonData);

/**
 * @brief Retrieves fiscal information from the device.
 * @param ptr A pointer to the ConnectionWrapper object.
 * @return A string (often in JSON format) containing fiscal information.
 */
extern "C" std::string getFiscalInfo(ConnectionWrapper* ptr);

/**
 * @brief Sets a callback function to handle incoming serial data.
 * @param ptr A pointer to the ConnectionWrapper object.
 * @param callback The function to be called when serial data is received.
 */
extern "C" void setSerialInCallback(ConnectionWrapper* ptr, SerialInCallback callback);

/**
 * @brief Sets a callback function to handle device state changes (e.g., connection/disconnection).
 * @param ptr A pointer to the ConnectionWrapper object.
 * @param callback The function to be called when the device state changes.
 */
extern "C" void setDeviceStateCallback(ConnectionWrapper* ptr, DeviceStateCallback callback);
