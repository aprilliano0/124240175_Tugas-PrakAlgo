#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
using namespace std;

const int maks_taks = 111; 

struct Task 
{
    string nama; string kategori; string deadline; string status;
};

int bacaTugasDariFile(Task tugasList[]) 
{
    ifstream file("WorkTracker.dat");
    if (!file) return 0;

    int count = 0;
    string line;
    while (getline(file, line) && count < maks_taks) 
    {
        stringstream ss(line);
        getline(ss, tugasList[count].nama, ',');
        getline(ss, tugasList[count].kategori, ',');
        getline(ss, tugasList[count].deadline, ',');
        getline(ss, tugasList[count].status, ',');
        count++;
    }
    file.close();
    return count; 
}

void simpanTugasKeFile(const Task tugasList[], int count) 
{
    ofstream file("WorkTracker.dat");
    for (int i = 0; i < count; i++) 
    {
        file << tugasList[i].nama << "," << tugasList[i].kategori << "," 
             << tugasList[i].deadline << "," << tugasList[i].status << "\n";
    }
    file.close();
}

void tambahTugas(Task tugasList[], int& count) 
{
    if (count >= maks_taks) 
    {
        cout << "Tidak dapat menambahkan tugas, kapasitas penuh!\n";
        return;
    }

    Task tugasBaru;
    cout << "Masukkan nama tugas: ";
    cin.ignore();
    getline(cin, tugasBaru.nama);
    

    for (int i = 0; i < count; i++) 
    {
        if (tugasList[i].nama == tugasBaru.nama) 
        {
            cout << "Tugas ini sudah terdaftar!\n";
            return;
        }
    }

    cout <<"Masukkan Kategori (pekerjaan/kuliah/rumah): ";
    getline(cin, tugasBaru.kategori);
    cout << "Masukkan deadline (DD/MM/YYYY): ";
    getline(cin, tugasBaru.deadline);
    tugasBaru.status = "Belum Selesai";

    tugasList[count] = tugasBaru; 
    count++; 
    simpanTugasKeFile(tugasList, count);
    cout << "Tugas berhasil ditambahkan!\n";
}

void tampilkanTugas(Task tugasList[], int count) 
{
    if (count == 0) 
    {
        cout << "Tidak ada tugas yang tersimpan.\n";
        return;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (tugasList[i].deadline < tugasList[j].deadline) 
            {
                swap(tugasList[i], tugasList[j]);
            }
        }
    }

    cout << "\nDaftar Tugas:\n";
    for (int i = 0; i < count; i++) 
    {
        cout << tugasList[i].nama << " | " << tugasList[i].kategori 
             << " | " << tugasList[i].deadline << " | " << tugasList[i].status << endl;
    }
}

void cariTugas(Task tugasList[], int count) 
{
    string kategori;
    cout << "Masukkan kategori: ";
    cin.ignore();
    getline(cin, kategori);

    bool ditemukan = false;
    for (int i = 0; i < count; i++) {
        if (tugasList[i].kategori == kategori) {
            cout << tugasList[i].nama << " | " << tugasList[i].deadline 
                 << " | " << tugasList[i].status << endl;
            ditemukan = true;
        }
    }

    if (!ditemukan) {
        cout << "Tidak ada tugas dalam kategori ini!\n";
    }
}

void updateStatus(Task tugasList[], int count) 
{
    string nama;
    cout << "Masukkan nama tugas: ";
    cin.ignore();
    getline(cin, nama);

    bool ditemukan = false;
    for (int i = 0; i < count; i++) 
    {
        if (tugasList[i].nama == nama) 
        {
            tugasList[i].status = "Selesai";
            ditemukan = true;
            break;
        }
    }

    if (ditemukan) 
    {
        simpanTugasKeFile(tugasList, count);
        cout << "Status tugas berhasil diperbarui!\n";
    } else {
        cout << "Tugas tidak ditemukan!\n";
    }
}

void hapusTugas(Task tugasList[], int& count) 
{
    string nama;
    cout << "Masukkan nama tugas yang ingin dihapus: ";
    cin.ignore();
    getline(cin, nama);

    bool ditemukan = false;
    for (int i = 0; i < count; i++) {
        if (tugasList[i].nama == nama && tugasList[i].status == "Selesai") 
        {
        
            for (int j = i; j < count - 1; j++) 
            {
                tugasList[j] = tugasList[j + 1];
            }
            count--;
            ditemukan = true;
            break;
        }
    }

    if (ditemukan) {
        simpanTugasKeFile(tugasList, count);
        cout << "Tugas berhasil dihapus!\n";
    } else {
        cout << "Tugas tidak ditemukan atau belum selesai!\n";
    }
}

bool isDeadlineExceeded(const string& deadline) 
{
    int day, month, year;
    sscanf(deadline.c_str(), "%d/%d/%d", &day, &month, &year);

    time_t now = time(0);
    tm* ltm = localtime(&now);

    int currentYear = 1900 + ltm->tm_year;
    int currentMonth = 1 + ltm->tm_mon;
    int currentDay = ltm->tm_mday;

    if (year < currentYear) return true;
    if (year == currentYear && month < currentMonth) return true;
    if (year == currentYear && month == currentMonth && day < currentDay) return true;

    return false;
}

void tugasterlambat(Task tugasList[], int count) 
{
    bool adaTugasTerlambat = false;
    cout << "\nPerhatian! Tugas berikut sudah melewati deadline:\n";
    for (int i = 0; i < count; i++) 
    {
        if (isDeadlineExceeded(tugasList[i].deadline) && tugasList[i].status == "Belum Selesai") 
        {
            cout << tugasList[i].nama << " | " << tugasList[i].kategori 
                 << " | " << tugasList[i].deadline << " | " << tugasList[i].status << endl;
            adaTugasTerlambat = true;
        }
    }

    if (!adaTugasTerlambat) 
    {
        cout << "Tidak ada tugas yang melewati deadline.\n";
    }
}

int main() 
{
    Task tugasList[maks_taks];
    int count = bacaTugasDariFile(tugasList); 

    int pilihan;
    do {
        cout << "\n===MENU WORKTRACKER===\n";
        cout << "1. Tambah Tugas\n";
        cout << "2. Tampilkan Tugas\n";
        cout << "3. Cari Tugas Berdasarkan Kategori\n";
        cout << "4. Perbarui Status Tugas\n";
        cout << "5. Hapus Tugas yang Selesai\n";
        cout << "6. Tampilkan Tugas yang Melewati Deadline\n";
        cout << "7. Keluar\n";
        cout << "Pilihan: ";
        cin >> pilihan;
        cin.ignore();  

        switch (pilihan) {
            case 1: tambahTugas(tugasList, count); break;
            case 2: tampilkanTugas(tugasList, count); break;
            case 3: cariTugas(tugasList, count); break;
            case 4: updateStatus(tugasList, count); break;
            case 5: hapusTugas(tugasList, count); break;
            case 6: tugasterlambat(tugasList, count); break;
            case 7: cout << "Terima kasih telah menggunakan WorkTracker! Tetap produktif!\n"; break;
            default: cout << "Pilihan tidak valid!\n";
        }
    } while (pilihan != 7);

    return 0;
}