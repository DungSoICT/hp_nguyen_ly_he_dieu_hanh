//**code_hoan_chinh_co_chu_thich :** `phân tích toàn diện quản lý danh sách tiến trình bằng linked list` ++ `process_list_linked_list.cpp` ++ `PCB` ++ `head pointer` ++ `tail pointer` ++ `traversal` ++ `insert` ++ `delete` ++ `context switch` ++ `time complexity` ++ `memory structure`

/*
Đề bài ngắn gọn:
Mô phỏng hệ điều hành quản lý danh sách tiến trình bằng linked list.
Ban đầu có các tiến trình P1 -> P2 -> P3 -> P4.
Thực hiện các thao tác: thêm P5 vào cuối ; thêm P0 vào đầu ; xóa P3 ; duyệt danh sách ;
mô tả context switch giữa P2 và P4 ; in ra độ phức tạp của các thao tác.
Mỗi node là một PCB chứa: pointer ; process state ; program counter ; registers ;
memory limits ; list of open files.
*/

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

// Cấu trúc PCB: lưu toàn bộ thông tin quản lý một tiến trình
struct PCB {
        string pid;                        // Mã tiến trình ; ví dụ P1 ; P2
        string processState;               // Trạng thái tiến trình ; ví dụ ready ; running ; waiting
        int programCounter;                // Bộ đếm chương trình
        vector<int> registers;             // Các thanh ghi
        pair<int, int> memoryLimits;       // Giới hạn bộ nhớ ; [start ; end]
        vector<string> openFiles;          // Danh sách file đang mở
        PCB* next;                         // Con trỏ liên kết sang PCB kế tiếp

        PCB(
            const string& pid_,
            const string& state_,
            int pc_,
            const vector<int>& regs_,
            pair<int, int> mem_,
            const vector<string>& files_
        ) : pid(pid_),
            processState(state_),
            programCounter(pc_),
            registers(regs_),
            memoryLimits(mem_),
            openFiles(files_),
            next(nullptr) {}
    };

// Lớp ProcessList: quản lý danh sách tiến trình bằng linked list
class ProcessList {
private:
        PCB* head;                         // Con trỏ đầu danh sách
        PCB* tail;                         // Con trỏ cuối danh sách
        int size;                          // Số lượng tiến trình hiện tại

        // Hàm phụ: chuyển vector<int> thành chuỗi để in đẹp
        string joinRegisters(const vector<int>& regs) const {
            ostringstream oss;             // Dùng để ghép chuỗi thanh ghi
            oss << "[";                    // Mở ngoặc danh sách
            for (size_t i = 0; i < regs.size(); ++i) {
                oss << regs[i];            // In giá trị thanh ghi thứ i
                if (i + 1 < regs.size()) oss << " ; "; // Ngăn cách bằng dấu ;
            }
            oss << "]";                    // Đóng ngoặc danh sách
            return oss.str();
        }

        // Hàm phụ: chuyển vector<string> thành chuỗi để in đẹp
        string joinFiles(const vector<string>& files) const {
            ostringstream oss;             // Dùng để ghép chuỗi file
            oss << "[";                    // Mở ngoặc danh sách
            for (size_t i = 0; i < files.size(); ++i) {
                oss << files[i];           // In tên file thứ i
                if (i + 1 < files.size()) oss << " ; "; // Ngăn cách bằng dấu ;
            }
            oss << "]";                    // Đóng ngoặc danh sách
            return oss.str();
        }

public:
    // Hàm khởi tạo: tạo danh sách tiến trình rỗng
    ProcessList() : head(nullptr), tail(nullptr), size(0) {}

    // Hàm hủy: giải phóng toàn bộ bộ nhớ của linked list
    ~ProcessList() {
        PCB* current = head;           // Bắt đầu từ head để duyệt toàn bộ danh sách
        while (current != nullptr) {   // Lặp cho tới khi hết danh sách
            PCB* temp = current;       // Giữ node hiện tại để xóa
            current = current->next;   // Chuyển sang node kế tiếp trước khi delete
            delete temp;               // Giải phóng node cũ
        }

        /*
        Cách hình dung:
        head -> A -> B -> C -> NULL
                ^
              temp xóa A ; sau đó sang B ; rồi sang C
        Đây là cách dọn toàn bộ linked list từ trái sang phải.
        */
    }

    // Hàm insertTail: thêm một PCB vào cuối danh sách ; ý (2)
void insertTail(PCB* newNode) {
        if (newNode == nullptr) return;                // Nếu node rỗng thì bỏ qua

        if (head == nullptr) {                         // Danh sách đang rỗng
            head = tail = newNode;                    // head và tail cùng trỏ node đầu tiên ; ý (1)
        } else {
            tail->next = newNode;                     // Node cuối hiện tại nối sang node mới ; ý (2)
            tail = newNode;                           // Cập nhật tail sang node mới ; ý (2)
        }
        ++size;                                       // Tăng số lượng tiến trình

        /*
        Visualization:
        Trước: head -> P1 -> P2 -> NULL ; tail = P2
        Sau  : head -> P1 -> P2 -> P3 -> NULL ; tail = P3

        Ý chính:
        - Có tail thì thêm cuối chỉ cần sửa 2 liên kết
        - Độ phức tạp: O(1)
        */
    }

    // Hàm insertHead: thêm một PCB vào đầu danh sách ; ý (3)
void insertHead(PCB* newNode) {
        if (newNode == nullptr) return;                // Nếu node rỗng thì bỏ qua

        if (head == nullptr) {                         // Danh sách đang rỗng
            head = tail = newNode;                    // head và tail cùng trỏ node mới ; ý (1)
        } else {
            newNode->next = head;                     // Node mới trỏ tới head cũ ; ý (3)
            head = newNode;                           // Cập nhật head sang node mới ; ý (3)
        }
        ++size;                                       // Tăng số lượng tiến trình

        /*
        Visualization:
        Trước: head -> P1 -> P2 -> NULL
        Sau  : head -> P0 -> P1 -> P2 -> NULL

        Ý chính:
        - Thêm đầu chỉ cần nối newNode với head cũ
        - Độ phức tạp: O(1)
        */
    }

    // Hàm deleteByPID: xóa tiến trình theo PID ; ý (4)
bool deleteByPID(const string& pid) {
        if (head == nullptr) return false;            // Danh sách rỗng thì không xóa được

        if (head->pid == pid) {                       // Trường hợp xóa node đầu ; ý (4)
            PCB* temp = head;                         // Giữ node cần xóa
            head = head->next;                        // Đẩy head sang node kế tiếp
            if (tail == temp) tail = head;            // Nếu chỉ có 1 node thì cập nhật tail luôn
            delete temp;                              // Giải phóng node cũ
            --size;                                   // Giảm số lượng tiến trình
            return true;                              // Báo xóa thành công
        }

        PCB* prev = head;                             // prev dùng để giữ node đứng trước
        PCB* current = head->next;                    // current là node đang xét
        while (current != nullptr) {                  // Duyệt tới hết danh sách
            if (current->pid == pid) {                // Tìm thấy PID cần xóa ; ý (4)
                prev->next = current->next;           // Bỏ qua current ; nối prev sang node sau ; ý (4)
                if (tail == current) tail = prev;     // Nếu xóa node cuối thì cập nhật tail
                delete current;                       // Giải phóng node bị xóa
                --size;                               // Giảm số lượng tiến trình
                return true;                          // Báo thành công
            }
            prev = current;                           // Dời prev tiến lên
            current = current->next;                  // Dời current tiến lên
        }
        return false;                                 // Không tìm thấy PID
        /*
        Visualization:
        Trước: P1 -> P2 -> P3 -> P4
                     ^     ^
                   prev  current
        Xóa P3:
        Sau  : P1 -> P2 ------> P4

        Ý chính:
        - Muốn xóa node giữa thì phải biết node đứng trước
        - Nếu xóa tail thì tail phải dời về prev
        - Độ phức tạp: O(n) nếu phải tìm PID
        */
    }

    // Hàm traverseAndPrint: duyệt toàn bộ danh sách và in thông tin PCB ; ý (5)
void traverseAndPrint() const {
        cout << "Danh sach tien trinh hien tai:\n";
        PCB* current = head;                           // Bắt đầu duyệt từ head ; ý (5)
        while (current != nullptr) {                   // Lặp cho tới NULL
            cout << "  - PID: " << current->pid
                 << " ; state: " << current->processState
                 << " ; PC: " << current->programCounter
                 << " ; registers: " << joinRegisters(current->registers)
                 << " ; memory: [" << current->memoryLimits.first
                 << " ; " << current->memoryLimits.second << "]"
                 << " ; open files: " << joinFiles(current->openFiles)
                 << "\n";
            current = current->next;                   // Sang node kế tiếp ; ý (5)
        }
        cout << "  - head = " << (head ? head->pid : "NULL")
             << " ; tail = " << (tail ? tail->pid : "NULL")
             << " ; size = " << size << "\n";

        /*
        Visualization:
        current = head
        current -> next -> next -> ... -> NULL

        Ý chính:
        - Duyệt linked list luôn đi từ head sang next
        - Mỗi bước đọc một PCB rồi chuyển sang PCB kế tiếp
        - Độ phức tạp: O(n)
        */
    }

    // Hàm findByPID: tìm PCB theo PID để phục vụ context switch ; ý (6)
PCB* findByPID(const string& pid) const {
        PCB* current = head;                           // Bắt đầu từ head
        while (current != nullptr) {                   // Duyệt toàn bộ danh sách
            if (current->pid == pid) {                // Nếu PID khớp
                return current;                       // Trả về con trỏ tới PCB tương ứng
            }
            current = current->next;                  // Sang node kế tiếp
        }
        return nullptr;                               // Không tìm thấy

        /*
        Visualization:
        head -> P0 -> P1 -> P2 -> P4 -> P5
                       so sánh từng PID
        Tìm thấy thì trả node đó về ngay.

        Ý chính:
        - Hàm này là linear search trên linked list
        - Độ phức tạp: O(n)
        */
    }

    // Hàm contextSwitch: mô phỏng chuyển CPU từ tiến trình fromPID sang toPID ; ý (6)
void contextSwitch(const string& fromPID, const string& toPID) {
        PCB* from = findByPID(fromPID);                // Tìm PCB tiến trình cũ
        PCB* to = findByPID(toPID);                    // Tìm PCB tiến trình mới

        cout << "\nMo phong context switch:\n";

        if (from == nullptr || to == nullptr) {        // Kiểm tra dữ liệu hợp lệ
            cout << "  - Khong the context switch vi thieu tien trinh.\n";
            return;
        }

        cout << "  - Tu " << from->pid << " sang " << to->pid << "\n";

        // Bước 1: lưu trạng thái tiến trình cũ ; ý (6)
        from->processState = "ready";                  // Tiến trình cũ quay lại ready
        from->programCounter += 4;                     // Giả lập CPU da chay them 1 vai lenh
        for (int& reg : from->registers) reg += 1;     // Giả lập thanh ghi thay đổi trong lúc chạy

        // Bước 2: nạp trạng thái tiến trình mới ; ý (6)
        to->processState = "running";                  // Tiến trình mới được CPU cấp quyền chạy

        // In chi tiết quá trình
        cout << "  - Luu trang thai " << from->pid
             << " ; state = " << from->processState
             << " ; PC = " << from->programCounter
             << " ; registers = " << joinRegisters(from->registers) << "\n";

        cout << "  - Nap trang thai " << to->pid
             << " ; state = " << to->processState
             << " ; PC = " << to->programCounter
             << " ; registers = " << joinRegisters(to->registers) << "\n";

        /*
        Visualization:
        CPU dang chay P2
        [save P2 vao PCB] -> [load P4 tu PCB] -> CPU chay P4

        Ý chính:
        - PCB giữ đủ dữ liệu để tạm dừng rồi quay lại sau
        - Save context: state ; PC ; registers
        - Load context: state ; PC ; registers của tiến trình mới
        - Đây là bản chất của context switching
        */
    }

    // Hàm printComplexity: in bảng độ phức tạp của các thao tác ; ý (7)
void printComplexity() const {
            cout << "\nDo phuc tap thao tac:\n";
            cout << "  - insert head = O(1)\n";
            cout << "  - insert tail = O(1) neu co tail ; neu khong co tail thi O(n)\n";
            cout << "  - delete by PID = O(n) neu phai tim node\n";
            cout << "  - traversal = O(n)\n";
            cout << "  - find by PID = O(n)\n";
            cout << "  - context switch logic = O(1) sau khi da co 2 PCB ; nhung neu tim theo PID thi tong the la O(n)\n";

            /*
            Cách đọc:
            - O(1): thao tác hầu như không phụ thuộc số node
            - O(n): phải duyệt danh sách theo số node
            - Linked list mạnh ở thêm/xóa đầu-cuối ; yếu ở tìm kiếm tuyến tính
            */
        }
    };

// Hàm taoPCB: tạo nhanh một PCB mẫu để code ngắn gọn và dễ đọc

PCB* taoPCB(
        const string& pid,                  // PID của tiến trình
        const string& state,                // Trạng thái ban đầu
        int pc,                             // Program counter ban đầu
        const vector<int>& regs,            // Giá trị các thanh ghi
        pair<int, int> mem,                 // Miền nhớ
        const vector<string>& files         // Danh sách file mở
    ) {
        PCB* node = new PCB(pid, state, pc, regs, mem, files); // Cấp phát node mới trên heap
        return node;                                           // Trả node về để gắn vào linked list

        /*
        Visualization:
        Input du lieu -> tao 1 PCB moi -> tra con tro PCB*
        Ham nay chi dong vai tro "xay gach" cho danh sach tien trinh.
        */
    }

int main() {
    ios::sync_with_stdio(false);        // Tăng tốc I/O
    cin.tie(nullptr);                   // Bỏ buộc tie giữa cin và cout

    ProcessList processList;            // Tạo danh sách tiến trình rỗng

    // Tạo trạng thái ban đầu: P1 -> P2 -> P3 -> P4 ; ý (1)
    processList.insertTail(taoPCB("P1", "ready",   100, {1, 2, 3}, {0,   255}, {"stdin", "fileA.txt"}));
    processList.insertTail(taoPCB("P2", "running", 200, {4, 5, 6}, {256, 511}, {"stdin", "db.log"}));
    processList.insertTail(taoPCB("P3", "waiting", 300, {7, 8, 9}, {512, 767}, {"disk0", "swap.tmp"}));
    processList.insertTail(taoPCB("P4", "ready",   400, {2, 4, 6}, {768, 999}, {"stdout", "report.txt"}));

    cout << "=== Trang thai ban dau ===\n";
    processList.traverseAndPrint();

    // Thêm P5 vào cuối ; ý (2)
    processList.insertTail(taoPCB("P5", "ready", 500, {10, 11, 12}, {1000, 1200}, {"sensor.dat"}));
    cout << "\n=== Sau khi them P5 vao cuoi ===\n";
    processList.traverseAndPrint();

    // Thêm P0 vào đầu ; ý (3)
    processList.insertHead(taoPCB("P0", "new", 50, {0, 0, 0}, { -1,  50}, {"boot.cfg"}));
    cout << "\n=== Sau khi them P0 vao dau ===\n";
    processList.traverseAndPrint();

    // Xóa P3 ; ý (4)
    bool deleted = processList.deleteByPID("P3");
    cout << "\n=== Sau khi xoa P3 ===\n";
    cout << "Ket qua xoa P3: " << (deleted ? "thanh cong" : "that bai") << "\n";
    processList.traverseAndPrint();

    // Context switch từ P2 sang P4 ; ý (6)
    processList.contextSwitch("P2", "P4");

    cout << "\n=== Sau context switch ===\n";
    processList.traverseAndPrint();

    // In độ phức tạp ; ý (7)
    processList.printComplexity();

    return 0;
}

/*
Huong dan input / output / testcase:

1) Chuong trinh nay dang duoc viet san testcase trong code ; khong can nhap input tu ban phim.
2) Chi can bien dich va chay:
   - Linux / macOS:
     g++ -std=c++17 -O2 -Wall -Wextra -o process_list process_list_linked_list.cpp
     ./process_list
   - Windows MinGW:
     g++ -std=c++17 -O2 -Wall -Wextra -o process_list.exe process_list_linked_list.cpp
     process_list.exe

3) Output se in ra:
   - Trang thai danh sach ban dau
   - Sau khi them P5 vao cuoi
   - Sau khi them P0 vao dau
   - Sau khi xoa P3
   - Mo phong context switch P2 -> P4
   - Bang do phuc tap thao tac

4) Testcase chung da tich hop:
   - Ban dau: P1 -> P2 -> P3 -> P4
   - Them cuoi: P5
   - Them dau: P0
   - Xoa: P3
   - Context switch: P2 -> P4

5) Ket qua logic mong doi:
   - Sau them cuoi: P1 -> P2 -> P3 -> P4 -> P5
   - Sau them dau: P0 -> P1 -> P2 -> P3 -> P4 -> P5
   - Sau xoa P3 : P0 -> P1 -> P2 -> P4 -> P5
   - head = P0 ; tail = P5
   - P2 sau save context se co state = ready
   - P4 sau load context se co state = running

6) Neu muon tu mo rong:
   - Them ham tim kiem theo PID
   - Them ham xoa dau ; xoa cuoi
   - Them ham sap xep theo PID
   - Them ham mo phong scheduler FCFS / Round Robin
*/
