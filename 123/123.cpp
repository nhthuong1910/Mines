#include<iostream>
#include<ctime>
#include<cstdlib>
#include<Windows.h>
#include<string>
#include<mmstream.h>

using namespace std;

#pragma comment(lib, "winmm.lib")

void mau_ki_tu(int x) {
    HANDLE mau;
    mau = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(mau, x);
}

void goto_xy(int x, int y) {
    HANDLE tieu_chuan_dau_ra;
    COORD vi_tri_con_tro = { x - 1, y - 1 };
    tieu_chuan_dau_ra = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(tieu_chuan_dau_ra, vi_tri_con_tro);
}

void to_mau(int x, int y, string a, int color) {
    goto_xy(x, y);
    mau_ki_tu(color);
    cout << a;
    cout << endl;
    mau_ki_tu(7);
}

const int MAXR = 100;
const int MAXC = 100;

void in_ra_ban_do(char map[MAXR][MAXC], int so_hang, int so_cot) {
    cout << "  ";
    for (int j = 0; j < so_cot; j++) {
        cout << j << " ";
    }
    cout << "\n";

    for (int i = 0; i < so_hang; i++) {
        cout << i << " ";
        for (int j = 0; j < so_cot; j++) {
            cout << map[i][j] << " ";
        }
        cout << "\n";
    }
}

void chon_do_kho(int& so_hang, int& so_cot, int& so_min) {
    int level;
    cout << "Nhap Muc Do Kho\n";
    cout << "Nhan 1 cho MOI NGUOI (9 * 9 o và 10 min)\n";
    cout << "Nhan 2 cho TRUNG BINH (16 * 16 o và 40 min)\n";
    cout << "Nhan 3 cho NANG CAO (24 * 24 o và 99 min)\n";
    cin >> level;
    PlaySound(TEXT("clickchoice.wav"), NULL, SND_FILENAME | SND_ASYNC);

    if (level == 1) {
        so_hang = 9;
        so_cot = 9;
        so_min = 10;
    }
    else if (level == 2) {
        so_hang = 16;
        so_cot = 16;
        so_min = 40;
    }
    else if (level == 3) {
        so_hang = 24;
        so_cot = 24;
        so_min = 99;
    }
    else {
        cout << "Do kho khong hop le. Hay chon lai.\n";
        chon_do_kho(so_hang, so_cot, so_min);
    }
}

void khoi_tao_ban_do(char bang_hien_thi[][MAXC], char ban_do_thuc_te[][MAXC], int so_hang, int so_cot) {
    for (int i = 0; i < so_hang; i++) {
        for (int j = 0; j < so_cot; j++) {
            bang_hien_thi[i][j] = ban_do_thuc_te[i][j] = '.';
        }
    }
}

void vi_tri_min(char ban_do_hien_thi[][MAXC], char ban_do_thuc_te[][MAXC], int so_hang, int so_cot, int so_min) {
    int random[MAXR * MAXC];
    for (int i = 0; i < so_hang * so_cot; i++) {
        random[i] = i;
    }

    srand(time(0));
    for (int i = 0; i < so_hang * so_cot; i++) {
        int j = rand() % (so_hang * so_cot);
        swap(random[i], random[j]);
    }

    for (int i = 0; i < so_min; i++) {
        int x = random[i] / so_cot;
        int y = random[i] % so_cot;
        ban_do_thuc_te[x][y] = '*';
    }

    for (int i = 0; i < so_hang; i++) {
        for (int j = 0; j < so_cot; j++) {
            if (ban_do_thuc_te[i][j] != '*') {
                int count = 0;
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        if (i + dx >= 0 && i + dx < so_hang && j + dy >= 0 && j + dy < so_cot) {
                            if (ban_do_thuc_te[i + dx][j + dy] == '*') {
                                count++;
                            }
                        }
                    }
                }
                ban_do_thuc_te[i][j] = count + '0';
            }
        }
    }
}

void in_bang(char ban_do_hien_thi[][MAXC], int so_hang, int so_cot) {
    for (int j = 0; j < so_cot; j++) {
    }
    cout << "\n";

    for (int i = 0; i < so_hang; i++) {
        for (int j = 0; j < so_cot; j++) {
            cout << ban_do_hien_thi[i][j] << " ";
        }
        cout << "\n";
    }
}

void mo_rong_ban_do(char ban_do_hien_thi[MAXR][MAXC], char ban_do_thuc_te[MAXR][MAXC], int so_hang, int so_cot, int hang, int cot) {
    if (hang < 0 || hang > so_hang || cot < 0 || cot > so_cot) {
        return;
    }
    if (ban_do_hien_thi[hang][cot] == 'F' || ban_do_hien_thi[hang][cot] == 'O') {
        return;
    }
    ban_do_hien_thi[hang][cot] = ban_do_thuc_te[hang][cot];

    if (ban_do_thuc_te[hang][cot] == '0') {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                int hang_moi = hang + dx;
                int cot_moi = cot + dy;
                if (hang_moi >= 0 && hang_moi < so_hang && cot_moi >= 0 && cot_moi < so_cot) {
                    mo_rong_ban_do(ban_do_hien_thi, ban_do_thuc_te, so_hang, so_cot, hang_moi, cot_moi);
                }
            }
        }
    }
}

void cap_nhat_bang(char ban_do_hien_thi[MAXR][MAXC], char ban_do_thuc_te[MAXR][MAXC], int so_hang, int so_cot, bool& tro_choi_ket_thuc) {
    char action;
    int hang, cot;
    cout << "Nhap hanh dong cua ban (F de dat co, R de mo, Q de thoat): ";
    cin >> action;
    PlaySound(TEXT("clickchoice.wav"), NULL, SND_FILENAME | SND_ASYNC);

    if (action == 'Q' || action == 'q') {
        tro_choi_ket_thuc = true;
        return;
    }

    cout << "Nhap hang va cot: ";
    cin >> hang >> cot;

    if (hang < 0 || hang >= so_hang || cot < 0 || cot >= so_cot) {
        cout << "Toa do khong hop le. Hay thu lai.\n";
        return;
    }

    switch (action) {
    case 'F':
    case 'f':
        if (ban_do_hien_thi[hang][cot] == '.') {
            PlaySound(TEXT("flag.wav"), NULL, SND_FILENAME | SND_ASYNC);
            ban_do_hien_thi[hang][cot] = 'F';
        }
        else if (ban_do_hien_thi[hang][cot] == 'F') {
            ban_do_hien_thi[hang][cot] = '.';
        }
        else {
            cout << "Ban khong the dat co o day. Hay thu lai.\n";
        }
        break;
    case 'R':
    case 'r':
        PlaySound(TEXT("move.wav"), NULL, SND_FILENAME | SND_ASYNC);
        if (ban_do_hien_thi[hang][cot] == '.' || ban_do_hien_thi[hang][cot] == 'F') {
            if (ban_do_thuc_te[hang][cot] == '*') {
                PlaySound(TEXT("boom.wav"), NULL, SND_FILENAME | SND_ASYNC);
                tro_choi_ket_thuc = true;
            }
            else {
                ban_do_hien_thi[hang][cot] = ban_do_thuc_te[hang][cot];
                if (ban_do_thuc_te[hang][cot] == '0') {
                   
                }
            }
        }
        else {
            cout << "Ban khong the mo o nay. Hay thu lai.\n";
        }
        break;
    default:
        cout << "Hanh dong khong hop le. Hay thu lai.\n";
        break;
    }
}

bool chien_thang_tro_choi(char ban_do_hien_thi[MAXR][MAXC], char ban_do_thuc_te[MAXR][MAXC], int so_hang, int so_cot, int so_min) {
    for (int i = 0; i < so_hang; i++) {
        for (int j = 0; j < so_cot; j++) {
            if (ban_do_hien_thi[i][j] == '.' && ban_do_thuc_te[i][j] == '*') {
                return false;
            }
        }
    }

    int count = 0;
    for (int i = 0; i < so_hang; i++) {
        for (int j = 0; j < so_cot; j++) {
            if (ban_do_thuc_te[i][j] != '*' && ban_do_hien_thi[i][j] != '.') {
                count++;
            }
        }
    }
    if (count == so_hang * so_cot - so_min) {
        return true;
    }
    else {
        return false;
    }
}
void hien_thi_bom(char ban_do_hien_thi[MAXR][MAXC], char ban_do_thuc_te[MAXR][MAXC], int so_hang, int so_cot) {
    for (int i = 0; i < so_hang; i++) {
        cout << endl;
        for (int j = 0; j < so_cot; j++) {
            if (ban_do_thuc_te[i][j] == '*') {
                ban_do_hien_thi[i][j] = '*';
            }
            cout << ban_do_hien_thi[i][j] << " ";
        }
    }
}
int main() {
    int so_hang, so_cot, so_min;
    char ban_do_hien_thi[MAXR][MAXC], ban_do_thuc_te[MAXR][MAXC];
    bool tieptuc = true;
    string a0 = "|====================================================| ";
    string a = "|           CHAO MUNG BAN DEN VOI TRO CHOI           | ";
    string b = "|--------------------MINESWEEPER---------------------| ";
    string c = "|    LUU Y: GAME CHI DANH CHO NGUOI TREN 18 TUOI !   | ";
    string d = "|           XIN HAY DUA RA LUA CHON CUA BAN          | ";
    string e = "|                    1.VAO GAME                      | ";
    string f = "|                    2.MO TA TRO CHOI                | ";
    string g = "|                    3.THOAT GAME                    | ";
    string h = "|====================================================| ";

    while (true) {
        PlaySound(TEXT("duringgame.wav"), NULL, SND_FILENAME | SND_ASYNC);
        int choice;
        to_mau(34, 1, a0, 8);
        to_mau(34, 2, a, 4);
        to_mau(34, 3, b, 6);
        to_mau(34, 4, c, 14);
        to_mau(34, 5, d, 9);
        to_mau(34, 6, e, 10);
        to_mau(34, 7, f, 5);
        to_mau(34, 8, g, 13);
        to_mau(34, 9, h, 8);
        cout << "\nNhap lua chon cua ban: ";
        cin >> choice;
        PlaySound(TEXT("clickchoice.wav"), NULL, SND_FILENAME | SND_ASYNC);
        PlaySound(TEXT("duringgame.wav"), NULL, SND_FILENAME | SND_ASYNC);
        if (choice == 1) {
            while (tieptuc) {
                bool tro_choi_ket_thuc = false;
                system("cls");
                chon_do_kho(so_hang, so_cot, so_min);
                khoi_tao_ban_do(ban_do_hien_thi, ban_do_thuc_te, so_hang, so_cot);
                vi_tri_min(ban_do_hien_thi, ban_do_thuc_te, so_hang, so_cot, so_min);
                while (!tro_choi_ket_thuc) {
                    system("cls");
                    in_bang(ban_do_hien_thi, so_hang, so_cot);
                    cap_nhat_bang(ban_do_hien_thi, ban_do_thuc_te, so_hang, so_cot, tro_choi_ket_thuc);
                    if (chien_thang_tro_choi(ban_do_hien_thi, ban_do_thuc_te, so_hang, so_cot, so_min)) {
                        cout << "Chuc mung! Ban da chien thang!\n";
                        hien_thi_bom(ban_do_hien_thi, ban_do_thuc_te, so_hang, so_cot);
                        tro_choi_ket_thuc = true;
                    }
                    else if (tro_choi_ket_thuc) {
                        system("cls");
                        cout << "Ban da cham phai mot qua bom. Ban da thua!\n";
                        hien_thi_bom(ban_do_hien_thi, ban_do_thuc_te, so_hang, so_cot);
                    }
                }
                cout << "\nBan co muon choi tiep khong? (y/n): ";
                char lua_chon;
                cin >> lua_chon;
                PlaySound(TEXT("clickchoice.wav"), NULL, SND_FILENAME | SND_ASYNC);
                PlaySound(TEXT("duringgame.wav"), NULL, SND_FILENAME | SND_ASYNC);
                if (lua_chon == 'n' || lua_chon == 'N') {
                    tieptuc = false;
                }
            }
            system("cls");
            string k8 = "CAM ON BAN DA CHOI GAME ! HAY DANH GIA CHUNG TOI NHE ( 0 - 5 ) *: ";
            to_mau(26, 8, k8, 14);
            int n;
            goto_xy(93, 8); cin >> n;
            while (n < 0 || n > 5) {
                cout << "Nhap sai. Danh gia lai (0 - 5): ";
                cin >> n;
            }
            if (n <= 3) {
                goto_xy(26, 10);
                cout << "CAM ON BAN DA DANH GIA, CHUNG TOI SE CAI THIEN GAME NHIEU HON !";
                return 0;
            }
            else if (n <= 5) {
                goto_xy(30, 10);
                cout << "CAM ON BAN DA DANH GIA, HEN GAP BAN LAN SAU !";
                return 0;
            }
        }
        else if (choice == 2) {
            system("cls");
            string k1 = "|==========================================================================|";
            string k2 = "|               DAY LA GAME DO 1 NHOM SINH VIEN NAM NHAT TAO RA !          |\n";
            string k3 = "|        NEN CO LE SE CON NHIEU THIEU SOT MONG CAC BAN BO QUA (^-^)        |\n";
            string k4 = "| MOI LOI GOP Y VA PHAN HOI VUI LONG GUI VE MAIL: thieugialongan@gmail.com |\n";
            string k5 = "|        XIN CHAN THANH CAM ON DA TRAI NGHIEM GAME CUA CHUNG TOI !         |\n";
            string k6 = "|      CHUNG TOI SE CO GANG HET SUC DE CAI THIEN VA PHAT TRIEN GAME        |\n";
            string k7 = "|==========================================================================|";
            to_mau(26, 1, k1, 14);
            to_mau(26, 2, k2, 14);
            to_mau(26, 3, k3, 14);
            to_mau(26, 4, k4, 14);
            to_mau(26, 5, k5, 14);
            to_mau(26, 6, k6, 14);
            to_mau(26, 7, k7, 14);
            cout << "Ban co muon thoat khoi game khong ? (Y/N): ";
            char lua_chon;
            cin >> lua_chon;
            PlaySound(TEXT("clickchoice.wav"), NULL, SND_FILENAME | SND_ASYNC);
            PlaySound(TEXT("duringgame.wav"), NULL, SND_FILENAME | SND_ASYNC);
            if (lua_chon == 'y' || lua_chon == 'Y') {
                return 0;
            }
            else {
                system("cls");
            }
        }
        else if (choice == 3) {
            system("cls");
            string k8 = "CAM ON BAN DA CHOI GAME ! HAY DANH GIA CHUNG TOI NHE ( 0 - 5 ) *: ";
            to_mau(26, 8, k8, 14);
            int n;
            goto_xy(93, 8); cin >> n;
            while (n >= 0 && n <= 5) {
                if (n >= 0 && n <= 3) {
                    goto_xy(26, 10); mau_ki_tu(14);
                    cout << "CAM ON BAN DA DANH GIA, CHUNG TOI SE CAI THIEN GAME NHIEU HON !" << endl;
                    to_mau(0, 0, "", 8);
                    return 0;
                }
                else if (n > 3 && n <= 5) {
                    goto_xy(30, 10); mau_ki_tu(14);
                    cout << "CAM ON BAN DA DANH GIA, HEN GAP BAN LAN SAU !";
                    to_mau(0, 0, "", 8);
                    return 0;
                }
            }
        }
    }
    return 0;
}