#include "datamodel.h"
#include "PlainDb.cpp"

dataModel::dataModel(QObject *parent) :
    QSqlQueryModel(parent)
{

}

QVariant dataModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);
       if (value.isValid() && role == Qt::DisplayRole)
       {
           if (index.column() == 0)
               return value.toString().prepend("#");
           else if (index.column() == 2)
               return value.toString().toUpper();
       }
       /*
       if (role == Qt::TextColorRole && index.column() == 1)
           return QVariant::fromValue(QColor(Qt::blue));*/
       return value;
}

void dataModel::showAll()
{
    setQuery(PlainDb::getInstance()->getQuery(), *PlainDb::getInstance()->db);

    setHeaderData(2, Qt::Horizontal, QObject::tr("Short Name"));
//    setHeaderData(1, Qt::Horizontal, QObject::tr(""));
//    setHeaderData(2, Qt::Horizontal, QObject::tr("Last name"));

}

void dataModel::refresh()
{
    setQuery(PlainDb::getInstance()->getQuery(), *PlainDb::getInstance()->db);
}

int dataModel::GetContactId(int row)
{
    return record(row).value(0).toInt();
}

Contact dataModel::GetContact(int id)
{
    return PlainDb::getInstance()->getContById(id);
}

void dataModel::deleteContact(int row)
{
    Contact con = GetContact(GetContactId(row));
    // Проверка наличия у контакта подчиненных узлов
    std::vector <Contact> conts;
    PlainDb::getInstance()->GetContactsListByUplevel(conts, con.getId());
    foreach (Contact cont, conts)
    { // настраиваем узлы аналогично удаляемому
        cont.setUpLevel(con.getUpLevel());
        PlainDb::getInstance()->updateContact(&cont);
    }
    PlainDb::getInstance()->deleteContact(&con);
}

void dataModel::setSeachString(QString str)
{
    PlainDb::getInstance()->setSeachString(str);
}
void dataModel::SetFilterByListId(QStringList &list)
{
    PlainDb::getInstance()->SetFilterByListId(list);
}
