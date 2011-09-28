/**************************************************************************\
*
 *  This file is part of the C++ Modular Development Framework.
 *  Copyright (C) 2009 by Christoph Bastuck.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using this framework with software that can not be combined with the GNU
 *  GPL please contact mail@cbastuck about acquiring a commercial license.
 *
 *  See http://www.cbastuck.de/ for more information.
 *
 \**************************************************************************/

#include "XMLProcessor.h"

#include <clay-core/xml/XMLBuffer.h>
#include <clay-core/xml/XMLDocument.h>
#include <clay-core/xml/XMLNodeChildIterator.h>

//xerces
#include <xercesc/dom/DOM.hpp>
//#include <xercesc/dom/DOMXPathExpression.hpp>
#include <xercesc/dom/DOMXPathResult.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>

//xqilla
#include <xqilla/xqilla-dom3.hpp>

namespace CLAY { namespace MODULE {

//---------------------------------------------
XMLProcessor::XMLProcessor(const tString& sId)
  : tBase(sId)
{
  XQillaPlatformUtils::initialize();
}

//---------------------------------------------
XMLProcessor::~XMLProcessor()
{
  //XQillaPlatformUtils::terminate(); //TODO do not terminate XERCES
}

//---------------------------------------------
void XMLProcessor::registerModuleOutputs()
{

}

//---------------------------------------------
void XMLProcessor::registerModuleInputs()
{
  registerInput(new XMLDocumentInput, "xml");
}

//---------------------------------------------
Module::ProcessResultCode XMLProcessor::process()
{
  XMLDocumentInput* pIn = static_cast<XMLDocumentInput*>(getInput("xml"));
  CLAY_ASSERT(pIn);
  if(pIn->isConnected())
  {
    XMLDocument* pDoc = pIn->getData();
    XMLBuffer aBuffer;
    if(!pDoc->writeToMemory(aBuffer))
    {
      return Module::Process_ERROR;
    }

    try
    {
      // Get the XQilla DOMImplementation object
      XERCES::DOMImplementation* xqillaImplementation = XERCES::DOMImplementationRegistry::getDOMImplementation(S2X("XPath2 3.0"));

      // Create a DOMLSParser object
      AutoRelease<XERCES::DOMLSParser> parser(xqillaImplementation->createLSParser(XERCES::DOMImplementationLS::MODE_SYNCHRONOUS, 0));
      parser->getDomConfig()->setParameter(XERCES::XMLUni::fgDOMNamespaces, true);
      parser->getDomConfig()->setParameter(XERCES::XMLUni::fgXercesSchema, true);
      parser->getDomConfig()->setParameter(XERCES::XMLUni::fgDOMValidateIfSchema, true);

      // Parse a DOMDocument
      XERCES::MemBufInputSource aInputSource(static_cast<const XMLByte*>(aBuffer.getData()), aBuffer.getSize(), S2X("fake-id"));
      XERCES::Wrapper4InputSource aLSInput(&aInputSource, false);

      XERCES::DOMDocument* pXercesDoc = parser->parse(&aLSInput);
      if(pXercesDoc == 0)
      {
        return Module::Process_ERROR;
      }

      // Parse an XPath 2 expression
      AutoRelease<XERCES::DOMXPathExpression> expression(pXercesDoc->createExpression(S2X(m_sXPathExpression.c_str()), 0));

      // Execute the query
      AutoRelease<XERCES::DOMXPathResult> result(expression->evaluate(pXercesDoc, XERCES::DOMXPathResult::ITERATOR_RESULT_TYPE, 0));

      // Create a DOMLSSerializer to output the nodes
      AutoRelease<XERCES::DOMLSSerializer> serializer(xqillaImplementation->createLSSerializer());
      AutoRelease<XERCES::DOMLSOutput> output(xqillaImplementation->createLSOutput());
      XERCES::StdOutFormatTarget target;
      output->setByteStream(&target);

      // Iterate over the results, printing them
      while(result->iterateNext())
      {
        serializer->write(result->getNodeValue(), output);
        std::cout << std::endl;
      }
    }
    catch(const XERCES::DOMXPathException& e)
    {
      CLAY_LOG_ERROR(X2S(e.getMessage()));
    }
    catch(...)
    {
      CLAY_LOG_ERROR("XMLProcessor unknown exception");
    }
  }

  return Module::Process_OK;
}

//---------------------------------------------
void XMLProcessor::setXPathExpression(const tString& sExp)
{
  m_sXPathExpression = sExp;
}

//---------------------------------------------
const tString& XMLProcessor::getXPathExpression() const
{
  return m_sXPathExpression;
}

} }
