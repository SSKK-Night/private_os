EFI_FILE_PROTOCOL* kernel_file;
root_dir->Open(
    root_dir, &kernel_file, L"\\kernel.elf",
    EFI_FILE_MODE_READ, 0);

UINTN file_info_size = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 12;
UINT8 file_info_buffer[file_info_size];
kernel_file->GetInfo(
    kernel_file, &gEfiFileInfoGuid,
    &file_info_size, file_info->FileSize);

EFI_FILE_INFO* file_info = (EFI_FILE_INFO*)file_info_buffer;
UINTN kernel_file_size = file_info->FileSize;

EFI_PHYSICAL_ADDRESS kernel_base_addr = 0x100000;
gBS->AllocatePages(
    AllocateAddress, EfiLoaderData,
    (kernel_file_size + 0xfff) / 0x1000, &kernel_base_addr);
if (EFI_ERROR(status)) {
    Print(L"failed to allocate pages: %r", status);
    Halt();
}
kernel_file->Read(kernel_file, &kernel_file_size, (VOID*)kernel_base_addr);
Print(L"Kernel: 0x%0lx (%lu bytes)\n", kernel_base_addr, kernel_file_size);

typedef struct {
    UINT64 Size, FileSize, PhysicalSize;
    EFI_TIME CreateTime, LastAccessTime, ModificationTime;
    UINT64 Attribute;
    CHAR16 FileName[];
} EFI_FILE_INFO;

EFI_STATUS status;
status = gBS->ExitBootServices(image_handle, memmap.map_key);
if (EFI_ERROR(status)) {
    Print(L"failed to get memory map: %r\n", status);
    while (1);
}
status = gBS->ExiBootServices(image_handle, memmap.map_kye);
if (EFI_ERROR(status)) {
    Print(L"Could not exit boot service: %r\n", status);
    while (1);
}


UINT64 entry_addr = *(UINT64*)(kernel_base_addr + 24);

typedef void EntryPointType(void);
EntryPointType* entry_point = (EntryPointType*)entry_addr;
entry_point();

EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
OpenGOP(image_handle, &gop);
Print(L"Resolution: %ux%u, Pixel Format: %s, %u pixels/line\n",
    gop->Mode->Info->HorizontalResolution,
    gop->Mode->Info->VerticalResolution,
    GetPixelFormatUnicode(gop->Mode->Info->PixelFormat),
    gop->Mode->Info->PixelsPerScanLine);
Print(L"Frame Buffer: 0x%0lx - 0x%0lx, Size: %lu bytes\n",
    gop->Mode->FrameBufferBase,
    gop->Mode->FrameBufferBase + gop->Mode->FrameBufferSize,
    gop->Mode->FrameBufferSize);

UINT8* frame_buffer = (UINT8*)gop->Mode->FrameBufferBase;
for (UINTN i = 0; i < gop->Mode->FrameBufferSize; ++i) {
    frame_buffer[i] = 255;
}

typedef void EntryPointType(UINT64, UINT64);
EntryPointType* entry_point = (EntryPointType*)entry_addr;
entry_point(gop->Mode->FrameBufferBase, gop->Mode->FrameBufferSize);