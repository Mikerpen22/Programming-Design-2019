#include <iostream>
using namespace std;

void cinloop(int array[], int a);
int totalMachineCost(int machine[], int turnOnCost[], int maxPotential[], int productPerDay1[], int productPerDay2[], int totalDay);

// GOAL: fill out productPerDay/purchasePerDay s.t. total cost is minimized
void planner(int** productPerDay, int* purchasePerDay, int* demands, int* purchaseCosts, int* stockCost)
{

}


int main(){
    int workstate = 0, totalDay = 0, machine[2] = {0}, turnOnCost[2] = {0}, demandCost[2] = {0}, maxPotential[2] = {0}, Rate[2] = {0}, 
    stockCost[3] = {0}, stockAmount[3] = {0};
    cin >> workstate >> totalDay;
    int * demands = new int[totalDay];
    int * purchaseCosts = new int[totalDay];

    // 2-D array(?)
    // int** productPerDay = new int*[workstate];
    // for(int i = 0; i < workstate; i++)
    // {
    // 	productPerDay[i] = new int[totalDay];
    // }
    int * productPerDay1 = new int[totalDay];
    int * productPerDay2 = new int[totalDay];

    int * purchasePerDay = new int[totalDay];
    cinloop(machine, 2);
    cinloop(turnOnCost, 2);
    cinloop(demandCost, 2);
    cinloop(maxPotential, 2);
    cinloop(Rate, 2);
    cinloop(stockCost, 3);
    cinloop(stockAmount, 3);
    cinloop(demands, totalDay);
    cinloop(purchaseCosts, totalDay);

    // 2-D array(?)
    // for(int i = 0; i < workstate; i++)
    // 	cinloop(productPerDay[i], totalDay);
    cinloop(productPerDay1, totalDay);
    cinloop(productPerDay2, totalDay);

    cinloop(purchasePerDay, totalDay);

    int result = 0;
    int totalMachinecost = 0, totalProductCost = 0, totalMaterialCost = 0, totalStockCost1 = 0, totalStockCost2 = 0, totalPurchaseCost = 0;
    for(int i = 0; i < totalDay; i++){ //if one of the three conditions is ture, break it and return -1; if not, count the costs in 5 segments 
        if( stockAmount[0] + purchasePerDay[i] < productPerDay1[i]){ //the purchase can more than the demand of station1
            result = -1;
            break;
        }
        stockAmount[0] = stockAmount[0] + purchasePerDay[i] - productPerDay1[i]; //update the original stock of station0
        if(stockAmount[1] + productPerDay1[i] < productPerDay2[i]){ //the station1 product amount has to meet the demand of station2
            result = -1;
            break;
        }
        stockAmount[1] = stockAmount[1] + productPerDay1[i] - productPerDay2[i];
        if( stockAmount[2] + productPerDay2[i] < demands[i]){ //the station2 product amout has to meet the demand of market
            result = -1;
            break;
        }
        stockAmount[2] = stockAmount[2] + productPerDay2[i] - demands[i];
        if((productPerDay1[i] > machine[0] * maxPotential[0])||(productPerDay2[i] > machine[1] * maxPotential[1])){
            result = -1;
            break;
        }
        
        totalProductCost += demandCost[0] * productPerDay1[i] + demandCost[1] * productPerDay2[i];
        totalMaterialCost += stockAmount[0] * stockCost[0];
        totalStockCost1 += stockAmount[1] * stockCost[1];
        totalStockCost2 += stockAmount[2] * stockCost[2];
        totalPurchaseCost +=  purchaseCosts[i] * purchasePerDay[i];
    }
    totalMachinecost = totalMachineCost( machine, turnOnCost, maxPotential, productPerDay1, productPerDay2, totalDay);

    if(result == -1){
        cout << "-1";
    }
    else{
        cout << totalMachinecost << "," << totalProductCost << "," << totalMaterialCost << "," << totalStockCost1 << ","<< totalStockCost2 << "," << totalPurchaseCost;
    }

    delete[] demands;
    delete[] purchaseCosts; 
    delete[] productPerDay1; 
    delete[] productPerDay2; 
    delete[] purchasePerDay; 
    

    return 0;
}

void cinloop(int array[], int a){
    for(int i = 0; i < a; i++){
        cin >> array[i];
    }
}
int totalMachineCost( int machine[], int turnOnCost[], int maxPotential[], int productPerDay1[], int productPerDay2[], int totalDay){
    int result = 0;
    for(int i = 0; i < totalDay; i++){
        int result1 = productPerDay1[i] % maxPotential[0];
        int result2 = productPerDay2[i] % maxPotential[1];
        int machineAmount1 = productPerDay1[i]/ maxPotential[0];
        int machineAmount2 = productPerDay2[i]/ maxPotential[1];
        if(( result1 != 0) && (result2 != 0)){
        result += turnOnCost[0] * (machineAmount1 + 1)
        + turnOnCost[1] * (machineAmount2 + 1);
        }
        else if(( result1 == 0 ) && ( result2 == 0 )){
        result += turnOnCost[0] * machineAmount1
        + turnOnCost[1] * machineAmount2;
        }
        else if(( result1 != 0 ) && ( result2 == 0 )){
        result += turnOnCost[0] * (machineAmount1 + 1)
        + turnOnCost[1] * machineAmount2;
        }
        else if(( result1 == 0 ) && ( result2 != 0 )){
        result += turnOnCost[0] * machineAmount1
        + turnOnCost[1] * (machineAmount2 + 1);
        }
    }
    return result;    
}
