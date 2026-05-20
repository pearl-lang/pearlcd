# Sözdizimi

Pearl sözdizimi küçük ve doğrudandır. Parser, programı yalnızca fonksiyonlardan oluşan bir üst seviye yapı olarak görür.

## Temel kurallar

- Program, bir veya daha fazla fonksiyon tanımından oluşur.
- Fonksiyonlar `fun isim(param1, param2) { ... }` biçimindedir.
- Bloklar süslü parantez ile yazılır.
- Statement sonları `\n` veya `;` ile ayrılır.
- Yorumlar `//` ile başlar.

## Anahtar kelimeler

- `var`
- `fun`
- `if` / `else`
- `while`
- `for`
- `return`
- `print`
- `justPrint`
- `true`, `false`, `null`

## Statement türleri

### Değişken bildirimi

```prl
var x = 5
```

### Atama

```prl
x = x + 1
```

### Çağrı

```prl
print(x)
justPrint(x, y)
foo(bar)
```

### Dönüş

```prl
return x
return
```

### Koşul

```prl
if (x > 0) {
  print(x)
} else {
  print(0)
}
```

### Döngüler

```prl
while (i < 10) {
  i = i + 1
}

for (var i = 0; i < 10; i = i + 1) {
  justPrint(i)
}
```

## İfadeler

İfadeler şu sırayla çözülür:

- atama
- eşitlik karşılaştırmaları `==`, `!=`
- karşılaştırmalar `<`, `<=`, `>`, `>=`
- toplama/çıkarma `+`, `-`
- çarpma/bölme `*`, `/`
- unary `!`

Örnek:

```prl
var total = x + y * 2
```

## Literal türleri

- sayı: `42`
- string: `"hello"`
- boolean: `true`, `false`
- null: `null`

## Mevcut parser notu

Parser şu an parantezli koşul ve döngü biçimini kullanır. Yani `if (expr) { ... }` ve `while (expr) { ... }` beklenir.

## Kısa örnek

```prl
fun main() {
  var x = 5
  var y = 10
  var total = x + y
  if (total > 10) {
    justPrint(total)
  } else {
    print(total)
  }
}
```