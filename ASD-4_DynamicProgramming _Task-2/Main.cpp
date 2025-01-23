#include <iostream>
#include <chrono>
#include <algorithm>

#define ANSI_COLOR_BLUE "\033[34m"
#define ANSI_COLOR_RESET "\033[0m"
#define ANSI_COLOR_GREEN "\033[32m"
#define ANSI_COLOR_RED "\033[31m"
#define ANSI_COLOR_PURPLE "\033[35m"
#define ANSI_COLOR_LIGHTBLUE "\033[94m"

using namespace std;

struct Rent {
	int reqN;
	chrono::system_clock::time_point start;
	chrono::system_clock::time_point finish;
	float profit;
};

class CarsRent {
private:
	chrono::year_month_day date;
	int n;
	vector<Rent> requests;
public:
	CarsRent(const int&);
	void Show();
	void GetProfits();
	static long long TimeToMinutes(chrono::system_clock::time_point&);
	static bool Compare(Rent&, Rent&);
	int LastApplication(const int&);
};

/**
 * @brief Параметричний конструктор класу
 * @param amount Кількість заявок
 */
CarsRent::CarsRent(const int& amount) {

	this->n = amount;
	auto today = chrono::floor<chrono::days>(chrono::system_clock::now());
	auto sys_date = std::chrono::sys_days(today);
	sys_date += std::chrono::days(1);
	this->date = chrono::year_month_day{ sys_date };
	this->requests.push_back({ 1, chrono::hours(24) + today, chrono::hours(24) + today + chrono::hours(3), 1000 });
	this->requests.push_back({ 2, chrono::hours(24) + today + chrono::hours(2), chrono::hours(24) + today + chrono::hours(3), 600 });
	this->requests.push_back({ 3, chrono::hours(24) + today, chrono::hours(24) + today + chrono::hours(3), 2700 });
	this->requests.push_back({ 4, chrono::hours(24) + today, chrono::hours(24) + today + chrono::hours(2), 400 });
	this->requests.push_back({ 5, chrono::hours(24) + today + chrono::hours(1), chrono::hours(24) + today + chrono::hours(2), 200 });
	this->requests.push_back({ 6, chrono::hours(24) + today + chrono::hours(5), chrono::hours(24) + today + chrono::hours(10), 1500 });
	this->requests.push_back({ 7, chrono::hours(24) + today + chrono::hours(3), chrono::hours(24) + today + chrono::hours(8), 1500 });
	this->requests.push_back({ 8, chrono::hours(24) + today + chrono::hours(2), chrono::hours(24) + today + chrono::hours(6), 600 });
	this->requests.push_back({ 9, chrono::hours(24) + today + chrono::hours(10), chrono::hours(24) + today + chrono::hours(12), 400 });
	this->requests.push_back({ 10, chrono::hours(24) + today + chrono::hours(12), chrono::hours(24) + today + chrono::hours(20), 600 });
}

/**
 * @brief Метод, який відображає інформацію щодо заявок
 */
void CarsRent::Show() {

	cout << "Date : " << ANSI_COLOR_LIGHTBLUE << this->date << ANSI_COLOR_RESET << endl;
	cout << "The amount of application : " << ANSI_COLOR_GREEN << this->n << ANSI_COLOR_RESET << endl;
	for (const auto& req : this->requests) {
		auto start_time = chrono::system_clock::to_time_t(req.start);
		auto finish_time = chrono::system_clock::to_time_t(req.finish);
		char time_bufferStart[26], time_bufferFinish[26];
		ctime_s(time_bufferStart, sizeof(time_bufferStart), &start_time);
		ctime_s(time_bufferFinish, sizeof(time_bufferFinish), &finish_time);
		cout << "!-Application #" << ANSI_COLOR_BLUE << req.reqN << ANSI_COLOR_RESET << endl
			<< "Start : " << ANSI_COLOR_GREEN << time_bufferStart << ANSI_COLOR_RESET 
			<< "Finish : " << ANSI_COLOR_RED << time_bufferFinish << ANSI_COLOR_RESET
			<< "Profit : " << ANSI_COLOR_PURPLE << req.profit << ANSI_COLOR_RESET << "$" << endl;
	}
}

/**
 * @brief Метод, який перетворює час заявок в кількість хвилин від початку епохи
 * @param t Час заявки
 * @return Кількість хвилин від початку епохи
 */
long long CarsRent::TimeToMinutes(chrono::system_clock::time_point& t) {
	
	return duration_cast<chrono::minutes>(t.time_since_epoch()).count();
}

/**
 * @brief Метод, який порівнює заявки за часом завершення
 * @param a Час завершення першої заявки
 * @param b Час завершення другої заявки
 * @return True, час завершення першої заявки менша другої, інакше - false
 */
bool CarsRent::Compare(Rent& a, Rent& b) {

	return TimeToMinutes(a.finish) < TimeToMinutes(b.finish);
}

/**
 * @brief Метод, який шукає останню заявку, що не перекривається з поточною
 * @param i Індекс поточної заявки
 * @return Індекс останньої завки, що не перекривається з поточною
 */
int CarsRent::LastApplication(const int& i) {

	for (int j = i - 1; j >= 0; j--) {
		if (this->TimeToMinutes(this->requests[j].finish) <= this->TimeToMinutes(this->requests[i].start)) {
			return j;
		}
	}
	return -99;
}

/**
 * @brief Метод, який шукає максимальний прибуток, за допомогою динамічного програмування
 */
void CarsRent::GetProfits() {

	sort(this->requests.begin(), this->requests.end(), Compare);

	vector<float> dp(this->n);
	dp[0] = this->requests[0].profit;

	cout << "dp : ";
	for (int i = 0; i < dp.size(); i++) {
		cout << dp[i] << " ";
	}cout << endl;

	for (int i = 1; i < this->n; i++) {
		float currentProfit = this->requests[i].profit;
		int lastApplication = this->LastApplication(i);

		if (lastApplication != -99) currentProfit += dp[lastApplication];

		dp[i] = max(dp[i - 1], currentProfit);
	}
	
	cout << "dp : ";
	for (int i = 0; i < dp.size(); i++) {
		cout << dp[i] << " ";
	}cout << endl;

	cout << "\n!-Max profit is : " << ANSI_COLOR_PURPLE << dp[this->n - 1] << ANSI_COLOR_RESET << "$" << endl;

	vector<int> selectedApps;
	int i = this->n - 1; 

	while (i >= 0) {
		if (i == 0) {
			selectedApps.push_back(this->requests[i].reqN);
			break;
		}
		int lastApplication = LastApplication(i);
		float currentProfit = this->requests[i].profit;
		if (lastApplication != -99) {
			currentProfit += dp[lastApplication];
		}
		if (currentProfit > dp[i - 1]) { 
			selectedApps.push_back(this->requests[i].reqN);
			i = lastApplication;
		}
		else {
			i--;
		}
	}
	reverse(selectedApps.begin(), selectedApps.end());
	cout << "Choosen requests : ";
	for (const int i : selectedApps) {
		cout << "#" << ANSI_COLOR_LIGHTBLUE << i << ANSI_COLOR_RESET << " ";
	}cout << endl;
}

int main() {

	CarsRent c(10);
	c.Show();
	c.GetProfits();

	return 0;
}