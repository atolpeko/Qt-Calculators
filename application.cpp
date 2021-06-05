//
//  application.cpp
//  Calculator
//

#ifdef WIN2
 #define PATH_SEPARATOR "\"
#else
 #define PATH_SEPARATOR "/"
#endif


#include "application.hpp"


GUI_app::GUI_app(int &argc, char **argv,
                 Presenter &basic_calculator,
                 Presenter &engineering_calculator)

: QApplication(argc, argv),
  m_basic_calc(basic_calculator),
  m_engineering_calc(engineering_calculator)
{
    setApplicationName("Calculator");
    setApplicationVersion("1.0");
    
    set_global_style();
    make_main_window();
    make_basic_calculator();
    make_engineering_calculator();
}

void GUI_app::set_global_style()
{
    QFile styles("resources" + QString(PATH_SEPARATOR) + "styles.qss");
    styles.open(QFile::ReadOnly);
    setStyleSheet(QString::fromLatin1(styles.readAll()));
    styles.close();
}

void GUI_app::make_main_window()
{
    m_calc_widgets = new QStackedWidget(&m_main_window);
    m_main_window.setCentralWidget(m_calc_widgets);
    m_main_window.setWindowTitle("Calculator");
    make_menu_bar();
    m_main_window.show();
}

void GUI_app::make_menu_bar()
{
    auto *menu_bar = new QMenuBar(&m_main_window);
    auto *view_menu = new QMenu("View", menu_bar);
    
    auto *to_basic = new QAction("Basic calculator", view_menu);
    to_basic->setShortcut(QKeySequence::Bold);
    connect(to_basic, &QAction::triggered,
            this, &GUI_app::slot_switch_to_basic_calc);
    
    auto *to_engin = new QAction("Engineering calculator", view_menu);
    to_engin->setShortcut(QKeySequence::New);
    connect(to_engin, &QAction::triggered,
            this, &GUI_app::slot_switch_to_engin_calc);
    
    view_menu->addAction(to_basic);
    view_menu->addAction(to_engin);
    
    menu_bar->addMenu(view_menu);
    m_main_window.setMenuBar(menu_bar);
}

void GUI_app::slot_switch_to_basic_calc()
{
    m_calc_widgets->setCurrentIndex(0);
    m_main_window.resize(250, 320);
}

void GUI_app::slot_switch_to_engin_calc()
{
    m_calc_widgets->setCurrentIndex(1);
    m_main_window.resize(550, 300);
}

void GUI_app::make_basic_calculator()
{
    m_basic_calc_wgt = new QWidget(&m_main_window);
    
    // Result output field
    auto *result_label = new QLabel(m_basic_calc_wgt);
    result_label->setAlignment(Qt::AlignRight);
    QFont result_label_font;
    result_label_font.setBold(true);
    result_label_font.setPointSize(35);
    result_label->setFont(result_label_font);
    result_label->setText("0");

    // Buttons and Layout setup
    auto *wl = new QGridLayout;
    wl->setSpacing(7);
    
    wl->addWidget(result_label, 0, 0, 1, 4);
    
    wl->addWidget(make_button(&GUI_app::slot_basic_calc, "AC", true));
    wl->addWidget(make_button(&GUI_app::slot_basic_calc, "±", true));
    wl->addWidget(make_button(&GUI_app::slot_basic_calc, "%", true));
    wl->addWidget(make_button(&GUI_app::slot_basic_calc, "/", false, true));

    wl->addWidget(make_button(&GUI_app::slot_basic_calc, "7"));
    wl->addWidget(make_button(&GUI_app::slot_basic_calc, "8"));
    wl->addWidget(make_button(&GUI_app::slot_basic_calc, "9"));
    wl->addWidget(make_button(&GUI_app::slot_basic_calc, "*", false, true));

    wl->addWidget(make_button(&GUI_app::slot_basic_calc, "4"));
    wl->addWidget(make_button(&GUI_app::slot_basic_calc, "5"));
    wl->addWidget(make_button(&GUI_app::slot_basic_calc, "6"));
    wl->addWidget(make_button(&GUI_app::slot_basic_calc, "-", false, true));

    wl->addWidget(make_button(&GUI_app::slot_basic_calc, "1"));
    wl->addWidget(make_button(&GUI_app::slot_basic_calc, "2"));
    wl->addWidget(make_button(&GUI_app::slot_basic_calc, "3"));
    wl->addWidget(make_button(&GUI_app::slot_basic_calc, "+", false, true));
    
    wl->addWidget(make_button(&GUI_app::slot_basic_calc, "0"), 5, 0, 1, 2);
    wl->addWidget(make_button(&GUI_app::slot_basic_calc, "."));
    wl->addWidget(make_button(&GUI_app::slot_basic_calc, "=", false, true));

    m_basic_calc_wgt->setLayout(wl);
    
    m_calc_widgets->addWidget(m_basic_calc_wgt);
}

void GUI_app::slot_basic_calc()
{
    QString condition = qobject_cast<QPushButton*>(sender())->text();
    QString result = m_basic_calc.calculate(condition);
 
    QLabel *result_label = m_basic_calc_wgt->findChild<QLabel*>();
    result_label->setText(result);
}

void GUI_app::make_engineering_calculator()
{
    m_engineering_calc_wgt = new QWidget(&m_main_window);

    // Result output field
    auto *result_label = new QLabel(m_engineering_calc_wgt);
    result_label->setAlignment(Qt::AlignRight);
    QFont result_label_font;
    result_label_font.setBold(true);
    result_label_font.setPointSize(35);
    result_label->setFont(result_label_font);
    result_label->setText("0");

    // Buttons and Layout setup
    auto *wl = new QGridLayout;
    wl->setSpacing(5);
    
    wl->addWidget(result_label, 0, 0, 1, 9);
    
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "eᵡ", true));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "10ᵡ", true));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "xʸ", true));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "x³", true));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "x²", true));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "AC", true));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "±", true));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "%", true));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "/", false, true));

    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "log₁₀", true));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "ln", true));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "ᵞ√x", true));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "∛x", true));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "√x", true));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "7"));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "8"));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "9"));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "*", false, true));

    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "sin", true));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "cos", true));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "tan", true));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "cot", true));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "cosh", true));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "4"));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "5"));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "6"));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "-", false, true));

    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "asin", true));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "acos", true));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "atan", true));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "acot", true));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "eᵡ-1", true));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "1"));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "2"));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "3"));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "+", false, true));

    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "π"), 5, 0, 1, 2);
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "e"), 5, 2, 1, 2);
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "RAND"));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "0"), 5, 5, 1, 2);
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "."));
    wl->addWidget(make_button(&GUI_app::slot_engin_calc, "=", false, true));

    m_engineering_calc_wgt->setLayout(wl);

    m_calc_widgets->addWidget(m_engineering_calc_wgt);
}

void GUI_app::slot_engin_calc()
{
    QString condition = qobject_cast<QPushButton*>(sender())->text();
    QString result = m_engineering_calc.calculate(condition);
 
    QLabel *result_label = m_engineering_calc_wgt->findChild<QLabel*>();
    result_label->setText(result);
}

QPushButton* GUI_app::make_button(void (GUI_app:: *slot)(),
                                  const QString &text,
                                  bool is_operation,
                                  bool is_marked)
{
    auto *button = new QPushButton(text);
    button->setMinimumSize(20, 40);
    
    if (is_operation) {
        button->setObjectName("operationButton");
    }
    if (is_marked) {
        button->setObjectName("markedButton");
    }
    connect(button, &QPushButton::clicked,
            this, slot);
    
    return button;
}
