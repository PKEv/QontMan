#include "datamodel.h"
#include "PlainDb.cpp"
#include <QDebug>

dataModel::dataModel(QObject *parent) :
    QSqlQueryModel(parent)
{

}

dataModel::~dataModel()
{
    delete PlainDb::getInstance();
}

QVariant dataModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    if(orientation == Qt::Vertical && role == Qt::DisplayRole)
    {
        return section;
    }
    if ((orientation == Qt::Horizontal) && (role == Qt::DisplayRole))
    {
        switch (section) {
        case 2:
            return QString(tr("Имя"));
            break;
        case 3:
            return QString(tr("Телефон"));
            break;
        case 4:
            return QString(tr("Факс"));
            break;
        case 5:
            return QString(tr("Эл.почта"));
            break;
        case 6:
            return QString(tr("Сайт"));
            break;
        case 7:
            return QString(tr("Заметка"));
            break;
        default:

            break;
        }
    }
    if (role == Qt::FontRole)
    {
        return QFont("Arial", 10, QFont::Bold, false);
    }

    return QVariant();
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
       if (role == Qt::BackgroundRole && (index.row()%2) )
           return QVariant::fromValue(QColor(qRgb(240, 240, 240)));
       return value;
}

void dataModel::showAll()
{
    setQuery(PlainDb::getInstance()->getQuery(), *PlainDb::getInstance()->db);
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
