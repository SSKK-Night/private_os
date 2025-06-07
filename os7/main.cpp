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