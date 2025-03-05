#include <iostream>
#include <algorithm>
using namespace std;

struct Buku
{
    string judul;
    int tahun;
};

const int maks_buku = 250;
Buku daftarbuku [maks_buku];
int jumlahbuku = 0;

void buku () 
{
    cout << "Jumlah buku : ";
    cin >> jumlahbuku;
    cin.ignore();
    for (int i = 0; i < jumlahbuku; i++) 
    {
        cout << "Judul Buku : ";
        getline(cin, daftarbuku[i].judul);
        cout << "Tahun Terbit Buku: ";
        cin >> daftarbuku[i].tahun;
        cin.ignore();
    }
}

void buku2 () 
{
    cout << "\nDaftar Buku :\n";
    for (int i = 0; i < jumlahbuku; i++) 
    {
        cout << daftarbuku[i].judul << " - " << daftarbuku[i].tahun << endl;
    }
}

void BubbleSort () 
{
    for (int i = 0; i < jumlahbuku - 1; i++) 
    {
        for (int j = 0; j < jumlahbuku - i - 1; j++) {
            if (daftarbuku[j].tahun > daftarbuku[j + 1].tahun) 
            {
                swap(daftarbuku[j], daftarbuku[j + 1]);
            }
        }
    }
}

void QuickSort (int awal, int akhir) 
{
    if (awal >= akhir) return;
    int pivot = daftarbuku[akhir].tahun;
    int i = awal - 1;
    for (int j = awal; j < akhir; j++) 
    {
        if (daftarbuku[j].tahun < pivot) {
            swap(daftarbuku[++i], daftarbuku[j]);
        }
    }
    swap(daftarbuku[i + 1], daftarbuku[akhir]);
    QuickSort (awal, i);
    QuickSort (i + 2, akhir);
}

void Sequential (int tahun) 
{
    bool ada = false;
    for (int i = 0; i < jumlahbuku; i++) 
    {
        if (daftarbuku[i].tahun == tahun) {
            cout << "Buku ditemukan : " << daftarbuku[i].judul << "-" << daftarbuku[i].tahun << endl;
            ada = true;
        }
    }
    if (!ada) cout << "Buku tidak ditemukan\n";
}

int BinarySearch (int tahun) 
{
    int kiri = 0, kanan = jumlahbuku - 1;
    while (kiri <= kanan) 
    {
        int mid = (kiri + kanan) / 2;
        if (daftarbuku[mid].tahun == tahun) return mid;
        else if (daftarbuku[mid].tahun < tahun) kiri = mid + 1;
        else kanan = mid - 1;
    }
    return -1;
}

int main () 
{
    int pilihan;
    do 
    {
        cout << "\nMenu:\n1. Input Data Buku\n2. Lihat Buku\n3. Urutkan Buku Dengan (Bubble Sort)\n4. Urutkan Buku Dengan (Quick Sort)\n5. Cari Buku Dengan (Sequential)\n6. Cari Buku Dengan (Binary)\n7. Keluar\nPilih Anda : ";
        cin >> pilihan;
        cin.ignore();
        switch (pilihan) 
        {
            case 1: buku (); break;
            case 2: buku2 (); break;
            case 3: BubbleSort (); 
            cout << "Berhasil mengurutkan data (Bubble Sort)\n"; break;
            case 4: QuickSort (0, jumlahbuku - 1); 
            cout << "Berhasil mengurtkan data (Quick Sort)\n"; break;
            case 5: 
            {
                int tahun;
                cout << "Cari tahun terbit : ";
                cin >> tahun;
                Sequential (tahun);
                break;
            }
            case 6: 
            {
                int tahun;
                cout << "Cari tahun terbit : ";
                cin >> tahun;
                QuickSort (0, jumlahbuku - 1);
                int indeks = BinarySearch (tahun);
                if (indeks != -1) cout << "Buku ditemukan : " << daftarbuku[indeks].judul << " - " << daftarbuku[indeks].tahun << endl;
                else cout << "Buku tidak ditemukan\n";
                break;
            }
            case 7: cout << "Program berhasil\n"; break;
            default: cout << "Pilihan tidak benar\n";
        }
    } while (pilihan != 7);
    return 0;
}