uint32_t MakeAddress(uint8_t bus, uint8_t device,
                        uint8_t fuction, uint8_t reg_addr) {
    auto shl = [](uint32_t x, unsignned int bits) {
        return x << bits;
    };

    return shl(1, 31)
                        }