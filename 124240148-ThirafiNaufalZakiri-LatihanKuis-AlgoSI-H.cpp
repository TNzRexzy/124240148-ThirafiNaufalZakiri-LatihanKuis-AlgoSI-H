#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>

using namespace std;

// Struktur data buku
struct Buku
{
    char id[10];
    char judul[50];
    char penulis[50];
    int stok;
};

// Fungsi buat login
bool login()
{
    string username, password;
    do
    {
        cout << "Masukkan username: ";
        cin >> username;
        cout << "Masukkan password: ";
        cin >> password;

        if (username == "naufal" && password == "148")
        {
            cout << "Halo, " << username << "! Selamat datang di database Perpustakaan\n";
            return true;
        }
        else
        {
            cout << "Username/password salah. Silahkan coba lagi.\n";
        }
    } while (true);
}

// Fungsi buat nambahin buku
void tambahBuku()
{
    ofstream file("data.dat", ios::app | ios::binary);
    if (!file)
    {
        cout << "Gagal membuka file!\n";
        return;
    }

    Buku buku;
    cout << "Masukkan ID buku: ";
    cin >> buku.id;
    cin.ignore();
    cout << "Masukkan Judul buku: ";
    cin.getline(buku.judul, 50);
    cout << "Masukkan Penulis: ";
    cin.getline(buku.penulis, 50);
    cout << "Masukkan Stok: ";
    cin >> buku.stok;

    file.write(reinterpret_cast<char *>(&buku), sizeof(Buku));
    file.close();
    cout << "Buku berhasil ditambahkan!\n";
}

// Fungsi buat nampilin daftar buku pake (Bubble Sort)
void tampilkanBuku()
{
    ifstream file("data.dat", ios::binary);
    if (!file)
    {
        cout << "Belum ada data buku.\n";
        return;
    }

    Buku buku[100], temp;
    int count = 0;

    while (file.read(reinterpret_cast<char *>(&buku[count]), sizeof(Buku)))
    {
        count++;
    }
    file.close();

    // Bubble Sort berdasarkan ID buku
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (strcmp(buku[j].id, buku[j + 1].id) > 0)
            {
                temp = buku[j];
                buku[j] = buku[j + 1];
                buku[j + 1] = temp;
            }
        }
    }

    // Nampilin buku
    cout << "==================================================\n";
    cout << left << setw(10) << "ID" << setw(30) << "Judul" << setw(20) << "Penulis" << setw(5) << "Stok" << endl;
    cout << "==================================================\n";

    for (int i = 0; i < count; i++)
    {
        cout << left << setw(10) << buku[i].id << setw(30) << buku[i].judul << setw(20) << buku[i].penulis << setw(5) << buku[i].stok << endl;
    }
}

// Fungsi buat pencarian buku
void cariBuku()
{
    ifstream file("data.dat", ios::binary);
    if (!file)
    {
        cout << "Belum ada data buku.\n";
        return;
    }

    char keyword[50];
    bool found = false;
    cout << "Masukkan kata kunci dari judul buku yang ingin dicari: ";
    cin.ignore();
    cin.getline(keyword, 50);

    Buku buku;
    while (file.read(reinterpret_cast<char *>(&buku), sizeof(Buku)))
    {
        if (strstr(buku.judul, keyword))
        {
            cout << "==================================================\n";
            cout << left << setw(10) << "ID" << setw(30) << "Judul" << setw(20) << "Penulis" << setw(5) << "Stok" << endl;
            cout << "==================================================\n";
            cout << left << setw(10) << buku.id << setw(30) << buku.judul << setw(20) << buku.penulis << setw(5) << buku.stok << endl;
            found = true;
        }
    }
    if (!found)
        cout << "Buku dengan kata kunci \"" << keyword << "\" tidak dapat ditemukan!\n";
    file.close();
}

// Fungsi buat hapus buku
void hapusBuku()
{
    ifstream file("data.dat", ios::binary);
    ofstream temp("temp.dat", ios::binary);
    if (!file || !temp)
    {
        cout << "Gagal membuka file!\n";
        return;
    }

    char idHapus[10];
    bool found = false;
    cout << "Masukkan ID buku yang ingin anda hapus: ";
    cin >> idHapus;

    Buku buku;
    while (file.read(reinterpret_cast<char *>(&buku), sizeof(Buku)))
    {
        if (strcmp(buku.id, idHapus) != 0)
        {
            temp.write(reinterpret_cast<char *>(&buku), sizeof(Buku));
        }
        else
        {
            found = true;
        }
    }

    file.close();
    temp.close();
    remove("data.dat");
    rename("temp.dat", "data.dat");

    if (found)
    {
        cout << "Buku dengan ID " << idHapus << " telah berhasil dihapus!\n";
    }
    else
    {
        cout << "Buku tidak ditemukan!\n";
    }
}

// Program utamanya
int main()
{
    if (!login())
        return 0;

    int pilihan;
    do
    {
        cout << "\n1. Tambahkan data buku\n2. Tampilkan daftar buku\n3. Cari buku\n4. Hapus buku\n5. Keluar\n>> ";
        cin >> pilihan;

        switch (pilihan)
        {
        case 1:
            tambahBuku();
            break;
        case 2:
            tampilkanBuku();
            break;
        case 3:
            cariBuku();
            break;
        case 4:
            hapusBuku();
            break;
        case 5:
            cout << "Menutup program....\n";
            break;
        default:
            cout << "Pilihan anda tidak valid!\n";
            break;
        }
    } while (pilihan != 5);

    return 0;
}
