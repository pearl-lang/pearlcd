# AST

AST, parse edilmiş programın ağaç biçimindeki temsilidir.

## Neden gerekli?

- Token detaylarını ortadan kaldırır.
- Program yapısını görünür hale getirir.
- Sonraki katmanlara temiz bir girdi sağlar.

## Düğüm türleri

`include/ast.h` içinde bulunan başlıca türler:

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

## Basit yapı

```text
PROGRAM
|-- FUNCTION main
    |-- PARAMS
    `-- BLOCK
        |-- VAR_DECL x = 5
        `-- CALL justPrint(x)
```

## Taşıdığı bilgi

AST düğümleri genellikle şunları taşır:

- düğüm türü
- isteğe bağlı metin değeri
- çocuk düğümler

## Tasarım hedefi

AST küçük, açık ve gezmesi kolay olmalıdır.