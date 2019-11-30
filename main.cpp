//
//  main.cpp
//  PD108-1_HW07-Problem 1
//
//  Created by 藍行知 on 2019/11/7.
//  Copyright © 2019 藍行知. All rights reserved.
//

#include <iostream>
using namespace std;
int checksatisfied(int T, int M[], int B[], int sco[], int In[], int D[], int dp1[], int dp2[], int db[]);
int machineoncost = 0, producingcost = 0, materialsavingcost = 0, productionsavingcost[2] = {0}, materialbuyingcost = 0;
int m[2] = {0}, coston[2] = {0}, costproduce[2] = {0}, Blimit[2] = {0}, R[2] = {0}, savecost[3] ={0}, initial[3] = {0};

int main()
{
    int length = 1000;
    int i = 2,t = 0;
    cin >> i >> t;
    for (int q = 0; q < i; q++)
        cin >> m[q];
    for (int r = 0; r < i; r++)
        cin >> coston[r];
    for (int z = 0; z < i; z++)
        cin >> costproduce[z];
    for (int t = 0; t < i; t++)
        cin >> Blimit[t];
    for (int u = 0; u < i; u++)
        cin >> R[u];
    for (int v = 0; v < i + 1; v++)
        cin >> savecost[v];
    for (int w = 0; w < i + 1; w++)
        cin >> initial[w];
    int *demand, *dailymaterialprice, *dailyproduction1, *dailyproduction2, *dailybuying;
    demand = new int[length];
    dailymaterialprice = new int[length];
    dailyproduction1 = new int[length];
    dailyproduction2 = new int[length];
    dailybuying = new int[length];
    for (int i = 0; i < t; i++)
        cin >> demand[i];
    for (int j = 0; j < t; j++)
        cin >> dailymaterialprice[j];
    for (int k = 0; k < t; k++)
        cin >> dailyproduction1[k];
    for (int x = 0; x < t; x++)
        cin >> dailyproduction2[x];
    for (int l = 0; l < t; l++)
        cin >> dailybuying[l];
    if (checksatisfied(t, m, Blimit, savecost, initial, demand, dailyproduction1, dailyproduction2, dailybuying) == -1)
        cout << "-1";
    else
    {
        for(int q = 0; q < t; q++)//計算各種成本
        {
            if (dailyproduction1[q] % Blimit[0] == 0)
                machineoncost += coston[0] * (dailyproduction1[q] / Blimit[0]);
            else
                machineoncost += coston[0] * ((dailyproduction1[q] / Blimit[0]) + 1);
            if (dailyproduction2[q] % Blimit[1] == 0)
                machineoncost += coston[1] * (dailyproduction2[q] / Blimit[1]);
            else
                machineoncost += coston[1] * ((dailyproduction2[q] / Blimit[1]) + 1);
            producingcost += dailyproduction1[q] * costproduce[0] + dailyproduction2[q] * costproduce[1];
            materialbuyingcost += dailybuying[q] * dailymaterialprice[q];
        }
        cout << machineoncost << "," << producingcost << "," << materialsavingcost << "," << productionsavingcost[0] << "," << productionsavingcost[1] << "," << materialbuyingcost;
    }
    delete[] demand;
    delete[] dailymaterialprice;
    delete[] dailyproduction1;
    delete[] dailyproduction2;
    delete[] dailybuying;
    return 0;
}

int checksatisfied(int T, int M[], int B[], int sco[], int In[], int D[], int dp1[], int dp2[], int db[])
{
    int unusedproduction1 = 0, unusedproduction2 = 0, unusedmaterial = 0;
    if (In[2] + dp2[0] >= D[0])//第一個工作站的生產量 -> 提供給第二個工作站，這是第一天
    {
        unusedproduction1 = dp1[0] + In[1] - dp2[0];
        unusedproduction2 = dp2[0] + In[2] - D[0];
        productionsavingcost[0] += unusedproduction1 * sco[1];
        productionsavingcost[1] += unusedproduction2 * sco[2];
    }
    else
        return -1;
    
    for (int n = 1; n < T; n++)//後幾天
    {
        if (unusedproduction2 + dp2[n] >= D[n])
        {
            unusedproduction1 += dp1[n] - dp2[n];
            unusedproduction2 += dp2[n] - D[n];
            productionsavingcost[0] += unusedproduction1 * sco[1];
            productionsavingcost[1] += unusedproduction2 * sco[2];
        }
        else
            return -1;
    }
    
    if (In[0] + db[0] >= dp1[0]) //首日原料的計算、原料儲存
    {
        unusedmaterial = In[0] + db[0] - dp1[0];
        materialsavingcost += unusedmaterial * sco[0];
    }
    else
        return -1;
    
    for (int o = 1; o < T; o++)//後幾天
    {
        if (unusedmaterial + db[o] >= dp1[o])
        {
            unusedmaterial += db[o] - dp1[o];
            materialsavingcost += unusedmaterial *sco[0];
        }
        else
            return -1;
    }
    for (int p = 0; p < T; p++)//計算是否超過生產上限
    {
        if(M[0] * B[0] < dp1[p] || M[1] * B[1] < dp2[p])
            return -1;
    }
    return 1;
}
