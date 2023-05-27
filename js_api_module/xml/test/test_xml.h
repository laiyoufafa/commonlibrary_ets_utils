/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef TEST_XML_H_
#define TEST_XML_H_

#include "../js_xml.h"

namespace OHOS::xml {
class XmlTest {
public:
    XmlTest() = default;
    ~XmlTest() = default;
    static XmlSerializer construct(napi_env env);
    static void SetDeclaration(napi_env env);
    static void SetNamespace(napi_env env);
    static void StartElement(napi_env env);
    static void WriteEscaped(napi_env env);
    static void XmlSerializerError(napi_env env);
    static void PushSrcLinkList();
    static size_t GetNSCount(size_t iTemp);
    static std::string XmlPullParserError();
    static TagEnum DealExclamationGroup(std::string xml);
    static TagEnum DealLtGroup();
    static TagEnum ParseTagType();
    static std::string SkipText(std::string strXml, std::string str);
    int TestGetColumnNumber(napi_env env);
    int TestGetLineNumber(napi_env env);
    std::string TestGetText(napi_env env);
    bool TestParseNsp(napi_env env);
    void TestParseDeclaration(napi_env env);
    std::string TestParseDelimiterInfo(napi_env env);
    bool TestParseEndTag(napi_env env);
    bool TestParseComment(napi_env env);
};

XmlSerializer XmlTest::construct(napi_env env)
{
    napi_value arrayBuffer = nullptr;
    void* pBuffer = nullptr;
    size_t size = 1024;  // 1024: buffer size
    napi_create_arraybuffer(env, size, &pBuffer, &arrayBuffer);
    OHOS::xml::XmlSerializer xmlSerializer(reinterpret_cast<char*>(pBuffer), size, "utf-8");
    return xmlSerializer;
}

void XmlTest::SetDeclaration(napi_env env)
{
    XmlSerializer xmlSerializer = construct(env);
    xmlSerializer.isHasDecl = true;
    xmlSerializer.SetDeclaration();
}

void XmlTest::SetNamespace(napi_env env)
{
    XmlSerializer xmlSerializer = construct(env);
    xmlSerializer.type = "isStart";
    xmlSerializer.SetNamespace("xml", "convert");
}

void XmlTest::StartElement(napi_env env)
{
    XmlSerializer xmlSerializer = construct(env);
    xmlSerializer.depth_ = 1;
    xmlSerializer.elementStack[0] = "x";
    xmlSerializer.elementStack.push_back("");
    xmlSerializer.StartElement("val");
}

void XmlTest::WriteEscaped(napi_env env)
{
    XmlSerializer xmlSerializer = construct(env);
    xmlSerializer.WriteEscaped("'\"&><q");
}

void XmlTest::XmlSerializerError(napi_env env)
{
    XmlSerializer xmlSerializer = construct(env);
    xmlSerializer.XmlSerializerError();
}

void XmlTest::PushSrcLinkList()
{
    std::string strXml = "<?xml version=\"1.0\" encoding=\"utf-8\"?>    <title>Happy</title>    <todo>Work</todo>";
    OHOS::xml::XmlPullParser xmlPullParser(strXml, "utf-8");
    xmlPullParser.PushSrcLinkList("str");
}

size_t XmlTest::GetNSCount(size_t iTemp)
{
    std::string strXml = "<?xml version=\"1.0\" encoding=\"utf-8\"?><todo>Work</todo>";
    OHOS::xml::XmlPullParser xmlPullParser(strXml, "utf-8");
    xmlPullParser.nspCounts_.push_back(0);
    xmlPullParser.nspCounts_.push_back(1);
    return xmlPullParser.GetNSCount(iTemp);
}

std::string XmlTest::XmlPullParserError()
{
    std::string strXml = "<?xml version=\"1.0\" encoding=\"utf-8\"?><todo>Work</todo>";
    OHOS::xml::XmlPullParser xmlPullParser("strXml", "utf-8");
    xmlPullParser.xmlPullParserError_= "IndexOutOfBoundsException";
    return xmlPullParser.XmlPullParserError();
}

TagEnum XmlTest::DealExclamationGroup(std::string xml)
{
    std::string strXml = "<?xml version=\"1.0\" encoding=\"utf-8\"?><todo>Work</todo>";
    OHOS::xml::XmlPullParser xmlPullParser("strXml", "utf-8");
    xmlPullParser.strXml_ = xml;
    return xmlPullParser.DealExclamationGroup();
}

TagEnum XmlTest::DealLtGroup()
{
    std::string strXml = "<?xml version=\"1.0\" encoding=\"utf-8\"?><todo>Work</todo>";
    OHOS::xml::XmlPullParser xmlPullParser(strXml, "utf-8");
    xmlPullParser.strXml_ = "?/";
    return xmlPullParser.DealLtGroup();
}

TagEnum XmlTest::ParseTagType()
{
    std::string strXml = "<?xml version=\"1.0\" encoding=\"utf-8\"?><todo>Work</todo>";
    OHOS::xml::XmlPullParser xmlPullParser(strXml, "utf-8");
    xmlPullParser.bStartDoc_ = false;
    xmlPullParser.max_ = 0;
    xmlPullParser.strXml_ = "%";
    return xmlPullParser.ParseTagType(false);
}

std::string XmlTest::SkipText(std::string strXml, std::string str)
{
    OHOS::xml::XmlPullParser xmlPullParser(strXml, "utf-8");
    xmlPullParser.SkipText(str);
    return xmlPullParser.XmlPullParserError();
}

int XmlTest::TestGetColumnNumber(napi_env env)
{
    OHOS::xml::XmlPullParser xml("1\n1", "utf8");
    xml.position_ = 3; // 3: index is three
    int res = xml.GetColumnNumber();
    return res;
}

int XmlTest::TestGetLineNumber(napi_env env)
{
    OHOS::xml::XmlPullParser xml("\n", "utf8");
    xml.position_ = 1;
    int res = xml.GetLineNumber();
    return res;
}

std::string XmlTest::TestGetText(napi_env env)
{
    OHOS::xml::XmlPullParser xml("1\n1", "utf8");
    xml.type = TagEnum::WHITESPACE;
    return xml.GetText();
}

bool XmlTest::TestParseNsp(napi_env env)
{
    OHOS::xml::XmlPullParser xml("", "utf8");
    xml.attriCount_ = 1;
    xml.attributes.push_back("");
    xml.attributes.push_back("");
    xml.attributes.push_back("xmlns");
    xml.nspCounts_.push_back(0);
    xml.name_ = ":";
    return xml.ParseNsp();
}

void XmlTest::TestParseDeclaration(napi_env env)
{
    OHOS::xml::XmlPullParser xml("", "utf8");
    xml.bufferStartLine_ = 1;
    xml.attributes.push_back("");
    xml.attributes.push_back("");
    xml.attributes.push_back("");
    xml.ParseDeclaration();
}

std::string XmlTest::TestParseDelimiterInfo(napi_env env)
{
    OHOS::xml::XmlPullParser xml("12", "utf8");
    xml.text_ = "123";
    std::string res = xml.ParseDelimiterInfo("456", true);
    xml.strXml_ = "456";
    xml.position_ = 0;
    res = xml.ParseDelimiterInfo("456", false);
    return res;
}

bool XmlTest::TestParseEndTag(napi_env env)
{
    OHOS::xml::XmlPullParser xml("123456789", "utf8");
    xml.relaxed = false;
    xml.depth = 1;
    xml.elementStack_.resize(20); // 20 :vector size
    xml.elementStack_[3] = "!"; // 3: index of three
    xml.ParseEndTag();
    xml.depth = 0;
    xml.ParseEndTag();
    return false;
}

bool XmlTest::TestParseComment(napi_env env)
{
    OHOS::xml::XmlPullParser xml("1", "utf8");
    xml.relaxed = true;
    xml.ParseComment(true);
    return false;
}
}
#endif