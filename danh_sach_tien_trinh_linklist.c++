/*🔍Process List Management with PCB Linked List */
/*code_hoan_chinh_co_chu_thich :'Quản lý danh sách tiến trình dạng linked list' ++ 'tạo PCB' ++ 'thêm tiến trình cuối danh sách' ++ 'thêm tiến trình đầu danh sách' ++ 'xóa tiến trình' ++ 'duyệt danh sách' ++ 'context switch' ++ 'tính độ phức tạp'*/

/*
Đề bài ngắn gọn:
Hệ điều hành quản lý danh sách tiến trình dạng linked list gồm P1→P2→P3→P4 ;
head = P1 ; tail = P4 ;
mỗi PCB chứa pointer ; process state ; program counter ; registers ; memory limits ; open files ;
thực hiện các thao tác: thêm P5 vào cuối ; thêm P0 vào đầu ; xóa P3 ; duyệt danh sách ;
mô tả context switch giữa P2 và P4 ; tính độ phức tạp các thao tác.
Nguồn dữ liệu đề bài: :contentReference[oaicite:0]{index=0}
*/

#include <iostream>
#include <string>
#include <vector>
#include <map>

    using namespace std;

struct PCB {
    string ten_tien_trinh;
    string trang_thai;
    int bo_dem_chuong_trinh;
    map<string, int> thanh_ghi;
    string gioi_han_bo_nho;
    vector<string> tep_dang_mo;
    PCB* con_tro_tiep;

    PCB(
        string ten,
        string state,
        int pc,
        map<string, int> registers,
        string memory_limits,
        vector<string> open_files
    ) {
        ten_tien_trinh = ten;
        trang_thai = state;
        bo_dem_chuong_trinh = pc;
        thanh_ghi = registers;
        gioi_han_bo_nho = memory_limits;
        tep_dang_mo = open_files;
        con_tro_tiep = nullptr;
    }
};

class DanhSachTienTrinh {
private:
    PCB* head;
    PCB* tail;

public:
    DanhSachTienTrinh() {
        head = nullptr;
        tail = nullptr;
    }

    PCB* lay_head() {
        return head;
    }

    PCB* lay_tail() {
        return tail;
    }

    PCB* tao_pcb
            (
            string ten_tien_trinh,
            string trang_thai = "ready",
            int bo_dem_chuong_trinh = 0,
            map<string, int> thanh_ghi = {{"AX", 0}, {"BX", 0}, {"CX", 0}},
            string gioi_han_bo_nho = "0KB-0KB",
            vector<string> tep_dang_mo = {}
    ) 
    {
        // tao_pcb -> tạo một PCB mới chứa đầy đủ thông tin quản lý tiến trình
        PCB* pcb_moi = new PCB(
            ten_tien_trinh,
            trang_thai,
            bo_dem_chuong_trinh,
            thanh_ghi,
            gioi_han_bo_nho,
            tep_dang_mo
        ); // cấp phát động vì linked list cần các node tồn tại sau khi hàm kết thúc

        return pcb_moi;

        /*
        Hướng dẫn đọc code:
        - Hàm tạo ra một PCB mới.
        - PCB gồm: tên tiến trình ; trạng thái ; program counter ; registers ; memory limits ; open files ; pointer.
        - Ví dụ: tao_pcb("P1", "ready", 100) tạo PCB cho P1 với trạng thái ready và program counter = 100.
        */
        }

    void them_vao_cuoi(PCB* pcb_moi) {
        // them_vao_cuoi -> thêm một PCB vào cuối danh sách tiến trình
        if (head == nullptr) {
            head = pcb_moi; // danh sách rỗng nên node mới là phần tử đầu
            tail = pcb_moi; // danh sách rỗng nên node mới cũng là phần tử cuối
        } else {
            tail->con_tro_tiep = pcb_moi; // tail cũ trỏ tới PCB mới
            tail = pcb_moi; // cập nhật tail thành PCB mới
        }

        tail->con_tro_tiep = nullptr; // tail luôn trỏ tới nullptr để đánh dấu cuối danh sách

        /*
        Hướng dẫn đọc code:
        - Nếu danh sách rỗng: head = tail = pcb_moi.
        - Nếu danh sách không rỗng: tail cũ nối sang pcb_moi ; sau đó tail = pcb_moi.
        - Ví dụ: P1→P2→P3→P4 ; thêm P5 cuối → P1→P2→P3→P4→P5.
        */
    }

    void them_vao_dau(PCB* pcb_moi) {
        // them_vao_dau -> thêm một PCB vào đầu danh sách tiến trình
        if (head == nullptr) {
            head = pcb_moi; // danh sách rỗng nên node mới là head
            tail = pcb_moi; // danh sách rỗng nên node mới là tail
            pcb_moi->con_tro_tiep = nullptr; // chỉ có một node nên trỏ tới nullptr
        } else {
            pcb_moi->con_tro_tiep = head; // PCB mới trỏ tới head cũ
            head = pcb_moi; // cập nhật head thành PCB mới
        }

        /*
        Hướng dẫn đọc code:
        - Thêm đầu cần cho node mới trỏ tới head cũ.
        - Sau đó cập nhật head thành node mới.
        - Ví dụ: P1→P2→P3→P4→P5 ; thêm P0 đầu → P0→P1→P2→P3→P4→P5.
        */
    }

    PCB* tim_tien_trinh(string ten_tien_trinh) {
        // tim_tien_trinh -> tìm PCB theo tên tiến trình
        PCB* hien_tai = head; // bắt đầu duyệt từ head

        while (hien_tai != nullptr) {
            if (hien_tai->ten_tien_trinh == ten_tien_trinh) {
                return hien_tai; // tìm thấy PCB cần tìm
            }

            hien_tai = hien_tai->con_tro_tiep; // chuyển sang node kế tiếp
        }

        return nullptr; // không tìm thấy tiến trình

        /*
        Hướng dẫn đọc code:
        - Hàm duyệt từ head đến nullptr.
        - Nếu tên tiến trình trùng với ten_tien_trinh thì trả về PCB đó.
        - Ví dụ: tìm_tien_trinh("P3") trong P1→P2→P3→P4 sẽ trả về PCB của P3.
        */
    }

    bool xoa_tien_trinh(string ten_tien_trinh) {
        // xoa_tien_trinh -> xóa PCB khỏi linked list theo tên tiến trình
        if (head == nullptr) {
            return false; // danh sách rỗng thì không có gì để xóa
        }

        if (head->ten_tien_trinh == ten_tien_trinh) {
            PCB* node_can_xoa = head; // lưu lại head cũ để xóa
            head = head->con_tro_tiep; // cập nhật head sang node kế tiếp

            if (head == nullptr) {
                tail = nullptr; // nếu sau khi xóa danh sách rỗng thì tail cũng phải là nullptr
            }

            delete node_can_xoa; // giải phóng PCB khỏi bộ nhớ
            return true;
        }

        PCB* truoc = head; // node đứng trước node cần xóa
        PCB* hien_tai = head->con_tro_tiep; // node đang xét

        while (hien_tai != nullptr) {
            if (hien_tai->ten_tien_trinh == ten_tien_trinh) {
                truoc->con_tro_tiep = hien_tai->con_tro_tiep; // nối node trước với node sau ; bỏ qua node cần xóa

                if (hien_tai == tail) {
                    tail = truoc; // nếu xóa tail thì cập nhật tail về node trước đó
                }

                delete hien_tai; // giải phóng PCB cần xóa
                return true;
            }

            truoc = hien_tai; // tiến lên một node
            hien_tai = hien_tai->con_tro_tiep; // tiến lên node kế tiếp
        }

        return false; // không tìm thấy tiến trình cần xóa

        /*
        Hướng dẫn đọc code:
        - Nếu xóa head thì chỉ cần head = head->con_tro_tiep.
        - Nếu xóa node giữa như P3 thì cần tìm node trước nó là P2.
        - Sau đó đổi P2.con_tro_tiep từ P3 sang P4.
        - Ví dụ: P0→P1→P2→P3→P4→P5 ; xóa P3 → P0→P1→P2→P4→P5.
        */
    }

    void duyet_danh_sach() {
        // duyet_danh_sach -> in toàn bộ danh sách tiến trình từ head đến tail
        PCB* hien_tai = head; // bắt đầu từ head

        cout << "Danh sach tien trinh: ";

        while (hien_tai != nullptr) {
            cout << hien_tai->ten_tien_trinh; // in tên tiến trình hiện tại

            if (hien_tai->con_tro_tiep != nullptr) {
                cout << " -> "; // nếu chưa phải node cuối thì in mũi tên nối
            }

            hien_tai = hien_tai->con_tro_tiep; // đi sang node kế tiếp
        }

        cout << endl;

        /*
        Hướng dẫn đọc code:
        - Hàm bắt đầu từ head.
        - Mỗi lần đọc một PCB rồi đi theo con_tro_tiep.
        - Dừng khi gặp nullptr.
        - Ví dụ: sau khi thêm P5 ; thêm P0 ; xóa P3 thì duyệt được P0→P1→P2→P4→P5.
        */
    }

    void in_chi_tiet_pcb(string ten_tien_trinh) {
        // in_chi_tiet_pcb -> in thông tin chi tiết của một PCB
        PCB* pcb = tim_tien_trinh(ten_tien_trinh); // tìm PCB theo tên

        if (pcb == nullptr) {
            cout << "Khong tim thay tien trinh " << ten_tien_trinh << endl;
            return;
        }

        cout << "PCB cua " << pcb->ten_tien_trinh << ":" << endl;
        cout << "- process state: " << pcb->trang_thai << endl;
        cout << "- program counter: " << pcb->bo_dem_chuong_trinh << endl;

        cout << "- registers: ";
        for (auto cap : pcb->thanh_ghi) {
            cout << cap.first << "=" << cap.second << " ";
        }
        cout << endl;

        cout << "- memory limits: " << pcb->gioi_han_bo_nho << endl;

        cout << "- open files: ";
        if (pcb->tep_dang_mo.empty()) {
            cout << "khong co";
        } else {
            for (string tep : pcb->tep_dang_mo) {
                cout << tep << " ";
            }
        }
        cout << endl;

        cout << "- pointer: ";
        if (pcb->con_tro_tiep != nullptr) {
            cout << pcb->con_tro_tiep->ten_tien_trinh;
        } else {
            cout << "NULL";
        }
        cout << endl;

        /*
        Hướng dẫn đọc code:
        - Hàm này dùng tim_tien_trinh trước.
        - Nếu tìm thấy thì in đầy đủ các trường trong PCB.
        - Ví dụ: in_chi_tiet_pcb("P2") sẽ cho biết P2 đang ready/running ; PC là bao nhiêu ; registers có giá trị gì.
        */
    }

    void context_switch(string ten_cu, string ten_moi) {
        // context_switch -> mô phỏng chuyển CPU từ tiến trình cũ sang tiến trình mới
        PCB* tien_trinh_cu = tim_tien_trinh(ten_cu); // tìm PCB tiến trình đang chạy
        PCB* tien_trinh_moi = tim_tien_trinh(ten_moi); // tìm PCB tiến trình sẽ chạy tiếp

        if (tien_trinh_cu == nullptr || tien_trinh_moi == nullptr) {
            cout << "Khong the context switch vi thieu tien trinh." << endl;
            return;
        }

        cout << "Context switch: " << ten_cu << " -> " << ten_moi << endl;

        tien_trinh_cu->bo_dem_chuong_trinh += 5; // giả lập P2 đã chạy thêm 5 lệnh trước khi bị chuyển
        tien_trinh_cu->thanh_ghi["AX"] += 1; // giả lập thanh ghi AX của tiến trình cũ thay đổi
        tien_trinh_cu->trang_thai = "ready"; // tiến trình cũ rời CPU nên chuyển về ready

        cout << "- Luu context cua " << ten_cu << " vao PCB(" << ten_cu << ")" << endl;
        cout << "  + program counter moi cua " << ten_cu << " = " << tien_trinh_cu->bo_dem_chuong_trinh << endl;
        cout << "  + AX moi cua " << ten_cu << " = " << tien_trinh_cu->thanh_ghi["AX"] << endl;
        cout << "  + state moi cua " << ten_cu << " = " << tien_trinh_cu->trang_thai << endl;

        tien_trinh_moi->trang_thai = "running"; // tiến trình mới được CPU chạy
        cout << "- Nap context cua " << ten_moi << " tu PCB(" << ten_moi << ")" << endl;
        cout << "  + program counter cua " << ten_moi << " = " << tien_trinh_moi->bo_dem_chuong_trinh << endl;
        cout << "  + AX cua " << ten_moi << " = " << tien_trinh_moi->thanh_ghi["AX"] << endl;
        cout << "  + state moi cua " << ten_moi << " = " << tien_trinh_moi->trang_thai << endl;

        /*
        Hướng dẫn đọc code:
        - Context switch gồm 2 phần chính: lưu context cũ ; nạp context mới.
        - Với P2→P4: lưu program counter ; registers ; state của P2 vào PCB(P2).
        - Sau đó nạp program counter ; registers ; state của P4 từ PCB(P4).
        - Ví dụ: CPU đang chạy P2 ; hệ điều hành chọn P4 ; P2 thành ready ; P4 thành running.
        */
    }

    void tinh_do_phuc_tap() {
        // tinh_do_phuc_tap -> in độ phức tạp của từng thao tác trong linked list
        cout << "Do phuc tap thao tac:" << endl;
        cout << "- Them P5 vao cuoi voi tail: O(1)" << endl;
        cout << "- Them P0 vao dau voi head: O(1)" << endl;
        cout << "- Xoa P3 neu phai tim tu head: O(n)" << endl;
        cout << "- Xoa P3 neu da biet node truoc P2: O(1)" << endl;
        cout << "- Duyet danh sach: O(n)" << endl;
        cout << "- Context switch P2 -> P4: O(1)" << endl;

        /*
        Hướng dẫn đọc code:
        - O(1) nghĩa là thời gian hằng số ; không phụ thuộc số lượng tiến trình.
        - O(n) nghĩa là có thể phải duyệt qua n tiến trình.
        - Trong bài này thêm cuối có tail nên O(1) ; thêm đầu có head nên O(1) ; duyệt toàn bộ danh sách nên O(n).
        */
    }

    ~DanhSachTienTrinh() {
        // destructor -> giải phóng toàn bộ PCB còn lại để tránh rò rỉ bộ nhớ
        PCB* hien_tai = head;

        while (hien_tai != nullptr) {
            PCB* node_sau = hien_tai->con_tro_tiep; // lưu node sau trước khi xóa node hiện tại
            delete hien_tai; // giải phóng node hiện tại
            hien_tai = node_sau; // chuyển sang node sau
        }

        head = nullptr;
        tail = nullptr;

        /*
        Hướng dẫn đọc code:
        - Vì dùng new để tạo PCB nên cần delete để giải phóng.
        - Destructor tự chạy khi object DanhSachTienTrinh kết thúc vòng đời.
        - Ví dụ: cuối hàm main ; danh_sach bị hủy ; toàn bộ PCB còn lại được giải phóng.
        */
    }
};

void in_tieu_de(string noi_dung) {
    // in_tieu_de -> in nhãn ngắn giúp dễ quan sát kết quả từng bước
    cout << endl;
    cout << "===== " << noi_dung << " =====" << endl;

    /*
    Hướng dẫn đọc code:
    - Hàm này chỉ giúp output dễ nhìn.
    - Ví dụ: in_tieu_de("Sau khi them P5") sẽ in một dòng phân cách có nội dung tương ứng.
    */
}

int main() {
    DanhSachTienTrinh danh_sach;

    in_tieu_de("Khoi tao danh sach P1 -> P2 -> P3 -> P4");

    PCB* P1 = danh_sach.tao_pcb(
        "P1",
        "ready",
        100,
        {{"AX", 1}, {"BX", 2}, {"CX", 3}},
        "0KB-99KB",
        {"input.txt"}
    );

    PCB* P2 = danh_sach.tao_pcb(
        "P2",
        "running",
        200,
        {{"AX", 10}, {"BX", 20}, {"CX", 30}},
        "100KB-199KB",
        {"data.csv"}
    );

    PCB* P3 = danh_sach.tao_pcb(
        "P3",
        "waiting",
        300,
        {{"AX", 7}, {"BX", 8}, {"CX", 9}},
        "200KB-299KB",
        {"log.txt"}
    );

    PCB* P4 = danh_sach.tao_pcb(
        "P4",
        "ready",
        400,
        {{"AX", 40}, {"BX", 50}, {"CX", 60}},
        "300KB-399KB",
        {"output.txt"}
    );

    danh_sach.them_vao_cuoi(P1); // danh sách: P1
    danh_sach.them_vao_cuoi(P2); // danh sách: P1→P2
    danh_sach.them_vao_cuoi(P3); // danh sách: P1→P2→P3
    danh_sach.them_vao_cuoi(P4); // danh sách: P1→P2→P3→P4

    danh_sach.duyet_danh_sach();

    cout << "head = " << danh_sach.lay_head()->ten_tien_trinh << endl;
    cout << "tail = " << danh_sach.lay_tail()->ten_tien_trinh << endl;

    in_tieu_de("Them P5 vao cuoi");

    PCB* P5 = danh_sach.tao_pcb(
        "P5",
        "ready",
        500,
        {{"AX", 5}, {"BX", 15}, {"CX", 25}},
        "400KB-499KB",
        {"p5.txt"}
    );

    danh_sach.them_vao_cuoi(P5); // P4.pointer = P5 ; tail = P5
    danh_sach.duyet_danh_sach();

    cout << "head = " << danh_sach.lay_head()->ten_tien_trinh << endl;
    cout << "tail = " << danh_sach.lay_tail()->ten_tien_trinh << endl;

    in_tieu_de("Them P0 vao dau");

    PCB* P0 = danh_sach.tao_pcb(
        "P0",
        "ready",
        50,
        {{"AX", 0}, {"BX", 0}, {"CX", 0}},
        "0KB-49KB",
        {"start.txt"}
    );

    danh_sach.them_vao_dau(P0); // P0.pointer = P1 ; head = P0
    danh_sach.duyet_danh_sach();

    cout << "head = " << danh_sach.lay_head()->ten_tien_trinh << endl;
    cout << "tail = " << danh_sach.lay_tail()->ten_tien_trinh << endl;

    in_tieu_de("Xoa P3");

    bool xoa_thanh_cong = danh_sach.xoa_tien_trinh("P3"); // P2.pointer = P4

    if (xoa_thanh_cong) {
        cout << "Da xoa P3 thanh cong." << endl;
    } else {
        cout << "Khong tim thay P3 de xoa." << endl;
    }

    danh_sach.duyet_danh_sach();

    cout << "head = " << danh_sach.lay_head()->ten_tien_trinh << endl;
    cout << "tail = " << danh_sach.lay_tail()->ten_tien_trinh << endl;

    in_tieu_de("In chi tiet PCB cua P2 va P4 truoc context switch");

    danh_sach.in_chi_tiet_pcb("P2");
    danh_sach.in_chi_tiet_pcb("P4");

    in_tieu_de("Context switch giua P2 va P4");

    danh_sach.context_switch("P2", "P4");

    in_tieu_de("In chi tiet PCB cua P2 va P4 sau context switch");

    danh_sach.in_chi_tiet_pcb("P2");
    danh_sach.in_chi_tiet_pcb("P4");

    in_tieu_de("Do phuc tap");

    danh_sach.tinh_do_phuc_tap();

    return 0;
}

/*
Testcase đầy đủ kiểm tra toàn bộ trường hợp:

1. Khởi tạo danh sách ban đầu:
Input thao tác:
- Tạo P1 ; P2 ; P3 ; P4.
- Thêm lần lượt vào cuối.
Expected output:
- Danh sách: P1 -> P2 -> P3 -> P4
- head = P1
- tail = P4

2. Thêm P5 vào cuối:
Input thao tác:
- them_vao_cuoi(P5)
Expected output:
- Danh sách: P1 -> P2 -> P3 -> P4 -> P5
- head = P1
- tail = P5
Giải thích:
- P4.con_tro_tiep = P5
- P5.con_tro_tiep = NULL
- tail = P5
- Độ phức tạp: O(1)

3. Thêm P0 vào đầu:
Input thao tác:
- them_vao_dau(P0)
Expected output:
- Danh sách: P0 -> P1 -> P2 -> P3 -> P4 -> P5
- head = P0
- tail = P5
Giải thích:
- P0.con_tro_tiep = P1
- head = P0
- tail không đổi
- Độ phức tạp: O(1)

4. Xóa P3:
Input thao tác:
- xoa_tien_trinh("P3")
Expected output:
- Đã xóa P3 thành công.
- Danh sách: P0 -> P1 -> P2 -> P4 -> P5
- head = P0
- tail = P5
Giải thích:
- Tìm node trước P3 là P2.
- Cập nhật P2.con_tro_tiep = P4.
- Giải phóng PCB(P3).
- Độ phức tạp: O(n) nếu phải tìm từ head ; O(1) nếu đã biết P2.

5. Duyệt danh sách:
Input thao tác:
- duyet_danh_sach()
Expected output:
- P0 -> P1 -> P2 -> P4 -> P5
Giải thích:
- Bắt đầu từ head = P0.
- Đi theo pointer cho đến NULL.
- Độ phức tạp: O(n).

6. Context switch P2 -> P4:
Input thao tác:
- context_switch("P2", "P4")
Expected output ý nghĩa:
- Lưu context của P2 vào PCB(P2): program counter ; registers ; process state.
- Nạp context của P4 từ PCB(P4): program counter ; registers ; process state.
- P2 chuyển về ready.
- P4 chuyển sang running.
- Độ phức tạp: O(1).

7. Kiểm tra xóa phần tử không tồn tại:
Input thao tác gợi ý thêm:
- xoa_tien_trinh("P9")
Expected output:
- false hoặc thông báo không tìm thấy P9.
Giải thích:
- Duyệt hết danh sách mà không thấy P9.
- Danh sách không thay đổi.

8. Kiểm tra xóa head:
Input thao tác gợi ý thêm:
- xoa_tien_trinh("P0")
Expected output:
- Danh sách: P1 -> P2 -> P4 -> P5
- head = P1
Giải thích:
- Nếu xóa head thì head = head.con_tro_tiep.

9. Kiểm tra xóa tail:
Input thao tác gợi ý thêm:
- xoa_tien_trinh("P5")
Expected output:
- Danh sách: P1 -> P2 -> P4
- tail = P4
Giải thích:
- Nếu xóa tail thì tail phải cập nhật về node trước đó.

10. Kiểm tra danh sách rỗng:
Input thao tác gợi ý thêm:
- Tạo DanhSachTienTrinh mới.
- Gọi xoa_tien_trinh("P1").
- Gọi duyet_danh_sach().
Expected output:
- Không lỗi chương trình.
- Xóa trả về false.
- Danh sách rỗng.
*/