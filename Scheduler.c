#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

typedef struct {
    int priority;
    int burst;
    _Atomic int activate;
    _Atomic int allowance;
    int dormant;
    char name[3];
} Profile;

void* execute(void* arg) {
    Profile* s = (Profile*)arg;
    while (!s->activate);
    int remainder = s->burst;
    int balance = 0;
    while (remainder > 0) {
        while (s->dormant);
        sleep(s->allowance);
        remainder-=s->allowance;
    }
    s->burst = 0;
    printf("%s is completed \n", s->name);

    return NULL;
}

int main (void) {
    int completed[] = {0, 0, 0, 0, 0};

    Profile profiles[] = {
        {4,4,0,0,0,"T1"},
        {2,5,0,0,0,"T2"},
        {3,5,0,0,0,"T3"},
        {3,3,0,0,0,"T4"},
        {10,2,0,0,0,"T5"}
    };

    pthread_t tasks[5];

    for (int x = 0; x < 5; x++) {
        pthread_create(&tasks[x], NULL, execute, &profiles[x]);
    }

    char user_input[2];
    printf("algorithm> ");
    fgets(user_input, sizeof(user_input), stdin);

    int not_done = 1;
    int complete[] = {0,0,0,0,0};
    int pj = 0;
    int initializing = 1;
    int sj = 0; 
    switch (user_input[0]) {
        case '1':
            while (not_done) {
                for (int x = 0; x < 5; x++) {
                    profiles[x].allowance = profiles[x].burst;
                    profiles[x].activate = 1;
                    pthread_join(tasks[x], NULL);
                }

                not_done = 0;
            }
            break;
        case '2':
            int first_unexecuted = 0;
            while (not_done) {
                for (int x = 0; x < 5; x++)
                    if (!complete[x])
                        if (profiles[sj].burst > profiles[x].burst)
                            sj = x;

                profiles[sj].allowance = profiles[sj].burst;
                profiles[sj].activate = 1;
                pthread_join(tasks[sj], NULL);
                complete[sj] = 1;

                int all_completed = 0;
                for (int x = 0; x < 5; x++)
                    if (complete[x] == 0) {
                        sj = x;
                        break;
                    } else 
                        all_completed++;

                if (all_completed == 5)
                    not_done = 0;
                else 
                    all_completed = 0;
            }
            break;
        case '3': 
            while (not_done) {
                for (int x = 0; x < 5; x++)
                    if (!complete[x])
                        if (profiles[pj].priority < profiles[x].priority)
                            pj = x;
                
                profiles[pj].allowance = profiles[pj].burst;
                profiles[pj].activate = 1;
                pthread_join(tasks[pj], NULL);
                complete[pj] = 1;

                for (int x = 0; x < 5; x++)
                    if (complete[x] == 0) {
                        pj = x;
                        break;
                    }

                int all_completed = 0;
                for (int x = 0; x < 5; x++) {
                    if (complete[x] == 1)
                        all_completed++;

                    if (all_completed == 5)
                        not_done = 0;
                }
            }
            break;
        case '4':
            while (not_done) {
                if (initializing) {
                    for (int x = 0; x < 5; x++) {
                        profiles[x].allowance = 1;
                        profiles[x].activate = 1;
                        profiles[x].dormant = 1;
                    }
                    initializing = 0;
                } else
                    for (int x = 0; x < 5; x++)
                        if (complete[x] == 0) {
                            profiles[x].dormant = 0;
                            profiles[x].dormant = 1;
                        }
        
                for (int x = 0; x < 5; x++) 
                    if (profiles[x].burst == 0 && complete[x] == 0)
                        complete[x] = 1;
                
                int all_completed = 0;

                for (int x = 0; x < 5; x++) {
                    if (complete[x] == 1)
                        all_completed++;

                    if (all_completed == 5)
                        not_done = 0;
                }
            }
            break;
        case '5': 
            while (not_done) {
                int peer_num = 0;
                for (int x = 0; x < 5; x++)
                    if (!complete[x])
                        if (profiles[pj].priority < profiles[x].priority) {
                            pj = x;
                            peer_num = 0;
                        } else if (pj != x) {
                            if (profiles[pj].priority == profiles[x].priority)
                                peer_num++;
                        }

                if (peer_num) {
                    peer_num++;
                    int peers[peer_num];
                    peer_num = 0;
                    int unfinished = 1;
                    for (int x = pj; x < 5; x++)
                        if (profiles[pj].priority == profiles[x].priority)
                            peers[peer_num++] = x;

                    while (unfinished) {
                        if (initializing) {
                            for (int x = 0; x < peer_num; x++) {
                                profiles[peers[x]].allowance = 1;
                                profiles[peers[x]].activate = 1;
                                profiles[peers[x]].dormant = 1;
                            }

                            initializing = 0;
                        } else {
                            for (int x = 0; x < peer_num; x++) {
                                if (profiles[peers[x]].burst != 0) {
                                    profiles[peers[x]].dormant = 0;
                                    profiles[peers[x]].dormant = 1;
                                }
                            }
                        }

                        int completion_index = 0;
                        for (int x = 0; x < peer_num; x++)
                            if (profiles[peers[x]].burst == 0)
                                completion_index++;

                        if (completion_index == peer_num) {
                            for (int x = 0; x < peer_num; x++)
                                complete[peers[x]] = 1;
                            peer_num = 0;
                            unfinished = 0;
                        }
                    }
                }
                
                profiles[pj].allowance = profiles[pj].burst;
                profiles[pj].activate = 1;
                pthread_join(tasks[pj], NULL);
                complete[pj] = 1;

                for (int x = 0; x < 5; x++)
                    if (complete[x] == 0) {
                        pj = x;
                        break;
                    }

                int all_completed = 0;
                for (int x = 0; x < 5; x++) {
                    if (complete[x] == 1)
                        all_completed++;

                    if (all_completed == 5)
                        not_done = 0;
                }
            }
            break;
    }
}