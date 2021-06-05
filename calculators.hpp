//
//  calculators.hpp
//  Qt-Calculators
//

#ifndef CALCULATORS_HPP
#define CALCULATORS_HPP


#include <cmath>

#include <string>
#include <map>
#include <exception>


namespace model {
    class Calculator_interface;
    class Basic_calculator;
    class Engineering_calculator;
}

namespace calc_implementation {
    struct Constants;
}


class model::Calculator_interface {
public:
    class insufficient_data : public std::exception {};
    class invalid_operation : public std::exception {};

public:
    virtual ~Calculator_interface() = default;

    virtual std::wstring& calculate() = 0;
    virtual void put_data(const std::wstring &data) = 0;
    virtual void reset() noexcept = 0;
    virtual void change_sign() = 0;
    virtual std::wstring get_state() const noexcept = 0;
    virtual bool ready_to_calculate() const noexcept = 0;
};


class model::Basic_calculator : public Calculator_interface {
protected:
    using p_binary_oper = double(*)(double, double);
    using p_unary_oper = double(*)( double);
    
public:
    Basic_calculator();
    virtual ~Basic_calculator() = default;

    std::wstring& calculate() override;
    void put_data(const std::wstring &data) override;
    void reset() noexcept override;
    void change_sign() override;
    std::wstring get_state() const noexcept override;
    bool ready_to_calculate() const noexcept override;

private:
    std::wstring& do_binary(p_binary_oper &operation);
    std::wstring& do_unary(p_unary_oper &operation);
    void put_operand(const std::wstring &data) noexcept;
    void put_operation(const std::wstring &data);
    void remove_extra_symbols() noexcept;

protected:
    //Operation-function_pointer pairs
    std::map<std::wstring, p_binary_oper> m_binary_operations;
    std::map<std::wstring, p_unary_oper> m_unary_operations;
    
    std::wstring m_first_operand;
    std::wstring m_second_operand;
    std::wstring m_operation;
    bool m_get_result_is_pressed;
};


class model::Engineering_calculator : public Basic_calculator {
public:
    Engineering_calculator();
    virtual ~Engineering_calculator() = default;
    
    void put_data(const std::wstring &data) override;
};


struct calc_implementation::Constants {
    static constexpr double Pi = 3.141592653;
    static constexpr double Exponent = 2.718281828;
    
    Constants() = delete;
};


#endif // CALCULATORS_HPP
