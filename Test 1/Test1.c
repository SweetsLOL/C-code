#include <stdio.h>
#include <strings.h>
#include <stdbool.h>
/**
* You are required to fill in the BLANK section as well as
* ADDING ANY LIBRARIES or REQUIRED CODE to get the program working as required
*/
/**
* This function determines the safety status of air quality
* based on AQI (Air Quality Index) and PM2.5 concentration.
* Returns a string representing the status category.
*/
char* getAirQualityStatus(int AQI, int PM2_5)
{
    if ((0 <= AQI && AQI < 50) && (0 <= PM2_5 && PM2_5 < 25)){
        return "Good";
    } else if ((50 <= AQI && AQI <= 100) || (25 <= PM2_5 && PM2_5 <= 50))
    {
        return "Moderate";
    } else if (100 < AQI || 50 < PM2_5){
        return "Unhealthy";
    } else {
        return "Unknown";
    }
}
// Return "Good" if AQI is below 50 and PM2.5 is below 25
// Return "Moderate" if AQI is between 50–100 or PM2.5 is between 25–50
// Return "Unhealthy" if AQI is above 100 or PM2.5 is above 50
// Otherwise, return "Unknown"

/**
* This program prompts the user to enter the AQI (Air Quality Index)
* and PM2.5 concentration, then displays the corresponding air quality status.
*/
int main()
{
    // Declare any other relevant variables
    char* statusCategory;
    int AQI;
    int PM2_5;
    bool repeat;
    
    do
    {
        repeat = false;
        printf("Enter the current Air Quality Index (AQI): \n");
        // Get AQI input
        scanf("%d", &AQI);
        printf("Enter the current PM2.5 concentration: \n");
        // Get PM2.5 input
        scanf("%d", &PM2_5);
        // Call getAirQualityStatus function and store its return value inside
        statusCategory = getAirQualityStatus(AQI, PM2_5);
        printf("The air quality status is: %s\n", statusCategory);
        // Ask if user wants to repeat program if status is “Unhealthy”
        // If yes, repeat program. If no, exit loop.
        if(strcmp(statusCategory, "Unhealthy") == 0){
            char repeat_choice[10];
            printf("Do you want to repeat the program? (Y for yes, anything else for no)\n");
            scanf("%s", repeat_choice);
            
            if(strcmp(repeat_choice, "Y") == 0 || strcmp(repeat_choice, "y") == 0){
            repeat = true;
            } else {
            repeat = false;
            }
        }

        
    } while (repeat);
    
    return 0;
}
