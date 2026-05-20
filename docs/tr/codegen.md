# C Codegen

C backend, AST’den doğrudan C kodu üretir.

## Temel davranış

- değişkenler C `int` olarak emit edilir
- atamalar doğrudan C ataması olarak yazılır
- fonksiyon çağrıları C çağrısına çevrilir
- `print` ve `justPrint` özel olarak ele alınır

## Çıktı biçimi

Üretilen kod, okunabilir ve sade tutulur. Amaç üretim kalitesi değil, çalışır ve anlaşılır bir ara aşama sağlamaktır.

## `print` davranışı

- tek string argüman için `puts`
- sayısal ifade için `printf`
- birden fazla argüman için format string üretimi

## `--run` bağlantısı

`--run` modu önce C kodu üretir, sonra bu kodu geçici dosyaya yazar, derler ve çalıştırır. Bu yüzden host compiler gerekir.

## Tasarım hedefi

Codegen katmanı küçük, doğrudan ve taşınabilir kalmalıdır.