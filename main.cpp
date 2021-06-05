//
//  main.cpp
//  Calculator
//

#include "application.hpp"
#include "calculators.hpp"
#include "presenter.hpp"


int main(int argc, char **argv)
{
    model::Basic_calculator calculator;
    model::Engineering_calculator engineering_calculator;

    Presenter basic_calc_controller(calculator);
    Presenter engineering_calc_controller(engineering_calculator);

    GUI_app app(argc, argv,
                basic_calc_controller,
                engineering_calc_controller);

    return GUI_app::exec();
}
