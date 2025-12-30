#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <conio.h> 

using namespace std;

struct Cihaz {
    char barkod[20];
    char model[50];
    char marka[30];
    double satisFiyati;
    int stokAdedi;
};

const char* VERITABANI = "magaza_db.dat";

void baslikYazdir();
void beklet();
void yeniCihazGiris();
void envanteriListele();
void urunAra();         
void satisYap(); 
void kritikStokAnalizi();
void urunGuncelle();
void kayitSil();

int main() {
    char secim;
    while (true) {
        system("cls"); 
        baslikYazdir();
        
        cout << " [1] Yeni Cihaz Girisi" << endl;
        cout << " [2] Envanter Listesi" << endl;
        cout << " [3] Urun Ara" << endl;          
        cout << " [4] Satis Yap (Stok Dus)" << endl; 
        cout << " [5] Kritik Stok Raporu (!)" << endl;
        cout << " [6] Urun Guncelle" << endl;
        cout << " [7] Cihaz Kaydi Sil" << endl;
        cout << " [0] Cikis" << endl;
        cout << "------------------------------------------" << endl;
        cout << " Islem Seciniz: ";
        
        secim = getche(); 

        switch (secim) {
            case '1': yeniCihazGiris(); break;
            case '2': envanteriListele(); break;
            case '3': urunAra(); break;           
            case '4': satisYap(); break;
            case '5': kritikStokAnalizi(); break;
            case '6': urunGuncelle(); break;
            case '7': kayitSil(); break;
            case '0': 
                cout << "\n Sistem kapatiliyor..." << endl;
                return 0;
            default:
                cout << "\n Gecersiz secim!" << endl;
                beklet();
        }
    }
}

void baslikYazdir() {
    cout << endl;
    cout << "==========================================" << endl;
    cout << "      MAGAZA STOK YONETIM SISTEMI     " << endl;
    cout << "==========================================" << endl;
    cout << endl;
}

void beklet() {
    cout << "\n Devam etmek icin bir tusa basin..." ;
    getch(); 
}

void yeniCihazGiris() {
    system("cls");
    baslikYazdir();
    cout << " >> YENI CIHAZ EKLEME MODULU <<" << endl << endl;

    Cihaz yeni;
    
    cout << " Barkod No      : "; cin >> yeni.barkod;
    
    cin.ignore(); 
    cout << " Marka          : "; cin.getline(yeni.marka, 30);
    cout << " Model          : "; cin.getline(yeni.model, 50);
    
    cout << " Satis Fiyati   : "; cin >> yeni.satisFiyati;
    cout << " Stok Adedi     : "; cin >> yeni.stokAdedi;

    ofstream dosya(VERITABANI, ios::binary | ios::app);
    if(dosya.write((char*)&yeni, sizeof(Cihaz))) {
        cout << endl << " [+] Kayit basariyla veritabanina islendi." << endl;
    } else {
        cout << endl << " [!] Dosya hatasi!" << endl;
    }
    dosya.close();
    beklet();
}

void envanteriListele() {
    system("cls");
    baslikYazdir();
    
    ifstream dosya(VERITABANI, ios::binary);
    if (!dosya) {
        cout << " Veritabani bulunamadi veya bos!" << endl;
        beklet();
        return;
    }

    Cihaz temp;
    cout << left << setw(15) << "BARKOD" 
         << left << setw(15) << "MARKA" 
         << left << setw(20) << "MODEL" 
         << left << setw(10) << "FIYAT" 
         << left << setw(10) << "STOK" << endl;
    cout << "----------------------------------------------------------------------" << endl;

    int sayac = 0;
    while(dosya.read((char*)&temp, sizeof(Cihaz))) {
        cout << left << setw(15) << temp.barkod 
             << left << setw(15) << temp.marka 
             << left << setw(20) << temp.model 
             << left << setw(10) << temp.satisFiyati 
             << left << setw(10) << temp.stokAdedi << endl;
        sayac++;
    }
    dosya.close();

    cout << endl << " Toplam " << sayac << " adet urun listelendi." << endl;
    beklet();
}

void urunAra() {
    system("cls");
    baslikYazdir();
    cout << " >> URUN ARAMA MODULU <<" << endl << endl;

    char aranan[50];
    cout << " Aranacak Kelime (Barkod/Marka/Model): ";
    cin >> aranan;

    ifstream dosya(VERITABANI, ios::binary);
    if (!dosya) {
        cout << " Veritabani bulunamadi!" << endl;
        beklet();
        return;
    }

    Cihaz temp;
    int bulunanSayisi = 0;
    
    cout << endl << " ARAMA SONUCLARI:" << endl;
    cout << "----------------------------------------------------------------------" << endl;
    cout << left << setw(15) << "BARKOD" 
         << left << setw(15) << "MARKA" 
         << left << setw(20) << "MODEL" 
         << left << setw(10) << "FIYAT" 
         << left << setw(10) << "STOK" << endl;
    cout << "----------------------------------------------------------------------" << endl;

    while(dosya.read((char*)&temp, sizeof(Cihaz))) {
        if (strstr(temp.barkod, aranan) || 
            strstr(temp.marka, aranan) || 
            strstr(temp.model, aranan)) {
            
            cout << left << setw(15) << temp.barkod 
                 << left << setw(15) << temp.marka 
                 << left << setw(20) << temp.model 
                 << left << setw(10) << temp.satisFiyati 
                 << left << setw(10) << temp.stokAdedi << endl;
            bulunanSayisi++;
        }
    }
    dosya.close();

    if(bulunanSayisi == 0) {
        cout << endl << " [!] Aradiginiz kriterlere uygun urun bulunamadi." << endl;
    } else {
        cout << endl << " Toplam " << bulunanSayisi << " kayit bulundu." << endl;
    }
    beklet();
}

void satisYap() {
    system("cls");
    baslikYazdir();
    cout << " >> SATIS ISLEMI (STOK DUS) <<" << endl;

    char arananBarkod[20];
    cout << " Satilacak Urun Barkodu: ";
    cin >> arananBarkod;

    fstream dosya(VERITABANI, ios::binary | ios::in | ios::out);
    Cihaz temp;
    bool bulundu = false;

    if (!dosya) {
        cout << " Veritabani acilamadi!" << endl;
        beklet();
        return;
    }

    while(dosya.read((char*)&temp, sizeof(Cihaz))) {
        if(strcmp(temp.barkod, arananBarkod) == 0) {
            bulundu = true;
            cout << endl << " Urun Bulundu: " << temp.marka << " " << temp.model << endl;
            cout << " Mevcut Stok: " << temp.stokAdedi << endl;

            if(temp.stokAdedi > 0) {
                int satisAdedi;
                cout << " Kac adet satiliyor? : ";
                cin >> satisAdedi;

                if (satisAdedi <= temp.stokAdedi) {
                    temp.stokAdedi -= satisAdedi;
                    
                    long long pos = -1 * static_cast<long long>(sizeof(Cihaz));
                    dosya.seekp(pos, ios::cur);
                    
                    dosya.write((char*)&temp, sizeof(Cihaz));
                    
                    double toplamTutar = satisAdedi * temp.satisFiyati;
                    cout << endl << " [OK] Satis onaylandi." << endl;
                    cout << " Toplam Tutar: " << toplamTutar << " TL" << endl;
                    cout << " Kalan Stok: " << temp.stokAdedi << endl;
                } else {
                    cout << endl << " [!] Yetersiz Stok!" << endl;
                }
            } else {
                cout << endl << " [!] Bu urun stokta tukenmis!" << endl;
            }
            break; 
        }
    }
    dosya.close();
    
    if(!bulundu) cout << endl << " [!] Barkod bulunamadi." << endl;
    beklet();
}

void kritikStokAnalizi() {
    system("cls");
    baslikYazdir();
    cout << " >> KRITIK STOK RAPORU (Stok < 5) <<" << endl << endl;

    ifstream dosya(VERITABANI, ios::binary);
    if (!dosya) {
        cout << " Veritabani henuz olusturulmadi." << endl;
        beklet();
        return;
    }

    Cihaz temp;
    int riskliUrunSayisi = 0;

    cout << left << setw(20) << "URUN" << "STOK DURUMU" << endl;
    cout << "--------------------------------" << endl;

    while(dosya.read((char*)&temp, sizeof(Cihaz))) {
        if(temp.stokAdedi < 5) {
            cout << left << setw(20) << temp.model << temp.stokAdedi << " adet kaldi!" << endl;
            riskliUrunSayisi++;
        }
    }
    dosya.close();

    if(riskliUrunSayisi == 0) {
        cout << " Harika! Kritik seviyede urun yok." << endl;
    } else {
        cout << endl << " Toplam " << riskliUrunSayisi << " urun icin siparis verilmeli." << endl;
    }
    beklet();
}

void urunGuncelle() {
    system("cls");
    baslikYazdir();
    cout << " >> URUN GUNCELLEME MODULU <<" << endl << endl;

    char guncellenecekBarkod[20];
    cout << " Guncellenecek Urun Barkodu: ";
    cin >> guncellenecekBarkod;

    fstream dosya(VERITABANI, ios::binary | ios::in | ios::out);
    Cihaz temp;
    bool bulundu = false;

    if (!dosya) {
        cout << " Veritabani acilamadi." << endl;
        beklet();
        return;
    }

    while(dosya.read((char*)&temp, sizeof(Cihaz))) {
        if(strcmp(temp.barkod, guncellenecekBarkod) == 0) {
            bulundu = true;
            cout << endl << " Mevcut Bilgiler:" << endl;
            cout << " Marka: " << temp.marka << ", Model: " << temp.model << endl;
            cout << " Fiyat: " << temp.satisFiyati << ", Stok: " << temp.stokAdedi << endl << endl;

            cout << " >> Yeni Bilgileri Giriniz <<" << endl;
            
            cin.ignore(); 
            cout << " Yeni Marka        : "; cin.getline(temp.marka, 30);
            cout << " Yeni Model        : "; cin.getline(temp.model, 50);
            cout << " Yeni Satis Fiyati : "; cin >> temp.satisFiyati;
            cout << " Yeni Stok Adedi   : "; cin >> temp.stokAdedi;

            long long pos = -1 * static_cast<long long>(sizeof(Cihaz));
            dosya.seekp(pos, ios::cur);
            
            dosya.write((char*)&temp, sizeof(Cihaz));
            
            cout << endl << " [+] Urun basariyla guncellendi." << endl;
            break;
        }
    }
    dosya.close();

    if(!bulundu) cout << endl << " [!] Barkod bulunamadi." << endl;
    beklet();
}

void kayitSil() {
    system("cls");
    baslikYazdir();
    
    char silinecekBarkod[20];
    cout << " Silinecek Urun Barkodu: ";
    cin >> silinecekBarkod;

    ifstream oku(VERITABANI, ios::binary);
    ofstream yedek("yedek.tmp", ios::binary);
    
    Cihaz temp;
    bool silindi = false;
    bool var = false;

    if(!oku) {
        cout << " Veritabani acilamadi." << endl;
        return;
    }

    while(oku.read((char*)&temp, sizeof(Cihaz))) {
        if(strcmp(temp.barkod, silinecekBarkod) != 0) {
            yedek.write((char*)&temp, sizeof(Cihaz));
        } else {
            var = true;
            cout << endl << " Urun Bulundu: " << temp.marka << " " << temp.model << endl;
            cout << " Silmek istediginize emin misiniz? (E/H): ";
            char onay = getche();
            
            if (onay == 'e' || onay == 'E') {
                silindi = true;
                cout << endl << " Kayit siliniyor..." << endl;
            } else {
                yedek.write((char*)&temp, sizeof(Cihaz));
            }
        }
    }

    oku.close();
    yedek.close();

    if (silindi) {
        remove(VERITABANI);
        rename("yedek.tmp", VERITABANI);
        cout << " Islem tamamlandi." << endl;
    } else {
        remove("yedek.tmp");
        if(var) cout << "\n Iptal edildi." << endl;
        else cout << "\n Barkod bulunamadi." << endl;
    }
    
    beklet();
}