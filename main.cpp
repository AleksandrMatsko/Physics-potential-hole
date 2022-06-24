#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

double epsilon = 0.00001;
double STEP = 0.0001;
double width_hole = 1;

double symmetricFunc(double x) {
    double arg_tan = sqrt((x + 1) / 2) * width_hole;
    double right_val = 1 / sqrt(1 / fabs(x) - 1);
    return tan(arg_tan) - right_val;
}

double asymmetricFunc(double x) {
    double arg_tan = sqrt((x + 1) / 2) * width_hole;
    double right_val = 1 / sqrt(1 / fabs(x) - 1);
    return 1 / tan(arg_tan) - right_val;
}

double sgn(double x) {
    if (x < 0) {
        return -1;
    }
    else if (x > 0) {
        return 1;
    }
    return 0;
}

double binorySearch(double left, double right, double (*func)(double)) {
    if (right < left) {
        return -2;
    }
    std::cout << left << " " << right << std::endl;
    while (true) {
        double mid = (right + left) / 2;
        double mid_func = func(mid);
        double left_func = func(left);
        double right_func = func(right);
        if (sgn(left_func) == sgn(right_func)) {
            return -3;
        }
        if (sgn(mid_func) == 0 || fabs(mid_func) < epsilon) {
            return mid;
        }
        else if (sgn(left_func) * sgn(mid_func) < 0) {
            right = mid;
        }
        else if (sgn(mid_func) * sgn(right_func) < 0) {
            left = mid;
        }
    }
}

int main() {

    std::cout << "Enter width of hole" << std::endl;
    std::cin >> width_hole;

    double start = -1 + STEP;
    double prev_func_sym = symmetricFunc(start);
    double prev_func_asym = asymmetricFunc(start);
    start += STEP;
    std::vector<std::pair<double, bool>> solutions;

    int iteration = 0;
    while (start < 0) {
        double cur_func_sym = symmetricFunc(start);
        if (sgn(cur_func_sym) * sgn(prev_func_sym) < 0 && fabs(cur_func_sym) < 10 && fabs(prev_func_sym) < 10) {
            std::cout << start - STEP << " " << prev_func_sym << std::endl;
            std::cout << start << " " << cur_func_sym << std::endl;
            double res = binorySearch(start - STEP, start, &symmetricFunc);
            if (res >= -1 && res <= 0) {
                solutions.push_back(std::pair<double, bool>(res, true));
            }
            std::cout << std::endl;
        }
        else if (sgn(cur_func_sym) == 0) {
            solutions.push_back(std::pair<double, bool>(start, true));
        }
        double cur_func_asym = asymmetricFunc(start);
        if (sgn(cur_func_asym) * sgn(prev_func_asym) < 0 && fabs(cur_func_asym) < 10 && fabs(prev_func_asym) < 10) {
            std::cout << start - STEP << " " << prev_func_asym << std::endl;
            std::cout << start << " " << cur_func_asym << std::endl;
            double res = binorySearch(start - STEP, start, &asymmetricFunc);
            if (res >= -1 && res <= 0) {
                solutions.push_back(std::pair<double, bool>(res, false));
            }
            std::cout << std::endl;
        }
        else if (sgn(cur_func_asym) == 0) {
            solutions.push_back(std::pair<double, bool>(start, true));
        }
        prev_func_asym = cur_func_asym;
        prev_func_sym = cur_func_sym;
        start += STEP;
        if (iteration % 100 == 0) {
            std::cout << "iteration: " << iteration << std::endl;
        }
        iteration += 1;
    }

    std::ofstream out;
    out.open("out.txt");

    out << solutions.size() << std::endl;
    for (std::pair<double, bool> solution : solutions) {
        out << solution.first << std::endl;
        double k1 = sqrt(2 * (solution.first + 1));
        out << k1 << std::endl;
        double k2 = sqrt(2 * fabs(solution.first));
        out << k2 << std::endl;
        double B = 1;
        out << B << std::endl;
        double C = exp(k2 * width_hole / 2);
        if (solution.second) {
            C *= cos(k1 * width_hole / 2);
        }
        else {
            C *= sin(k1 * width_hole / 2);
        }
        out << C << std::endl;
    }
    out.close();
    std::cout << "done" << std::endl;
    return 0;
}
