/******************************************************************************
 * Copyright 2022 NVIDIA Corporation. All rights reserved.
 *****************************************************************************/

/// \file
/// \brief View model class for element selection.
///        This class contains the data model for the set of potentially selectable elements.
///        The data is unfiltered. For filtering, the corresponding proxy model is used.


#ifndef MDL_SDK_EXAMPLES_MDL_BROWSER_VM_SEL_MODEL_H
#define MDL_SDK_EXAMPLES_MDL_BROWSER_VM_SEL_MODEL_H

#include <QObject>
#include <qabstractitemmodel.h>
#include "vm_sel_element.h"
#include <stack>

class VM_nav_package;

class VM_sel_model : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit VM_sel_model(QObject* parent = nullptr);
    virtual ~VM_sel_model();

    // number of elements in this list
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
        int rowCount(const QModelIndex& = QModelIndex()) const override;

    // no further columns
    int columnCount(const QModelIndex& = QModelIndex()) const override;

    // get and set data from QML
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override { return VM_sel_element::roleNames(); }

    // get an item that is potentially selectable (before filtering)
    Q_INVOKABLE VM_sel_element* get_element(int index) const;

    // filter the shown elements based on the selected package(s)
    // in case multiple packages are added, they are logically concatenated with a OR
    Q_INVOKABLE void set_browser_package_filter(VM_nav_package* package);

signals:
    void countChanged();

private:
    VM_sel_element* pull_pooled();
    void push_pooled(VM_sel_element* element);

    QVector<VM_sel_element*> m_elements;
    std::stack<VM_sel_element*> m_pool;

};

#endif
