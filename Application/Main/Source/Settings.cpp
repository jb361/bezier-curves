/*!
    @file Settings.cpp @author Joel Barrett @date 01/01/12 @brief Settings for the application.
*/

#include "Settings.h"

#pragma warning (push)
#pragma warning (disable : 4244)

namespace Application
{
    void Settings::Load(const char* pFilename)
    {
        // load and parse the xml document
        TiXmlDocument doc(pFilename);
        if (!doc.LoadFile()) {
            throw std::runtime_error("The file '" + std::string(pFilename) + "' couldn't be found");
        }
        TiXmlHandle hDoc(&doc);
        TiXmlHandle hRoot(0);

        // Root
        {
            TiXmlElement* pNode = hDoc.FirstChildElement().ToElement();
            if (!pNode) {
                throw std::runtime_error("The file '" + std::string(pFilename) + "' doesn't contain a valid root element");
            }
            name_ = pNode->Value();
            hRoot = TiXmlHandle(pNode);
        }

        // Window
        {
            TiXmlElement* pNode = hRoot.FirstChild("Window").FirstChild().ToElement();

            window_.title = pNode->GetText();
            pNode = pNode->NextSiblingElement();

            window_.xPos = atoi(pNode->GetText());
            pNode = pNode->NextSiblingElement();

            window_.yPos = atoi(pNode->GetText());
            pNode = pNode->NextSiblingElement();

            window_.width = atoi(pNode->GetText());
            pNode = pNode->NextSiblingElement();

            window_.height = atoi(pNode->GetText());
        }

        // Model
        {
            TiXmlElement* pNode = hRoot.FirstChild("Model").FirstChild().ToElement();
            modelFilename_ = pNode->GetText();
        }

        // Camera
        {
            TiXmlElement* pNode = hRoot.FirstChild("Camera").FirstChild("Position").ToElement();

            camera_.position = Vector3f(atof(pNode->FirstChild("X")->ToElement()->GetText()),
                                        atof(pNode->FirstChild("Y")->ToElement()->GetText()),
                                        atof(pNode->FirstChild("Z")->ToElement()->GetText()));

            pNode = pNode->NextSiblingElement("LookAt");

            camera_.lookAt = Vector3f(atof(pNode->FirstChild("X")->ToElement()->GetText()),
                                      atof(pNode->FirstChild("Y")->ToElement()->GetText()),
                                      atof(pNode->FirstChild("Z")->ToElement()->GetText()));
        }
        
        // Track
        {
            TiXmlElement* pCurveNode = hRoot.FirstChild("Track").FirstChild().ToElement();
            
            // for each curve in the track
            for (pCurveNode; pCurveNode; pCurveNode = pCurveNode->NextSiblingElement())
            {
                CurveSettings curve;
                TiXmlElement* pCPNode = pCurveNode->FirstChild()->ToElement();

                // for each control point in the curve
                for (pCPNode; pCPNode; pCPNode = pCPNode->NextSiblingElement())
                {
                    curve.ctrlPoints.push_back(Vector3f(atof(pCPNode->FirstChild("X")->ToElement()->GetText()),
                                                        atof(pCPNode->FirstChild("Y")->ToElement()->GetText()),
                                                        atof(pCPNode->FirstChild("Z")->ToElement()->GetText())));
                }
                track_.push_back(curve);
            }
        }

        // Light
        {
            TiXmlElement* pNode = hRoot.FirstChild("Light").FirstChild("Ambient").ToElement();

            light_.ambient = Vector4f(atof(pNode->FirstChild("R")->ToElement()->GetText()),
                                      atof(pNode->FirstChild("G")->ToElement()->GetText()),
                                      atof(pNode->FirstChild("B")->ToElement()->GetText()),
                                      atof(pNode->FirstChild("A")->ToElement()->GetText()));

            pNode = pNode->NextSiblingElement("Diffuse");

            light_.diffuse = Vector4f(atof(pNode->FirstChild("R")->ToElement()->GetText()),
                                      atof(pNode->FirstChild("G")->ToElement()->GetText()),
                                      atof(pNode->FirstChild("B")->ToElement()->GetText()),
                                      atof(pNode->FirstChild("A")->ToElement()->GetText()));

            pNode = pNode->NextSiblingElement("Position");

            light_.position = Vector3f(atof(pNode->FirstChild("X")->ToElement()->GetText()),
                                       atof(pNode->FirstChild("Y")->ToElement()->GetText()),
                                       atof(pNode->FirstChild("Z")->ToElement()->GetText()));
        }
    }
}

#pragma warning (pop)
