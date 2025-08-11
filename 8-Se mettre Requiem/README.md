# Se mettre Requiem

Pengali dua bilangan positif yang hanya menggunakan *bitwise operator*. Menggunakan *Number Theoretic Transform* untuk menghitung perkalian angka yang sangat besar dalam waktu yang *reasonable*. <br>
Ada dua versi dari program, yaitu versi yang mengakses index dari array melalui macro dan yang langsung mengakses menggunakan subscript. Versi subscript sebenarnya hanya untuk membandingkan performa dengin versi macro ketika mengoptimasinya, cuman sayang kalo dibuang. **NOTE**: perbedaan performa ternyata tidak jauh<br>
**Versi utama** dari program ini adalah versi macro yang disimpan pada folder `macro_index`

## Author
| Nama              | NIM      |
| ----------------- | -------- |
| M. Rayhan Farrukh | 13523035 |

<br>

## Tabel Spesifikasi
| Spesifikasi          | Status |
| -------------------- | ------ |
| Handle $0 \leq x \leq 2^{32}$  | ✅ |
| Handle $0 \leq x \leq 10^{1000}$         | ✅ |
| Handle $0 \leq x \leq 10^{1000000}$    |  ✅ |


## How to Use
Compile dengan `make all`, lalu jalani dengan `make run`

## *Benchmark*
| Digits | Time(s) |
| ---- | ------ |
|50000  |	9.2736580  |
|100000 |	19.4443638 |
|150000 |	39.756514  |
|200000 |	41.043018  |
|250000 |	42.571988  |
|300000 |	83.534341  |
|350000 |	85.13502   |
|400000 |	86.540340  |
|450000 |	87.663124  |
|500000 |	89.188734  |
|550000 |	173.577047 |
|600000 |	175.917703 |
|650000 |	177.425068 |
|700000 |	178.93785  |
|750000 |	179.23796  |
|800000 |	181.049945 |
|850000 |	184.15389  |
|900000 |	184.180155 |
|950000 |	185.544420 |
|1000000 |	183.037914 |