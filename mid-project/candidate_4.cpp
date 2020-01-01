#include <iostream>
using namespace std;
 
class Machine
{
private:
    int efficiency, output, limit, machineCost, productCost;
public:
    void set_eff(int eff)
    {
        efficiency = eff;
    }
    void set_machine_cost(int onCost)
    {
        machineCost = onCost;
    }
    void set_product_cost(int pcost)
    {
        productCost = pcost;
    }
    void set_limit(int machineLimit)
    {
        limit = machineLimit;
    }
    int produce(int efficiency, int input)
    {
        return input * efficiency;
    }
    int get_limit()
    {
        return limit;
    }
    int get_on_cost()
    {
        return machineCost;
    }
    int get_produce_cost()
    {
        return productCost;
    }
    int get_eff()
    {
        return efficiency;
    }
};
 
class Warehouse
{
private:
	int* stockCostType = new int[3];
public:
	int* stockType = new int[3];
    void set_stock_cost(int a, int b, int c)
    {
        stockCostType[0] = a;
        stockCostType[1] = b;
        stockCostType[2] = c;
    }
    void set_stock(int p, int q, int r)
    {
        stockType[0] = p;
        stockType[1] = q;
        stockType[2] = r;
    }
    int get_stock_cost(int fromStation)  // 0: raw stock cost, 1: mid stock cost, 2: end stock cost
    {
        if (fromStation == 0)
        {
            return stockCostType[0];
        }
        else if (fromStation == 1)
        {
            return stockCostType[1];
        }
        else if(fromStation == 2)
        {
            return stockCostType[2];
        }
        else
        {
            return -1;
        }
    }
    int get_stock_amount(int fromStation)  // 0: raw stock  1: mid stock  2: end stock
    {
        if (fromStation == 0)
        {
            return stockType[0];
        }
        else if (fromStation == 1)
        {
            return stockType[1];
        }
        else if(fromStation == 2)
        {
            return stockType[2];
        }
        else
        {
            return -1;
        }
    }
	
	// Destructor
	~Warehouse()
	{
		delete[] stockType;
		delete[] stockCostType;
	}
};
 
bool feasible(Warehouse w, int** plnOpt, Machine** mOfStation, int nOfMachines[], int* rawBought, int* rawPrice, int* D, int T, int numOfStations, int totalCost[])
{
    for(int t = 0; t < T; t++)
    { 
        // Check whether the final output >= demands for day t
        if (w.stockType[2] + plnOpt[numOfStations - 1][t] >= D[t])
        {
            w.stockType[2] = w.stockType[2] + plnOpt[numOfStations - 1][t] - D[t];
            totalCost[4] += w.stockType[2] * w.get_stock_cost(2); // Total Storage Cost for End product(category 2nd)
 
            // For each station:
            for(int j = 0; j < numOfStations; j++)
            {
                // Calculate how many machines are required & sum up the machine turn on cost
                int limitOfMachine = mOfStation[j][0].get_limit();  
                int onCostOfMachine = mOfStation[j][0].get_on_cost();
                if(plnOpt[j][t] % limitOfMachine != 0)
                {
                    int k = ((plnOpt[j][t] / limitOfMachine) + 1);
                    if(k > nOfMachines[j])
                    {
                        return false;
                    }
                    totalCost[0] += (k * onCostOfMachine);
                } 
                else
                {
                    int k = (plnOpt[j][t] / limitOfMachine);
                    if(k > nOfMachines[j])
                    {
                        return false;
                    }
                    totalCost[0] += (k * onCostOfMachine);
                }
 
                // Sum up producing cost
                totalCost[1] += mOfStation[j][0].get_produce_cost() * plnOpt[j][t]; 
            }
        }
        else
        {
            return false;
        }
 
        // For each stations:
        for(int i = 0; i < numOfStations; i++)
        {
            int eff =  mOfStation[i][0].get_eff();
 
            // Check whether machines upper limit is exceeded
            if(plnOpt[i][t] > mOfStation[i][0].get_limit() * nOfMachines[i])
            {
                return false;
            }
 
            // Check whether raw/mid stock both meets the planned output
            if(i == 0 && (w.get_stock_amount(i) + rawBought[t]) >= plnOpt[i][t] * eff)
            {
                w.stockType[0] = w.stockType[0] + rawBought[t] - plnOpt[i][t] * eff;
                totalCost[2] += w.stockType[0] * w.get_stock_cost(i);
            }
            else if(i != 0 && w.get_stock_amount(i) + plnOpt[i - 1][t] >= plnOpt[i][t] * eff)
            {
                w.stockType[1] = w.stockType[1] + plnOpt[i - 1][t] - plnOpt[i][t] * eff;
                totalCost[3] += w.stockType[1] * w.get_stock_cost(i);
            }
            else
            {
                return false;
            }
        }       
        // Update Total Cost on Raw Material Purchase
        totalCost[5] += rawPrice[t] * rawBought[t];
    }
    return true;
}

// Generates valid plans for machine's ouput for t days
void planner(int** plannedOutput, int* rawBought, int* demands, Machines** machinesInStation, int* rawPrice)
{
	// For each day:
	// (raw material price || storage cost) higher than a certain level  --> Focus on less buying || less storing!(i.e. buy a lot raw material on the cheapest day)
	// If pro-storage:
	//		only produce to the amount that merely meets todays' needs
	// else if pro-buying:  
	//		deliberately prevent storage
}


int main()
{
    int nOfStations = 0, days = 0;
    int nOfMachines[10] = {0}; // In fact we can just have 2 elements since we have at most 2 stations.
    cin >> nOfStations >> days;

    Machine** machinesInStation = new Machine*[nOfStations];
    // Creating Machines instances & set properties
    for(int i = 0; i < nOfStations; i++)
    {
        cin >> nOfMachines[i];
        machinesInStation[i] = new Machine[nOfMachines[i]];
    }
    for(int i = 0; i < nOfStations; i++)
    {
        int c_ON = 0;
        cin >> c_ON;
        machinesInStation[i] -> set_machine_cost(c_ON);
    }
    for(int i = 0; i < nOfStations; i++)
    {
        int c_D = 0;
        cin >> c_D;
        machinesInStation[i] -> set_product_cost(c_D);
    }
    for(int i = 0; i < nOfStations; i++)
    {
        int B = 0;
        cin >> B;
        machinesInStation[i] -> set_limit(B);
    }
    for(int i = 0; i < nOfStations; i++)
    {
        int r = 0;
        cin >> r;
        machinesInStation[i] -> set_eff(r);
    }
 
    // Set costs/amounts of stock in Warehouse
    Warehouse warehouse;
    int stock_cost_raw, stock_cost_mid, stock_cost_end;
    int stock_raw, stock_mid, stock_end;
    cin >> stock_cost_raw >> stock_cost_mid >> stock_cost_end;
    warehouse.set_stock_cost(stock_cost_raw, stock_cost_mid, stock_cost_end);
    cin >> stock_raw >> stock_mid >> stock_end;
    warehouse.set_stock(stock_raw, stock_mid, stock_end);
 
    int* demands = new int[days];
    int* rawPrice = new int[days];
    int* rawBought = new int[days];
    int** plannedOutput = new int*[nOfStations];
    int totalCost[6] = {0}; // [OnCost, ProduceCost, RawStockCost, MidStockCost, EndStockCast, RawPurchaseCost]
 
    for(int i = 0; i < nOfStations; i++)
    {
        plannedOutput[i] = new int[days];
    }
    for(int i = 0; i < days; i++)
    {
        cin >> demands[i];
    }
    for(int i = 0; i < days; i++)
    {
        cin >> rawPrice[i];
    }
    
	// TODO: We want to generate our own plannedOutput & rawBought for each day now
	planner(plannedOutput, rawBought, demands, machinesInStation, rawPrice);

    // Driver:
    if(!feasible(warehouse, plannedOutput, machinesInStation, nOfMachines, rawBought, rawPrice, demands, days, nOfStations, totalCost))
    {
        cout << "-1";
    } 
    else
    {
        for(int i = 0; i < 6; i++)
        {
            if(i == 0)
            {
                cout << totalCost[i];
            }
            else
            {
                cout << "," << totalCost[i];
            }
        }
    }
    return 0;
}