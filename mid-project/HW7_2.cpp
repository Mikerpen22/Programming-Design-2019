#include <iostream>
using namespace std;
int *machineamount, *machineoncost, *perkilogramcost, *Blimit, *Rate, *stockcost, *initialstock, *demand, *materialprice, **productionperday, **machineonperday, *materialbuyingperday, *threshold, *materialusingperday, minthreshold = 1000000;
int thresholdCalculation(int i);
int totaldemandCalculation(int totalday);
 
int thresholdCalculation(int i)
{
    int minnumber = 0;
    for (int j = 0; j < i; j++) //計算生產線瓶頸
    {
        int finalRate = 1;
        for(int l = 0; l <= j; l++)
            finalRate *= Rate[l];
        threshold[j] = machineamount[j] * Blimit[j] * finalRate;
        if (minthreshold > threshold[j]) //生產瓶頸出現時只能消耗掉這麼多的原始材料（多餘的半成品會留下來）
        {
            minthreshold = threshold[j];
            minnumber = j;
        }
    }
    return minnumber;
}
 
int totaldemandCalculation(int totalday) //計算總需求量
{
    int total = 0;
    for (int j = 0; j < totalday; j++)
        total += demand[j];
    return total;
}
 
int main()
{
    int i = 0, t = 0; //輸入工作站數量i、日期t
    cin >> i >> t;
    machineamount = new int[i]; //各種變數宣告
    machineoncost = new int[i];
    perkilogramcost = new int[i];
    Blimit = new int[i];
    Rate = new int[i];
    stockcost = new int[i + 1];
    initialstock = new int[i + 1];
    demand = new int [t];
    materialprice = new int[t];
    productionperday = new int *[i];
    for (int j = 0; j < i; j++)
        productionperday[j] = new int [t];
    machineonperday = new int *[i];
    for (int j = 0; j < i; j++)
        machineonperday[j] = new int [t];
    materialbuyingperday = new int [t];
    threshold = new int [i];
    materialusingperday = new int[t];


    bool* dayLimitReached = new bool[t];
    for(int r = 0; r < t; r++)
    {
        dayLimitReached[r] = false;
    }
     
    for (int j = 0; j < i; j++)//輸入各種變數
        cin >> machineamount[j];
    for (int j = 0; j < i; j++)
        cin >> machineoncost[j];
    for (int j = 0; j < i; j++)
        cin >> perkilogramcost[j];
    for (int j = 0; j < i; j++)
        cin >> Blimit[j];
    for (int j = 0; j < i; j++)
        cin >> Rate[j];
    for (int j = 0; j < i + 1; j++)
        cin >> stockcost[j];
    for (int j = 0; j < i + 1; j++)
        cin >> initialstock[j];
    for (int j = 0; j < t; j++)
        cin >> demand[j];
    for (int j = 0; j < i; j++)
        cin >> materialprice[j];
     
    int thethresholdnumber = 0, totaldemand = 0, totalrate = 1, finalproductnum = 0, plandays = 0;
     
    thethresholdnumber = thresholdCalculation(i); //生產線瓶頸
     
    totaldemand = totaldemandCalculation(t); //計算全部的需求

    for(int j = 0; j < i; j++) //總共的rate
        totalrate *= Rate[j];
     
    finalproductnum = minthreshold / totalrate; //最大產能下，最後完成品數量
     
    for(int k = 0; k < t; k++) // 如果當天的需求大於生產線瓶頸，就幾天前先做完並儲存在完成品區
    {
        demand[k] -= 10;
        initialstock[0] = 0;
        initialstock[1] = 0;
        if (demand[k] <= finalproductnum) //如果當天的demand小於生產線瓶頸
        {
            materialusingperday[k] = demand[k] * totalrate; //當天使用的材料為demand * totalrate
            totaldemand -= demand[k]; //總需求減少當天demand
        }
        else if (demand[k] > finalproductnum) //如果當天的demand大於生產線瓶頸
        {
            materialusingperday[k] = minthreshold; //當天生產到上限
            dayLimitReached[k] = true;
            int surplusmaterial = demand[k] * totalrate - minthreshold; //所需超過的材料量 為 當天的demand * totalrate - 當天使用的材料量
            for (int l = k - 1; k >= 0; k--) //查找今天以前至第一天
            {
                if (!dayLimitReached[l]) //如果使用的材料不到最大產能使用的材料量
                {
                    int materialwillplus = minthreshold - materialusingperday[l]; //宣告 會加上去的材料量 為 最大產能使用的材料量 - 查找天的使用材料量
                    if (materialwillplus < surplusmaterial) //如果會加上去的材料量小於所需超過的材料量
                    {
                        materialusingperday[l] = minthreshold; //查找的天產到最大產能
                        dayLimitReached[l] = true;
                    }
                    else if (materialwillplus > surplusmaterial) //如果會加上去的材料量大於所需超過的材料量
                    {
                            materialusingperday[l] += surplusmaterial; //查找的天產完多餘的所需超過材料量
                            materialwillplus = surplusmaterial; //會加上去的材料量為所需超過的材料量
                    }
                    surplusmaterial -= materialwillplus; //所需超過的材料量減去加上去的材料量
                }
                if (surplusmaterial == 0)
                    break;
            }
            totaldemand -= demand[k]; //整理完後，總需求減少當天的demand
        }
        if (totaldemand == 0)
            break;
    }
     
    for (int k = 0; k < t; k++)
        materialbuyingperday[k] = materialusingperday[k];
     
    for (int k = 0; k < t; k++) //在計畫天數內
    {
        double machinecount = (double)materialusingperday[k] / ((double)Rate[0] * (double)Blimit[0]);
        if (machinecount < 1)
        {
            machineonperday[0][k] = 1;
            productionperday[0][k] = materialusingperday[k] / Rate[0];
        }
        else if (machinecount >= 1)
        {
            if(machinecount - (int)machinecount > 0)
                machineonperday[0][k] = (int)machinecount + 1;
            else
                machineonperday[0][k] = (int)machinecount;
            productionperday[0][k] = materialusingperday[k] / Rate[0];
        }
        for (int j = 1; j < i; j++) //從工作站2開始
        {
            double machinecount = (double)productionperday[j - 1][k] / ((double)Rate[j] * (double)Blimit[j]);
            if (machinecount < 1)
            {
                machineonperday[j][k] = 1;
                productionperday[j][k] = productionperday[j - 1][k] / Rate[j];
            }
            else if (machinecount >= 1)
            {
                if(machinecount - (int)machinecount > 0)
                    machineonperday[j][k] = (int)machinecount + 1;
                else
                    machineonperday[j][k] = (int)machinecount;

                productionperday[j][k] = productionperday[j - 1][k] / Rate[j];
            }
        }
    }
     
    for(int j = 0; j < i; j++) //輸出每天的開機數量
    {
        cout << machineonperday[j][0];
        for (int k = 1; k < t; k++)
            cout << " " << machineonperday[j][k];
        cout << endl;
    }
     
    for(int j = 0; j < i; j++) //輸出每天的產量
    {
        cout << productionperday[j][0];
        for (int k = 1; k < t; k++)
            cout << " " << productionperday[j][k];
        cout << endl;
    }
     
    int commacount = 0;
    for(int j = 0; j < t; j++) //輸出每天的購買材料數
    {
        if (commacount == 0)
        {
            cout << materialbuyingperday[j];
            commacount = 1;
        }
        else
            cout << " " << materialbuyingperday[j];
    }
         
    delete[] machineamount;
    delete[] machineoncost;
    delete[] perkilogramcost;
    delete[] Blimit;
    delete[] Rate;
    delete[] stockcost;
    delete[] initialstock;
    delete[] demand;
    delete[] materialprice;
    for (int j = 0; j < i; j ++)
        delete[] productionperday[j];
    delete[] productionperday;
    for (int j = 0; j < i; j++)
        delete[] machineonperday[j];
    delete[] machineonperday;
    delete[] materialbuyingperday;
    delete[] threshold;
    delete[] materialusingperday;
    return 0;
}