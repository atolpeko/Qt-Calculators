//
//  application.hpp
//  Calculator
//

#ifndef CALC_APPLICATION_HPP
#define CALC_APPLICATION_HPP


#include <QApplication>
#include <QFile>
#include <QStyle>
#include <QMainWindow>
#include <QStackedWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include "QLabel"
#include <QVBoxLayout>
#include <QPushButton>

#include "presenter.hpp"


class GUI_app : public QApplication {
public:
    GUI_app(int &argc, char **argv,
            Presenter& basic_calculator,
            Presenter& engineering_calculator);

private slots:
    void slot_switch_to_basic_calc();
    void slot_switch_to_engin_calc();
    void slot_basic_calc();
    void slot_engin_calc();
    
private:
    void set_global_style();
    void make_main_window();
    void make_menu_bar();
    void make_basic_calculator();
    void make_engineering_calculator();
    QPushButton* make_button(void (GUI_app:: *slot)(),
                             const QString &text,
                             bool is_operation = false,
                             bool is_marked = false);
private:
    Presenter &m_basic_calc;
    Presenter &m_engineering_calc;
    
    QMainWindow m_main_window;
    QStackedWidget *m_calc_widgets;
    QWidget *m_basic_calc_wgt;
    QWidget *m_engineering_calc_wgt;
};


#endif //CALC_APPLICATION_HPP
