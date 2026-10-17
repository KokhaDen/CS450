#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

typedef struct {
    int breakdancing;
    int apiculture;
    int basket;
    int xbasket;
    int sword;
} Player;

// I know this is bad practice :P but it worked
int compare_breakdancing(const void *a, const void *b) {
    const Player* player_a = a;
    const Player* player_b = b;

    int val_a = player_a->breakdancing;
    int val_b = player_b->breakdancing;

    if (val_a > val_b) return -1;
    if (val_a < val_b) return 1;
    return 0;
}

int compare_apiculture(const void *a, const void *b) {
    const Player* player_a = a;
    const Player* player_b = b;

    int val_a = player_a->apiculture;
    int val_b = player_b->apiculture;

    if (val_a > val_b) return -1;
    if (val_a < val_b) return 1;
    return 0;
}

int compare_basket(const void *a, const void *b) {
    const Player* player_a = a;
    const Player* player_b = b;

    int val_a = player_a->basket;
    int val_b = player_b->basket;

    if (val_a > val_b) return -1;
    if (val_a < val_b) return 1;
    return 0;
}

int compare_xbasket(const void *a, const void *b) {
    const Player* player_a = a;
    const Player* player_b = b;

    int val_a = player_a->xbasket;
    int val_b = player_b->xbasket;

    if (val_a > val_b) return -1;
    if (val_a < val_b) return 1;
    return 0;
}

int compare_sword(const void *a, const void *b) {
    const Player* player_a = a;
    const Player* player_b = b;

    int val_a = player_a->sword;
    int val_b = player_b->sword;

    if (val_a > val_b) return -1;
    if (val_a < val_b) return 1;
    return 0;
}

int compare_total(const void *a, const void *b) {
    const Player* player_a = a;
    const Player* player_b = b;

    int val_a = player_a->breakdancing + player_a->apiculture + player_a->basket + player_a->xbasket + player_a->sword;
    int val_b = player_b->breakdancing + player_b->apiculture + player_b->basket + player_b->xbasket + player_b->sword;

    if (val_a > val_b) return -1;
    if (val_a < val_b) return 1;
    return 0;
}

// uses qsort for the sorting, standard library
int standard(Player* players, int player_count) {
    printf("SKILL_BREAKDANCING\n");
    qsort(players, player_count, sizeof(Player), compare_breakdancing);
    
    for(int i = 0; i < player_count; i++) {
        printf("%d\n", players[i].breakdancing);
    }
    printf("\n");

    printf("SKILL_APICULTURE\n");
    qsort(players, player_count, sizeof(Player), compare_apiculture);
    
    for(int i = 0; i < player_count; i++) {
        printf("%d\n", players[i].apiculture);
    }
    printf("\n");

    printf("SKILL_BASKET\n");
    qsort(players, player_count, sizeof(Player), compare_basket);
    
    for(int i = 0; i < player_count; i++) {
        printf("%d\n", players[i].basket);
    }
    printf("\n");

    printf("SKILL_XBASKET\n");
    qsort(players, player_count, sizeof(Player), compare_xbasket);
    
    for(int i = 0; i < player_count; i++) {
        printf("%d\n", players[i].xbasket);
    }
    printf("\n");

    printf("SKILL_SWORD\n");
    qsort(players, player_count, sizeof(Player), compare_sword);
    
    for(int i = 0; i < player_count; i++) {
        printf("%d\n", players[i].sword);
    }
    printf("\n");

    printf("TOTAL_XP\n");
    qsort(players, player_count, sizeof(Player), compare_total);
    
    for(int i = 0; i < player_count; i++) {
        printf("%d\n", players[i].breakdancing + players[i].apiculture + players[i].basket + players[i].xbasket + players[i].sword);
    }
    return 0;
}

// got lots of help from https://stackoverflow.com/questions/28189289/merge-sort-in-c
void merge(int arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int leftA[n1], rightA[n2];

    for(i = 0; i < n1; i++) {
        leftA[i] = arr[left + i];
    }

    for(j = 0; j < n2; j++) {
        rightA[j] = arr[mid + j + 1];
    }

    i = 0;
    j = 0;

    for (k = left; k <= right; k++) {
        // left array empty
        if (i >= n1) {
            arr[k] = rightA[j];
            j++;
        // right empty
        } else if (j >= n2) {
            arr[k] = leftA[i];
            i++;
        // otherwise compare vals
        } else if(leftA[i] >= rightA[j]) {
            arr[k] = leftA[i];
            i++;
        } else {
            arr[k] = rightA[j];
            j++;
        }
    }
    
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

// I use a combination of counting for values below 10k (since majority of players are)
// for values greater than 10k, I used merge sort
void custom_sort(Player* players, int player_count, int skill) {
    int freq[10000] = {0};
    int score;

    int* merge = malloc(player_count * sizeof(int));

    int mp = 0;

    // no more ctrl-c ctrl-v for each skill now...
    for (int i = 0; i < player_count; i++) {
        if(skill == 1) {
            score = players[i].breakdancing;
        } else if(skill == 2) {
            score = players[i].apiculture;
        } else if (skill == 3) {
            score = players[i].basket;
        } else if (skill == 4) {
            score = players[i].xbasket;
        } else if (skill == 5) {
            score = players[i].sword;
        } else {
            score = players[i].breakdancing + players[i].apiculture + players[i].basket + players[i].xbasket + players[i].sword;
        }

        if(score >= 0 && score < 10000) {
            freq[score]++;
        } else {
            merge[mp] = score;
            mp++;
        }
    }

    mergeSort(merge, 0, mp - 1);

    for(int i = 0; i < mp; i++) {
        printf("%d\n", merge[i]);
    }

    for(int j = 9999; j >= 0; j--) {
        while (freq[j] > 0) {
            printf("%d\n", j);
            freq[j]--;
        }
    }

    free(merge);
}

int custom(Player* players, int player_count) {
    printf("SKILL_BREAKDANCING\n");
    custom_sort(players, player_count, 1);
    printf("\n");

    printf("SKILL_APICULTURE\n");
    custom_sort(players, player_count, 2);
    printf("\n");

    printf("SKILL_BASKET\n");
    custom_sort(players, player_count, 3);
    printf("\n");

    printf("SKILL_XBASKET\n");
    custom_sort(players, player_count, 4);
    printf("\n");

    printf("SKILL_SWORD\n");
    custom_sort(players, player_count, 5);
    printf("\n");

    printf("TOTAL_XP\n");
    custom_sort(players, player_count, 6);

    return 0;
}

int main(int argc, char* argv[]) {
    setvbuf(stdin, NULL, _IOFBF, 0x100000);
    setvbuf(stdout, NULL, _IOFBF, 0x100000);

    int max_players = 1000;
    int player_count = 0;
    Player* players = malloc(max_players * sizeof(Player));
    
    if (argc != 2) {
        fprintf(stderr, "Please specify 'standard' or 'custom'\n");
        return 1;
    }

    while (scanf("%d %d %d %d %d", 
            &players[player_count].breakdancing,
            &players[player_count].apiculture,
            &players[player_count].basket,
            &players[player_count].xbasket,
            &players[player_count].sword) == 5) {
        player_count++;

        // resizing if reached max
        if(player_count == max_players) {
            max_players = max_players * 2;
            players = realloc(players, max_players * sizeof(Player));
        }
    }

    if (strcmp("standard", argv[1]) == 0) {
        double time = 0;
        clock_t start = clock();
        standard(players, player_count);
        clock_t end = clock();
        time += (double)(end - start) / CLOCKS_PER_SEC;
        fprintf(stderr, "Total standard time: %f seconds\n", time);
    } else if (strcmp("custom", argv[1]) == 0) {
        double time = 0;
        clock_t start = clock();
        custom(players, player_count);
        clock_t end = clock();
        time += (double)(end - start) / CLOCKS_PER_SEC;
        fprintf(stderr, "Total custom time: %f seconds\n", time);
    } else {
        printf("Invalid arguement. Please specify 'standard' or 'custom'\n");
    }

    free(players);
}

