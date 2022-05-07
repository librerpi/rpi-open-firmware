register | offset
--- | ---
`SYS_REV_CTRL` | `0x00`

## `SYS_REV_CTRL` register
contains the major version# for the genet controller

the bcm2711 uses genet version 5, which is what this documents

bits | usage
-----|------
24:27 | major version#, not a simple int, refer to linux src


## dma descriptors

the hw rx and tx rings are arrays of 256 elements each of:
```
struct {
  uint32_t length_status;
  uint32_t addr_lo;
  uint32_t addr_hi;
}
```
beware, other genet versions may have only 32bit addressing, and lack the `addr_hi` field

on genet 5, the rx ring begins at offset `0x2000` and the tx ring at offset `0x4000`

## dma length+status field


rx only flags
bits  | usage
------|-----
0     | `RX_OV`
1     | `RX_CRC_ERROR`
2     | `RX_RXER`
3     | `RX_NO`
4     | `RX_LG`
5     | `RX_MULT`
6     | `RX_BRDCAST`
7:11  | `RX_FI`
12    | `RX_CHK_V12`
15    | `RX_CHK_V3PLUS`

tx only flags
bits  | usage
------|-----
4     | `TX_DO_CSUM`
5     | `TX_OW_CRC`
6     | `TX_APPEND_CRC`
8     | `TX_UNDERRUN`

common flags
bits  | usage
------|-----
12    | `WRAP`
13    | `SOP`
14    | `EOP`
15    | `OWN`
16:27 | the length of the packet
