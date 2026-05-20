# Lexer ve Parser

Bu sayfa lexer ve parser arasındaki sınırı detaylandırır.

## Lexer

Lexer, ham kaynak metni token’lara dönüştürür. Şu token grupları desteklenir:

- anahtar kelimeler
- identifier’lar
- sayılar
- string literal’lar
- operatörler
- ayırıcılar

Desteklenen önemli token’lar:

- `var`
- `fun`
- `if`
- `else`
- `while`
- `for`
- `return`
- `print`
- `justPrint`
- `true`
- `false`
- `null`

## Parser

Parser token akışını AST’ye dönüştürür. Ana görevleri:

- token sırasını dil kuralları ile eşleştirmek
- fonksiyonlar, bloklar ve ifadeler üretmek
- syntax hatalarını satır numarasıyla raporlamak

## Operatör önceliği

Mevcut öncelik sırası özetle şöyledir:

1. assignment
2. equality
3. comparison
4. addition/subtraction
5. multiplication/division
6. unary

## Notlar

- Yorumlar `//` ile başlar.
- Statement sonları satır sonu veya `;` ile ayrılır.
- Parser, sade ve doğrudan bir gramer hedefler.

## Örnek

```prl
fun main() {
  var x = 5
  var y = 10
  justPrint(x + y)
}
```