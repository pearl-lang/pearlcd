# IR

IR katmanı AST’yi daha düşük seviyeli, metinsel bir ara temsile indirger.

## Amaç

- debug etmeyi kolaylaştırmak
- codegen öncesi ara görünüm sağlamak
- sonraki aşamaların testini basitleştirmek

## Yapı

Bu IR, ağır bir SSA tasarımı değildir. Okunabilirlik ön plandadır.

## Beklenen kullanım

```bash
./build/pearlcd --lower examples/test.prl
```

## Ne gösterir?

- fonksiyon yapıları
- statement akışı
- expression ilişkileri
- çağrı ve atama şekilleri

## Not

IR çıktısı, kaynak koda bire bir eşleşmek zorunda değildir; amacı compiler pipeline’ını görünür kılmaktır.