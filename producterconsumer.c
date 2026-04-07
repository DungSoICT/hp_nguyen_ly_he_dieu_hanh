#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define TOTAL_ITEMS 20

/* Vùng đệm hữu hạn ++ Biến đếm Counter
   Buffer là mảng vòng có kích thước cố định BUFFER_SIZE
   IN là vị trí ghi tiếp theo
   OUT là vị trí đọc tiếp theo
   Counter là số phần tử hiện có trong buffer , luôn cần giữ 0 ≤ Counter ≤ BUFFER_SIZE */
int Buffer[BUFFER_SIZE];
int IN = 0;
int OUT = 0;
int Counter = 0;

/* Đồng bộ tiến trình ++ Cơ chế chờ bận
   Biến done dùng để báo Producer đã sản xuất xong toàn bộ dữ liệu
   Consumer sẽ dựa vào done và Counter để quyết định dừng */
int done = 0;

/* Hàm hiển thị trạng thái hệ thống
   Giúp nhìn rõ lời giải đang hoạt động ở đâu trong quá trình chạy */
void print_state(const char *who, const char *action, int value) {
    printf("%s | %s | value = %d | IN = %d | OUT = %d | Counter = %d | Buffer = [",
           who, action, value, IN, OUT, Counter);

    for (int i = 0; i < BUFFER_SIZE; i++) {
        printf("%d", Buffer[i]);
        if (i < BUFFER_SIZE - 1) printf(", ");
    }
    printf("]\n");
}

/* Hàm sinh dữ liệu
   Producer tạo dữ liệu cụ thể để đưa vào buffer */
int produce_item(int x) {
    return x * 10;
}

/* Hàm xử lý dữ liệu
   Consumer tiêu thụ dữ liệu lấy ra từ buffer */
void consume_item(int item) {
    printf("Consumer xử lý dữ liệu: %d\n", item);
}

/* Producer thread */
void *producer(void *arg) {
    for (int i = 1; i <= TOTAL_ITEMS; i++) {
        int nextProduced = produce_item(i);

        /* Lời giải ý 2: điều kiện hoạt động của Producer ++ busy waiting
           yếu cố cần có:
           - Counter = BUFFER_SIZE thì buffer đầy
           - Producer không được ghi khi buffer đầy
           áp dụng:
           - Dùng while(Counter == BUFFER_SIZE) để chờ bận
           - Đây chính là cơ chế đồng bộ kiểu busy waiting trong bài toán gốc */
        while (Counter == BUFFER_SIZE) {
            /* chờ bận , không làm gì */
        }

        /* Lời giải ý 1: phân tích cấu trúc vùng đệm ++ mảng vòng
           yếu cố cần có:
           - Ghi dữ liệu vào Buffer[IN]
           - Sau đó phải cập nhật IN theo công thức vòng
           áp dụng:
           - Buffer[IN] = nextProduced
           - IN = (IN + 1) % BUFFER_SIZE để không vượt biên */
        Buffer[IN] = nextProduced;
        IN = (IN + 1) % BUFFER_SIZE;

        /* Lời giải ý 2: sau khi ghi thành công phải tăng Counter
           yếu cố cần có:
           - Producer vừa đưa thêm 1 sản phẩm vào vùng đệm
           áp dụng:
           - Counter++ để phản ánh số phần tử mới trong buffer */
        Counter++;

        print_state("Producer", "cho vô buffer", nextProduced);

        usleep(100000);
    }

    /* Producer báo đã hoàn tất sản xuất */
    done = 1;
    return NULL;
}

/* Consumer thread */
void *consumer(void *arg) {
    while (1) {

        /* Lời giải ý 3: điều kiện hoạt động của Consumer ++ busy waiting
           yếu cố cần có:
           - Counter = 0 thì buffer rỗng
           - Consumer không được đọc khi chưa có dữ liệu
           áp dụng:
           - Dùng while(Counter == 0) để chờ
           - Nếu Producer đã xong và Counter = 0 thì thoát hẳn */
        while (Counter == 0) {
            if (done) {
                return NULL;
            }
            /* chờ bận , không làm gì */
        }

        /* Lời giải ý 3: lấy dữ liệu ra khỏi buffer
           yếu cố cần có:
           - Dữ liệu hợp lệ luôn nằm ở Buffer[OUT]
           áp dụng:
           - nextConsumed = Buffer[OUT]
           - Cập nhật OUT theo mảng vòng */
        int nextConsumed = Buffer[OUT];
        Buffer[OUT] = 0;  /* chỉ để dễ quan sát trạng thái */
        OUT = (OUT + 1) % BUFFER_SIZE;

        /* Lời giải ý 3: sau khi đọc thành công phải giảm Counter
           yếu cố cần có:
           - Vừa lấy đi 1 phần tử khỏi buffer
           áp dụng:
           - Counter-- để phản ánh số phần tử còn lại */
        Counter--;

        print_state("Consumer", "lấy ra khỏi buffer", nextConsumed);

        consume_item(nextConsumed);

        usleep(150000);
    }
}

/* Hàm main */
int main() {
    pthread_t producer_thread;
    pthread_t consumer_thread;

    /* Khởi tạo buffer về 0 để dễ theo dõi */
    for (int i = 0; i < BUFFER_SIZE; i++) {
        Buffer[i] = 0;
    }

    /* Lời giải ý 4: cơ chế đồng bộ hiện tại
       yếu cố cần có:
       - Producer và Consumer chạy đồng thời
       - Cùng truy cập Buffer , IN , OUT , Counter
       áp dụng:
       - Tạo 2 luồng song song để mô phỏng đúng bản chất producer-consumer
       - Phiên bản này cố ý dùng busy waiting theo đúng chủ đề bài toán */
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    /* Lời giải ý 5: kiểm tra tính đúng đắn cuối cùng
       yếu cố cần có:
       - Sau khi chương trình kết thúc , toàn bộ dữ liệu đã được tiêu thụ
       - Counter kỳ vọng quay về 0
    áp dụng:
       - In trạng thái cuối để kiểm chứng 0 ≤ Counter ≤ BUFFER_SIZE
       - Thường kết thúc với Counter = 0 nếu Consumer đã lấy hết */
    printf("\ntrạng thái cuối cùng:\n");
    print_state("Main", "kết thúc chương trình", -1);

    return 0;
}