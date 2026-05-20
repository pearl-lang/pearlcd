# Build, Test ve Release

## Gereksinimler

- CMake
- Flex
- Bison
- C derleyicisi
- `pandoc` (doküman üretimi için)

## Yerel build

En kısa yol:

```bash
cmake -S . -B build
cmake --build build
```

Yardımcı script:

```bash
bash scripts/build.sh
bash scripts/build.sh mingw
bash scripts/build.sh linux out/build-linux
```

## Doküman üretimi

Türkçe ve İngilizce dokümanlar `scripts/docs.sh` ile üretilir.

```bash
bash scripts/docs.sh tr
bash scripts/docs.sh en
bash scripts/docs.sh all
```

Çıktılar `docs/dist/` altında oluşur.

## Release paketleme

Release için paketleme script’i:

```bash
bash scripts/package_release.sh dist
```

Bu script:

- uygun build klasörü oluşturur
- binary üretir
- Linux için `.tar.gz`
- Windows için `.zip`
- SHA256 checksum dosyaları üretir

## CI/CD

- `ci.yml`: push/pull request sırasında Linux ve Windows build + smoke test
- `release.yml`: release oluşturulunca Linux ve Windows artifact üretimi ve GitHub Release’e yükleme

## Ignore edilen çıktılar

- `build/`
- `build-release*/`
- `dist/`
- `docs/dist/`

Bu çıktıların repoya girilmemesi gerekir.