//
//  presenter.cpp
//  Qt-Calculators
//

#include "presenter.hpp"


Presenter::Presenter(model::Calculator_interface &calculator)
: m_calculator(calculator) {}

QString& Presenter::calculate(const QString &condition) noexcept
{
    auto do_calculate = [this]()->QString& {
        if (m_calculator.ready_to_calculate()) {
            m_history = QString::fromStdWString(m_calculator.calculate());
        }
        return m_history;
    };
    
    if (condition == "AC") {
        m_calculator.reset();
        m_history = "0";
        return m_history;
    } else if (condition == "±") {
        m_calculator.change_sign();
        m_history = QString::fromStdWString(m_calculator.get_state());
        return m_history;
    }
    
    try {
        m_calculator.put_data(condition.toStdWString());
        m_history = QString::fromStdWString(m_calculator.get_state());
        return do_calculate();
        
    } catch (model::Calculator_interface::invalid_operation&) {
        m_calculator.reset();
        m_history = "Invalid";
        return m_history;
    } 
}
