# Semantic Kontroller

Semantic katman, AST parse edildikten sonra isim ve kapsam kurallarını denetler.

## Şu anki kontroller

- aynı scope içinde tekrar eden isimler
- bilinmeyen identifier kullanımı
- geçersiz assignment hedefleri
- çözülemeyen call isimleri

## Kapsam yaklaşımı

- Fonksiyonlar kendi scope’larını oluşturur.
- Bloklar yeni scope açabilir.
- Değişken arama dış scope’lara doğru genişleyebilir.

## Bilinmesi gerekenler

- Henüz type checking yoktur.
- Semantic katman syntax hatalarını değil, anlam hatalarını yakalar.
- `print` ve `justPrint` yerleşik çağrılar olarak ele alınır.

## Örnek hata türü

```prl
fun main() {
  justPrint(x)
}
```

Bu örnek, `x` tanımlı değilse semantic hata üretir.

## Tasarım hedefi

Semantic katman kısa, okunabilir ve genişletilebilir kalmalıdır.