# Mimari

PearlCD küçük adımlarla ilerleyen bir derleyici boru hattı kullanır.

## Ana akış

1. **Lexer** kaynak kodu token’lara böler.
2. **Parser** token’lardan AST üretir.
3. **Semantic** isim ve kapsam kontrolü yapar.
4. **IR** AST’yi metinsel ara temsile indirger.
5. **C codegen** AST’den C kodu üretir.
6. **Run modu** üretilen C’yi host derleyiciyle çalıştırır.

## AST

AST düğümleri `include/ast.h` içindedir. Başlıca düğümler:

- `PROGRAM`
- `FUNCTION`
- `PARAMS`
- `BLOCK`
- `VAR_DECL`
- `ASSIGNMENT`
- `CALL`
- `RETURN`
- `IF`
- `WHILE`
- `FOR`
- `BINARY`
- `UNARY`
- `IDENTIFIER`
- `LITERAL`

AST’nin görevi token detaylarını bırakıp programın yapısını açık hale getirmektir.

## Semantic katman

Semantic kontrol şu işleri yapar:

- aynı scope içinde tekrar eden isimleri yakalar
- bilinmeyen identifier kullanımlarını yakalar
- geçersiz assignment hedeflerini yakalar
- çözülmeyen çağrıları yakalar

Şimdilik tip sistemi yoktur; odak isim çözümleme ve scope yönetimidir.

## IR

IR katmanı okunabilir metinsel bir ara çıktı üretir. Bu çıktı bir SSA formu değildir; debug etmeye uygun, sade bir temsil olarak tasarlanmıştır.

## C codegen

C backend doğrudan C üretir. `print` ve `justPrint` özel olarak işlenir. `main` fonksiyonunda açık dönüş yoksa `return 0` eklenir.

## Platform davranışı

- Linux/macOS tarafında standart POSIX araçları kullanılabilir.
- Windows tarafında MSYS2 + MinGW kullanılır.
- `--run` geçici dosya üretir ve host compiler’a dayanır.

## Tasarım hedefi

Mimari, her katmanın tek sorumluluğa sahip olduğu küçük ve test edilebilir bir yapı olarak kalmalıdır.