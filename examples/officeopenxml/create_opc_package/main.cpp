/****************************************************************************
**
** Copyright (C) 2014 Debao Zhang <hello@debao.me>
**
** This library is free software; you can redistribute it and/or modify
** it under the terms of the GNU Library General Public License version
** 2.1 or version 3 as published by the Free Software Foundation.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Library General Public License for more details.
**
** You should have received a copy of the GNU Library General Public License
** along with this library; see the file LICENSE.LGPLv21 and LICENSE.LGPLv3
** included in the packaging of this file. If not, write to the Free
** Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
** 02110-1301, USA.
**
****************************************************************************/
#include <opcpackage.h>
#include <opcpackagepart.h>
#include <opcpackageproperties.h>

using namespace QtOfficeOpenXml;

/*
 * Shows how to generate an xlsx document using opc classes.
*/

const static char * const wbContentType = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml";
const static char * const wbRelationshipType = "http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument";
const static char * const wsContentType = "application/vnd.openxmlformatsofficedocument.spreadsheetml.worksheet+xml";
const static char * const wsRelationshipType = "http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet";

int main(int /*argc*/, char * /*argv[]*/)
{
    //Open the package in writeOnly mode.
    Opc::Package *package = Opc::Package::open("hello.xlsx", QIODevice::WriteOnly);

    //Create the workbook part.
    Opc::PackagePart *wb = package->createPart("/workbook.xml", wbContentType);
    package->createRelationship(wb->partName(), Opc::Internal, wbRelationshipType);

    //Write data to workbook.
    QIODevice *wbStream = wb->getDevice();
    wbStream->write(
                "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
                "<workbook xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\">"
                "<sheets>"
                "<sheet name=\"Sheet1\" sheetId=\"1\" r:id=\"rId1\"/>"
                "</sheets>"
                "</workbook>");
    wb->releaseDevice();

    //Create the sheet1 part.
    Opc::PackagePart *ws1 = package->createPart("/sheet1.xml", wsContentType);
    wb->createRelationship(ws1->partName(), Opc::Internal, wsRelationshipType);

    //Write data to sheet1.
    QIODevice * ws1Stream = ws1->getDevice();
    ws1Stream->write("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
                     "<worksheet xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\">"
                     "<sheetData>"
                     "<row r=\"1\">"
                     "<c r=\"A1\"><v>1</v></c>"
                     "<c r=\"B1\"><v>2</v></c>"
                     "<c r=\"C1\"><v>3</v></c>"
                     "</row>"
                     "</sheetData>"
                     "</worksheet>");
    ws1->releaseDevice();

    Opc::PackageProperties *coreProperties = package->packageProperties();
    coreProperties->setCreator("QtOfficeOpenXml");
    coreProperties->setTitle("Create Package");
    coreProperties->setDescription("Demo for Opc classes.");

    package->close();
    delete package;
}
