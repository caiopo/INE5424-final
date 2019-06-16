#include <process.h>
#include <time.h>

using namespace EPOS;

const int NUM_THREADS = 10;

OStream cout;

Thread * threads[NUM_THREADS];

int print_cpu_id(int thread_id) {
    for (int i = 0; i < 10; i++) {
        cout << "Thread " << thread_id << " / Core " << Machine::cpu_id() << " / Iteration " << i << endl;
        Alarm::delay(100000 * thread_id); // 1s
    }

    return 0;
}


int main() {
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i] = new Thread(&print_cpu_id, i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i]->join();
        delete threads[i];
    }
}