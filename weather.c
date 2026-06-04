#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100

struct node {
    int data;
    struct node *next;      
} *top = NULL;

void push(int temp[], int n) {
    struct node *temp1;
    for (int i = n - 1; i >= 0; i--) {  
        temp1 = (struct node*)malloc(sizeof(struct node));
        temp1->data = temp[i];
        temp1->next = top;
        top = temp1;
    }
}

void calculateWarmerDays(int n, int result[]) {
    struct node *a = top;
    for (int i = 0; i < n; i++) {
        int days = 0;
        struct node *b = a->next;
        for (int j = i + 1; j < n; j++) {
            if (b->data > a->data) {   
                days = j - i;
                break;
            }
            b = b->next;
        }
        result[i] = days;
        a = a->next;
    }
}

void findHottest(int temp[], int n) {
    int max = temp[0], day = 0;
    for (int i = 1; i < n; i++) {
        if (temp[i] > max) {
            max = temp[i];
            day = i;
        }
    }
    printf("\nHottest Day: Day %d with %d°C\n", day + 1, max);
}

void findColdest(int temp[], int n) {
    int min = temp[0], day = 0;
    for (int i = 1; i < n; i++) {
        if (temp[i] < min) {
            min = temp[i];
            day = i;
        }
    }
    printf("Coldest Day: Day %d with %d°C\n", day + 1, min);
}

void analyzeTrend(int temp[], int n) {
    int half = n / 2;
    double sum1 = 0, sum2 = 0;

    for (int i = 0; i < half; i++) 
        sum1 += temp[i];
    for (int i = half; i < n; i++) 
        sum2 += temp[i];

    double avg1 = sum1 / half;
    double avg2 = sum2 / (n - half);

    printf("\nTemperature Trend Analysis:\n");
    printf("First Half Avg = %.2f°C, Second Half Avg = %.2f°C\n", avg1, avg2);

    if (avg2 > avg1 + 1) 
        printf("Overall Trend: Warming Up\n");
    else if (avg2 < avg1 - 1)
        printf("Overall Trend: Cooling Down\n");
    else
        printf("Overall Trend: Stable\n");
}

void anomalyAlerts(int temp[], int n) {
    printf("\n Temperature Anomaly Alerts:\n");
    for (int i = 1; i < n; i++) {
        if (abs(temp[i] - temp[i - 1]) >= 5) {
            printf("Day %d anomaly: Change of %d°C from previous day (%d°C → %d°C)\n",
                   i + 1, abs(temp[i] - temp[i - 1]), temp[i - 1], temp[i]);
        }
    }
}


void agricultureRisk(int temp[], int n) {
    printf("\nAgriculture Frost/Heat Risk Alerts:\n");
    for (int i = 0; i < n; i++) {
        if (temp[i] < 5) {
            printf("Day %d: Frost risk! Temperature = %d°C \n", i + 1, temp[i]);
        } else if (temp[i] > 35) {
            printf("Day %d: Heat stress risk! Temperature = %d°C \n", i + 1, temp[i]);
        }
    }
}
void linearTrendForecast(int temp[], int n) {
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    for (int i = 0; i < n; i++) {
        sumX += (i + 1);
        sumY += temp[i];
        sumXY += (i + 1) * temp[i];
        sumX2 += (i + 1) * (i + 1);
    }

    double b = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    double a = (sumY - b * sumX) / n;

    printf("\n Linear Trend Forecasting:\n");
    printf("Equation: y = %.2f + %.2f*x\n", a, b);

    if (b > 0.2)
        printf("Trend Direction: Upward 🌡 (warming)\n");
    else if (b < -0.2)
        printf("Trend Direction: Downward ❄ (cooling)\n");
    else
        printf("Trend Direction: Stable 🌤\n");

    // --- Predict next 3 days ---
    int totalDays = n + 3;
    int extendedTemp[totalDays];
    for (int i = 0; i < n; i++) extendedTemp[i] = temp[i];

    printf("\nPredicted Temperatures for Next 3 Days:\n");
    for (int i = 1; i <= 3; i++) {
        double y_pred = a + b * (n + i);
        extendedTemp[n + i - 1] = (int)(y_pred + 0.5); 
        printf("Day %d (Future): %.2f°C\n", n + i, y_pred);
    }

    printf("\n Analysis on Predicted 3-Day Forecast:\n");
    findHottest(extendedTemp, totalDays);
    findColdest(extendedTemp, totalDays);
    analyzeTrend(extendedTemp, totalDays);
    anomalyAlerts(extendedTemp, totalDays);
    agricultureRisk(extendedTemp, totalDays);
}

int main() {
    int n;
    printf("Enter number of days: ");
    scanf("%d", &n);

    int temp[n], result[n];
    char input[10];

    // Starting date input
    int startDay, startMonth, startYear;
    printf("Enter starting date (dd mm yyyy): ");
    scanf("%d %d %d", &startDay, &startMonth, &startYear);

    printf("Enter daily temperatures (use 'x' for missing data):\n");
    for (int i = 0; i < n; i++) {
        scanf("%s", input);

        if (strcmp(input, "x") == 0 || strcmp(input, "X") == 0) {
            temp[i] = -999; 
        } else {
            temp[i] = atoi(input); 
        }
    }
    for (int i = 0; i < n; i++) {
        if (temp[i] == -999) {
            int left = (i > 0) ? temp[i - 1] : 0;
            int right = (i < n - 1 && temp[i + 1] != -999) ? temp[i + 1] : left;
            temp[i] = (left + right) / 2;
            printf("Missing value on Day %d replaced with %d°C\n", i + 1, temp[i]);
        }
    }

    push(temp, n);
    calculateWarmerDays(n, result);

    printf("\nDays to wait for a warmer temperature:\n");

    int d = startDay, m = startMonth, y = startYear;
    for (int i = 0; i < n; i++) {
        printf("%02d-%02d-%04d (Day %d, %d°C): %d days\n", d, m, y, i + 1, temp[i], result[i]);

        d++;
        if ((m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10) && d > 31) { d = 1; m++; }
        else if (m == 12 && d > 31) { d = 1; m = 1; y++; }
        else if ((m == 4 || m == 6 || m == 9 || m == 11) && d > 30) { d = 1; m++; }
        else if (m == 2) {
            int leap = (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0));
            int daysInFeb = leap ? 29 : 28;
            if (d > daysInFeb) { d = 1; m++; }
        }
    }

    findHottest(temp, n);
    findColdest(temp, n);
    analyzeTrend(temp, n);  
    anomalyAlerts(temp, n);  
    agricultureRisk(temp, n);
    linearTrendForecast(temp, n);

    return 0;
}