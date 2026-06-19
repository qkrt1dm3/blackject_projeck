#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// 플레이어 구조체 정의
typedef struct {
    char name[30];
    int asset;
} Player;


// 새로 추가할 아르바이트 미니게임 함수
void PlayAlbaMiniGame(Player *p) {
    printf("\n--- [미니게임: 10초 안에 집중해서 접시 닦기!] ---\n");
    printf("사장님: '내가 부르는 숫자를 10초 안에 똑같이 말해야 일당을 주겠네!'\n");
    
    // 10000 ~ 99999 사이의 5자리 랜덤 숫자 생성
    int target_num = rand() % 90000 + 10000; 
    int input_num = 0;
    
    printf("사장님: '%d' !!! (10초 카운트 시작)\n", target_num);
    printf("입력 >> ");

    // 시간 측정 시작
    time_t start_time = time(NULL); 
    scanf("%d", &input_num);
    // 시간 측정 종료
    time_t end_time = time(NULL);   

    // 걸린 시간 계산
    int elapsed_time = (int)(end_time - start_time);

    if (elapsed_time > 10) {
        printf("\n[시간 초과] %d초나 걸렸습니다! 너무 느려서 사장님께 쫓겨났습니다...\n", elapsed_time);
        printf("사장님: '동작이 굼떠서 안되겠구만! 수고비 100원만 받고 나가게.'\n");
        p->asset += 100; // 포인터를 통해 메인 변수 값 직접 수정
    }
    else if (target_num == input_num) {
        printf("\n[성공] %d초 만에 완벽하게 해냈습니다! 일당 1000원을 벌었습니다!\n", elapsed_time);
        p->asset += 1000;
    } 
    else {
        printf("\n[실패] %d초 만에 입력했지만... 딴생각을 하다가 접시를 깨뜨렸습니다...\n", elapsed_time);
        printf("사장님: '쯧쯧, 수리비 제하고 300원만 가져가게.'\n");
        p->asset += 300;
    }
}

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
    int change = 0;
    printf("\n현재 플레이 중인 닉네임을 새로 변경하시겠습니까?\n");
    printf("1. 변경 | 2. 뒤로가기 : "); // fgets 기준
    scanf("%d", &change);
    switch (change)
    {
    case 1:

        ClearBuffer();
        printf("변경할 이름을 입력하세요 >> ");
        fgets(new_name,sizeof(new_name), stdin);
        RemoveNewline(new_name);

        for (int i = 0; i < record_count; i++) {
            if (strcmp(name_list[i], search_name) == 0) {
                strcpy(name_list[i], new_name);
                }
            }   

        strcpy(p->name, new_name);
        printf("-> 플레이어 이름이 [%s](으)로 성공적으로 변경되었습니다!\n", p->name);
        break;

    default:
        break;
    }
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
        printf("1. 블랙잭 한 판 하기 | 2. 게임 종료 | 3. 기록 조회 및 이름 변경 | 4. 아르바이트 하기 >> ");
        scanf("%d", &menu);

        switch (menu) {
            case 1:
                if (p.asset <= 0) {
                    printf("돈이 없어서 게임을 할 수 없습니다. 알바를 먼저 하세요!\n");
                    continue;
                }
                StartFunction();
                break;
            case 2:
                EndFunction();
                return 0;
            case 3:
                ManagePlayerRecord(&p, history_names, history_assets, record_count);
                continue;

            case 4: 
                PlayAlbaMiniGame(&p); // 구조체의 주소를 넘겨줍니다.
                
                // 알바 끝나고 결과 기록 자동 저장 후 메뉴판으로 이동
                AddGameRecord(history_assets, history_names, &record_count, p.asset, p.name);
                continue;

            default:
                printf("잘못된 입력입니다. 다시 선택해주세요.\n");
                continue;
        }

        // 1. 베팅 금액 입력
        int bet = 0;
        printf("베팅할 금액을 입력하세요 (현재 소지금: %d원) >> ", p.asset);
        scanf("%d", &bet);

        if (bet <= 0) {
            printf("⚠️ 최소 베팅 금액은 1원 이상입니다! 100원 자동 베팅으로 전환합니다.\n");
            bet = (p.asset < 100) ? p.asset : 100;
        }
        else if (bet > p.asset) {
            printf("🚨 경고: 소지금보다 많은 금액을 베팅할 수 없습니다!\n");
            printf("배짱이 대단하시군요! 가진 돈 %d원 전부를 올인(All-in)합니다!\n", p.asset);
            bet = p.asset;
        }
        else if (bet >= p.asset * 0.7) {
            printf("🔥 [위험 경고] 현재 자산의 70%% 이상을 베팅하셨습니다! 고위험 승부처입니다.\n");
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
            printf("\n🎉 [JACKPOT!!] 축하합니다! 완벽한 블랙잭입니다! 🎉\n");
            printf("딜러: '아니, 이걸 21점을 맞춰버리다니... 운이 정말 좋으시군요.'\n");
            printf("💰 베팅 금액의 1.5배가 지갑으로 즉시 입금됩니다! (+%d원)\n", (int)(bet * 1.5));
            p.asset += (int)(bet * 1.5);
        } 
        else if (dealer_score < 21 && my_score > dealer_score) {
            printf("\n⚔️ [승리] 딜러보다 높은 점수를 기록하여 판돈을 따냈습니다! ⚔️\n");
            printf("플레이어 [%s]님의 깔끔한 승리입니다! (+%d원)\n", p.name, bet);
            p.asset += bet;
        } 
        else if (dealer_score > 21) {
            printf("\n💥 [딜러 버스트] 딜러가 무리하게 카드를 뽑다가 21점을 초과했습니다!\n");
            printf("안전하게 자리를 지킨 [%s]님의 승리입니다. (+%d원)\n", p.name, bet);
            p.asset += bet;
        }
        else if (my_score < dealer_score) {
            printf("\n💸 [패배] 딜러에게 밀려 점수 싸움에서 패배했습니다...\n");
            printf("사장님: '허허, 도박이 다 그렇지 않겠나? 다음 판에 만회해보게나.' (-%d원)\n", bet);
            p.asset -= bet;
        } 
        else if (my_score > 21) {
            printf("\n🚨 [버스트 패배] 욕심이 과했습니다! 21점을 초과하여 즉시 패배합니다.\n");
            printf("카드가 처참하게 찢어지는 소리와 함께 베팅 금액이 소멸합니다. (-%d원)\n", bet);
            p.asset -= bet;
        }
        else if (my_score == dealer_score) {
            printf("\n🤝 [PUSH] 믿을 수 없는 무승부! 딜러와 점수가 정확히 일치합니다.\n");
            printf("베팅 금액을 안전하게 돌려받습니다.\n");
        }

        ClearBuffer();

        // 조건 3, 14: 한 판이 끝날 때마다 배열 주소와 포인터를 함수로 넘겨 기록을 자동 누적
        AddGameRecord(history_assets, history_names, &record_count, p.asset, p.name);
    }

    return 0;
}