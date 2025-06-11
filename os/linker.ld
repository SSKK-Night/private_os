ENTRY(_start)

SECTIONS {
  /* 1MiB にロード開始 */
  . = 1M;

  /* Multibootヘッダ用セクションを先頭に配置 */
  .multiboot ALIGN(4) : {
    KEEP(*(.multiboot))    /* .multiboot セクションを必ず保持 */
  }

  /* 通常のコード・データ */
  .text : {
    *(.text*)
  }
  .data : {
    *(.data*)
  }
  .bss : {
    *(.bss*)
  }
}
