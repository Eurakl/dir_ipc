-c
-heap 0x2000
-stack 0x2000
--export=boot

MEMORY
{
    /* Local L2, 0.5~1MB*/
    BOOT (RWX)     : org = 0x00800000  len = 0x00000400
    LL2(RWX)       : o = 0x00800400  l = 0x00080000
    MSMCSRAM (RWX) : org = 0xc000000, len = 0x400000
    DDR3           : org = 0x80000000, len = 0x10000000
}

SECTIONS
{
    .bootload > BOOT
    .text    > LL2
    .cinit   > LL2
    .const   > LL2
    .switch  > LL2
    .stack   > LL2

    GROUP {
        .neardata
        .rodata
        .bss
    } > LL2

    .far     > LL2
    .fardata > LL2
    .cio     > LL2
    .sysmem  > LL2
    .IPC 			>	 0xc200000    START(Flag)
    .IPC 			>	 0xc200004    START(Pkt_Test)
}


