#include <stdio.h>
#include <graphics.h>
#include <math.h>

int tower[3][100];
int top[3];
int l, b, u;
char stepText[100];

void push(int to, int diskno) {
    tower[to - 1][++top[to - 1]] = diskno;
}

int pop(int from) {
    return (tower[from - 1][top[from - 1]--]);
}

void draw() {
    int j, i, disk;
    cleardevice();
    for (j = 1; j <= 3; j++) {
        setfillstyle(SOLID_FILL, 9);
        bar(j * l, u, j * l + 5, b);
        for (i = 0; i <= top[j - 1]; i++) {
            disk = tower[j - 1][i];
            setfillstyle(SOLID_FILL, 2 + disk);
            bar(j * l - 15 - disk * 5, b - (i + 1) * 10, j * l + 5 + 15 + disk * 5, b - i * 10);
        }
    }
}

void diskAnimation(int from, int to, int diskno) {
    int x, y, dif, sign;

    x = from * l;
    y = b - (top[from - 1] + 1) * 10;

    for (; y > u - 20; y -= 15) {
        draw();
        setfillstyle(SOLID_FILL, 2 + diskno);
        bar(x - 15 - diskno * 5, y - 10, x + 5 + 15 + diskno * 5, y);
        delay(10);
    }
    y = u - 20;
    dif = to * l - x;
    sign = dif / abs(dif);

    for (; abs(x - to * l) > 25; x += sign * 15) {
        draw();
        setfillstyle(SOLID_FILL, 2 + diskno);
        bar(x - 15 - diskno * 5, y - 10, x + 5 + 15 + diskno * 5, y);
        delay(10);
    }
    x = to * l;

    for (; y < b - (top[to - 1] + 1) * 10; y += 15) {
        draw();
        setfillstyle(SOLID_FILL, 2 + diskno);
        bar(x - 15 - diskno * 5, y - 10, x + 5 + 15 + diskno * 5, y);
        delay(10);
    }

    push(to, diskno);
    draw();
}

void moveTopDisk(int from, int to) {
    int i,j;
    int diskno;
    diskno = pop(from);

    // Check if the move is valid: larger disk cannot be placed on a smaller one
    if (top[to - 1] == -1 || diskno < tower[to - 1][top[to - 1]]) {
        cleardevice(); // Clear the screen before starting the animation

        // Redraw the towers before the animation starts
        for ( j = 1; j <= 3; j++) {
            setfillstyle(SOLID_FILL, 9);
            bar(j * l, u, j * l + 5, b);
            for ( i = 0; i <= top[j - 1]; i++) {
                int disk = tower[j - 1][i];
                setfillstyle(SOLID_FILL, 2 + disk);
                bar(j * l - 15 - disk * 5, b - (i + 1) * 10, j * l + 5 + 15 + disk * 5, b - i * 10);
            }
        }

        diskAnimation(from, to, diskno); // Perform the animation
        // Update the screen to show the final state after animation
        draw();
       if(areAllDisksInThirdTower()){
        outtextxy(10,50, "You win!");
                    delay(2000);
        }
        sprintf(stepText, "Move disk %d from tower %d to tower %d", diskno, from, to);
        outtextxy(10, 30, stepText);
       
    } else {
        printf("Invalid move. Larger disk cannot be placed on top of a smaller one.\n");
        push(from, diskno);  // Return the disk to its original position
    }
}

void resetTowers(int t) {
        int i;
    cleardevice();
    for ( i = 0; i < 3; i++) {
        top[i] = -1;
    }

    for (i = t; i > 0; i--) {
        push(1, i);
    }
    draw();
}
void Solve(int t, int from, int aux, int to) {
    if (t == 1) {
        moveTopDisk(from, to);
    } else {
        Solve(t - 1, from, to, aux);
        moveTopDisk(from, to);
        Solve(t - 1, aux, from, to);
    }
}
int areAllDisksInThirdTower() {
    return top[0] == -1 && top[1] == -1 && top[2] >= 0;
}
int main() {
    int gd = DETECT, gm, n, from, to,i;
    int t;
    int choice;
    printf("Enter number of disks (less than or equal to 10)");
    scanf("%d", &n);
    t=n;
    initgraph(&gd, &gm, "..\\BGI");

    for ( i = 0; i < 3; i++) {
        top[i] = -1;
    }

    for ( i = n; i > 0; i--) {
        push(1, i);
    }
    l = getmaxx() / 4;
    b = getmaxy() - 50;
    u = getmaxy() / 3 + 100;
    draw();

    do {
        outtextxy(10,10,"please choose: 1.move  2.Solve  3.Exit");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                outtextxy(10, 20,"Enter the initial and final towers (1, 2, or 3): ");
                scanf("%d %d", &from, &to);
                if (from >= 1 && from <= 3 && to >= 1 && to <= 3) {
                    moveTopDisk(from, to);
                } else {
                    printf("Invalid tower numbers. Towers must be 1, 2, or 3.\n");
                }
                if (areAllDisksInThirdTower()) {
                    outtextxy(10,50, "You win!");
                    delay(2000);
                    choice=3;
                }
                break;
            case 2:
            resetTowers(n); // Reset the towers to their initial state
            draw();
                Solve(t, 1, 2, 3);
                if (areAllDisksInThirdTower()) {
                    outtextxy(10,50, "You win!");
                    delay(2000);
                    choice=3;
                }
                break;
            default:
                break;
        }
    } while (choice != 3);
    getch();
    closegraph();
    return 0;
}