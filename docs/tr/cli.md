# Komut Satırı Kullanımı

`pearlcd` tek bir binary üzerinden birkaç farklı çalışma modu sunar. Tüm modlar `src/main.c` içinde seçilir.

## Genel davranış

- Girdi dosyası verilmezse `stdin` okunur.
- Tek bir dosya yolu verilebilir.
- `-` argümanı da `stdin` anlamına gelir.
- Çıktılar çoğu zaman terminale yazılır; `--run` ise geçici dosyalar kullanır.

## Modlar

### `--lex`

Kaynağı token’lara ayırır ve lexer çıktısını gösterir.

```bash
./build/pearlcd --lex examples/hello.prl
```

### `--parse`

Token akışını AST’ye çevirir ve ağacı basar.

```bash
./build/pearlcd --parse examples/test.prl
```

### `--check`

Sadece isim ve kapsam kontrollerini çalıştırır. Tip kontrolü yoktur.

```bash
./build/pearlcd --check examples/test.prl
```

### `--lower`

AST’yi metinsel IR formatına indirger.

```bash
./build/pearlcd --lower examples/test.prl
```

### `--codegen`

AST’den C kodu üretir ve standard output’a yazar.

```bash
./build/pearlcd --codegen examples/test.prl > out.c
```

### `--run`

Önce C kodu üretir, sonra host derleyici ile derler ve çalıştırır. Bu mod sistemde bir C compiler gerektirir.

```bash
./build/pearlcd --run examples/test.prl
```

## Hata çıktıları

- Dosya açılamazsa `perror` ile hata basılır.
- Geçersiz seçenek verilirse kullanım bilgisi gösterilir.
- Ayrıştırma hataları satır numarası ile raporlanır.

## Pratik not

`--run` modu taşınabilir bir sanal makine değil; sistemde çalışan derleyiciye bağlı hızlı bir doğrulama yoludur.