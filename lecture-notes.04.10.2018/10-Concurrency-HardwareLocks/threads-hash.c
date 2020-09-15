#define SKIP_MAIN
#include "threads-list-simple.c"

#define HASH_BUCKETS (7)
typedef struct __hash_t {
    list_t hlists[HASH_BUCKETS];
    pthread_mutex_t m[HASH_BUCKETS];
} hash_t;

void Hash_Init(hash_t *H) {
    int i;
    for (i = 0; i < HASH_BUCKETS; i++) {
	List_Init(&H->hlists[i]);
    }
}

void Hash_Insert(hash_t *H, int key) {
    int b = key % HASH_BUCKETS;
    List_Insert(&H->hlists[b], key);
}

void Hash_Print(hash_t *H) {
    int i;
    for (i = 0; i < HASH_BUCKETS; i++) {
	printf("LIST %d: ", i);
	List_Print(&H->hlists[i]);
    }
}

int main(int argc, char *argv[]) {
    hash_t myhash;
    Hash_Init(&myhash);
    Hash_Insert(&myhash, 10);
    Hash_Insert(&myhash, 5);
    Hash_Insert(&myhash, 30);
    Hash_Print(&myhash);
    return 0;
}

