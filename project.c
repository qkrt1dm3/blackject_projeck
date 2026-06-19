#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 플레이어 구조체 정의
typedef struct {
    char name[30];
    int asset;
} Player;

// 게임 기록 구조체
typedef struct {
    int round;
    int win;
    int lose;
    int draw;
} Game;

void StartFunction(void) {
    printf("게임을 시작합니다!\n");
}

void EndFunction(void) {
    printf("게임을 종료합니다!\n");
}

int main(void) {
    srand((unsigned int)time(NULL));

    Player p;
    p.asset = 1000;

    // 게임 기록 정보 변수
    Game g = {0, 0, 0, 0};

    printf("--- [초간단 1인 블랙잭 게임] ---\n");
    printf("플레이어 이름을 입력하세요: ");
    scanf("%s", p.name);

    printf("\n[%s]님 환영합니다! 초기 자산 %d원으로 게임을 시작합니다.\n",
           p.name, p.asset);

    while (1) {
        printf("\n=============================\n");
        printf(" 현재 자산: %d원\n", p.asset);

        // 게임 기록 출력
        printf(" 전적 : %d승 %d패 %d무 (총 %d판)\n",
               g.win, g.lose, g.draw, g.round);

        printf("=============================\n");

        if (p.asset <= 0) {
            printf("파산하셨습니다! 게임을 종료합니다.\n");
            break;
        }

        int menu = 0;
        printf("1. 블랙잭 한 판 하기 | 2. 게임 종료 >> ");
        scanf("%d", &menu);

        switch (menu) {
        case 1:
            StartFunction();
            break;

        case 2:

            // 최종 결과 출력
            printf("\n===== 전적 =====\n");
            printf("플레이어 : %s\n", p.name);
            printf("총 %d판 플레이\n", g.round);
            printf("%d승 %d패 %d무\n",g.win, g.lose, g.draw);

            if (g.round > 0) {
                printf("승률 : %.1f%%\n",(double)g.win / g.round * 100);
            }

            EndFunction();
            return 0;

        default:
            printf("잘못된 입력입니다. 다시 선택해주세요.\n");
            continue;
        }

        // 플레이 횟수 카운트
        g.round++;

        // 1. 베팅 금액 입력
        int bet = 0;
        printf("베팅할 금액을 입력하세요 >> ");
        scanf("%d", &bet);

        if (bet <= 0 || bet > p.asset) {
            printf("금액이 잘못되었습니다. 올인합니다!\n");
            bet = p.asset;
        }

        // 2. 카드 점수 계산
        int my_score = (rand() % 10 + 2) + (rand() % 10 + 2);
        int dealer_score = (rand() % 10 + 2) + (rand() % 10 + 2);

        printf("\n당신의 시작 점수: %d\n", my_score);
        printf("딜러의 보여지는 카드 점수: %d\n", dealer_score - 2);

        // 3. 플레이어 행동
        char choice;

        while (my_score < 21) {
            printf("(Hit(h) 또는 Stand(s)) >> ");
            scanf(" %c", &choice);

            if (choice == 'h' || choice == 'H') {
                int card = rand() % 10 + 2;
                my_score += card;

                printf("새 카드를 뽑았습니다 (+%d) -> 총 점수: %d\n",
                       card, my_score);
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

        // 4. 결과 판정
        printf("\n[최종 결과] 당신: %d점 | 딜러: %d점\n",
               my_score, dealer_score);

        if (my_score == 21) {
            printf("블랙잭! 배팅 금액에 1.5배가 지급됩니다! (+%d원)\n",
                   (int)(bet * 1.5));

            p.asset += (int)(bet * 1.5);

            // 승리 횟수 카운트 (21점 달성 시)
            g.win++;
        }

        else if (dealer_score < 21 && my_score > dealer_score) {
            printf("축하합니다! 승리하셨습니다. (+%d원)\n", bet);

            p.asset += bet;

            // 승리 횟수 카운트 (딜러보다 높을때)
            g.win++;
        }

        else if (dealer_score > 21) {
            printf("딜러가 버스트! 승리하셨습니다. (+%d원)\n", bet);

            p.asset += bet;

            // 승리 횟수 카운트 (딜러가 버스트일 경우)
            g.win++;
        }

        else if (my_score < dealer_score) {
            printf("딜러의 점수가 더 높습니다. 패배! (-%d원)\n", bet);

            p.asset -= bet;

            // 패배 횟수 증가
            g.lose++;
        }

        else if (my_score > 21) {
            printf("21점을 초과(버스트)하여 패배했습니다! (-%d원)\n", bet);

            p.asset -= bet;

            // 패배 횟수 증가
            g.lose++;
        }

        else if (my_score == dealer_score) {
            printf("비겼습니다! 베팅 금액을 돌려받습니다.\n");

            // 무승부 횟수 증가
            g.draw++;
        }
    }

    return 0;
}
