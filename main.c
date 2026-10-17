#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct Definitions

typedef struct menuOgesi //liste
{
    int id;
    char isim[50];
    float fiyat;
    int hazirlanmaSuresi;
    struct menuOgesi *sonraki;  //baðlantýlý listedeki sonraki elemaný tutmak için kullanýlýr.
} menuOgesi;

typedef struct siparis //kuyruk
{
    int siparisId;
    int menuOgesiId;
    char kullaniciAdi[50];
    char kullaniciTelefonu[15];
    char kullaniciAdresi[100];
    struct siparis *sonraki;
} siparis;

typedef struct gelir      //Geliri tutar.Ayný zamanda gelen sipariþle ilgili bilgileri de tutar. (Yýðýn)
{
    int siparisId;
    float miktar; 
    char tarih[11];
    struct gelir *sonraki;
} gelir;


menuOgesi *menuBasi = NULL; //menudeki ilk sipariþi 0.eleman olarak atadýk.
siparis *siparisKuyruguBasi = NULL; //baþlangýçta hiç sipariþ yok.
siparis *siparisKuyruguSonu = NULL;  //sipariþ menusunun son elemanýný tutar.
gelir *gelirYigini = NULL;    //miktarlerin sýralý þekilde tutulduðu yýðýndýr.



void menuOgesiEkle(); //kullanýcýdan yemek bilgilerini alýr ve menü listesine ekler.
void menuOgesiniListele();  //menudeki yemekleri listeler.
void menuOgesiniSil();   //menuden yemek siler.
void siparisEkle(); //yeni yemek listesi ekler.
void siparisleriListele();   //mevcut menuleri listeler.
void siparisIslenmesi(); //sipariþleri iþleme alýr.Sipraiþ iþlendikten sonra kuyruktan çýkarýlýr.
void gelirEkle(int siparisId, float miktar, const char *tarih); //bir sipariþten elde edilen miktari ekler. Sipariþ ID'sini, miktari ve tarihi alýr ve bu miktar bilgilerini miktar yýðýnýna ekler.
void gelirleriGoruntule();  //tüm miktar kaynaklarýný görüntüler.
void menuyuDosyayaKaydet(); //menudeki yemek bilgilerini dosyaya kaydeder.Tüm yemek bilgilerini(id,ad,fiyat..) dosyaya yazar.
void siparisleriDosyayaKaydet();  //mevcut sipariþ bilgilerini dosyaya yazar. (sipariþ ID, yemek ID, müþteri adý, telefon, adres vb.) 
void dosyadanYemekleriYukle();   //bir dosyadan yemek verilerini yükler.
void dosyadansiparisleriYukle(); //bir dosyadan sipariþ verilerini yükler.
void siparisiSil();   //sipariþ kuyruðundan sipariþ silmek için kullanýlýr.

// Main Menu

int main()
{
   // dosyadanYemekleriYukle();
   // dosyadansiparisleriYukle();
    int secenek;
    while (1)
    {
        printf("\nYemek Siparis ve Dagitim Sistemi\n");
        printf("1. Menuye Yemek Ekle\n");
        printf("2. Yemekleri Listele\n");
        printf("3. Menuden Yemek Sil\n");
        printf("4. Yeni Siparis Ekle\n");
        printf("5. Siparisleri Listele\n");
        printf("6. Siparisi Isle\n");
        printf("7. Siparis Sil\n");
        printf("8. miktarleri Goruntule\n");
        printf("9. Cikis ve Verileri Kaydet\n");
        printf("Seciminiz: ");
        scanf("%d", &secenek);
        getchar();

        switch (secenek)
        {
        case 1:
            menuOgesiEkle();
            break;
        case 2:
            menuOgesiniListele();
            break;
        case 3:
            menuOgesiniSil();
            break;
        case 4:
            siparisEkle();
            break;
        case 5:
            siparisleriListele();
            break;
        case 6:
            siparisIslenmesi();
            break;
        case 7:
            siparisiSil();
            break;
        case 8:
            gelirleriGoruntule();
            break;
        case 9:
            menuyuDosyayaKaydet();
            siparisleriDosyayaKaydet();
            exit(0);
        default:
            printf("Gecersiz secim. Tekrar deneyin.\n");
        }
    }
    return 0;
}



void menuOgesiEkle()
{
   
    int newId;
    printf("Yemek ID: ");
    scanf("%d", &newId);

    menuOgesi *simdiki = menuBasi;
    while (simdiki)
    {
        if (simdiki->id == newId)
        {
            printf("Bu ID'ye sahip bir yemek zaten mevcut.Baska id ile deneyiniz.\n");
            return;
        }
        simdiki = simdiki->sonraki;
    }

    menuOgesi *yeniOge = (menuOgesi *)malloc(sizeof(menuOgesi)); //yeni yemek için bellekten yer ayýrýr.
    yeniOge->id = newId;
    getchar();  //scanf'in býraktýðý boþ satýrlarý siler.

    printf("Yemek Adi: ");
    fgets(yeniOge->isim, 50, stdin);   //dosyadan ya da kullanýcýdan giriþ almak için kuallnýlýr.

    
    if (yeniOge->isim[strlen(yeniOge->isim) - 1] == '\n') { //fgetsle alýnan kullanýcý verisinde fgets satýr sonuna \n ekler ama string dizilerinde karakterlerin sonunda \0 olamlsý gerektiði için bu iþlem uygulanýr.
        yeniOge->isim[strlen(yeniOge->isim) - 1] = '\0';
    }

    printf("Fiyat: ");
    scanf("%f", &yeniOge->fiyat);
    printf("Hazirlama Suresi (dakika): ");
    scanf("%d", &yeniOge->hazirlanmaSuresi);
    yeniOge->sonraki = menuBasi; 
    menuBasi = yeniOge;

    
    FILE *file = fopen("data.txt", "a"); //a, dosyayý ekleme formatýnda açar.
    if (file)
    {
        fprintf(file, "M,%d,%s,%.2f,%d\n", yeniOge->id, yeniOge->isim, yeniOge->fiyat, yeniOge->hazirlanmaSuresi);   //dosyaya yazar bunlarý tek sýra halinde.
        fclose(file);
        printf("Yemek bilgileri dosyaya basarili sekilde yazildi.\n");
    }
    else
    {
        printf("Dosya acilamadi. Yemek bilgileri kaydedilemedi.\n"); //hata mesajý.
    }

    printf("Yemek menusu dosyaya eklendi.\n");
}

void menuOgesiniListele()
{
    if (!menuBasi) //menu dolu mu boþ mu kontrol eder boþ ise hata mesajý verir.
    {
        printf("Menu bostur.Ekleme yapabilirsiniz.\n");
        return;
    }
    menuOgesi *simdiki = menuBasi;  //þimdiki deðeri en baþtakine eþitledik bu sayede menu içinde dolaþýlýr.
    while (simdiki)
    {
        printf("ID: %d, Ad: %s, Fiyat: %.2f, Hazirlama Suresi: %d dakika\n",simdiki->id, simdiki->isim, simdiki->fiyat, simdiki->hazirlanmaSuresi);
        simdiki = simdiki->sonraki;  //simdiki iþlemi bittikten sonra bir sonrakine atama yapýlýr ve bu iþlem en sona gelene kadar devam eder.
    }
}

void menuOgesiniSil()
{
    if (!menuBasi) //menude eleman var mý yoksa mesaj yazdýr.
    {
        printf("Menu bostur.Silebilmek icin once ekleme yapiniz\n");
        return;
    }
    int id;
    printf("Silinecek Yemek ID: "); //silinecek yemeðin bilgisi alýnýr.
    scanf("%d", &id);
    menuOgesi *simdiki = menuBasi, *onceki = NULL; //iki iþaretçi oluþturulur. ilki baþý tutarken ikincisi bir öncekini tutar. ilki baþý tuttuðu için öncekisini NULL atýyoruz.
    while (simdiki && simdiki->id != id)  //þu anki id ile girilen id uyuþuyor mu diye kontrol edilir uyuþtuðu sürece iþlemler yapýlýr.
    {
        onceki = simdiki; //önceki þimdikini iþaret etsin.
        simdiki = simdiki->sonraki; //þimdiki sonrakini iþaret etsin.
    }
    if (!simdiki) //simdiki NULL ise eþleþme bulunamadý.
    {
        printf("Girilen id'ye ait yemek bulunamadi.\n");
        return;
    }
    if (!onceki)   //onceki NULL ise simdiki ilk ögeyi iþaret ediyordur.
    {
        menuBasi = simdiki->sonraki; //öge menü baþýnda olduðu için bir sonrakine atama yapýlýr.
    }
    else
    {
        onceki->sonraki = simdiki->sonraki;  //silinen öge menunun sonunda veya ortasýndaysa yapýlýr.
    }
    free(simdiki); //simdiki sil demek.
    printf("Yemek silindi.\n");

    // Yemek bilgilerini data.txt dosyasinda guncelle
    FILE *file = fopen("data.txt", "w");  //dosya var ise w türünde yazýlýr.
    if (file)   
    {
        menuOgesi *temp = menuBasi; //menudeki ögeleri dosyaya sýrayla yazdýrmak için geçiçi temp kullanýlýr.
        while (temp) //menudeki ögeleri yazdýrmak için döngü baþlatýlýr.
        {
            fprintf(file, "M,%d,%s,%.2f,%d\n",temp->id, temp->isim, temp->fiyat, temp->hazirlanmaSuresi);     //file içine bunlarý yaz.
            temp = temp->sonraki;
        }
        fclose(file);
        printf("Dosya basariyla guncellendi.\n");
    }
    else
    {
        printf("Uzgunuz,Dosya guncellenemedi.\n");
    }
}
void siparisEkle()
{
    int mevcutSiparisId; 
    printf("Siparis ID: ");
    scanf("%d", &mevcutSiparisId);

    siparis *simdiki = siparisKuyruguBasi; // Yeni sipariþ eklemeden önce mevcut sipariþlerin olup olmadýðýný kontrol etmek için kuyruðu gezeriz.
    while (simdiki) // Kuyruk doluysa gezinmeye devam edilir.
    {
        if (simdiki->siparisId == mevcutSiparisId) // Eðer ayný id varsa kullanýcýya bu sipariþin daha önceden eklendiði bildirilir.
        {
            printf("Bu ID'ye sahip bir siparis zaten eklenmis.\n");
            return;
        }
        simdiki = simdiki->sonraki; // Kuyrukta dolaþmak için bir sonraki elemana geçiþ yapýlýr.
    }

    int yemekSayisi; // Kullanýcýdan kaç yemek gireceðini almak için bir deðiþken
    printf("Kac yemek gireceksiniz? ");
    scanf("%d", &yemekSayisi);
    int i;

    // Sipariþ bilgilerini almak için döngü
    for (i = 0; i < yemekSayisi; i++)
    {
        siparis *yeniSiparis = (siparis *)malloc(sizeof(siparis)); // Yeni sipariþ için bellekten yer ayrýldý.
        yeniSiparis->siparisId = mevcutSiparisId; // Yeni oluþturulan sipariþin ID'si, kullanýcý tarafýndan girilen mevcutSiparisId ile eþitlenir.
        printf("Yemek %d ID: ", i + 1);
        scanf("%d", &yeniSiparis->menuOgesiId);
        getchar(); // Boþluk karakterlerini temizlemek için

        printf("Musteri Adi: ");
        fgets(yeniSiparis->kullaniciAdi, 50, stdin);
        // '\n' karakterini temizlemek
        if (yeniSiparis->kullaniciAdi[strlen(yeniSiparis->kullaniciAdi) - 1] == '\n') {
            yeniSiparis->kullaniciAdi[strlen(yeniSiparis->kullaniciAdi) - 1] = '\0';
        }

        printf("Musteri Telefonu: ");
        fgets(yeniSiparis->kullaniciTelefonu, 15, stdin);
        // '\n' karakterini temizlemek
        if (yeniSiparis->kullaniciTelefonu[strlen(yeniSiparis->kullaniciTelefonu) - 1] == '\n') {
            yeniSiparis->kullaniciTelefonu[strlen(yeniSiparis->kullaniciTelefonu) - 1] = '\0';
        }

        printf("Musteri Adresi: ");
        fgets(yeniSiparis->kullaniciAdresi, 100, stdin);
        // '\n' karakterini temizlemek
        if (yeniSiparis->kullaniciAdresi[strlen(yeniSiparis->kullaniciAdresi) - 1] == '\n') {
            yeniSiparis->kullaniciAdresi[strlen(yeniSiparis->kullaniciAdresi) - 1] = '\0';
        }
        yeniSiparis->sonraki = NULL;

        if (!siparisKuyruguSonu) // Eðer kuyruk boþsa yeni sipariþ hem kuyruðun baþý hem de sonu olur.
        {
            siparisKuyruguBasi = siparisKuyruguSonu = yeniSiparis; // Kuyruk boþsa yeni sipariþe yönlendirilir.
        }
        else
        {
            siparisKuyruguSonu->sonraki = yeniSiparis; // Kuyruk boþ deðilse mevcut kuyruðun sonuna yeni sipariþ eklenir.
            siparisKuyruguSonu = yeniSiparis; // Kuyruk sonu iþaretçisi olarak güncellenir.
        }
        printf("Yemek %d eklendi.\n", i + 1);

        // Sipariþi siparis.txt'ye kaydet
        FILE *file = fopen("siparis.txt", "a"); // Dosya yoksa oluþturulur, mevcut içerik korunur ve dosya sonuna eklenir.
        if (file)
        {
            fprintf(file, "%d,%d,%s,%s,%s\n", yeniSiparis->siparisId, yeniSiparis->menuOgesiId, yeniSiparis->kullaniciAdi, yeniSiparis->kullaniciTelefonu, yeniSiparis->kullaniciAdresi);
            fclose(file);
        }
        else
        {
            printf("Siparis dosyaya kaydedilemedi!\n");
        }
    }
}

void siparisleriListele() //sipariþlerin sýralanacaðý fonksiyon.
{
    if (!siparisKuyruguBasi)   //kuyruk listesi boþsa 
    {
        printf("Siparis listesi bos.\n");
        return;
    }
    siparis *simdiki = siparisKuyruguBasi;   //kuyruktaki ilk sipariþi tutar.
    while (simdiki)  //her bir sipariþi geçerken döner ve NULL olduðu zaman döngü sonlanýr.
    {
        printf("Siparis ID: %d, Yemek ID: %d, Musteri Adi: %s, Telefon: %s, Adres: %s\n",simdiki->siparisId, simdiki->menuOgesiId, simdiki->kullaniciAdi,simdiki->kullaniciTelefonu, simdiki->kullaniciAdresi);
       // printf("Teslimat gerceklesti.\n");
		simdiki = simdiki->sonraki;
    }
}

void siparisIslenmesi()
{
    if (!siparisKuyruguBasi) //kuyruðun boþ olup olmadýðýný kontrol eder.
    {
        printf("Islenecek siparis yok.\n");
        return;
    }
    siparis *simdiki = siparisKuyruguBasi;  //ilk sipariþi tutar.
    printf("Islenen Siparis ID: %d\n", simdiki->siparisId);  //iþlnecek olan sipariþ bilgisi alýnýr.
    float fiyat = 0;   //her sipariþ için farklý deðer olacaðý için her sipariþ için fiyat bilgisi 0'dan baþlar.
    menuOgesi *menusimdiki = menuBasi;   //menüdeki ilk öge.
    while (menusimdiki)
    {
        if (menusimdiki->id == simdiki->menuOgesiId)   //menu id ile menuOgesiId eþitse fiyat girilen fiyata eþitlenir.
        {
            fiyat = menusimdiki->fiyat;
            break;
        }
        menusimdiki = menusimdiki->sonraki;  //bir sonraki sipariþe geçilir.
    }
    gelirEkle(simdiki->siparisId, fiyat, "2025-01-08");    //sipariþ miktarini tutmak için kullanýlýr.
    siparisKuyruguBasi = siparisKuyruguBasi->sonraki;  //kuyruktaki ilk sipariþ çýkarýlýr ve bir sonraki iþaret edilir.
    if (!siparisKuyruguBasi)  //kuyruðun sonunda öge kalmýþ mý kontrolü edilir.
    {
        siparisKuyruguSonu = NULL;
    }
    free(simdiki);  //serbest býrakýlýr.
    siparisleriDosyayaKaydet();   //sipariþleri dosyaya kaydet fonksiyonu çalýþýr.
}

void siparisiSil()
{
    if (!siparisKuyruguBasi)  //kuyruk boþ mu ona bakar ve eðer boþsa hata mesajý verir.
    {
        printf("Silinecek siparis yok.\n");
        return;
    }
    int id;
    printf("Silinecek Siparis ID: ");   //silinecek sipariþþin idsi alýnýr.
    scanf("%d", &id);

    siparis *simdiki = siparisKuyruguBasi, *onceki = NULL;    //simdiki deðiþkenini ilk ögeye atadýk öncekisi NULL oldu.
    while (simdiki && simdiki->siparisId != id)   //her bir sipariþi gezer bu döngü eðer sipariþ id'si kullanýcýdan alýnan id'e eþit deðilse  
    {
        onceki = simdiki; //deðerler bir sonrakine geçer önceki þimdi olur
        simdiki = simdiki->sonraki; //þimdiki bir sonrakini iþaret eder.
    }
    if (!simdiki)   //simdiki NULL olmuþsa aranan sipariþ kuyruk içinde bulunamamýþtýr.
    {
        printf("Siparis bulunamadi.\n");
        return;
    }
    if (!onceki)  //eðer oncekiious deðeri NULL ise ilk sipariþ silinecektir.
    {
        siparisKuyruguBasi = simdiki->sonraki;  //silinen ilk sipariþten sonra kuyruk baþýndaki eleman bire sonrakine atanýr.
    }
    else   //eðer oncekiious deðeri NULL deðilse sipariþ ara bir sipariþtir ve aradan silinir.
    {
        onceki->sonraki = simdiki->sonraki;  //önceki deðer bir sonrakine þimdiki de kendinden sonraki deðeri göstermeye baþlar.
    }
    if (simdiki == siparisKuyruguSonu)    //kuyruðun son sipariþi silinmek istenirse 
    {
        siparisKuyruguSonu = onceki;   //kuyruk sonu sondan bir öncekini iþaret eder.
    }
    free(simdiki);   //bellekten silinen eleman serbest býrakýlýr.
    printf("Siparis silindi.\n");

    siparisleriDosyayaKaydet();     // Siparisleri siparis.txt dosyasinda guncelle
}

void gelirEkle(int siparisId, float miktar, const char *tarih)  //miktarin iliþkisi olduðu sipariþ id'si,fiyat,tarih parametrelerini alýr.
{
    gelir *yeniGelir = (gelir *)malloc(sizeof(gelir));   //miktar için bellketen yeni bir yer ayrýlýr.
    yeniGelir->siparisId = siparisId;  //yeni id tanýmlanýr.
    yeniGelir->miktar = miktar;    //yeni miktar tanýmlanýr.
    strcpy(yeniGelir->tarih, tarih); //fonksiyona gönderilen tarih stringi yeni miktar kaydýnýn tarih stringine kopyalanýr.
    yeniGelir->sonraki = gelirYigini;  //yeni miktar kaydýný mevcut miktarler stackine ekler.
    gelirYigini = yeniGelir;    //gelirYigini listenin baþýný tuttuðu için yeniGelir artýk listenin baþýný tutacak elemaný gösterir.
    printf("miktar eklendi: %.2f\n", miktar);
}

void gelirleriGoruntule()
{
    if (!gelirYigini) //miktar kaydý boþsa hata mesajý ver.
    {
        printf("miktar kaydi yok.\n");
        return;
    }
    gelir *simdiki = gelirYigini;  //simdiki þu anki miktar kaydýný temsil eder ve ilk baþta gelirYigini'ý iþaret eder. 
    float toplam = 0; //her miktar kaydý buna eklenerek toplam miktar hesaplanacak.
    while (simdiki)  //simdiki herhangi bir ögeyi iþsaret ettiði sürece çalýþýr.
    {
        printf("Siparis ID: %d, Tutar: %.2f, Tarih: %s\n",simdiki->siparisId, simdiki->miktar, simdiki->tarih); //sipariþ bilgileri alýnýr.
        toplam += simdiki->miktar;  //gelen sipariþ miktari toplam miktare eklenir.
        simdiki = simdiki->sonraki; //bir sonraki sipariþe geçilir.
    }
    printf("Toplam miktar: %.2f\n", toplam);
}

void menuyuDosyayaKaydet()
{
    FILE *file = fopen("data.txt", "w"); //fopen dosya yoksa yaratýr, w ise dosyanýn yazma modunda açýlacaðýný belirtir.
    if (!file) //fopen dosyayý açamazsa yani dosya NULL deðer alýrsa hata mesajý verir.
    {
        printf("Dosya kaydedilemedi.\n");
        return;
    }
    menuOgesi *menusimdiki = menuBasi; //simdiki deðeri baþý iþsaret eder ki döngü baþtan baþlasýn.
    while (menusimdiki) //döngüde simdiki bir deðeri iþaret ettiði sürece devam eder.
    {
        fprintf(file, "M,%d,%s,%.2f,%d\n",menusimdiki->id, menusimdiki->isim, menusimdiki->fiyat, menusimdiki->hazirlanmaSuresi);
        menusimdiki = menusimdiki->sonraki;  //bir öge bilgileri yazýldýktan sonra bir sonrakine geçer.
    }
    fclose(file); //dosyayý kapatýr.
    printf("Veriler kaydedildi.\n");
}

void siparisleriDosyayaKaydet()  //sipariþleri dosyaya kaydet
{
    FILE *file = fopen("siparis.txt", "w"); //dosya fopen ile açýlýr ve yazma modunda açýlýr baþka bir þeyler yazýyorsa silip üstüne bir daha yazar.
    if (!file) //dosya açýlamazsa NULL deðer döner ve hiçbir þey kaydedilemez.
    {
        printf("Siparis dosyasi kaydedilemedi.\n");
        return;
    }
    siparis *simdikiSiparis = siparisKuyruguBasi;   //sipariþin þu anki deðeri ilk deðere atanýr.
    while (simdikiSiparis)  //þimdiki deðer herhangi bir deðeri iþaret ediyorsa döngü dönmeye devam eder.
    {
        fprintf(file, "%d,%d,%s,%s,%s\n",simdikiSiparis->siparisId, simdikiSiparis->menuOgesiId, simdikiSiparis->kullaniciAdi,simdikiSiparis->kullaniciTelefonu, simdikiSiparis->kullaniciAdresi);
        simdikiSiparis = simdikiSiparis->sonraki;   //dosyaya yazýlan elemandan sonra bir sonrakinin bilgileri yazýlýr.
    }
    fclose(file);
    printf("Siparisler kaydedildi.\n");
}

void dosyadanYemekleriYukle()    //dosyadaki verileri okur.
{
    FILE *file = fopen("data.txt", "r");  //açýlan dosyayý read modunda açar yani bu okuma modudur.
    if (!file)  //dosya açýlamazsa hata mesajý verir.
    {
        printf("Veriler yuklenemedi. Yeni bir sistem baslatiliyor.\n");
        return;
    }
    char line[200];   //her satýrda okunacak olan veriyi geçici bir diziye atamak için bir dizi oluþturuyoruz.
    while (fgets(line, 200, file))  //dosyadan satýr satýr veri okur. en fazla 200 karakter okuyarak line dizisine atama yapar.
    {
        if (line[0] == 'M') //baþ karakteri M ise menu ögesi ile baþlýyordur 
        {
            menuOgesi *yeniOge = (menuOgesi *)malloc(sizeof(menuOgesi));    //menu ögesine yeni bellek tahsisi yapýlýr.
            sscanf(line, "M,%d,%49[^,],%f,%d", &yeniOge->id, yeniOge->isim, &yeniOge->fiyat, &yeniOge->hazirlanmaSuresi);
            yeniOge->sonraki = menuBasi;      //sscanf kullanýlmasý verilerin ayrýþtýrýlmasý için önemlidir.Büyük kolaylýk saðlar.
            menuBasi = yeniOge; //yeni eklenen menuyu iþaret eder.
        }
    }
    fclose(file);
    printf("Veriler yuklendi.\n");
}
void dosyadansiparisleriYukle()
{
    FILE *file = fopen("siparis.txt", "r");
    if (!file)
    {
        printf("Siparis verileri yuklenemedi. Yeni siparis listesi baslatiliyor.\n");
        return;
    }
    char line[200];
    while (fgets(line, 200, file))
    {
        siparis *yeniSiparis = (siparis *)malloc(sizeof(siparis));
        char *start = line;  // Hangi kýsmý okuduðumuzu tutacak.

        // siparisId
        char *end = strchr(start, ',');  // Ýlk ',' karakterini bul.
        *end = '\0';                     // ',' yerine null karakter koyarak parçala.
        yeniSiparis->siparisId = atoi(start); // Parçayý integer'a çevir.
        start = end + 1;                 // Sýradaki kýsmýn baþlangýcýný ayarla.

        // menuOgesiId
        end = strchr(start, ',');
        *end = '\0';
        yeniSiparis->menuOgesiId = atoi(start);
        start = end + 1;

        // kullaniciAdi
        end = strchr(start, ',');
        *end = '\0';
        strcpy(yeniSiparis->kullaniciAdi, start);
        start = end + 1;

        // kullaniciTelefonu
        end = strchr(start, ',');
        *end = '\0';
        strcpy(yeniSiparis->kullaniciTelefonu, start);
        start = end + 1;

        // kullaniciAdresi
        end = strchr(start, '\n'); // Son kýsmý bul (newline).
        if (end)
            *end = '\0'; // '\n' yerine null koy.
        strcpy(yeniSiparis->kullaniciAdresi, start); //string ifade olduðu için strcpy ile kopyalama iþlemi yapýlýr.

        yeniSiparis->sonraki = NULL;
        if (!siparisKuyruguSonu)
        {
            siparisKuyruguBasi = siparisKuyruguSonu = yeniSiparis;
        }
        else
        {
            siparisKuyruguSonu->sonraki = yeniSiparis;
            siparisKuyruguSonu = yeniSiparis;
        }
    }
    fclose(file);
    printf("Siparis verileri yuklendi.\n");
}

