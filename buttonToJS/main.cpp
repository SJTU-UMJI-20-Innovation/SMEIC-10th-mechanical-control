#include <vector>
#include <iostream>
using namespace std;
vector<vector<double>> test;

#define LEFT 0

unsigned short stringHash(string temp) {
    unsigned long long num = 0;
    for (int i = 0; i < temp.length(); ++i)
        num = num * 2333 + temp[i];
//        cout << num << endl;
    return ((unsigned short)num);
}

int stringToInt(string temp, int* p){// stringToIntAbnormal: abnormal
    int index = 1;
    if (*p > temp.length() - 1){
        cout << "error in stringToInt!!! case 1: "<< temp << " " << *p << endl;
        return 0;
    }
    if (temp[*p] == ' '){
        cout << "error in stringToInt!!! case 2: "<< temp << " " << *p << endl;
        return 0;
    }
    if (temp[*p] == '-') {
        index = -1;
        (*p)++;
    }
    int sum = 0;
    for (int i = *p; i < temp.length(); ++i) {
        if (temp[i] == ' ') {
            *p = i;
            return index * sum;
        }
        if (temp[i] < '0' || temp [i] > '9'){
            cout << "error in stringToInt!!! case 3: "<< temp << " " << *p << endl;
            return 0;
        }
        sum = sum * 10 + temp[i] - '0';
    }
    return index * sum;
}

double stringToDouble(string temp, int* p){//stringToDoubleAbnormal: abnormal
    double index = 1.0;
    if (*p > temp.length() - 1){
        cout << "error in stringToDouble!!! case 1: "<< temp << " " << *p << endl;
        return 0;
    }
    if (temp[*p] == ' '){
        cout << "error in stringToDouble!!! case 2: "<< temp << " " << *p << endl;
        return 0;
    }
    if (temp[*p] == '-') {
        index = -1.0;
        (*p)++;
    }
    double sum = 0;
    bool ifAfterPoint = false;
    double afterPointNum = 0.1;
    for (int i = *p; i < temp.length(); ++i) {
        if (temp[i] == ' ') {
            *p = i;
            return index * sum;
        }
        if (temp[i] == '.')
            if (ifAfterPoint){
                cout << "error in stringToDouble!!! case 3: "<< temp << " " << *p << endl;
                return 0;
            }
            else {
                ifAfterPoint = true;
                continue;
            }
        if (temp[i] < '0' || temp [i] > '9'){
            cout << "error in stringToDouble!!! case 4: "<< temp << " " << *p << endl;
            return 0;
        }
        else
        if (ifAfterPoint) {
            sum = sum + (double)(temp[i] - '0') * afterPointNum;
            afterPointNum *= (double)0.1;
        }
        else
            sum = sum * 10 + temp[i] - '0';
    }
    return index * sum;
}

int main() {
    freopen("js.txt", "w", stdout);
    freopen("button.txt", "r", stdin);
//    cout << stringHash("gtSl") << endl;
//    cout << stringHash("rtSvTwo") << endl;
//    cout << stringHash("rtSvOne") << endl;
//    cout << stringHash("") << endl;
//    cout << stringHash("mvMvBs") << endl;
    cout << "\"use strict\"\n"
            "const socket = new WebSocket('ws://192.168.1.3:2051');\n"
            "\n"
            "const   baseSpeed       = (10.0 / 20.0),\n"
            "        liftSpeed       = (18.0 / 30.0),\n"
            "        rotationSpeed   = (180.0 / 6.0),\n"
            "        timeDelta       = 5.0;\n\n" << endl;
    string cmd, cmdName;
    while (getline(cin, cmdName)){
        if (cmdName[0] == '!')
            continue;
        if (cmdName == "file end")
            break;
        cout << "function " << cmdName << "(inputState){\n";
        cout << "    var {time, ...state} = inputState;\n"
             << "    state.time = 0;\n";
        while (getline(cin, cmd)) {
            if (cmd[0] == '!')
                continue;
            if (cmd == "cmd end") {
                cout << "    state.time += timeDelta;\n"
                     << "    return state;\n";
                break;
            }
            cout << "    socket.send(\"" << cmd << "\");\n";
            string cmdHead = "";
            int p = 0;
            for (; p < cmd.length(); ++p)
                if (cmd[p] != ' ')
                    cmdHead += cmd[p];
                else
                    break;
            p++;
            switch (stringHash(cmdHead)) {
                case 2265://rtRtBs
                    if (stringToInt(cmd, &p) == LEFT) {
                        p++;
                        double index = stringToDouble(cmd, &p);
                        cout << "    state.time += Math.abs(state.leftRotate - (" << index << ")) / rotationSpeed;\n"
                             << "    state.leftRotate = (" << index << ");\n";
                    } else {
                        p++;
                        double index = stringToDouble(cmd, &p);
                        cout << "    state.time += Math.abs(state.rightRotate - (" << index << ")) / rotationSpeed;\n"
                             << "    state.rightRotate = (" << index << ");\n";
                    }
                    break;
                case 43457://lfLfBs
                    if (stringToInt(cmd, &p) == LEFT) {
                        p++;
                        double index = stringToDouble(cmd, &p);
                        cout << "    state.time += Math.abs(state.leftLift - (" << index << ")) / liftSpeed;\n"
                             << "    state.leftLift = (" << index << ");\n";
                    } else {
                        p++;
                        double index = stringToDouble(cmd, &p);
                        cout << "    state.time += Math.abs(state.rightLift - (" << index << ")) / liftSpeed;\n"
                             << "    state.rightLift = (" << index << ");\n";
                    }
                    break;
                case 13395: {//mvMvBs
                    double index = stringToDouble(cmd, &p);
                    cout << "    state.time += Math.abs(state.base - (" << index << ")) / baseSpeed;\n"
                         << "    state.base = (" << index << ");\n";
                    break;
                }
                case 3722: {//dlAm
                    stringToInt(cmd, &p);
                    p++;
                    double index = stringToDouble(cmd, &p);
                    cout << "    state.time += " << index << ";\n";
                    break;
                }
                case 14289:
                case 22773:
                case 50297://mvAm rtSvOne rtSvTwo
                    cout << "    state.time += 1;\n";
                    break;
                case 5092: {//gtLq
                    stringToInt(cmd, &p);
                    p++;
                    double index = stringToDouble(cmd, &p);
                    cout << "    state.time += " << index << ";\n";
                    break;
                }
                case 21418: {//gtSl
                    double index = stringToDouble(cmd, &p);
                    cout << "    state.time += " << index << ";\n";
                    break;
                }
            }
        }
        cout << "}\n\n";
    }
    fclose(stdin);
    fclose(stdout);
}