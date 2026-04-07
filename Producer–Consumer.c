
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX 1000
#define PRODUCER_COUNT 3
#define CONSUMER_COUNT 3
#define ITEMS_PER_PRODUCER 20

/*
  Tên chủ đề: Bài toán Producer–Consumer
  Tên đoạn mã: SafeQueue
  Tên ý tưởng: Bounded buffer = queue có sức chứa tối đa MAX; khi full thì producer phải chờ, khi empty thì consumer phải chờ.
  Tên ý tưởng: Mutex = khóa vùng dữ liệu chung để nhiều thread không sửa queue cùng lúc.
  Tên ý tưởng: Condvar = biến điều kiện; nó cho thread "ngủ chờ" đến khi điều kiện đúng thay vì quay vòng tốn CPU.
  Tên ý tưởng: notify_one / notify_all = trong C với pthread_cond_signal tương ứng đánh thức 1 thread, pthread_cond_broadcast tương ứng đánh thức tất cả thread đang chờ.
*/
typedef struct 
{
    int data[MAX];
    int head;
    int tail;
    int size;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} SafeQueue;

/*
  Tên đoạn mã: safe_queue_init
  Cách hoạt động: khởi tạo queue rỗng; head là vị trí lấy ra, tail là vị trí thêm vào, size là số phần tử hiện có.
*/
void safe_queue_init(SafeQueue *q) {
    q->head = 0;
    q->tail = 0;
    q->size = 0;
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->not_empty, NULL);
    pthread_cond_init(&q->not_full, NULL);
}

/*
  Tên đoạn mã: safe_queue_destroy
  Cách hoạt động: giải phóng tài nguyên đồng bộ sau khi dùng xong.
*/
void safe_queue_destroy(SafeQueue *q) {
    pthread_mutex_destroy(&q->mutex);
    pthread_cond_destroy(&q->not_empty);
    pthread_cond_destroy(&q->not_full);
}

/*
  Tên đoạn mã: enqueue
  Tên ý tưởng: producer chỉ được thêm khi size < MAX.
  Cách hoạt động: lock mutex → nếu queue đầy thì wait trên not_full → còn chỗ thì ghi dữ liệu vào tail → cập nhật tail và size → signal not_empty để báo đã có dữ liệu.
  Điểm quan trọng: dùng while chứ không dùng if vì sau khi thức dậy điều kiện có thể đã bị thread khác làm thay đổi.
*/
void enqueue(SafeQueue *q, int value) {
    pthread_mutex_lock(&q->mutex);

    while (q->size == MAX) {
        pthread_cond_wait(&q->not_full, &q->mutex);
    }

    q->data[q->tail] = value;
    q->tail = (q->tail + 1) % MAX;
    q->size++;

    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->mutex);
}

/*
  Tên đoạn mã: dequeue
  Tên ý tưởng: consumer chỉ được lấy khi size > 0.
  Cách hoạt động: lock mutex → nếu queue rỗng thì wait trên not_empty → có dữ liệu thì lấy ở head → cập nhật head và size → signal not_full để báo đã có chỗ trống.
  Điểm quan trọng: cũng phải dùng while để kiểm tra lại điều kiện sau khi thức dậy.
*/
int dequeue(SafeQueue *q) {
    pthread_mutex_lock(&q->mutex);

    while (q->size == 0) {
        pthread_cond_wait(&q->not_empty, &q->mutex);
    }

    int value = q->data[q->head];
    q->head = (q->head + 1) % MAX;
    q->size--;

    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->mutex);
    return value;
}

/*
  Tên đoạn mã: dequeue_busy_wait
  Tên ý tưởng: Busy waiting = liên tục khóa, kiểm tra, mở khóa rồi lặp lại.
  Cách hoạt động: nếu queue rỗng thì không ngủ chờ đúng nghĩa mà cứ kiểm tra mãi; có thêm usleep thì chỉ giảm nóng CPU chứ vẫn là polling.
  Kết luận: cách này thread-safe ở mức bảo vệ dữ liệu, nhưng không phải synchronous blocking queue tốt vì vẫn lãng phí CPU và có độ trễ.
*/
int dequeue_busy_wait(SafeQueue *q) {
    for (;;) {
        pthread_mutex_lock(&q->mutex);

        if (q->size > 0) {
            int value = q->data[q->head];
            q->head = (q->head + 1) % MAX;
            q->size--;
            pthread_mutex_unlock(&q->mutex);
            return value;
        }

        pthread_mutex_unlock(&q->mutex);
        usleep(1000);
    }
}

typedef struct {
    SafeQueue *queue;
    int id;
} ThreadArg;

/*
  Tên đoạn mã: producer_thread
  Cách hoạt động: mỗi producer tạo ITEMS_PER_PRODUCER dữ liệu và gọi enqueue; nếu queue đầy thì producer sẽ tự ngủ bên trong enqueue.
*/
void *producer_thread(void *arg) {
    ThreadArg *info = (ThreadArg *)arg;

    for (int i = 0; i < ITEMS_PER_PRODUCER; i++) {
        int value = info->id * 1000 + i;
        enqueue(info->queue, value);
        printf("Producer %d -> %d\n", info->id, value);
        usleep(50000);
    }

    return NULL;
}

/*
  Tên đoạn mã: consumer_thread
  Cách hoạt động: mỗi consumer lấy đúng số lượng phần tử đã định; nếu queue rỗng thì consumer sẽ tự ngủ bên trong dequeue.
*/
void *consumer_thread(void *arg) {
    ThreadArg *info = (ThreadArg *)arg;
    int items_to_consume = (PRODUCER_COUNT * ITEMS_PER_PRODUCER) / CONSUMER_COUNT;

    for (int i = 0; i < items_to_consume; i++) {
        int value = dequeue(info->queue);
        printf("Consumer %d <- %d\n", info->id, value);
        usleep(80000);
    }

    return NULL;
}

int main(void) {
    SafeQueue queue;
    pthread_t producers[PRODUCER_COUNT];
    pthread_t consumers[CONSUMER_COUNT];
    ThreadArg producer_args[PRODUCER_COUNT];
    ThreadArg consumer_args[CONSUMER_COUNT];

    safe_queue_init(&queue);

    for (int i = 0; i < PRODUCER_COUNT; i++) {
        producer_args[i].queue = &queue;
        producer_args[i].id = i + 1;
        pthread_create(&producers[i], NULL, producer_thread, &producer_args[i]);
    }

    for (int i = 0; i < CONSUMER_COUNT; i++) {
        consumer_args[i].queue = &queue;
        consumer_args[i].id = i + 1;
        pthread_create(&consumers[i], NULL, consumer_thread, &consumer_args[i]);
    }

    for (int i = 0; i < PRODUCER_COUNT; i++) {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < CONSUMER_COUNT; i++) {
        pthread_join(consumers[i], NULL);
    }

    safe_queue_destroy(&queue);
    return 0;
}