#include <iostream>
#include <cstdio>
#include <cstring>
#include <conio.h>
using namespace std;

struct Mahasiswa {
    int nim;
    char nama[100];
    Mahasiswa* kiri;
    Mahasiswa* kanan;
};

Mahasiswa* akar = NULL;

void simpanMhs(Mahasiswa* node, FILE* file) {
    if (node != NULL) {
        fwrite(&(node->nim), sizeof(int), 1, file);
        fwrite(node->nama, sizeof(char), 100, file);
        simpanMhs(node->kiri, file);
        simpanMhs(node->kanan, file);
    }
}

void simpanKeFile() {
    FILE* file = fopen("data.dat", "wb");
    if (file != NULL) {
        simpanMhs(akar, file);
        fclose(file);
        cout << "Data berhasil disimpan ke file.\n";
    } else {
        cout << "Gagal menyimpan data ke file.\n";
    }
}

void tambah() {
    int nim;
    char nama[100];
    cout << "NIM: "; cin >> nim; cin.ignore();
    cout << "Nama: "; cin.getline(nama, 100);

    Mahasiswa* baru = new Mahasiswa;
    baru->nim = nim;
    strcpy(baru->nama, nama);
    baru->kiri = baru->kanan = NULL;

    if (akar == NULL) {
        akar = baru;
        simpanKeFile();
        return;
    }

    Mahasiswa* sekarang = akar;
    while (true) {
        if (nim < sekarang->nim) {
            if (sekarang->kiri == NULL) {
                sekarang->kiri = baru;
                break;
            }
            sekarang = sekarang->kiri;
        } else if (nim > sekarang->nim) {
            if (sekarang->kanan == NULL) {
                sekarang->kanan = baru;
                break;
            }
            sekarang = sekarang->kanan;
        } else {
            cout << "NIM sudah ada, data tidak ditambahkan.\n";
            delete baru;
            return;
        }
    }

    simpanKeFile(); 
}


void cetakInorder(Mahasiswa* node) {
    if (node != NULL) {
        cetakInorder(node->kiri);
        cout << "NIM: " << node->nim << "\nNama: " << node->nama << "\n\n";
        cetakInorder(node->kanan);
    }
}

void cetak() {
    if (akar == NULL) {
        cout << "Data kosong.\n";
        return;
    }
    cetakInorder(akar);
}


void cari() {
    int nim;
    cout << "Masukkan NIM yang dicari: "; cin >> nim;
    Mahasiswa* sekarang = akar;
    while (sekarang != NULL) {
        if (nim == sekarang->nim) {
            cout << "Ditemukan: NIM = " << sekarang->nim << "\nNama = " << sekarang->nama << "\n";
            return;
        }
        sekarang = (nim < sekarang->nim) ? sekarang->kiri : sekarang->kanan;
    }
    cout << "Data tidak ditemukan.\n";
}

void hapus(int nim) {
    Mahasiswa* sekarang = akar;
    Mahasiswa* induk = NULL;

    while (sekarang != NULL && sekarang->nim != nim) {
        induk = sekarang;
        sekarang = (nim < sekarang->nim) ? sekarang->kiri : sekarang->kanan;
    }

    if (sekarang == NULL) {
        cout << "Data tidak ditemukan.\n";
        return;
    }

    //node tanpa anak
    if (sekarang->kiri == NULL && sekarang->kanan == NULL) {
        if (induk == NULL) akar = NULL;
        else if (induk->kiri == sekarang) induk->kiri = NULL;
        else induk->kanan = NULL;
    }
    //satu anak
    else if (sekarang->kiri == NULL || sekarang->kanan == NULL) {
        Mahasiswa* anak = (sekarang->kiri != NULL) ? sekarang->kiri : sekarang->kanan;
        if (induk == NULL) akar = anak;
        else if (induk->kiri == sekarang) induk->kiri = anak;
        else induk->kanan = anak;
    }
    //dua anak
    else {
        Mahasiswa* ganti = sekarang->kanan;
        Mahasiswa* indukGanti = sekarang;

        while (ganti->kiri != NULL) {
            indukGanti = ganti;
            ganti = ganti->kiri;
        }

        sekarang->nim = ganti->nim;
        strcpy(sekarang->nama, ganti->nama);

        if (indukGanti->kiri == ganti) indukGanti->kiri = ganti->kanan;
        else indukGanti->kanan = ganti->kanan;

        delete ganti;
        simpanKeFile();
        return;
    }

    delete sekarang;
    simpanKeFile();
}

void baca() {
    FILE* file = fopen("data.dat", "rb");
    if (!file) return;

    while (true) {
        int nim;
        char nama[100];
        if (fread(&nim, sizeof(int), 1, file) != 1) break;
        if (fread(nama, sizeof(char), 100, file) != 100) break;

        Mahasiswa* baru = new Mahasiswa;
        baru->nim = nim;
        strcpy(baru->nama, nama);
        baru->kiri = baru->kanan = NULL;

        if (akar == NULL) {
            akar = baru;
        } else {
            Mahasiswa* sekarang = akar;
            while (true) {
                if (nim < sekarang->nim) {
                    if (sekarang->kiri == NULL) {
                        sekarang->kiri = baru;
                        break;
                    }
                    sekarang = sekarang->kiri;
                } else if (nim > sekarang->nim) {
                    if (sekarang->kanan == NULL) {
                        sekarang->kanan = baru;
                        break;
                    }
                    sekarang = sekarang->kanan;
                } else {
                    delete baru;
                    break;
                }
            }
        }
    }

    fclose(file);
}


int main() {
    baca();
    int pilihan;

    while (true) {
        cout << "SILAHKAN";
        cout << "\n1. Tambah data";
        cout << "\n2. Cari data";
        cout << "\n3. Cetak data";
        cout << "\n4. Hapus data";
        cout << "\n5. Keluar";
        cout << "\nPilih: "; cin >> pilihan;
        cin.ignore();

        switch (pilihan) {
            case 1: tambah(); break;
            case 2: cari(); break;
            case 3: cetak(); break;
            case 4:
                int nim;
                cout << "Masukkan NIM yang ingin dihapus: "; cin >> nim;
                hapus(nim);
                break;
            case 5: cout << "Keluar...\n"; return 0;
            default: cout << "Pilihan tidak valid.\n";
        }

        cout << "\nTekan ENTER untuk lanjut...";
        cin.ignore();
        cin.get();
        system("cls");
    }
}