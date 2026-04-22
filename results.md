<p align="center">
    <img src="assets/rol_ror_godbolt.png" width="75%">
</p>
<p align="center">
    <img src="hist/always_one.png" width="75%">
</p>
<p align="center">
    <img src="hist/first_letter.png" width="75%">
</p>
<p align="center">
    <img src="hist/strlen.png" width="75%">
</p>
<p align="center">
    <img src="hist/checksum_500.png" width="75%">
</p>
<p align="center">
    <img src="hist/checksum.png" width="75%">
</p>
<p align="center">
    <img src="hist/djb2.png" width="75%">
</p>
<p align="center">
    <img src="hist/rol.png" width="75%">
</p>
<p align="center">
    <img src="hist/ror.png" width="75%">
</p>
<p align="center">
    <img src="hist/crc32.png" width="75%">
</p>

|version|time, s|Ускорение относительно предыдущей версии | Ускорение относительно начальной версии |
|:---|---:|---:|---:|
| baseline              | 9.512 ± 0.059 | 1.00 | 1.00 |
| +avx_strcmp           | 3.749 ± 0.025 | 2.53 | 2.53 |
| +asm crc32            | 3.044 ± 0.018 | 1.23 | 3.12 |
| +asm hash_table find  | 2.888 ± 0.019 | 1.05 | 3.29 |

