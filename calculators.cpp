//
//  calculators.cpp
//  Calculator
//

#include "calculators.hpp"


using namespace model;
using namespace calc_implementation;


Basic_calculator::Basic_calculator()
: m_get_result_is_pressed(false)
{
    m_binary_oper[L"+"] = [](double x, double y) { return x + y; };
    m_binary_oper[L"-"] = [](double x, double y) { return x - y; };
    m_binary_oper[L"*"] = [](double x, double y) { return x * y; };
    m_binary_oper[L"/"] = [](double x, double y) { return (y == 0.0) ? 0.0 : x / y; };
    
    m_unary_oper[L"%"] = [](double x) { return x / 100.0; };
}

std::wstring& Basic_calculator::calculate()
{
    // Single-operand operations should be executed without pressing '='
    auto u_operation = m_unary_oper.find(m_operation.data());
    if (u_operation != m_unary_oper.end()) {
        return do_unary(u_operation->second);
    }
    
    auto b_operation = m_binary_oper.find(m_operation.data());
    if (b_operation == m_binary_oper.end()) {
        throw invalid_operation();
    }
    if (m_get_result_is_pressed) {
        m_get_result_is_pressed = false;
        return do_binary(b_operation->second);
    } else {
        throw insufficient_data();
    }
}

std::wstring& Basic_calculator::do_binary(p_binary_oper operation)
{
    if (m_first_operand.empty() || m_second_operand.empty()) {
        throw insufficient_data();
    }
    double first_operand = std::stod(m_first_operand);
    double second_operand = std::stod(m_second_operand);
    double result = operation(first_operand, second_operand);
    
    m_first_operand = std::to_wstring(result);
    remove_extra_chars();
    m_second_operand.clear();
    m_operation.clear();
    
    return m_first_operand;
}

std::wstring& Basic_calculator::do_unary(p_unary_oper operation)
{
    if (m_first_operand.empty()) {
        throw insufficient_data();
    }
    if (!m_second_operand.empty()) {
        throw invalid_operation();
    }
    double operand = std::stod(m_first_operand);
    double result = operation(operand);
    
    m_first_operand = std::to_wstring(result);
    remove_extra_chars();
    m_operation.clear();
    
    return m_first_operand;
}

void Basic_calculator::put_data(const std::wstring &data)
{
    auto data_is_number = [this, data]()->bool  {
        for (auto &symbol : data) {
            if ((symbol < '0' || symbol > '9') && symbol != '.') {
                return false;
            }
        }
        return true;
    };
    
    if (data == L"=") {
        if (!m_first_operand.empty() && !m_second_operand.empty()) {
            m_get_result_is_pressed = true;
        }
        return;
    }
    if (data_is_number()) {
        put_operand(data);
    } else {
        put_operation(data);
    }
}

void Basic_calculator::put_operand(const std::wstring &data) noexcept
{
    auto dot_is_valid = [data](std::wstring &operand)->bool {
        return !operand.empty() &&
        (std::find(operand.begin(), operand.end(), L'.') == operand.end());
    };
    
    if (m_operation.empty()) { // First operand
        if (data == L"." && !dot_is_valid(m_first_operand)) {
            return;
        }
        m_first_operand.append(data);
    } else { // Second operand
        if (data == L"." && !dot_is_valid(m_second_operand)) {
            return;
        }
        m_second_operand.append(data);
    }
}

void Basic_calculator::put_operation(const std::wstring &operation)
{
    if (m_unary_oper.find(operation) == m_unary_oper.end() &&
        m_binary_oper.find(operation) == m_binary_oper.end()) {
        throw invalid_operation();
    }
    if (m_first_operand.empty()) {
        return;
    }
    m_operation = std::move(operation);
}

void Basic_calculator::reset() noexcept
{
    m_first_operand.clear();
    m_second_operand.clear();
    m_operation.clear();
    m_get_result_is_pressed = false;
}

void Basic_calculator::change_sign()
{
    std::wstring *required_operand = nullptr;
    if (!m_first_operand.empty() && !m_second_operand.empty()) {
        required_operand = &m_second_operand;
    } else if (!m_first_operand.empty()) {
        required_operand = &m_first_operand;
    } else {
        throw invalid_operation();
    }

    if (required_operand->at(0) == '-') {
        required_operand->erase(required_operand->begin());
    } else {
        required_operand->insert(required_operand->begin(), '-');
    }
}

std::wstring Basic_calculator::get_state() const noexcept
{
    if (!m_first_operand.empty()) {
        return m_first_operand + m_operation + m_second_operand;
    } else {
        return L"0";
    }
}

bool Basic_calculator::ready_to_calculate() const noexcept
{
    if (m_operation.empty()) {
        return false;
    }
    if (m_operation == L"±") {
        return !m_first_operand.empty() || !m_second_operand.empty();
    }
    
    auto u_operation = m_unary_oper.find(m_operation.data());
    if (u_operation != m_unary_oper.end()) {
        return !m_first_operand.empty() && m_second_operand.empty();
    }
    auto b_operation = m_binary_oper.find(m_operation.data());
    if (b_operation != m_binary_oper.end()) {
        return !m_first_operand.empty() && !m_second_operand.empty() &&
               !m_operation.empty() && m_get_result_is_pressed;
    }
}

void Basic_calculator::remove_extra_chars() noexcept
{
    if (m_first_operand == L"inf" || m_first_operand == L"nan") {
        return;
    }
    auto dot = std::find(m_first_operand.begin(), m_first_operand.end(), '.');
    
    // If the number is an integer, remove all symbols after the dot
    bool is_int = true;
    for (auto symbol = dot + 1; symbol != m_first_operand.end(); ++symbol) {
        if (*symbol != L'0') {
            is_int = false;
        }
    }
    if (is_int) {
        m_first_operand.erase(dot, m_first_operand.end());
        return;
    }
    // If the result is not an integer, remove zero-substring at the end of it
    for (auto last = dot + 1; last != m_first_operand.end(); ++last) {
        if (*last > L'0') {
            if (*(last + 1) == L'0') {
                m_first_operand.erase(last + 1, m_first_operand.end());
                return;
            }
        }
    }
}


Engineering_calculator::Engineering_calculator()
{
    m_binary_oper[L"xʸ"] = [](double x, double y) { return std::pow(x, y); };
    m_binary_oper[L"ᵞ√x"] = [](double x, double y) { return std::pow(x, 1.0 / y); };
    
    m_unary_oper[L"10ᵡ"] = [](double x) { return std::pow(10.0, x); };
    m_unary_oper[L"eᵡ"] = [](double x) { return std::exp(x); };
    m_unary_oper[L"x²"] = [](double x) { return std::pow(x, 2); };
    m_unary_oper[L"x³"] = [](double x) { return std::pow(x, 3); };
    m_unary_oper[L"√x"] = [](double x) { return std::sqrt(x); };
    m_unary_oper[L"∛x"] = [](double x) { return std::cbrt(x); };
    m_unary_oper[L"log₁₀"] = [](double x) { return std::log10(x); };
    m_unary_oper[L"ln"] = [](double x) { return std::log(x); };
    m_unary_oper[L"sin"] = [](double x) { return std::sin(x); };
    m_unary_oper[L"cos"] = [](double x) { return std::cos(x); };
    m_unary_oper[L"cosh"] = [](double x) { return std::cosh(x); };
    m_unary_oper[L"tan"] = [](double x) { return std::tan(x); };
    m_unary_oper[L"cot"] = [](double x) { return std::cos(x) / std::sin(x); };
    m_unary_oper[L"asin"] = [](double x) { return std::asin(x); };
    m_unary_oper[L"acos"] = [](double x) { return std::acos(x); };
    m_unary_oper[L"atan"] = [](double x) { return std::atan(x); };
    m_unary_oper[L"acot"] = [](double x) { return Constants::Pi - std::atan(-x); };
    m_unary_oper[L"eᵡ-1"] = [](double x) { return std::exp(x) - 1.0; };
}

void Engineering_calculator::put_data(const std::wstring &data)
{
    auto put_constant = [this](std::wstring constant) mutable {
        if (m_operation.empty()) {
            m_first_operand = constant;
        } else {
            m_second_operand = constant;
        }
    };
    
    if (data == L"π") {
        put_constant(std::to_wstring(Constants::Pi));
    } else if (data == L"e") {
        put_constant(std::to_wstring(Constants::Exponent));
    } else if (data == L"RAND") {
        put_constant(std::to_wstring(std::rand() % 1000));
    } else {
        this->Basic_calculator::put_data(data);
    }
}
