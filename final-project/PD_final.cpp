#include <iostream>
#include <string>
#include <cstdlib>
#define N 31 //共會挑選31支股票

using namespace std;

class Stock //這是每支股票都會有的資訊，詳情請見另一個txt檔。該內容依序為下面的各個變數，包含stockID/industry/term/price/risk/growth/content
{
private:
    int stockID;
    int industry; //會有四種情境：傳產為0，科技為1，泛公股金融為2，民營銀行為3
    bool term;    //會有兩種可能，短期為0，長期為1
    int price;    //會有三種價格區段，低價組為0，中價位為1，高價為2
    bool risk;    //會有兩種風險，低風險為0，高風險為1
    int growth;   //價值型為0，成長型為1，兩種符合為2
    string content;

public:
    Stock() {}
    Stock(int each_stockID, int each_industry, bool each_term, int each_price, bool each_risk, int each_growth, string each_content)
    {
        stockID = each_stockID;
        industry = each_industry; //會有四種情境：傳產為0，科技為1，泛公股金融為2，民營銀行為3
        term = each_term;         //會有兩種可能，短期為0，長期為1
        price = each_price;       //會有三種價格區段，低價組為0，中價位為1，高價為2
        risk = each_risk;         //會有三種風險，低風險為0，高風險為1
        growth = each_growth;     //價值型為0，成長型為1，兩種符合為2
        content = each_content;
    }
    ~Stock();
    int check(int *suggest_metric, bool *preferredIndustry)
    {
        if (suggest_metric[0] != term)
        { 
            return false;
        }
        if (suggest_metric[1] != price)
        {
            return false;
        }
        if (suggest_metric[2] < 6 && risk == 1) //最高12分，最低0分，所以切0-5為低風險愛好者；另外，可呈現高風險者，一樣可推薦低風險股，故如此判斷
        {
            return false;
        }
        if (suggest_metric[3] != growth)
        {
            return false;
        }
        if (preferredIndustry[industry] == false) //該股票所處產業未在使用者偏好中
        {
            return false;
        }
        return stockID; //只有通過上面排除條款者，才會保留這隻股票，否則不選，所以最後也要加一個判斷式，若沒有符合條件者，輸出某個指令
    }
    string get_content()
    {
        return content; //最後會輸出對該股票敘述
    }
};

string Suggestion(int *suggest_metric, bool *preferredIndustry, Stock *stockList)
{
    string result = "";
    int select[N] = {0};
    int cnt = 0;
    for (int i = 0; i < N; i++)
    {
        int output = stockList[i].check(suggest_metric, preferredIndustry);
        if (output)
        {
            select[cnt++] = output;
        }
    }
    int idx = rand() % cnt; //最多隨機挑兩支股票，一起輸出給它，最後面的content可以用getline處理（留待冠瑋寫完）
    result += stockList[select[idx]].get_content() + "\n";
    int tmp = 0;
    while (tmp == idx)
    {
        tmp = rand() % cnt;
    }
    result += stockList[select[tmp]].get_content() + "\n";
    return result;
}

int main(int argc, char const *argv[])
{
    srand(time(0));
    int choice = 0;
    int suggest_metric[4] = {0};     // 4種分數：可投資期間/可承受價格/可承受風險/成長型或價值型，後面根據這個分數看要設定哪個區間得什麼股票
    bool preferredIndustry[4] = {0}; //分三種產業，第一個為傳產，第二個為科技，第三個是泛公股金融，第四個是民營銀行，若不分產業就標定四者；若不分金融股就標定[2][3]
    bool terminate = false;
    Stock stockList[N]; //再補股票數

    // Age
    cout << "請選擇年齡：（請輸入號碼）" << endl;
    cout << "1. 20以下"
         << "\n"
         << "2. 20-29"
         << "\n"
         << "3. 30-39"
         << "\n"
         << "4. 40-49"
         << "\n"
         << "5. 50-59"
         << "\n"
         << "6. 60以上" << endl;
    cin >> choice;
    if (choice == 1)
    {
        terminate = true;
    }

    //
    if (terminate)
    {
        cout << "小朋友快回家";
        std::exit(1);
    }
    else
    {
        // 行業:
        cout << "-> 請選擇台股50中，所偏好行業：（請輸入號碼）" << endl; //只會有幾個行業而已，可以給三個選項就好
        cout << "1. 傳統產業"
             << "\n"
             << "2. 科技產業"
             << "\n"
             << "3. 金融業"
             << "\n"
             << "4. 不分產業" << endl;
        cin >> choice; //若選擇特定產業，只會推薦該產業股票；不分產業就隨機推薦。至多兩支。
        if (choice == 1 || choice == 2)
        {
            preferredIndustry[choice - 1] = true;
        }
        else if (choice == 4)
        {
            preferredIndustry[0] = true;
            preferredIndustry[1] = true;
            preferredIndustry[2] = true;
            preferredIndustry[3] = true;
        }
        else
        {
            cout << "-> 請選擇偏好哪一種金融股：（請輸入號碼）" << endl; //只會有幾個行業而已，可以給三個選項就好
            cout << "1. 泛公股"
                 << "\n"
                 << "2. 民營銀行"
                 << "\n"
                 << "3. 沒有類別偏好" << endl;
            cin >> choice;
            if (choice < 3)
            {
                preferredIndustry[choice + 1] = true;
            }
            else
            {
                preferredIndustry[2] = true;
                preferredIndustry[3] = true;
            }
        }
        cout << "\n";

        // 長短期:
        int termPts = 0;
        cout << "-> 通常您儲蓄時，會選擇哪一種定存:（請輸入號碼） " << endl;
        cout << "1. 三年內無法提領的定存，年報酬率2%"
             << "\n"
             << "2. 十年內無法提領的定存，年報酬率5%"
             << "\n"
             << "3. 我都不要！我要活存！" << endl;
        cin >> choice;
        if (choice == 2)
        {
            termPts += 1;
        }
        cout << "\n";

        cout << "\n"
             << "您期望的投資期間?（請輸入號碼）" << endl;
        cout << "1. 1年以內"
             << "\n"
             << "2. 1-3年"
             << "\n"
             << "3. 3-10年"
             << "\n"
             << "4. 10年以上 " << endl;
        cin >> choice;
        if (choice >= 3)
        {
            termPts++;
        }
        cout << "\n";
        suggest_metric[0] = termPts; // 算完可投資期間分數後存到suggest_metric，越大代表越長期

        // 要推薦的股價範圍：
        int pricePts = 0;
        // 年收
        cout << "請問目前的年收入?（請輸入號碼）" << endl;
        cout << "1. 50萬以下"
             << "\n"
             << "2. 50-100萬"
             << "\n"
             << "3. 100萬以上" << endl;
        cin >> choice;
        cout << "\n";

        // 詢問要投入多少
        cout << "您目前預計投入多少資金?（請輸入號碼）" << endl;
        cout << "1. 100萬以下"
             << "\n"
             << "2. 100-200萬"
             << "\n"
             << "3. 200-300萬"
             << "\n"
             << "4. 300萬以上" << endl;
        cin >> choice;
        if (choice == 2 || choice == 3)
        {
            pricePts += 1;
        }
        else if (choice == 4)
        {
            pricePts += 2;
        }
        cout << "\n";

        suggest_metric[1] = pricePts; // 算完可投資金額分數後存到suggest_metric，越大代表越可以推薦高價股

        // 風險承受判斷
        int riskPts = 0; // higher means higher risk tolerance

        cout << "您所希望一年的投資報酬率，以及相對應的損失風險?（請輸入號碼）" << endl;
        cout << "1. 低於5%"
             << "\n"
             << "2. 5-10%"
             << "\n"
             << "3. 10-20"
             << "\n"
             << "4. 20-30"
             << "\n"
             << "5. 30以上" << endl;
        cin >> choice; // 希望一年的投資報酬率，以及相對應的損失風險，前兩個選項0分，中兩個1分，最後一個2分
        if (choice == 3 || choice == 4)
        {
            riskPts += 1;
        }
        else if (choice == 5)
        {
            riskPts += 2;
        }
        cout << "\n";

        cout << "風險和報酬間您會如何取捨?（請輸入號碼）" << endl;
        cout << "1. 風險考量的比重高於報酬"
             << "\n"
             << "2. 風險和報酬考量的比重一樣"
             << "\n"
             << "3. 報酬考量的比重高於風險" << endl;
        cin >> choice;
        riskPts += (choice - 1); //最高加2分
        cout << "\n";

        cout << "若投資100萬，發生突發事件的一個禮拜內，您可以容忍的損失金額為何?（請輸入號碼）" << endl;
        cout << "1. 5萬以下"
             << "\n"
             << "2. 5~15萬"
             << "\n"
             << "3. 15~25萬"
             << "\n"
             << "4. 25~40萬"
             << "\n"
             << "5. 40萬以上" << endl;
        cin >> choice;
        riskPts += (choice - 1); //最高加4分
        cout << "\n";

        cout << "請問目前有無須償還之貸款?（請輸入號碼）" << endl;
        cout << "1. 無"
             << "\n"
             << "2. 每月1萬以下"
             << "\n"
             << "3. 每月1~3萬"
             << "\n"
             << "4. 每月3~5萬"
             << "\n"
             << "5. 每月5萬以上" << endl;
        cin >> choice;
        riskPts += (choice - 1); //最高加4分
        cout << "\n";

        suggest_metric[2] = riskPts; // 算完可承擔風險分數後存到suggest_metric，越大代表越能承受風險
                                     // 用 suggest_metric跟preferredIndustry，可以給下一個階段做歸類哪種股票
        cout << "您比較偏好下列哪種選擇？" << endl;
        cout << "1. 價值股：價值被低估但風險較低的公司"
             << "\n"
             << "2. 成長股：市場評估有潛力，但尚未開始獲利的新創公司"
             << "\n"
             << "3. 期望同時符合價值股跟成長股" << endl;
        cin >> choice;
        suggest_metric[3] = (choice - 1); //若選1，[3]是0，選價值型；選2，[3]是1，選成長型;選3，[3]是2，給符合兩種指標的；不符合成長股或價值股的就丟掉
        cout << "\n";

        return 0;
    }
}
