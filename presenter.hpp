//
//  presenter.hpp
//  Calculator
//

#ifndef CALC_PRESENTER_HPP
#define CALC_PRESENTER_HPP


#include <QString>
#include "calculators.hpp"


class Presenter {
public:
    Presenter(model::Calculator_interface& calculator);
    QString& calculate(const QString &condition) noexcept;
    
private:
    model::Calculator_interface &m_calculator;
    QString m_history;
};


#endif //CALC_PRESENTER_HPP
