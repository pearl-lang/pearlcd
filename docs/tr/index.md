# PearlCD Dokümantasyonu

## Genel Bakış

PearlCD, Pearl dili için küçük ve sade bir derleyici prototipidir. Amaç, dili gereksiz ayrıntılar eklemeden okunabilir ve test edilebilir bir biçimde derlemektir. Proje C tabanlıdır ve yapılandırma/derleme akışı CMake, Flex ve Bison etrafında kuruludur.

Ana işlem hattı şöyledir:

1. Kaynak kodu lexer ile token’lara ayrılır.
2. Parser, token’lardan bir AST (Abstract Syntax Tree / Soyut Sözdizim Ağacı) üretir.
3. Semantic katman, isim ve kapsam kontrollerini yapar.
4. IR katmanı, AST’yi metinsel bir ara temsile indirger.
5. C codegen katmanı, AST’den C kodu üretir.
6. `--run` modu, üretilen C kodunu geçici dosyaya yazıp host derleyici ile çalıştırır.

Bu proje “küçük ama anlaşılır” yaklaşımını benimser. Yani öncelik, özellik sayısını artırmak değil; mevcut özellikleri tutarlı, taşınabilir ve test edilebilir tutmaktır.

## Depo Yapısı

Önemli klasörler:

- `src/`: ana kaynak kodu
- `include/`: public header dosyaları
- `concept/`: mimari ve dil notları
- `docs/`: kullanıcıya yönelik dokümantasyon kaynakları
- `examples/`: örnek Pearl programları
- `scripts/`: build, paketleme ve doküman üretim yardımcıları
- `toolchain/`: platforma özel CMake toolchain dosyaları
- `tests/`: birim testleri

## Detaylı Konular

- [Komut Satırı Arayüzü](cli.md)
- [Sözdizimi](syntax.md)
- [Lexer ve Parser](lexer-parser.md)
- [AST](ast.md)
- [Semantic Kontroller](semantic.md)
- [IR](ir.md)
- [C Codegen](codegen.md)

## Build, Test ve Release

Detaylar için [Build, Test ve Release](build-release.md) sayfasına bak.

## Sınırlamalar

Bu sürümün bilinçli sınırlamaları:

- global tip sistemi yok
- LLVM backend aktif değil
- C backend daha çok prototip düzeyinde
- `--run` host derleyiciye bağımlı
- dil yüzeyi kasıtlı olarak küçük tutuluyor

## Geliştirme Notları

Yeni bir özellik eklerken şu sırayı korumak faydalıdır:

1. lexer token’ını ekle
2. parser kuralını yaz
3. AST düğümünü tanımla
4. semantic kontrolünü ekle
5. IR/C codegen desteğini tamamla
6. örnek ve test ekle

Bu yaklaşım, kod tabanını küçük ve anlaşılır tutar.
