//
//  presenter.hpp
//  Qt-Calculators
//

#ifndef PRESENTER_HPP
#define PRESENTER_HPP


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


#endif // PRESENTER_HPP
