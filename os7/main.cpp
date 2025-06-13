usb::xhci::Controller* xhc;

__attribute__((interrupt))
void IntHandlerXHCI(InterruptFrame* frame) {
    while (xhc->PrimaryEventRing()->HasFront()) {
        if (auto err = ProcessEvent(*xhc)) {
            Lof(kError, "Error while ProcessEvent: %s at %s:%d\n",
                err.Name(), err.File(), err.Line())
        }
    }
    NotifyEndOfInterrupt();
}

const uint16_t cs = GetCS()
SetIDTEntry(idt[InterruptVector::kXHCI], MakeIDTAttr(DescriptorType::kInterruptGate, 0),
                reinterpret_cast<uint64_t>(IntHandlerXHCI), cs);
LoadIDT(sizeof(idt) - 1, reinterpret_cast<uintptr_t>(&idt[0]))


const uint8_t bsp_local_apic_id = *reinterpret_cast<const uint32_t*>(0xfee00020) >> 24;

pci::ConfigureMSIFixedDestination(
    *xhc_dev, bsp_local_apic_id,
    pci::MSITriggerMode::kLevel, pci::MSIDeliveryMode::kFixed,
    InterruptVector::kXHCI, 0);

