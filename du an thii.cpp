#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_ROOMS 100
#define MAX_BOOKINGS 200

typedef struct {
    char roomId[5];
    int type;      
    double price;
    int status;   
} Room;
typedef struct {
    char roomId[5];
    char cusName[30];
    int days;
    double totalCost;
} Booking;
Room rooms[MAX_ROOMS];
Booking bookings[MAX_BOOKINGS];
int roomsCount = 0;
int bookingCount = 0;
void readLine(char *str, int size) {
    fgets(str, size, stdin);
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}
int isValidRoomId(const char *id) {
    int len = strlen(id);
    return (len >= 3 && len <= 4);
}
int findRoom(const char *id) {
    for (int i = 0; i < roomsCount; i++) {
        if (strcmp(rooms[i].roomId, id) == 0)
            return i;
    }
    return -1;
}
void addRoom() {
    if (roomsCount >= MAX_ROOMS) {
        printf("Danh sach phong da day!\n");
        return;
    }
    char id[10], temp[20];
    printf("Nhap roomId(tu 3 den 4 ky tu): ");
    readLine(id, sizeof(id));
    if (!isValidRoomId(id)) {
        printf("roomId khong hop le!\n");
        return;
    }
    if (findRoom(id) != -1) {
        printf("roomId da ton tai!\n");
        return;
    }
    printf("Nhap loai phong (1: Don, 2: Doi): ");
    readLine(temp, sizeof(temp));
    int type = atoi(temp);
    if (type != 1 && type != 2) {
        printf("Loai phong khong hop le!\n");
        return;
    }
    printf("Nhap gia phong (>0): ");
    readLine(temp, sizeof(temp));
    double price = atof(temp);
    if (price <= 0) {
        printf("Gia phong khong hop le!\n");
        return;
    }
    Room r;
    strcpy(r.roomId, id);
    r.type = type;
    r.price = price;
    r.status = 0;
    rooms[roomsCount++] = r;
    printf("==> Them phong thanh cong!\n");
}
void updateRoom() {
    if (roomsCount == 0) {
        printf("Khong co phong nao trong he thong.\n");
        return;
}
    char roomId[10], temp[20];
    printf("Nhap roomId muon cap nhat: ");
    readLine(roomId, sizeof(roomId));
    int index = findRoom(roomId);
    if (index == -1) {
        printf("Phong khong ton tai!\n");
        return;
    }
    Room *r = &rooms[index];
    printf("\n--- THONG TIN HIEN TAI ---\n");
    printf("RoomId: %s\n", r->roomId);
    printf("Loai phong: %d\n", r->type);
    printf("Gia phong: %.2f\n", r->price);
    printf("Trang thai: %d\n", r->status);
    printf("---------------------------\n\n");
    printf("Nhap loai phong moi (1: Don, 2: Doi): ");
    readLine(temp, sizeof(temp));
    int newType = atoi(temp);
    if (newType != 1 && newType != 2) {
        printf("Loai phong khong hop le!\n");
        return;
    }
    printf("Nhap gia phong moi (>0): ");
    readLine(temp, sizeof(temp));
    double newPrice = atof(temp);

    if (newPrice <= 0) {
        printf("Gia phong khong hop le!\n");
        return;
    }
    r->type = newType;
    r->price = newPrice;
    printf("==> Cap nhat phong thanh cong!\n");
}
void sortRoomsByPriceDesc() {
    if (roomsCount < 2) {
        printf("Khong du phong de sap xep!\n");
        return;
    }
    for (int i = 0; i < roomsCount - 1; i++) {
        for (int j = i + 1; j < roomsCount; j++) {
            if (rooms[i].price < rooms[j].price) {
                Room temp = rooms[i];
                rooms[i] = rooms[j];
                rooms[j] = temp;
            }
        }
    }
    printf("==> Da sap xep phong theo gia giam dan!\n");
}
void checkIn() {
    if (roomsCount == 0) {
        printf("Chua co phong nao trong he thong!\n");
        return;
    }
    char roomId[10], temp[20], name[30];
    printf("Nhap roomId muon check-in: ");
    readLine(roomId, sizeof(roomId));

    int index = findRoom(roomId);
    if (index == -1) {
        printf("Phong khong ton tai!\n");
        return;
    }
    if (rooms[index].status != 0) {
        printf("Phong khong trong hoac dang bao tri!\n");
        return;
    }
    printf("Nhap ten khach hang: ");
    readLine(name, sizeof(name));
    printf("Nhap so ngay o: ");
    readLine(temp, sizeof(temp));
    int days = atoi(temp);
    if (days <= 0) {
        printf("So ngay khong hop le!\n");
        return;
    }
    Booking b;
    strcpy(b.roomId, roomId);
    strcpy(b.cusName, name);
    b.days = days;
    b.totalCost = days * rooms[index].price;
    bookings[bookingCount++] = b;
    rooms[index].status = 1;
    printf("==> Check-in thanh cong! Tong tien: %.2f\n", b.totalCost);
}
void lockRoom() {
    char roomId[10];
    printf("Nhap roomId muon bao tri: ");
    readLine(roomId, sizeof(roomId));
    int index = findRoom(roomId);
    if (index == -1) {
        printf("Phong khong ton tai!\n");
        return;
    }
    if (rooms[index].status == 1) {
        printf("Phong dang co khach, khong the bao tri!\n");
        return;
    }
    rooms[index].status = 2;
    printf("==> Phong da chuyen sang trang thai BAO TRI!\n");
}
void displayRoomsWithPaging() {
    if (roomsCount == 0) {
        printf("Danh sach phong rong!\n");
        return;
    }
    int pageSize = 5;
    int totalPages = (roomsCount + pageSize - 1) / pageSize;
    int currentPage = 0;
    char choice;

    do {
        system("cls");
        printf("\n========== DANH SACH PHONG ==========\n");
        printf("Trang %d / %d\n", currentPage + 1, totalPages);
        printf("+------+--------+----------+------------+------------+\n");
        printf("| STT  | RoomID | Loai     | Gia        | Trang thai |\n");
        printf("+------+--------+----------+------------+------------+\n");

        int start = currentPage * pageSize;
        int end = start + pageSize;
        if (end > roomsCount)
            end = roomsCount;
        for (int i = start; i < end; i++) {
            char typeStr[10];
            char statusStr[15];
            if (rooms[i].type == 1)
                strcpy(typeStr, "Don");
            else
                strcpy(typeStr, "Doi");
            if (rooms[i].status == 0)
                strcpy(statusStr, "Trong");
            else if (rooms[i].status == 1)
                strcpy(statusStr, "Dang o");
            else
                strcpy(statusStr, "Bao tri");
            printf("| %-4d | %-6s | %-8s | %-10.0lf | %-10s |\n",
                   i + 1,
                   rooms[i].roomId,
                   typeStr,
                   rooms[i].price,
                   statusStr);

            printf("+------+--------+----------+------------+------------+\n");
        }
        printf("\n1: Trang sau | 2: Trang truoc | 0: Thoat\n");
        printf("Chon: ");

        char temp2[10];
        readLine(temp2, sizeof(temp2));
        choice = temp2[0];
        if (choice == '1') {
            if (currentPage < totalPages - 1)
                currentPage++;
        }
        else if (choice == '2') {
            if (currentPage > 0)
                currentPage--;
        }
    } while (choice != '0');
}
void searchRoomByType() {
    char temp[10];
    printf("Nhap loai phong can tim (1: Don, 2: Doi): ");
    readLine(temp, sizeof(temp));
    int type = atoi(temp);
    int found = 0;
    for (int i = 0; i < roomsCount; i++) {
        if (rooms[i].type == type && rooms[i].status == 0) {
            printf("Room: %s | Gia: %.2f\n", rooms[i].roomId, rooms[i].price);
            found = 1;
        }
    }
    if (!found)
        printf("Khong tim thay phong phu hop!\n");
}
void bookingHistory() {
    char roomId[10];
    printf("Nhap roomId can xem lich su: ");
    readLine(roomId, sizeof(roomId));
    int found = 0;
    printf("\n--- LICH SU THUE ---\n");
    for (int i = 0; i < bookingCount; i++) {
        if (strcmp(bookings[i].roomId, roomId) == 0) {
            printf("Ten KH: %s | So ngay: %d | Tong tien: %.2f\n",
                   bookings[i].cusName,
                   bookings[i].days,
                   bookings[i].totalCost);
            found = 1;
        }
    }
    if (!found)
        printf("Phong nay chua co lich su thue!\n");
}
void initSampleData() {
    int index = 0;
    int floor = 1;

    for (int i = 0; i < 50; i++) {
        Room r;
        sprintf(r.roomId, "%d", 100 + i + 1);
        r.type = (i % 2 == 0) ? 1 : 2;
        r.price = 300000 + (i % 5) * 50000;
        if (i % 3 == 0)
            r.status = 0; 
        else if (i % 3 == 1)
            r.status = 1; 
        else
            r.status = 2; 
        rooms[index++] = r;
    }
    roomsCount = 50;
    printf("==> Da khoi tao 50 phong (10 trang, moi trang 5 phong)\n");
}
int main() {
    int choice;
    char temp[10];
    initSampleData();

    do {
        printf("\n================= MENU =================\n");
        printf("|--------------------------------------|\n");
        printf("|1. Them phong                         |\n");
        printf("|--------------------------------------|\n");
        printf("|2. Cap nhat phong                     |\n");
        printf("|--------------------------------------|\n");
        printf("|3. Bao tri phong                      |\n");
        printf("|--------------------------------------|\n");
        printf("|4. Hien thi danh sach phong           |\n");
        printf("|--------------------------------------|\n");
        printf("|5. Tim phong trong theo loai          |\n");
        printf("|--------------------------------------|\n");
        printf("|6. Sap xep theo gia giam dan          |\n");
        printf("|--------------------------------------|\n");
        printf("|7. Check-in                           |\n");
        printf("|--------------------------------------|\n");
        printf("|8. Lich su thue phong                 |\n");
        printf("|--------------------------------------|\n");
        printf("|0. Thoat                              |\n");
        printf("|======================================|\n");
        printf("Chon: ");

        readLine(temp, sizeof(temp));
        choice = atoi(temp);

        switch (choice) {
        case 1:
            printf("\n=== F01: THEM PHONG ===\n");
            addRoom();
            break;

        case 2:
            printf("\n=== F02: CAP NHAT PHONG ===\n");
            updateRoom();
            break;

        case 3:
            printf("\n=== F03: BAO TRI PHONG ===\n");
            lockRoom();
            break;

        case 4:
            printf("\n=== F04: HIEN THI DANH SACH PHONG ===\n");
            displayRoomsWithPaging();
            break;

        case 5:
            printf("\n=== F05: TIM PHONG TRONG THEO LOAI ===\n");
            searchRoomByType();
            break;

        case 6:
            printf("\n=== F06: SAP XEP PHONG THEO GIA GIAM DAN ===\n");
            sortRoomsByPriceDesc();
            break;

        case 7:
            printf("\n=== F07: CHECK-IN ===\n");
            checkIn();
            break;

        case 8:
            printf("\n=== F08: LICH SU THUE PHONG ===\n");
            bookingHistory();
            break;

        case 0:
            printf("Da thoat chuong trinh!\n");
            break;

        default:
            printf("Lua chon khong hop le!\n");
        }

    } while (choice != 0);

    return 0;
}
