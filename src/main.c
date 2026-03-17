#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <stdint.h>
#include <signal.h>

#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

uint64_t T = 0;
uint64_t X = 1000000;
int t_updated = 0;
volatile int running = 1; 

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void handle_sig(int sig) {
    running = 0;
    pthread_cond_broadcast(&cond);
}

uint64_t current_time_ns() {
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }
    return (uint64_t)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}

/* Thread 1: SAMPLE */
static void * thread_sample(void *arg) {
    struct timespec req;
    while (running) {
        uint64_t now = current_time_ns();
        
        pthread_mutex_lock(&mtx);
        T = now;
        t_updated = 1;
        uint64_t current_x = X; 
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mtx);

        req.tv_sec = current_x / 1000000000ULL;
        req.tv_nsec = current_x % 1000000000ULL;
        
        if (nanosleep(&req, NULL) == -1 && errno != EINTR) {
        }
    }
    return NULL;
}

/* Thread 2: INPUT */
static void * thread_input(void *arg) {
    while (running) {
        FILE *fp = fopen("logs/freq.txt", "r");
        if (fp != NULL) {
            uint64_t new_x;
            if (fscanf(fp, "%llu", &new_x) == 1) {
                pthread_mutex_lock(&mtx);
                X = new_x;
                pthread_mutex_unlock(&mtx);
            }
            fclose(fp);
        }
        usleep(100000); 
    }
    return NULL;
}

/* Thread 3: LOGGING */
static void * thread_logging(void *arg) {
    FILE *fp = fopen("logs/time_and_interval.txt", "w");
    if (fp == NULL) {
        perror("fopen logs/time_and_interval.txt");
        return NULL;
    }

    uint64_t prev_T = 0;

    while (running) {
        pthread_mutex_lock(&mtx);
        while (t_updated == 0 && running) {
            pthread_cond_wait(&cond, &mtx);
        }
        if (!running) {
            pthread_mutex_unlock(&mtx);
            break;
        }
        uint64_t current_T = T;
        uint64_t current_X = X; 
        t_updated = 0;
        pthread_mutex_unlock(&mtx);

        uint64_t interval = (prev_T == 0) ? 0 : (current_T - prev_T);
        prev_T = current_T;

        fprintf(fp, "%llu %llu %llu\n", current_T, interval, current_X);
        fflush(fp); 
    }

    fclose(fp);
    return NULL;
}

int main(int argc, char *argv[]) {
    int s;
    pthread_t t1, t2, t3;

    signal(SIGINT, handle_sig);
    signal(SIGTERM, handle_sig);

    FILE *fp = fopen("logs/freq.txt", "w");
    if (fp) {
        fprintf(fp, "1000000\n");
        fclose(fp);
    }

    s = pthread_create(&t1, NULL, thread_sample, NULL);
    if (s != 0) handle_error_en(s, "pthread_create sample");

    s = pthread_create(&t2, NULL, thread_input, NULL);
    if (s != 0) handle_error_en(s, "pthread_create input");

    s = pthread_create(&t3, NULL, thread_logging, NULL);
    if (s != 0) handle_error_en(s, "pthread_create logging");

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    exit(EXIT_SUCCESS);
}
