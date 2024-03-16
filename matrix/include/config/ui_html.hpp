#pragma once

constexpr char* html_wifi_setup =
"<!DOCTYPE html>"
"<html>"
"<head>"
"    <title>WiFi Connection Form</title>"
"</head>"
"<body>"
""
"    <h2>Connect to WiFi</h2>"
""
"    <form action=\"wifi/\" method=\"get\">"
"        <label for=\"ssid\">SSID:</label><br>"
"        <input type=\"text\" id=\"ssid\" name=\"ssid\"><br>"
"        <label for=\"pwd\">Password:</label><br>"
"        <input type=\"password\" id=\"pwd\" name=\"pwd\"><br><br>"
"        <input type=\"submit\" value=\"Connect\">"
"    </form>"
""
"</body>"
"</html>";

