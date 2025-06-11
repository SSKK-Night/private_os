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