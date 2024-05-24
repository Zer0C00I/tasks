#ifndef THERMOSTAT_LOGIC_H
#define THERMOSTAT_LOGIC_H

#endif // THERMOSTAT_LOGIC_H

#include <iostream>
#include <map>
#include <vector>
#include <stdexcept>
#include <ctime>
#include <unistd.h> 
#include <json/json.h>
#include <cpprest/json.h> 


using namespace std;

class Thermostat {
private:
    string serialnumber;
    string host;
    int port;
    string username;
    string password;
    time_t last_request;

    const string base_url = "http://{host}:{port}/{endpoint}.cgi";

    void sleep_for_one_second() {
        time_t current_time = time(nullptr);
        if (current_time - last_request < 1) {
            sleep(1);
        }
    }

    string get_url(string endpoint) {
        return base_url;
    }

public:
    Thermostat(string serialnumber, string host, int port=80, string username="", string password="") :
        serialnumber(serialnumber), host(host), port(port), username(username), password(password) {
        time_t now = time(nullptr);
        last_request = now;

        try {
            http_client client(base_url);
            http_response response = client.request(methods::GET).get();
            if (response.status_code() != status_codes::OK) {
                throw runtime_error("Connection to Thermostat failed");
            }
        } catch (const exception& e) {
            throw;
        }
    }

    json::value get(string endpoint, json::value data) {
        sleep_for_one_second();

        try {
            http_client client(base_url);
            http_request request(methods::GET);
            request.set_request_uri(endpoint);
            request.headers().set_content_type("application/json");
            request.set_body(data);

            http_response response = client.request(request).get();
            if (response.status_code() == status_codes::OK) {
                return response.extract_json().get();
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }

        return json::value::null();
    }

    json::value post(string endpoint, json::value data) {
        sleep_for_one_second();

        try {
            http_client client(base_url);
            http_request request(methods::POST);
            request.set_request_uri(endpoint);
            request.headers().set_content_type("application/json");
            request.set_body(data);

            http_response response = client.request(request).get();
            if (response.status_code() == status_codes::OK) {
                return response.extract_json().get();
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }

        return json::value::null();
    }

    json::value status() {
        json::value data;
        data["cmd"] = json::value::number(4);
        data["sn"] = json::value::string(serialnumber);
        return post("api", data);
    }

    json::value lock_check() {
        json::value data = status();
        if (data.is_object() && data.has_field(U("m.3"))) {
            int lock_type = data[U("m.3")].as_integer();
            switch (lock_type) {
            case 0:
                return json::value::string("No lock");
            case 1:
                return json::value::string("Cloud lock");
            case 2:
                return json::value::string("LAN lock");
            case 3:
                return json::value::string("Both cloud and LAN lock");
            default:
                return json::value::string("Unknown lock type");
            }
        } else {
            return json::value::string("Lock type not found in response");
        }
    }

    bool is_on() {
        json::value data = post("api", json::value::object({{"cmd", json::value::number(1)}, {"sn", json::value::string(serialnumber)}}));
        if (data.is_object() && data.has_field(U("par"))) {
            for (auto&& item : data[U("par")].as_array()) {
                if (item[U("0")].as_integer() == 125) {
                    return item[U("2")].as_string() == "0";
                }
            }
        }
        return false;
    }

    double temperature() {
        json::value data = status();
        if (data.is_object() && data.has_field(U("t.1"))) {
            return stod(data[U("t.1")].as_string()) / 16.0;
        }
        return 0.0;
    }
