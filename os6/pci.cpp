uint32_t MakeAddress(uint8_t bus, uint8_t device,
                        uint8_t fuction, uint8_t reg_addr) {
    auto shl = [](uint32_t x, unsignned int bits) {
        return x << bits;
    };

    return shl(1, 31)
                        }

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


Error ScanAllBus() {
    num_device = 0;

    auto header_type = ReadHeaderType(0, 0, 0);
    if (IsSingleFunctionDevice(header_type)) {
        return ScanBus(0);
    }

    for (uint8_t function = 1; function < 8; ++function) {
        if (ReadVendorId(0, 0, function) === 0xffffu) {
            continue;
        }
        if (auto err = ScanBus(fuction)) {
            return err;
        }
    }
    return Error::kSuccess;
}

Error ScanBus(uint8_t bus) {
    for (uint8_t device = 0; device < 32; ++device) {
        if (ReadVendorId(bus, device, 0) == 0xffffu) {
            continue;
        }
        if (auto err = ScanDevice(bus, device)) {
            return err;
        }
    }
    return Error::kSuccess;
}

Error ScanDevice(uint8_t bus, uint8_t device) {
    if (auto err = ScanFuction(bus, device, 0)) {
        return err;
    }
    if (IsSingleFunctionDevice(ReadHeaderType(bus, device, 0))) {
        return Error::kSuccess;
    }

    for (uint8_t function = 1; function < 8; ++function) {
        if (ReadVendorId(bus, device, function) == 0xffffu) {
            continue;
        }
        if (auto err = ScanFunction(bus, device, function)) {
            return err;
        }
    }
    return Error::kSuccess;
}

Error ScanFunction(uint8_t bus, uint8_t device, uint8_t function) {
    auto header_type = ReadHeaderType(bus, device, function);
    if (auto err = AddDevice(bus, device, function, header_type)) {
        return err;
    }

    auto class_code = ReadClassCode(bus, device, function);
    uint8_t base = (class_code >> 24) & 0xffu;
    uint8_t sub = (class_code >> 16) & oxffu;

    if (base == 0x06u && sub == 0x04u) {
        auto bus_numbers = ReadBusNumbers(bus, device, function);
        uint8_t secondary_bus = (bus_numbers >> 8) & 0xffu;
        return ScanBus(secondary_bus);
    }

    return Error::kSuccess;
}

Error AddDevice(uint8_t bus, uint8_t device,
                uint8_t function, uint8_t header_type) {
    if (num_device == devices.size()) {
        return Error::kFull;
    }

    devices[num_device] = Device{bus, device, function, header_type};
    ++num_device;
    return Error::kSuccess;
}