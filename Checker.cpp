#include <iostream>
#include <string>
#include <cassert>
using namespace std;

enum Status { OK, LOW_BREACH, HIGH_BREACH };

struct CheckResult {
    Status status;
    string message;
};

// Generic limit checker
CheckResult checkLimit(float value, float min, float max, const string& paramName) {
    if (value < min) {
        return {LOW_BREACH, paramName + " is too low!"};
    }
    if (value > max) {
        return {HIGH_BREACH, paramName + " is too high!"};
    }
    return {OK, ""};
}

// Battery parameter check functions (pure)
CheckResult checkTemperature(float temperature) {
    return checkLimit(temperature, 0, 45, "Temperature");
}

CheckResult checkSOC(float soc) {
    return checkLimit(soc, 20, 80, "State of Charge");
}

CheckResult checkChargeRate(float chargeRate) {
    return checkLimit(chargeRate, 0, 0.8, "Charge Rate");
}

// Main battery health check (pure)
bool batteryIsOk(float temperature, float soc, float chargeRate) {
    CheckResult tempCheck = checkTemperature(temperature);
    CheckResult socCheck = checkSOC(soc);
    CheckResult chargeRateCheck = checkChargeRate(chargeRate);

    bool isOk = true;

    if (tempCheck.status != OK) {
        cout << tempCheck.message << endl;
        isOk = false;
    }
    if (socCheck.status != OK) {
        cout << socCheck.message << endl;
        isOk = false;
    }
    if (chargeRateCheck.status != OK) {
        cout << chargeRateCheck.message << endl;
        isOk = false;
    }

    return isOk;
}

// ✅ Unit Tests to cover all scenarios
void runTests() {
    assert(batteryIsOk(25, 70, 0.7) == true);          // All OK
    assert(batteryIsOk(-1, 70, 0.7) == false);         // Low temperature
    assert(batteryIsOk(50, 70, 0.7) == false);         // High temperature
    assert(batteryIsOk(25, 10, 0.7) == false);         // Low SOC
    assert(batteryIsOk(25, 90, 0.7) == false);         // High SOC
    assert(batteryIsOk(25, 70, 0.9) == false);         // High charge rate
    assert(batteryIsOk(0, 20, 0.8) == true);           // On lower boundaries
    assert(batteryIsOk(45, 80, 0.8) == true);          // On upper boundaries
}

int main() {
    runTests();
    cout << "All tests passed!\n";
    return 0;
}
