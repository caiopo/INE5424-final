#include <process.h>
#include <time.h>

using namespace EPOS;

OStream cout;

const int IO_THREADS = 4;
const int CPU_THREADS = 8;

Thread * io_threads[IO_THREADS];
Thread * cpu_threads[CPU_THREADS];
int results[CPU_THREADS];

int io_thread(int thread_id) {
    for(int i = 0; i < 10; i++) {
        cout << "IO Thread " << thread_id << " / Core " << Machine::cpu_id() << " / Iteration " << i << endl;
        Alarm::delay(10000);
    }
    return 0;
}

int cpu_thread(int thread_id) {
    int result = 0;
    for(int i = 0; i < 1000000000; i++) {
        result = i;
    }
    return result / (thread_id+1);
}

int main() {
    for (int j = 0; j < CPU_THREADS; j++) {
        cpu_threads[j] = new Thread(&cpu_thread, j);
    }

    for (int i = 0; i < IO_THREADS; i++) {
        io_threads[i] = new Thread(&io_thread, i);
    }

    for (int i = 0; i < IO_THREADS; i++) {
        io_threads[i]->join();
    }

    for (int j = 0; j < CPU_THREADS; j++) {
        results[j] = cpu_threads[j]->join();
    }

    for (int i = 0; i < IO_THREADS; i++) {
        cout << "IO Thread " << i <<  " / Priority " << io_threads[i]->priority() << endl;
    }

    for (int j = 0; j < CPU_THREADS; j++) {
         cout << "CPU Thread " << j << " / Priority " << cpu_threads[j]->priority() << " / Result " << results[j] << endl;
    }

    for (int i = 0; i < IO_THREADS; i++) {
        delete io_threads[i];
    }

    for (int j = 0; j < CPU_THREADS; j++) {
        delete cpu_threads[j];
    }

    return 0;
}