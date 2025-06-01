#include <iostream>
#include <string>
using namespace std;

struct Pelaku {
    string nama;
    string posisi;
    double nominal;
    Pelaku* left;
    Pelaku* right;

    Pelaku(string n, string p, double d) {
        nama = n;
        posisi = p;
        nominal = d;
        left = right = nullptr;
    }
};

struct Aksi {
    string jenis; 
    Pelaku* data;
    Aksi* next;
};

Aksi* undoTop = nullptr;

void pushUndo(const string& jenis, Pelaku* data) {
    Aksi* node = new Aksi{jenis, data, undoTop};
    undoTop = node;
}

bool undoEmpty() {
    return undoTop == nullptr;
}

Aksi popUndo() {
    if (undoTop == nullptr) {
        return {"", nullptr, nullptr};
    }
    Aksi aksi = *undoTop;
    Aksi* temp = undoTop;
    undoTop = undoTop->next;
    delete temp;
    return aksi;
}

Pelaku* tambahPelaku(Pelaku* root, Pelaku* node) {
    if (root == nullptr)
        return node;
    if (node->nama < root->nama)
        root->left = tambahPelaku(root->left, node);
    else
        root->right = tambahPelaku(root->right, node);
    return root;
}

void inorderTampil(Pelaku* root) {
    if (root != nullptr) {
        inorderTampil(root->left);
        cout << "Nama: " << root->nama << ", Posisi: " << root->posisi
             << ", Nominal: " << root->nominal << endl;
        inorderTampil(root->right);
    }
}

Pelaku* cariMin(Pelaku* root) {
    while (root->left != nullptr)
        root = root->left;
    return root;
}
Pelaku* hapusPelaku(Pelaku* root, string nama) {
    if (root == nullptr) return nullptr;

    if (nama < root->nama)
        root->left = hapusPelaku(root->left, nama);
    else if (nama > root->nama)
        root->right = hapusPelaku(root->right, nama);
    else {
       
        Pelaku* backup = new Pelaku(root->nama, root->posisi, root->nominal);
        pushUndo("hapus", backup);

        if (root->left == nullptr && root->right == nullptr) {
            delete root;
            return nullptr;
        }
        else if (root->left == nullptr) {
            Pelaku* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == nullptr) {
            Pelaku* temp = root->left;
            delete root;
            return temp;
        }
        else {
            Pelaku* minRight = cariMin(root->right);
            root->nama = minRight->nama;
            root->posisi = minRight->posisi;
            root->nominal = minRight->nominal;
            root->right = hapusPelaku(root->right, minRight->nama);
        }
    }
    return root;
}

Pelaku* undoTerakhir(Pelaku* root) {
    if (undoEmpty()) {
        cout << "Tidak ada aksi yang bisa di-undo.\n";
        return root;
    }

    Aksi aksi = popUndo();

    if (aksi.jenis == "tambah") {
        root = hapusPelaku(root, aksi.data->nama);
        cout << "Undo berhasil: Data " << aksi.data->nama << " dihapus kembali.\n";
        delete aksi.data;
    } else if (aksi.jenis == "hapus") {
        Pelaku* baru = new Pelaku(aksi.data->nama, aksi.data->posisi, aksi.data->nominal);
        root = tambahPelaku(root, baru);
        cout << "Undo berhasil: Data " << aksi.data->nama << " dikembalikan.\n";
        delete aksi.data;
    }

    return root;
}

int main() {
    Pelaku* root = nullptr;
    int pilihan;

    do {
        cout << "\n====== MENU ======\n";
        cout << "1. Tambah Data Pelaku\n";
        cout << "2. Tampilkan Semua Data (A-Z)\n";
        cout << "3. Hapus Data Pelaku\n";
        cout << "4. Undo Aksi Terakhir\n";
        cout << "0. Keluar\n";
        cout << "Pilih menu: ";
        cin >> pilihan;
        cin.ignore();

        if (pilihan == 1) {
            string nama, posisi;
            double nominal;
            cout << "Masukkan Nama Pelaku: ";
            getline(cin, nama);
            cout << "Masukkan Posisi: ";
            getline(cin, posisi);
            cout << "Masukkan Nominal Dana: ";
            cin >> nominal;
            cin.ignore();

            Pelaku* cek = root;
            bool duplikat = false;
            while (cek) {
                if (nama == cek->nama) {
                    duplikat = true;
                    break;
                }
                cek = (nama < cek->nama) ? cek->left : cek->right;
            }

            if (duplikat) {
                cout << "Nama sudah ada, data tidak ditambahkan.\n";
            } else {
                Pelaku* baru = new Pelaku(nama, posisi, nominal);
                root = tambahPelaku(root, baru);
                Pelaku* backup = new Pelaku(nama, posisi, nominal);
                pushUndo("tambah", backup);
                cout << "Data berhasil ditambahkan.\n";
            }

        } else if (pilihan == 2) {
            cout << "\n-- Data Pelaku --\n";
            inorderTampil(root);

        } else if (pilihan == 3) {
            string nama;
            cout << "Masukkan nama pelaku yang ingin dihapus: ";
            getline(cin, nama);
            root = hapusPelaku(root, nama);
            cout << "Data pelaku dihapus (jika ditemukan).\n";

        } else if (pilihan == 4) {
            root = undoTerakhir(root);

        } else if (pilihan == 0) {
            cout << "Program selesai.\n";

        } else {
            cout << "Pilihan tidak valid.\n";
        }

    } while (pilihan != 0);
    while (!undoEmpty()) {
        Aksi aksi = popUndo();
        delete aksi.data;
    }

    return 0;
}