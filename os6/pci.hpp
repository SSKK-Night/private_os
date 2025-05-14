
const uint16_t kConfigAddress = 0x0cf8;

const uint16_t kConfigData = 0x0cfc;


void WriteAddress(uint32_t address) {
    IoOut32(kConfigAddress, address);
}

void WriteData(uint32_t value) {
    IoOut32(kConfigData, value);
}

uint32_t ReadData() {
    return IoIn32(kConfigData);
}

uint16_t ReadVendorId(uint8_t bus, uint8_t device, uint8_t function) {
    WriteAddress(MakeAddress(bus, device, function, 0x00));
    return ReadData() & 0xffffu;
}

inline std::array<Device, 32> devices;

inline int num_device;

Error ScanAllBus();