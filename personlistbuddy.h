﻿#ifndef PERSONLISTBUDDY_H
#define PERSONLISTBUDDY_H
#include <QWidget>
#include <QLabel>
#include <QEvent>

class PersonListBuddy : public QWidget
{
    Q_OBJECT
public:
    explicit PersonListBuddy(QWidget *parent = 0);
    void initUi();//初始化Ui
    QWidget *head;  //头像
    QLabel *name;  //用户名
    QLabel *sign;  //个性签名
    QString headPath;//头像路径
    bool eventFilter(QObject *obj, QEvent *event);//事件过滤器

};

#endif // PERSONLISTBUDDY_H