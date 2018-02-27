#include <iostream>
#include <fstream>
#include <thread>
using namespace std;

// We don't need a mutex lock here. We will use this only for reading
float mean = 0;

void generateFile(int numLines){

    srand (static_cast <unsigned> (time(0)));

    ofstream dataFile;
    dataFile.open ("randomData.txt");
    for (int i = 0; i < numLines; i++){
        float randomNum = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/300));
        //cout << randomNum << endl;
        dataFile << randomNum << "\n";
    }
    dataFile.close();
}

void showMean(){
    ifstream dataFile ("randomData.txt");

    if (dataFile.is_open())
    {
        float number;
        //float mean = 0;
        float total = 0;
        int count = 1;
        while (dataFile >> number)
        {
            total += number;
            mean = total/count;
            count++;
        }
        dataFile.close();
        cout << "Mean is " << mean << endl;
    } else {
        cout << "Unable to open file";
    }
}

void showMeanWithErrorCompensation(){
    ifstream dataFile ("randomData.txt");

    if (dataFile.is_open())
    {
        float number;
        //float mean = 0.0;
        float total = 0.0;
        float error = 0.0;
        float subTotal = 0.0;

        int count = 1;
        while (dataFile >> number)
        {
            // Kahan way to exclude error
            number = number - error;
            total += number;
            subTotal = total + number;
            error = (subTotal - total) - number;
            //

            mean = total/count;
            count++;
        }
        dataFile.close();
        cout << "Kahan mean is " << mean << endl;
    } else {
        cout << "Unable to open file";
    }
}

void userEventHandler(){
    cout << "Write 's'+enter to show the current mean 'q' to quit" << endl;

    char input;
    while (cin >> input){
        if (input == 'q'){
            cout << "Quiting interactive options" << endl;
            break;
        }
        cout << "Current mean is: " << mean << endl;
    }
}

int main() {
    //generateFile(90000000);

    // When used with float the result is different from double.
    // When using Kahan with float the results are the same as we use showMean() with double.
    // When using Kahan way with double the results are the same as they are with float.
    // So it is obvious that Kahan way corrects the mistake

    std::thread t(userEventHandler);
    //showMean();
    showMeanWithErrorCompensation();
    t.join();
    return 0;
}