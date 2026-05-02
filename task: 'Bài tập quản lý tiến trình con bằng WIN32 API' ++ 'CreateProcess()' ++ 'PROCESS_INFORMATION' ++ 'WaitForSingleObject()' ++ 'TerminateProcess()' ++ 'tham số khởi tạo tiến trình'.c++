/*🔍'Win32 Process Management' */
/*code_hoan_chinh_co_chu_thich :'Quản lý tiến trình con bằng WIN32 API' ++ 'tạo tiến trình con' ++ 'chờ tiến trình con' ++ 'kết thúc tiến trình nếu quá thời gian' ++ 'đóng handle'*/

/*
Đề bài:
Viết chương trình C++ trên Windows dùng WIN32 API để tạo tiến trình con chạy Notepad mở file D:\test\input.txt;
chương trình cha chờ tiến trình con tối đa 5000ms;
nếu tiến trình con tự kết thúc trước 5000ms thì không làm gì thêm;
nếu sau 5000ms tiến trình con vẫn chạy thì dùng TerminateProcess() để kết thúc cưỡng bức với mã kết thúc = 1;
cuối cùng đóng các handle để tránh rò rỉ tài nguyên.
*/

#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

/* tạo_tien_trinh_con: tạo tiến trình con bằng CreateProcess() */
bool tao_tien_trinh_con(PROCESS_INFORMATION &thong_tin_tien_trinh) {
    STARTUPINFO thong_tin_khoi_dong;                                      // STARTUPINFO: chứa cấu hình khởi động của tiến trình mới

    ZeroMemory(&thong_tin_khoi_dong, sizeof(thong_tin_khoi_dong));        // xóa sạch dữ liệu rác trong STARTUPINFO
    ZeroMemory(&thong_tin_tien_trinh, sizeof(thong_tin_tien_trinh));      // xóa sạch dữ liệu rác trong PROCESS_INFORMATION

    thong_tin_khoi_dong.cb = sizeof(thong_tin_khoi_dong);                 // cb: bắt buộc gán kích thước cấu trúc STARTUPINFO

    TCHAR dong_lenh[] = TEXT("notepad.exe D:\\test\\input.txt");          // LPTSTR: chuỗi dòng lệnh; cần là mảng có thể sửa đổi

    BOOL ket_qua_tao = CreateProcess(
        NULL,                                                             // LPCTSTR lpApplicationName: NULL vì tên chương trình đã nằm trong dong_lenh
        dong_lenh,                                                        // LPTSTR lpCommandLine: lệnh chạy Notepad và tham số file cần mở
        NULL,                                                             // LPSECURITY_ATTRIBUTES lpProcessAttributes: dùng bảo mật mặc định cho tiến trình
        NULL,                                                             // LPSECURITY_ATTRIBUTES lpThreadAttributes: dùng bảo mật mặc định cho luồng chính
        FALSE,                                                            // BOOL bInheritHandles: không cho tiến trình con kế thừa handle từ cha
        CREATE_NEW_CONSOLE,                                               // DWORD dwCreationFlags: tạo tiến trình với console mới nếu phù hợp
        NULL,                                                             // LPVOID lpEnvironment: dùng môi trường mặc định của tiến trình cha
        NULL,                                                             // LPCTSTR lpCurrentDirectory: dùng thư mục hiện tại mặc định
        &thong_tin_khoi_dong,                                             // LPSTARTUPINFO lpStartupInfo: truyền thông tin khởi động
        &thong_tin_tien_trinh                                             // LPPROCESS_INFORMATION lpProcessInformation: nhận handle và ID của process/thread mới
    );

    if (ket_qua_tao == FALSE) {                                           // nếu CreateProcess() thất bại
        cout << "CreateProcess() that bai; ma loi = " << GetLastError() << endl; // GetLastError(): lấy mã lỗi gần nhất của Windows
        return false;                                                     // báo tạo tiến trình thất bại
    }

    cout << "Tao tien trinh con thanh cong." << endl;                     // thông báo tạo thành công
    cout << "Process ID = " << thong_tin_tien_trinh.dwProcessId << endl;  // dwProcessId: mã định danh tiến trình con
    cout << "Thread ID = " << thong_tin_tien_trinh.dwThreadId << endl;    // dwThreadId: mã định danh luồng chính của tiến trình con

    return true;                                                          // báo tạo tiến trình thành công

    /*
    Hướng dẫn đọc hàm:
    1) Tạo 2 cấu trúc STARTUPINFO và PROCESS_INFORMATION;
    2) Xóa dữ liệu rác bằng ZeroMemory();
    3) Chuẩn bị chuỗi lệnh "notepad.exe D:\test\input.txt";
    4) Gọi CreateProcess() để tạo tiến trình con;
    5) Nếu thành công thì PROCESS_INFORMATION sẽ chứa hProcess, hThread, process ID, thread ID.

    Ví dụ hoạt động:
    Khi chạy chương trình cha; hàm này sẽ mở Notepad và yêu cầu Notepad mở file D:\test\input.txt.
    */
}

/* cho_tien_trinh_cho_va_ket_thuc_neu_qua_han: chờ tiến trình con tối đa 5000ms; nếu timeout thì kết thúc cưỡng bức */
void cho_tien_trinh_cho_va_ket_thuc_neu_qua_han(PROCESS_INFORMATION &thong_tin_tien_trinh) {
    const DWORD thoi_gian_cho_ms = 5000;                                  // dwMs: thời gian chờ tối đa = 5000ms = 5 giây
    const UINT ma_ket_thuc_cuong_buc = 1;                                 // uExitCode: mã kết thúc dùng khi TerminateProcess()

    cout << "Dang cho tien trinh con toi da " << thoi_gian_cho_ms << "ms..." << endl;

    DWORD ket_qua_cho = WaitForSingleObject(
        thong_tin_tien_trinh.hProcess,                                    // HANDLE hHandle: handle của tiến trình con cần chờ
        thoi_gian_cho_ms                                                  // DWORD dwMs: chờ tối đa 5000ms; không dùng INFINITE vì đề bài yêu cầu timeout
    );

    if (ket_qua_cho == WAIT_OBJECT_0) {                                    // WAIT_OBJECT_0: tiến trình con đã kết thúc trong thời gian chờ
        cout << "Tien trinh con da tu ket thuc truoc khi qua han." << endl;
    } else if (ket_qua_cho == WAIT_TIMEOUT) {                              // WAIT_TIMEOUT: hết 5000ms nhưng tiến trình con vẫn chưa kết thúc
        cout << "Qua " << thoi_gian_cho_ms << "ms; tien trinh con van chay." << endl;
        cout << "Goi TerminateProcess() de ket thuc cuong buc." << endl;

        BOOL ket_qua_ket_thuc = TerminateProcess(
            thong_tin_tien_trinh.hProcess,                                // HANDLE hProcess: tiến trình con bị kết thúc
            ma_ket_thuc_cuong_buc                                         // UINT uExitCode: mã kết thúc = 1
        );

        if (ket_qua_ket_thuc == FALSE) {                                   // nếu TerminateProcess() thất bại
            cout << "TerminateProcess() that bai; ma loi = " << GetLastError() << endl;
        } else {                                                           // nếu TerminateProcess() thành công
            cout << "Da gui lenh ket thuc tien trinh con voi exit code = " << ma_ket_thuc_cuong_buc << endl;
        }
    } else {                                                               // các trường hợp lỗi khác khi WaitForSingleObject()
        cout << "WaitForSingleObject() that bai; ma loi = " << GetLastError() << endl;
    }

    /*
    Hướng dẫn đọc hàm:
    1) WaitForSingleObject(hProcess, 5000) nghĩa là cha chờ con tối đa 5 giây;
    2) Nếu kết quả là WAIT_OBJECT_0 -> con đã tự kết thúc -> không cần ép dừng;
    3) Nếu kết quả là WAIT_TIMEOUT -> con còn chạy sau 5 giây -> gọi TerminateProcess(hProcess, 1);
    4) Nếu kết quả khác -> thao tác chờ bị lỗi -> in mã lỗi bằng GetLastError().

    Ví dụ hoạt động:
    Nếu bạn mở Notepad rồi đóng trong vòng 5 giây; chương trình in "tự kết thúc";
    nếu bạn không đóng Notepad sau 5 giây; chương trình sẽ ép đóng Notepad.
    */
}

/* dong_handle_tien_trinh: đóng các handle đã nhận từ PROCESS_INFORMATION */
void dong_handle_tien_trinh(PROCESS_INFORMATION &thong_tin_tien_trinh) {
    if (thong_tin_tien_trinh.hThread != NULL) {                            // kiểm tra handle luồng chính có hợp lệ không
        CloseHandle(thong_tin_tien_trinh.hThread);                         // đóng handle luồng để tránh rò rỉ tài nguyên
        thong_tin_tien_trinh.hThread = NULL;                               // gán NULL để tránh dùng lại handle cũ
    }

    if (thong_tin_tien_trinh.hProcess != NULL) {                           // kiểm tra handle tiến trình có hợp lệ không
        CloseHandle(thong_tin_tien_trinh.hProcess);                        // đóng handle tiến trình để tránh rò rỉ tài nguyên
        thong_tin_tien_trinh.hProcess = NULL;                              // gán NULL để tránh dùng lại handle cũ
    }

    cout << "Da dong handle cua tien trinh va luong." << endl;

    /*
    Hướng dẫn đọc hàm:
    1) CreateProcess() trả về hProcess và hThread;
    2) Khi không cần dùng nữa; chương trình cha phải gọi CloseHandle();
    3) CloseHandle() không nhất thiết kết thúc tiến trình; nó chỉ đóng thẻ quản lý ở phía chương trình cha.

    Ví dụ hoạt động:
    Sau khi đã chờ hoặc kết thúc tiến trình con; gọi hàm này để dọn dẹp handle.
    */
}

/* main: điều phối toàn bộ chương trình */
int main() {
    PROCESS_INFORMATION thong_tin_tien_trinh;                              // lưu thông tin tiến trình con sau khi CreateProcess() thành công

    bool tao_thanh_cong = tao_tien_trinh_con(thong_tin_tien_trinh);        // gọi hàm tạo tiến trình con

    if (tao_thanh_cong == false) {                                         // nếu tạo tiến trình thất bại
        cout << "Khong the tiep tuc vi tien trinh con chua duoc tao." << endl;
        return 1;                                                          // kết thúc chương trình cha với mã lỗi 1
    }

    cho_tien_trinh_cho_va_ket_thuc_neu_qua_han(thong_tin_tien_trinh);      // chờ tiến trình con; nếu quá hạn thì kết thúc cưỡng bức
    dong_handle_tien_trinh(thong_tin_tien_trinh);                          // đóng hProcess và hThread

    return 0;                                                              // chương trình cha kết thúc bình thường

    /*
    Hướng dẫn đọc hàm main:
    1) Khai báo PROCESS_INFORMATION để nhận thông tin tiến trình con;
    2) Gọi tao_tien_trinh_con();
    3) Nếu tạo thất bại thì dừng chương trình;
    4) Nếu tạo thành công thì chờ tối đa 5000ms;
    5) Nếu con không tự kết thúc thì ép kết thúc;
    6) Cuối cùng đóng handle.

    Ví dụ hoạt động:
    Chạy chương trình -> Notepad mở file D:\test\input.txt;
    nếu đóng Notepad trong 5 giây -> chương trình báo con tự kết thúc;
    nếu không đóng Notepad -> chương trình ép đóng sau 5 giây.
    */
}

/*
Testcase đầy đủ:

Testcase 1: tiến trình con tự kết thúc trước timeout
- Chuẩn bị: tạo file D:\test\input.txt; chạy chương trình; đóng Notepad trong vòng 5 giây.
- Kết quả mong đợi: CreateProcess() thành công; WaitForSingleObject() trả WAIT_OBJECT_0; không gọi TerminateProcess(); CloseHandle() được gọi.

Testcase 2: tiến trình con bị timeout
- Chuẩn bị: tạo file D:\test\input.txt; chạy chương trình; không đóng Notepad sau 5 giây.
- Kết quả mong đợi: CreateProcess() thành công; WaitForSingleObject() trả WAIT_TIMEOUT; gọi TerminateProcess(hProcess, 1); CloseHandle() được gọi.

Testcase 3: file input không tồn tại
- Chuẩn bị: xóa hoặc không tạo D:\test\input.txt; chạy chương trình.
- Kết quả có thể gặp: Notepad vẫn có thể mở và hỏi tạo file mới; tiến trình Notepad vẫn được tạo; nếu không đóng trong 5 giây thì bị TerminateProcess().
- Ý nghĩa: CreateProcess() kiểm tra việc tạo Notepad; không đảm bảo file tham số tồn tại.

Testcase 4: không tìm thấy notepad.exe hoặc lỗi quyền hệ thống
- Chuẩn bị: môi trường Windows lỗi PATH hoặc bị chặn quyền chạy Notepad.
- Kết quả mong đợi: CreateProcess() trả FALSE; chương trình in mã lỗi từ GetLastError(); không gọi WaitForSingleObject(); không gọi TerminateProcess().

Testcase 5: kiểm tra rò rỉ handle
- Chuẩn bị: chạy chương trình nhiều lần.
- Kết quả mong đợi: mỗi lần chạy đều gọi CloseHandle(hThread) và CloseHandle(hProcess); không giữ handle thừa trong chương trình cha.

Cách biên dịch gợi ý trên Windows:
- Với Visual Studio Developer Command Prompt: cl /EHsc ten_file.cpp
- Chạy file .exe sau khi biên dịch.
*/