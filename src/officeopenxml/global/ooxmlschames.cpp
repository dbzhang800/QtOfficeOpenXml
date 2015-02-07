/****************************************************************************
**
** Copyright (C) 2014-2015 Debao Zhang <hello@debao.me>
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
#include "ooxmlschames.h"
#include "ooxmlschames_p.h"

namespace QtOfficeOpenXml {
namespace Ooxml {

SchamesPrivate::SchamesPrivate()
{
    //Add strict and transitional namespace pairs
    addNamespace(NS_Descriptions_Base,
                 QStringLiteral("http://descriptions.openxmlformats.org/description/base"),
                 QStringLiteral("http://purl.oclc.org/ooxml/descriptions/base"));
    addNamespace(NS_Descriptions_Full,
                 QStringLiteral("http://descriptions.openxmlformats.org/description/full"),
                 QStringLiteral("http://purl.oclc.org/ooxml/descriptions/full"));
    addNamespace(NS_Drawingml_Chart,
                 QStringLiteral("http://schemas.openxmlformats.org/drawingml/2006/chart"),
                 QStringLiteral("http://purl.oclc.org/ooxml/drawingml/chart"));
    addNamespace(NS_Drawingml_ChartDrawing,
                 QStringLiteral("http://schemas.openxmlformats.org/drawingml/2006/chartDrawing"),
                 QStringLiteral("http://purl.oclc.org/ooxml/drawingml/chartDrawing"));
    addNamespace(NS_Drawingml_Diagram,
                 QStringLiteral("http://schemas.openxmlformats.org/drawingml/2006/diagram"),
                 QStringLiteral("http://purl.oclc.org/ooxml/drawingml/diagram"));
    addNamespace(NS_Drawingml_Main,
                 QStringLiteral("http://schemas.openxmlformats.org/drawingml/2006/main"),
                 QStringLiteral("http://purl.oclc.org/ooxml/drawingml/main"));
    addNamespace(NS_Drawingml_Picture,
                 QStringLiteral("http://schemas.openxmlformats.org/drawingml/2006/picture"),
                 QStringLiteral("http://purl.oclc.org/ooxml/drawingml/picture"));
    addNamespace(NS_Drawingml_SpreadsheetDrawing,
                 QStringLiteral("http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing"),
                 QStringLiteral("http://purl.oclc.org/ooxml/drawingml/spreadsheetDrawing"));
    addNamespace(NS_Drawingml_WordprocessingDrawing,
                 QStringLiteral("http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing"),
                 QStringLiteral("http://purl.oclc.org/ooxml/drawingml/wordprocessingDrawing"));
    addNamespace(NS_OfficeDocument_Bibliography,
                 QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/bibliography"),
                 QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/bibliography"),
                 QStringLiteral(""));
    addNamespace(NS_OfficeDocument_CustomProperties,
                 QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/custom-properties"),
                 QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/customProperties"),
                 QStringLiteral(""));
    addNamespace(NS_OfficeDocument_CustomXml,
                 QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/customXml"),
                 QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/customXml"),
                 QStringLiteral(""));
    addNamespace(NS_OfficeDocument_CustomXmlDataProps,
                 QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/customXmlDataProps"),
                 QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/customXmlDataProps"),
                 QStringLiteral(""));
    addNamespace(NS_OfficeDocument_DocPropsVTypes,
                 QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes"),
                 QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/docPropsVTypes"),
                 QStringLiteral(""));
    addNamespace(NS_OfficeDocument_ExtendedProperties,
                 QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/extended-properties"),
                 QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/extendedProperties"),
                 QStringLiteral(""));
    addNamespace(NS_OfficeDocument_Math,
                 QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/math"),
                 QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/math"),
                 QStringLiteral(""));
    addNamespace(NS_OfficeDocument_Relationships,
                 QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships"),
                 QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships"),
                 QStringLiteral(""));
    addNamespace(NS_Presentationml_Main,
                 QStringLiteral("http://schemas.openxmlformats.org/presentationml/2006/main"),
                 QStringLiteral("http://purl.oclc.org/ooxml/presentationml/main"),
                 QStringLiteral(""));
    addNamespace(NS_SchemaLibrary_Main,
                 QStringLiteral("http://schemas.openxmlformats.org/schemaLibrary/2006/main"),
                 QStringLiteral("http://purl.oclc.org/ooxml/schemaLibrary/main"),
                 QStringLiteral(""));
    addNamespace(NS_Spreadsheetml_Main,
                 QStringLiteral("http://schemas.openxmlformats.org/spreadsheetml/2006/main"),
                 QStringLiteral("http://purl.oclc.org/ooxml/spreadsheetml/main"),
                 QStringLiteral(""));
    addNamespace(NS_Wordprocessingml_Main,
                 QStringLiteral("http://schemas.openxmlformats.org/wordprocessingml/2006/main"),
                 QStringLiteral("http://purl.oclc.org/ooxml/wordprocessingml/main"),
                 QStringLiteral(""));
    addNamespace(NS_Drawingml_LockedCanvas,
                 QStringLiteral("http://schemas.openxmlformats.org/drawingml/2006/lockedCanvas"),
                 QStringLiteral("http://purl.oclc.org/ooxml/drawingml/lockedCanvas"),
                 QStringLiteral(""));
    addNamespace(NS_Drawingml_Compatibility,
                 QStringLiteral("http://schemas.openxmlformats.org/drawingml/2006/compatibility"),
                 QStringLiteral("http://purl.oclc.org/ooxml/drawingml/compatibility"),
                 QStringLiteral(""));
    addNamespace(NS_OfficeDocument_SharedTypes,
                 QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/sharedTypes"),
                 QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/sharedTypes"),
                 QStringLiteral(""));

    // This is a namespace conversion. Workaround for a bug in ISO spec. https://www.assembla.com/code/IS29500/subversion/changesets/160
    QString customXmlNs_strict = QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/customXml");
    QString customXmlNs = QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/customXml");
    QSharedPointer<OoxmlSchamesData> data = QSharedPointer<OoxmlSchamesData>(new OoxmlSchamesData(NS_OfficeDocument_CustomXml, customXmlNs, customXmlNs_strict));
    namespaceSchameHash_strict.insert(customXmlNs_strict, data);

    //Add normal namespaces.
    addNamespace(NS_Dc_Dcmitype, QStringLiteral("http://purl.org/dc/dcmitype/"));
    addNamespace(NS_Dc_Elements, QStringLiteral("http://purl.org/dc/elements/1.1/"));
    addNamespace(NS_Dc_Terms, QStringLiteral("http://purl.org/dc/terms/"));
    addNamespace(NS_XmlSchema, QStringLiteral("http://www.w3.org/2001/XMLSchema"));
    addNamespace(NS_XmlSchemaInstance, QStringLiteral("http://www.w3.org/2001/XMLSchema-instance"));

    //Add strict and transitional relationship pairs
    addRelationship(RS_OfficeDocument_AFChunk,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/aFChunk"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/aFChunk"));
    addRelationship(RS_OfficeDocument_AttachedTemplate,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/attachedTemplate"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/attachedTemplate"));
    addRelationship(RS_OfficeDocument_Audio,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/audio"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/audio"));
    addRelationship(RS_OfficeDocument_CalcChain,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/calcChain"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/calcChain"));
    addRelationship(RS_OfficeDocument_Chart,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/chart"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/chart"));
    addRelationship(RS_OfficeDocument_ChartSheet,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/chartsheet"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/chartsheet"));
    addRelationship(RS_OfficeDocument_ChartUserShapes,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/chartUserShapes"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/chartUserShapes"));
    addRelationship(RS_OfficeDocument_CommentAuthors,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/commentAuthors"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/commentAuthors"));
    addRelationship(RS_OfficeDocument_Comments,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/comments"));
    addRelationship(RS_OfficeDocument_Connections,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/connections"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/connections"));
    addRelationship(RS_OfficeDocument_Control,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/control"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/control"));
    addRelationship(RS_OfficeDocument_CustomProperties,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/custom-properties"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/customProperties"));
    addRelationship(RS_OfficeDocument_CustomProperty,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/customProperty"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/customProperty"));
    addRelationship(RS_OfficeDocument_CustomXml,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/customXml"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/customXml"));
    addRelationship(RS_OfficeDocument_CustomXmlProps,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/customXmlProps"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/customXmlProps"));
    addRelationship(RS_OfficeDocument_DiagramColoros,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/diagramColors"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/diagramColors"));
    addRelationship(RS_OfficeDocument_DiagramData,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/diagramData"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/diagramData"));
    addRelationship(RS_OfficeDocument_DiagramLayout,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/diagramLayout"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/diagramLayout"));
    addRelationship(RS_OfficeDocument_DiagramQuickStyle,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/diagramQuickStyle"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/diagramQuickStyle"));
    addRelationship(RS_OfficeDocument_Dialogsheet,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/dialogsheet"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/dialogsheet"));
    addRelationship(RS_OfficeDocument_Drawing,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/drawing"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/drawing"));
    addRelationship(RS_OfficeDocument_Endnotes,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/endnotes"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/endnotes"));
    addRelationship(RS_OfficeDocument_Extended,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/extendedProperties"));
    addRelationship(RS_OfficeDocument_ExternalLink,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/externalLink"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/externalLink"));
    addRelationship(RS_OfficeDocument_ExternalLinkPath,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/externalLinkPath"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/externalLinkPath"));
    addRelationship(RS_OfficeDocument_Font,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/font"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/font"));
    addRelationship(RS_OfficeDocument_FontTable,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/fontTable"));
    addRelationship(RS_OfficeDocument_Footer,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/footer"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/footer"));
    addRelationship(RS_OfficeDocument_Footnotes,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/footnotes"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/footnotes"));
    addRelationship(RS_OfficeDocument_Frame,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/frame"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/frame"));
    addRelationship(RS_OfficeDocument_GlossaryDocument,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/glossaryDocument"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/glossaryDocument"));
    addRelationship(RS_OfficeDocument_HandoutMaster,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/handoutMaster"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/handoutMaster"));
    addRelationship(RS_OfficeDocument_Header,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/header"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/header"));
    addRelationship(RS_OfficeDocument_HtmlPubSaveAs,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/htmlPubSaveAs"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/htmlPubSaveAs"));
    addRelationship(RS_OfficeDocument_Hyperlink,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/hyperlink"));
    addRelationship(RS_OfficeDocument_Image,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/image"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/image"));
    addRelationship(RS_OfficeDocument_MailMergeHeaderSource,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/mailMergeHeaderSource"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/mailMergeHeaderSource"));
    addRelationship(RS_OfficeDocument_MailMergeRecipientData,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/mailMergeRecipientData"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/mailMergeRecipientData"));
    addRelationship(RS_OfficeDocument_MailMergeSource,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/mailMergeSource"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/mailMergeSource"));
    addRelationship(RS_OfficeDocument_Metadata,
            QStringLiteral("http://schemas.openxmlformats.org/package/2006/relationships/metadata/thumbnail"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/metadata/thumbnail"));
    addRelationship(RS_OfficeDocument_Movie,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/movie"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/movie"));
    addRelationship(RS_OfficeDocument_NotesMaster,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesMaster"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/notesMaster"));
    addRelationship(RS_OfficeDocument_NotesSlide,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesSlide"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/notesSlide"));
    addRelationship(RS_OfficeDocument_Numbering,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/numbering"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/numbering"));
    addRelationship(RS_OfficeDocument_OfficeDocument,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/officeDocument"));
    addRelationship(RS_OfficeDocument_OleObject,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/oleObject"));
    addRelationship(RS_OfficeDocument_Package,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/package"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/package"));
    addRelationship(RS_OfficeDocument_PivotCacheDefinition,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotCacheDefinition"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/pivotCacheDefinition"));
    addRelationship(RS_OfficeDocument_PivotCacheRecords,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotCacheRecords"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/pivotCacheRecords"));
    addRelationship(RS_OfficeDocument_PivotTable,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotTable"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/pivotTable"));
    addRelationship(RS_OfficeDocument_PresProps,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/presProps"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/presProps"));
    addRelationship(RS_OfficeDocument_PrinterSettings,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/printerSettings"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/printerSettings"));
    addRelationship(RS_OfficeDocument_QueryTable,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/queryTable"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/queryTable"));
    addRelationship(RS_OfficeDocument_RevisionHeaders,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/revisionHeaders"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/revisionHeaders"));
    addRelationship(RS_OfficeDocument_RevisionLog,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/revisionLog"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/revisionLog"));
    addRelationship(RS_OfficeDocument_Settings,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/settings"));
    addRelationship(RS_OfficeDocument_SharedStrings,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/sharedStrings"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/sharedStrings"));
    addRelationship(RS_OfficeDocument_SheetMetadata,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/sheetMetadata"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/sheetMetadata"));
    addRelationship(RS_OfficeDocument_Slide,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/slide"));
    addRelationship(RS_OfficeDocument_SlideLayout,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideLayout"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/slideLayout"));
    addRelationship(RS_OfficeDocument_SlideMaster,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideMaster"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/slideMaster"));
    addRelationship(RS_OfficeDocument_SlideUpdateInfo,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideUpdateInfo"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/slideUpdateInfo"));
    addRelationship(RS_OfficeDocument_SlideUpdateUrl,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideUpdateUrl"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/slideUpdateUrl"));
    addRelationship(RS_OfficeDocument_Styles,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/styles"));
    addRelationship(RS_OfficeDocument_SubDocument,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/subDocument"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/subDocument"));
    addRelationship(RS_OfficeDocument_Table,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/table"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/table"));
    addRelationship(RS_OfficeDocument_TableSingleCells,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/tableSingleCells"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/tableSingleCells"));
    addRelationship(RS_OfficeDocument_TableStyles,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/tableStyles"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/tableStyles"));
    addRelationship(RS_OfficeDocument_Tags,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/tags"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/tags"));
    addRelationship(RS_OfficeDocument_Theme,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/theme"));
    addRelationship(RS_OfficeDocument_ThemeOverride,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/themeOverride"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/themeOverride"));
    addRelationship(RS_OfficeDocument_Transform,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/transform"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/transform"));
    addRelationship(RS_OfficeDocument_Usernames,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/usernames"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/usernames"));
    addRelationship(RS_OfficeDocument_Video,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/video"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/video"));
    addRelationship(RS_OfficeDocument_ViewProps,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/viewProps"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/viewProps"));
    addRelationship(RS_OfficeDocument_VolatileDependencies,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/volatileDependencies"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/volatileDependencies"));
    addRelationship(RS_OfficeDocument_WebSettings,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/webSettings"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/webSettings"));
    addRelationship(RS_OfficeDocument_Worksheet,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/worksheet"));
    addRelationship(RS_OfficeDocument_XmlMaps,
            QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/xmlMaps"),
            QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/xmlMaps"));
}


SchamesPrivate::~SchamesPrivate()
{

}

void SchamesPrivate::addNamespace(int id, const QString &transitional, const QString &strict, const QString &prefix)
{
    QSharedPointer<OoxmlSchamesData> data = QSharedPointer<OoxmlSchamesData>(new OoxmlSchamesData(id, prefix, strict, transitional));
    namespaceSchameHash.insert(id, data);

    if (strict.isEmpty()) {
        data->strict = data->transitional;
        namespaceSchameHash_normal.insert(transitional, data);
    } else {
        namespaceSchameHash_strict.insert(strict, data);
        namespaceSchameHash_transitional.insert(transitional, data);
    }
}

void SchamesPrivate::addRelationship(int id, const QString &transitional, const QString &strict)
{
    QSharedPointer<OoxmlSchamesData> data = QSharedPointer<OoxmlSchamesData>(new OoxmlSchamesData(id, QString(), strict, transitional));
    relationshipSchameHash.insert(id, data);

    if (strict.isEmpty()) {
        data->strict = data->transitional;
        relationshipSchameHash_normal.insert(transitional, data);
    }
    relationshipSchameHash_strict.insert(strict, data);
    relationshipSchameHash_transitional.insert(transitional, data);
}

/*! \class Schames
 */

Schames::Schames()
    :d(new SchamesPrivate)
{
}

Schames::~Schames()
{
    delete d;
}

QString Schames::namespaceUri(NamespaceId id, bool strict) const
{
    if (strict)
        return d->namespaceSchameHash[id]->strict;
    return d->namespaceSchameHash[id]->transitional;
}

QString Schames::namespaceUri(const QString &uri, bool strict) const
{
    if (strict && d->namespaceSchameHash_transitional.contains(uri))
        return d->namespaceSchameHash_transitional[uri]->strict;

    if (!strict && d->namespaceSchameHash_strict.contains(uri))
        return d->namespaceSchameHash_strict[uri]->transitional;
    return uri;
}

bool Schames::isStrictNamespace(const QString &uri) const
{
    return d->namespaceSchameHash_strict.contains(uri);
}

bool Schames::isTransitionalNamespace(const QString &uri) const
{
    return d->namespaceSchameHash_transitional.contains(uri);
}

QString Schames::relationshipUri(RelationshipId id, bool strict) const
{
    if (strict)
        return d->relationshipSchameHash[id]->strict;
    return d->relationshipSchameHash[id]->transitional;
}

QString Schames::relationshipUri(const QString &uri, bool strict) const
{
    if (strict && d->relationshipSchameHash_transitional.contains(uri))
        return d->relationshipSchameHash_transitional[uri]->strict;

    if (!strict && d->relationshipSchameHash_strict.contains(uri))
        return d->relationshipSchameHash_strict[uri]->transitional;
    return uri;
}

bool Schames::isStrictRelationship(const QString &uri) const
{
    return d->relationshipSchameHash_strict.contains(uri);
}

bool Schames::isTransitionalRelationship(const QString &uri) const
{
    return d->relationshipSchameHash_transitional.contains(uri);
}

} // namespace Ooxml
} // namespace QtOfficeOpenXml
