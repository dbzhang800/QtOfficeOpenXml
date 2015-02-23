#include <QtOfficeOpenXml>

using namespace QtOfficeOpenXml;

int main()
{
    Sml::Document xlsx;
    xlsx.addSheet("Sheet1");
    xlsx.currentWorksheet()->writeNumber("A1", 1);
    xlsx.currentWorksheet()->writeNumber("C5", 10000);

    xlsx.save();//Book1.xlsx
    xlsx.saveAs("Book2.xlsx", Ooxml::StrictSchame);

    return 0;
}
