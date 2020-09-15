
// Spinning ticket lock
typedef struct ticketlock {
    uint ticket;        // current ticket number being served
    uint turn;          // next ticket number to be given
    struct proc *proc;  // process currently holding the lock
    // for debugging:
    char *name;         // Name of lock
    struct cpu *cpu;    // The cpu holding the lock
    uint pcs[10];       // that locked the lock
} ticketlock;

// adding three systems calls
void initlock_t(struct ticketlock *lk);
void acquire_t(struct ticketlock *lk);
void release_t(struct ticketlock *lk);

// atomic fetch-and-add operation
static inline uint 
fetch_and_add(volatile uint *addr, uint val)
{
    asm volatile("lock; xaddl %%eax, %2;" :
                "=a" (val)  :
                "a" (val) , "m" (*addr) :
                "memory");
    return val;
}