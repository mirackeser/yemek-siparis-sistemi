<div align="center">

# 🍽️ Yemek Sipariş ve Dağıtım Sistemi

**Veri yapıları kullanılarak C dilinde geliştirilmiş terminal tabanlı yemek sipariş yönetim sistemi**

[![Language](https://img.shields.io/badge/Language-C-blue?style=for-the-badge&logo=c)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey?style=for-the-badge)](.)
[![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)](LICENSE)
[![IDE](https://img.shields.io/badge/IDE-Dev--C%2B%2B-orange?style=for-the-badge)](https://sourceforge.net/projects/orwelldevcpp/)

</div>

---

## 📌 Proje Hakkında

Bu proje, **Veri Yapıları** dersi kapsamında geliştirilmiş, gerçek dünya senaryosunu simüle eden bir **yemek sipariş ve dağıtım yönetim sistemidir**. Terminal arayüzü üzerinden menü yönetimi, sipariş alma, işleme ve gelir takibi yapılabilmektedir.

Projenin temel hedefi; bağlı listeler, kuyruklar ve yığınlar gibi temel veri yapılarının gerçek bir uygulamada nasıl kullanıldığını göstermektir.

---

## 🏗️ Kullanılan Veri Yapıları

| Veri Yapısı | Kullanım Alanı | Yapı |
|---|---|---|
| **Bağlı Liste** (Linked List) | Menü öğelerini yönetmek | `menuOgesi` struct'ı ile tek yönlü bağlı liste |
| **Kuyruk** (Queue) | Sipariş kuyruğunu yönetmek | FIFO yapısı, `siparisKuyruguBasi/Sonu` işaretçileriyle |
| **Yığın** (Stack) | Gelir kayıtlarını tutmak | LIFO yapısı, `gelirYigini` işaretçisiyle |

---

## ✨ Özellikler

- 📋 **Menü Yönetimi** — Yemek ekleme, listeleme ve silme
- 🛒 **Sipariş Alma** — Kullanıcı adı, telefon ve adres bilgisiyle sipariş oluşturma
- ⚙️ **Sipariş İşleme** — FIFO sırasına göre sipariş işleme ve kuyruktan çıkarma
- 🗑️ **Sipariş Silme** — Kuyruktan belirli bir siparişi kaldırma
- 💰 **Gelir Takibi** — Sipariş bazlı gelir kayıtları ve toplam gelir görüntüleme
- 💾 **Kalıcı Depolama** — Veriler `data.txt` ve `siparis.txt` dosyalarına kaydedilir; program yeniden başlatıldığında yüklenir

---

## 📁 Proje Yapısı

```
yemek-siparis-sistemi/
├── main.c           # Tüm kaynak kodu (veri yapıları + iş mantığı)
├── data.txt         # Menü verilerinin kalıcı depolandığı dosya
├── siparis.txt      # Sipariş verilerinin kalıcı depolandığı dosya
├── Makefile         # Cross-platform derleme dosyası
└── README.md        # Proje dokümantasyonu
```

---

## 🚀 Kurulum ve Çalıştırma

### Ön Gereksinimler

- GCC derleyici (`gcc --version` ile kontrol edebilirsiniz)
- Make (opsiyonel)

### Linux / macOS

```bash
# Repoyu klonla
git clone https://github.com/KULLANICI_ADINIZ/yemek-siparis-sistemi.git
cd yemek-siparis-sistemi

# Derle ve çalıştır
make run
```

### Manuel Derleme

```bash
gcc -Wall -Wextra -std=c99 -o yemek_sistemi main.c
./yemek_sistemi
```

### Windows (MinGW)

```bash
gcc -o yemek_sistemi.exe main.c
yemek_sistemi.exe
```

---

## 🖥️ Kullanım

Program başlatıldığında aşağıdaki ana menü görüntülenir:

```
Yemek Siparis ve Dagitim Sistemi
1. Menuye Yemek Ekle
2. Yemekleri Listele
3. Menuden Yemek Sil
4. Yeni Siparis Ekle
5. Siparisleri Listele
6. Siparisi Isle
7. Siparis Sil
8. Gelirleri Goruntule
9. Cikis ve Verileri Kaydet
```

### Örnek Akış

1. **Menüye yemek ekle** (Seçenek 1) → ID, isim, fiyat ve hazırlama süresi girin
2. **Sipariş oluştur** (Seçenek 4) → Menü ID'si, müşteri adı, telefon ve adres girin
3. **Siparişi işle** (Seçenek 6) → Kuyruğun başındaki sipariş işlenerek gelir kaydına eklenir
4. **Gelirleri görüntüle** (Seçenek 8) → Tüm gelir kayıtlarını ve toplam tutarı görün
5. **Çıkış ve kaydet** (Seçenek 9) → Veriler dosyalara kaydedilir

---

## 🔧 Teknik Detaylar

### Veri Yapısı Implementasyonu

#### Bağlı Liste — Menü Yönetimi
```c
typedef struct menuOgesi {
    int id;
    char isim[50];
    float fiyat;
    int hazirlanmaSuresi;
    struct menuOgesi *sonraki;  // Sonraki düğüme işaretçi
} menuOgesi;
```

#### Kuyruk — Sipariş Yönetimi
```c
typedef struct siparis {
    int siparisId;
    int menuOgesiId;
    char kullaniciAdi[50];
    char kullaniciTelefonu[15];
    char kullaniciAdresi[100];
    struct siparis *sonraki;
} siparis;

siparis *siparisKuyruguBasi = NULL;  // Kuyruğun başı (dequeue noktası)
siparis *siparisKuyruguSonu = NULL;  // Kuyruğun sonu (enqueue noktası)
```

#### Yığın — Gelir Kayıtları
```c
typedef struct gelir {
    int siparisId;
    float miktar;
    char tarih[11];
    struct gelir *sonraki;
} gelir;

gelir *gelirYigini = NULL;  // Yığının tepesi
```

### Dosya I/O
- Menü verileri `data.txt` dosyasına `M,id,isim,fiyat,sure` formatında yazılır
- Sipariş verileri `siparis.txt` dosyasına CSV formatında yazılır
- Program başlangıcında her iki dosya da belleğe yüklenir

---

## 📚 Öğrenme Kazanımları

Bu proje geliştirilirken aşağıdaki konular pekiştirilmiştir:

- ✅ Dinamik bellek yönetimi (`malloc`, `free`)
- ✅ İşaretçi (pointer) kullanımı ve pointer aritmetiği
- ✅ Struct tabanlı veri modelleme
- ✅ Bağlı liste, kuyruk ve yığın veri yapılarının uygulanması
- ✅ Dosya I/O işlemleri (`fopen`, `fclose`, `fprintf`, `fscanf`, `fgets`)
- ✅ Bellek sızıntısı (memory leak) önleme

---

## 🤝 Katkıda Bulunma

1. Bu repoyu fork edin
2. Feature branch oluşturun (`git checkout -b feature/yeni-ozellik`)
3. Değişikliklerinizi commit edin (`git commit -m 'feat: yeni özellik eklendi'`)
4. Branch'inizi push edin (`git push origin feature/yeni-ozellik`)
5. Pull Request açın

---

## 📄 Lisans

Bu proje [MIT Lisansı](LICENSE) kapsamında lisanslanmıştır.

---

<div align="center">

**⭐ Projeyi beğendiyseniz yıldız vermeyi unutmayın!**

Geliştirici: **Miraç Keser**

</div>
