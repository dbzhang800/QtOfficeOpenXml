#include <QtOfficeOpenXml>

using namespace QtOfficeOpenXml;

void createXlsxDocument(const QString &name);

int main()
{
    //We create a xlsx file first.
    QString documentName("abcd.xlsx");
    createXlsxDocument(documentName);

    //Read the file.
    Sml::Document xlsx(documentName);
    qDebug()<<xlsx.cellAt("A1")->rawValue();
    qDebug()<<xlsx.cellAt("Sheet1!C5")->rawValue();

    return 0;
}

void createXlsxDocument(const QString &documentName)
{
    Sml::Document xlsx;
    xlsx.addSheet("Sheet1");
    xlsx.currentWorksheet()->writeNumber("A1", 1);
    xlsx.currentWorksheet()->writeNumber("C5", 10000);

    xlsx.saveAs(documentName);
}
