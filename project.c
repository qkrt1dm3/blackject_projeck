#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 플레이어 구조체 정의
typedef struct {
    char name[30];
    int asset;
} Player;

void StartFunction(void) {
    printf("게임을 시작합니다!\n");
}

void EndFunction(void) {
    printf("게임을 종료합니다!\n");
}

int main(void) {
    srand((unsigned int)time(NULL)); // 매번 다른 랜덤 숫자를 위한 설정

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

        // [수정됨] 10초 타임어택 접시 닦기 미니게임
        if (p.asset <= 0) {
            int alba_choice = 0;
            printf("파산하셨습니다! 아르바이트를 해서 돈을 벌어오시겠습니까?\n");
            printf("1. 식당 접시닦기 미니게임 (10초 타임어택) | 2. 게임 포기하기 >> ");
            scanf("%d", &alba_choice);

            if (alba_choice == 1) {
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
                    p.asset += 100;
                }
                else if (target_num == input_num) {
                    printf("\n[성공] %d초 만에 완벽하게 해냈습니다! 일당 1000원을 벌었습니다!\n", elapsed_time);
                    p.asset += 1000;
                } 
                else {
                    printf("\n[실패] %d초 만에 입력했지만... 딴생각을 하다가 접시를 깨뜨렸습니다...\n", elapsed_time);
                    printf("사장님: '쯧쯧, 수리비 제하고 300원만 가져가게.'\n");
                    p.asset += 300;
                }
                continue; // 아르바이트가 끝났으니 메뉴로 돌아감
            } else {
                printf("\n게임을 완전히 종료합니다.\n");
                break;
            }
        }

        int menu = 0;
        printf("1. 블랙잭 한 판 하기 | 2. 게임 종료 >> ");
        scanf("%d", &menu);

        switch (menu) {
            case 1:
                StartFunction(); // 게임 시작 함수 호출
                break;
            case 2:
                EndFunction(); // 게임 종료 함수 호출
                return 0;
            default:
                printf("잘못된 입력입니다. 다시 선택해주세요.\n");
                continue;
        }

        // 1. 베팅 금액 입력
        int bet = 0;
        printf("베팅할 금액을 입력하세요 >> ");
        scanf("%d", &bet);

        if (bet <= 0 || bet > p.asset) {
            printf("금액이 잘못되었습니다. 가진 돈 전부를 올인합니다!\n");
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
            else {
                printf("잘못된 입력입니다. 다시 선택해주세요.\n");
            }
        }

        // 4. 결과 판정 후 구조체의 자산(asset) 갱신
        printf("\n[최종 결과] 당신: %d점 | 딜러: %d점\n", my_score, dealer_score);

        if (my_score > 21) {
            printf("21점을 초과(버스트)하여 패배했습니다! (-%d원)\n", bet);
            p.asset -= bet; 
        }
        else if (my_score == 21) {
            printf("블랙잭! 배팅 금액에 1.5배가 지급됩니다! (+%d원)\n", (int)(bet * 1.5));
            p.asset += (int)(bet * 1.5); 
        } 
        else if (dealer_score > 21 || my_score > dealer_score) {
            printf("축하합니다! 승리하셨습니다. (+%d원)\n", bet);
            p.asset += bet; 
        } 
        else if (my_score < dealer_score) {
            printf("딜러의 점수가 더 높습니다. 패배! (-%d원)\n", bet);
            p.asset -= bet;
        } 
        else {
            printf("비겼습니다! 베팅 금액을 돌려받습니다.\n");
        }
    }

    return 0;
}
