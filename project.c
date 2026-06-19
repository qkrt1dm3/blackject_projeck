#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// 플레이어 구조체 정의
typedef struct {
    char name[30];
    int asset;
} Player;

void ClearBuffer(void) { // 조건 8 fgets로 입력 버퍼 비우기
    while(getchar() != '\n');
}

void RemoveNewline(char *str) { // 조건 9 fgets로 입력받은 문자열 엔터 문자 제거
    int len = strlen(str);
    if(len >0 && str[len-1] == '\n') {
        str[len-1] = '\0';
    }
}

void AddGameRecord(int *asset_list, char (*name_list)[30], int *record_count, int final_asset, char *current_name) {
    asset_list[*record_count] = final_asset;               // 1차원 정수형 배열에 자산 저장
    strcpy(name_list[*record_count], current_name);        // 조건 11: strcpy 활용하여 이름 저장
    (*record_count)++;                                     // 조건 4: (*book_count)++ 형태의 포인터 누적 수행
}

// 조건 7, 10, 11: strcmp와 strcpy를 활용한 플레이어 기록 조회 및 이름 수정 함수
void ManagePlayerRecord(Player *p, char (*name_list)[30], int *asset_list, int record_count) {
    char search_name[30];
    char new_name[30];
    int found = 0;
    int target_index = -1; 

    printf("\n--- [게임 기록 조회 및 닉네임 변경]---\n");
    printf("기록을 조회할 플레이어 이름을 입력하세요: ");
    scanf("%s", search_name);

    for (int i = 0; i < record_count; i++) {
        if (strcmp(name_list[i], search_name) == 0) {
            printf("-> [기록 %d번] 플레이어: %s | 최종 자산: %d원\n", i+1, name_list[i], asset_list[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("오류: 입력하신 이름의 게임 기록이 존재하지 않습니다. \n");
        return;
    }
    ClearBuffer();
    printf("\n현재 플레이 중인 닉네임을 새로 변경하시겠습니까?\n");
    printf("변경할 새 이름을 입력하세요 >>"); // fgets 기준
    fgets(new_name,sizeof(new_name), stdin);
    RemoveNewline(new_name);

    strcpy(p->name, new_name);
    printf("-> 플레이어 이름이 [%s](으)로 성공적으로 변경되었습니다!\n", p->name);
}


void StartFunction(void) {
    printf("게임을 시작합니다!\n");
}

void EndFunction(void) {
    printf("게임을 종료합니다!\n");
}

int main(void) {
    srand((unsigned int)time(NULL)); // 매번 다른 랜덤 숫자를 위한 설정

    char history_names[100][30];
    int history_assets[100];
    int record_count = 0;

    Player p; // 구조체 변수 딱 1개만 선언!
    p.asset = 1000; // 초기 자산 1000원 설정

    printf("--- [초간단 1인 블랙잭 게임] ---\n");
    printf("플레이어 이름을 입력하세요: ");
    scanf("%s", p.name); // 구조체 name 멤버에 저장
    
    printf("\n[%s]님 환영합니다! 초기 자산 %d원으로 게임을 시작합니다.\n", p.name, p.asset);

    // 게임 무한 반복 루프
    while (1) {
        printf("\n=============================\n");
        printf(" 현재 자산: %d원\n", p.asset);
        printf("=============================\n");

        if (p.asset <= 0) {
            printf("파산하셨습니다! 게임을 종료합니다.\n");
            break;
        }

        int menu = 0;
        printf("1. 블랙잭 한 판 하기 | 2. 게임 종료 | 3. 기록 조회 및 이름 변경>> ");
        scanf("%d", &menu);

        switch (menu) {
            case 1:
                StartFunction(); // 게임 시작 함수 호출
                break;
            case 2:
                EndFunction(); // 게임 종료 함수 호출
                    return 0;
            case 3:
                ManagePlayerRecord(&p, history_names, history_assets, record_count);
                continue;
            default:
                printf("잘못된 입력입니다. 다시 선택해주세요.\n");
                continue;
        }

        // 1. 베팅 금액 입력
        int bet = 0;
        printf("베팅할 금액을 입력하세요 >> ");
        scanf("%d", &bet);

        if (bet <= 0 || bet > p.asset) {
            printf("금액이 잘못되었습니다. 올인합니다!\n");
            bet = p.asset;
        }

        // 2. 카드 점수 계산 (2~11 사이의 숫자를 랜덤으로 두 장씩 합산)
        int my_score = (rand() % 10 + 2) + (rand() % 10 + 2);
        int dealer_score = (rand() % 10 + 2) + (rand() % 10 + 2);

        printf("\n당신의 시작 점수: %d\n", my_score);
        printf("딜러의 보여지는 카드 점수: %d\n", dealer_score - 2); // 딜러 카드는 살짝 감춤

        // 3. 플레이어 히트(Hit) / 스탠드(Stand) 결정
        char choice;
        while (my_score < 21) {
            printf("(Hit(h) 또는 Stand(s)) >> ");
            scanf(" %c", &choice); // %c 앞 공백은 버퍼 비우기용

            if (choice == 'h' || choice == 'H') {
                int card = rand() % 10 + 2;
                my_score += card;
                printf("새 카드를 뽑았습니다 (+%d) -> 총 점수: %d\n", card, my_score);
            } 
            else if (choice == 's' || choice == 'S') {
                printf("스탠드 선택! 딜러의 턴으로 넘어갑니다...\n");

                if (dealer_score < 17) {
                    printf("딜러의 점수가 17 미만이므로 딜러가 카드를 더 뽑습니다.\n");
                    dealer_score += (rand() % 10 + 2);  
                }
                break;
            } 
            else if (choice != 'h' && choice != 'H' && choice != 's' && choice != 'S') {
                printf("잘못된 입력입니다. 다시 선택해주세요.\n");
            }
            
            else {
                break;
            }
        }


        // 4. 결과 판정 후 구조체의 자산(asset) 바로 갱신
        printf("\n[최종 결과] 당신: %d점 | 딜러: %d점\n", my_score, dealer_score);

        if (my_score == 21) {
            printf("블랙잭! 배팅 금액에 1.5배가 지급됩니다! (+%d원)\n", (int)(bet * 1.5));
            p.asset += (int)(bet * 1.5); // 구조체 변수에 직접 더하기
        } 
        else if (dealer_score < 21 && my_score > dealer_score) {
            printf("축하합니다! 승리하셨습니다. (+%d원)\n", bet);
            p.asset += bet; // 구조체 변수에 직접 더하기
        } 
        else if (dealer_score > 21) {
            printf("딜러가 버스트! 승리하셨습니다. (+%d원)\n", bet);
            p.asset += bet; // 구조체 변수에 직접 더하기
        }
        else if (my_score < dealer_score) {
            printf("딜러의 점수가 더 높습니다. 패배! (-%d원)\n", bet);
            p.asset -= bet;
        } 
        else if ( my_score > 21) {
            printf("21점을 초과(버스트)하여 패배했습니다! (-%d원)\n", bet);
            p.asset -= bet; // 구조체 변수에 직접 빼기
        }
        else if (my_score == dealer_score) {
            printf("비겼습니다! 베팅 금액을 돌려받습니다.\n");
        }

        // 조건 3, 14: 한 판이 끝날 때마다 배열 주소와 포인터를 함수로 넘겨 기록을 자동 누적
        AddGameRecord(history_assets, history_names, &record_count, p.asset, p.name);
    }

    return 0;
}