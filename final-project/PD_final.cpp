#include <iostream>
using namespace std;


int main(int argc, char const *argv[])
{
	int choice = 0;
	int* suggest_metric = new int[3]; 	// 3種分數：可投資期間/可承受風險/可承受價格，後面根據這個分數看要設定哪個區間得什麼股票
	bool* preferredIndustry = new bool[9]; 	// 喜歡哪種行業，那天討論完應該是不會用到了
	bool terminate = false;

	// Age
	cout << "請選擇年齡：" << endl;
	cout << "1. 20以下" << "\n" << "2. 20-29" << "\n" << "3. 30-39" << "\n" << "4. 40-49" << "\n" << "5. 50-59" << "\n" << "6. 60以上" << endl;
	cin >> choice;
	if(choice == 1)
	{
		terminate = true;
	}

	// 
	if(terminate)
	{
		cout << "小朋友快回家";
		std::exit(1);
	}
	else
	{
		// 行業:
		cout << "->請選擇偏好行業：" << endl;
		cout << "1. 農林漁牧" << "\n" << "2. 製造業/營造業" << "\n" << "3. 運輸/倉儲/通信業" << "\n" << "4. 工商服務業" << "\n" << "5. 公共行政業" << "\n" << "6. 金融保險業" << "\n" << "7. 社會/個人服務業" << "\n" << "其他" << endl;
		cin >> choice;
		preferredIndustry[choice - 1] = true;
		cout << "\n";
		
		// 長短期:
		int termPts = 0;
		cout << "->通常您儲蓄時，會選擇哪一種定存: " << endl;
		cout << "1. 三年內無法提領的定存，年報酬率2%" << "\n" << "2. 十年內無法提領的定存，年報酬率5%" << "\n" << "3. 我都不要！我要活存！" << endl;
		cin >> choice;		
		if(choice == 2)
		{
			termPts += 1;
		}
		cout << "\n";

		cout << "\n" << "您期望的投資期間?" << endl;
		cout << "1. 1年以內" << "\n" << "2. 1-3年" << "\n" << "3. 3-10年" << "\n" << "4. 10年以上 "<< endl;
		cin >> choice;
		if(choice == 3 || choice == 4)
		{
			termPts += 1;
		}
		cout << "\n";
		suggest_metric[0] = termPts;	// 算完可投資期間分數後存到suggest_metric，越大代表越長期


		// 要推薦的股價範圍：
		int pricePts = 0;
		// 年收
		cout << "請問目前的年收入?" << endl;
		cout << "1. 50萬以下" << "\n" << "2. 50-100萬" << "\n" << "3. 100萬以上" << endl;
		cin >> choice;
		cout << "\n";

		// 詢問要投入多少
		cout << "您目前預計投入多少資金?" << endl;
		cout << "1. 100萬以下" << "\n" << "2. 100-200萬" << "\n" << "3. 200-300萬" << "\n" << "4. 300萬以上" << endl;
		cin >> choice;
		if(choice == 2 || choice == 3) 
		{
			pricePts += 1;
		}
		else if(choice == 4)
		{
			pricePts += 2;
		}
		cout << "\n";

		suggest_metric[1] = pricePts;	// 算完可投資金額分數後存到suggest_metric，越大代表越可以推薦高價股


		// 風險承受判斷
		int riskPts = 0;	// higher means higher risk tolerance

		cout << "您所希望一年的投資報酬率，以及相對應的損失風險?" << endl;
		cout << "1. 低於5%" << "\n" << "2. 5-10%" << "\n" << "3. 10-20" << "\n" << "4. 20-30" << "\n" << "5. 30以上" << endl;
		cin >> choice; 	// 希望一年的投資報酬率，以及相對應的損失風險，前兩個選項1分，中兩個2分，最後一個3分
		riskPts += (choice - 1);
		cout << "\n";


		cout << "風險和報酬間您會如何取捨?" << endl;
		cout << "1. 風險考量的比重高於報酬" << "\n" << "2. 風險和報酬考量的比重一樣" << "\n" << "3. 報酬考量的比重高於風險" << endl;
		cin >> choice;
		riskPts += (choice - 1);
		cout << "\n";

		cout << "若投資100萬，發生突發事件的一個禮拜內，您可以容忍的損失金額為何?" << endl;
		cout << "1. 5萬以下" << "\n" << "2. 5~15萬" << "\n" << "3. 15~25萬" << "\n" << "4. 25~40萬" << "\n" << "5. 40萬以上" << endl;
		cin >> choice;
		riskPts += (choice - 1);
		cout << "\n";

		cout << "請問目前有無須償還之貸款?" << endl;
		cout << "1. 無" << "\n" << "2. 每月1萬以下" << "\n" << "3. 每月1~3萬" << "\n" << "4. 每月3~5萬" << "\n" << "5. 每月5萬以上" << endl;
		cin >> choice;
		riskPts += (choice - 1);
		cout << "\n";

		suggest_metric[2] = riskPts;	// 算完可承擔風險分數後存到suggest_metric，越大代表越能承受風險
										// 用 suggest_metric跟preferredIndustry，可以給下一個階段做歸類哪種股票

		delete[] preferredIndustry;
		return 0;
	}
}
