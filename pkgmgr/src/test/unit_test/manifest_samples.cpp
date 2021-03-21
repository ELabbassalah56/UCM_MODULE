// --------------------------------------------------------------------------
// |              _    _ _______     .----.      _____         _____        |
// |         /\  | |  | |__   __|  .  ____ .    / ____|  /\   |  __ \       |
// |        /  \ | |  | |  | |    .  / __ \ .  | (___   /  \  | |__) |      |
// |       / /\ \| |  | |  | |   .  / / / / v   \___ \ / /\ \ |  _  /       |
// |      / /__\ \ |__| |  | |   . / /_/ /  .   ____) / /__\ \| | \ \       |
// |     /________\____/   |_|   ^ \____/  .   |_____/________\_|  \_\      |
// |                              . _ _  .                                  |
// --------------------------------------------------------------------------
//
// All Rights Reserved.
// Any use of this source code is subject to a license agreement with the
// AUTOSAR development cooperation.
// More information is available at www.autosar.org.
//
// Disclaimer
//
// This work (specification and/or software implementation) and the material
// contained in it, as released by AUTOSAR, is for the purpose of information
// only. AUTOSAR and the companies that have contributed to it shall not be
// liable for any use of the work.
//
// The material contained in this work is protected by copyright and other
// types of intellectual property rights. The commercial exploitation of the
// material contained in this work requires a license to such intellectual
// property rights.
//
// This work may be utilized or reproduced without any modification, in any
// form or by any means, for informational purposes only. For any other
// purpose, no part of the work may be utilized or reproduced, in any form
// or by any means, without permission in writing from the publisher.
//
// The work has been developed for automotive applications only. It has
// neither been developed, nor tested for non-automotive applications.
//
// The word AUTOSAR and the AUTOSAR logo are registered trademarks.
// --------------------------------------------------------------------------

#include "manifest_samples.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{
namespace testdata
{

using String = ara::core::String;
using boost::property_tree::ptree;

const String sample_application_manifest
    = "["
      "   {"
      "       \"key\": \"platform_application\","
      "       \"value\": {"
      "           \"bool\": true"
      "       },"
      "       \"checksum\": 1639441683"
      "   },"
      "   {"
      "       \"key\": \"application_states\","
      "       \"value\": {"
      "           \"string[]\": [ ]"
      "       },"
      "       \"checksum\": 1639441683"
      "   },"
      "   {"
      "       \"key\": \"timer_resolution_ns\","
      "       \"value\": {"
      "           \"uint32\": 1000"
      "       },"
      "       \"checksum\": 1405615546"
      "   },"
      "   {"
      "       \"key\": \"startup_configs\","
      "       \"value\": {"
      "           \"object[]\": ["
      "               {"
      "                   \"key\": \"Machine: Startup, Driving\","
      "                   \"value\": {"
      "                       \"object[]\": ["
      "                           {"
      "                               \"key\": \"depends\","
      "                               \"value\": {"
      "                                   \"string[]\": [ \"Calculator\", \"DiagnosticManager\" ]"
      "                               },"
      "                               \"checksum\": 2301152528"
      "                           },"
      "                           {"
      "                               \"key\": \"machine_states\","
      "                               \"value\": {"
      "                                   \"string[]\": [ \"Startup\", \"Driving\" ]"
      "                               },"
      "                               \"checksum\": 2301152528"
      "                           },"
      "                           {"
      "                               \"key\": \"function_group_states\","
      "                               \"value\": {"
      "                                   \"string[]\": [ ]"
      "                               },"
      "                               \"checksum\": 2301152528"
      "                           },"
      "                           {"
      "                               \"key\": \"resource_group\","
      "                               \"value\": {"
      "                                   \"string\": \"Resource group 1\""
      "                               },"
      "                               \"checksum\": 2301152528"
      "                           },"
      "                           {"
      "                               \"key\": \"scheduling_policy\","
      "                               \"value\": {"
      "                                   \"string\": \"Round Robin\""
      "                               },"
      "                               \"checksum\": 2301152528"
      "                           },"
      "                           {"
      "                              \"key\": \"scheduling_priority\","
      "                               \"value\": {"
      "                                   \"uint32\": 12"
      "                               },"
      "                               \"checksum\": 2301152528"
      "                           },"
      "                           {"
      "                               \"key\": \"arguments\","
      "                               \"value\": {"
      "                                   \"string[]\": [ ]"
      "                               },"
      "                               \"checksum\": 2301152528"
      "                           }"
      "                       ]"
      "                   },"
      "                   \"checksum\": 870226170"
      "               }"
      "           ]"
      "       },"
      "       \"checksum\": 870226170"
      "   }"
      "]";

const String sample_application_manifest_not_met
    = "["
      "   {"
      "       \"key\": \"platform_application\","
      "       \"value\": {"
      "           \"bool\": true"
      "       },"
      "       \"checksum\": 1639441683"
      "   },"
      "   {"
      "       \"key\": \"application_states\","
      "       \"value\": {"
      "           \"string[]\": [ ]"
      "       },"
      "       \"checksum\": 1639441683"
      "   },"
      "   {"
      "       \"key\": \"timer_resolution_ns\","
      "       \"value\": {"
      "           \"uint32\": 1000"
      "       },"
      "       \"checksum\": 1405615546"
      "   },"
      "   {"
      "       \"key\": \"startup_configs\","
      "       \"value\": {"
      "           \"object[]\": ["
      "               {"
      "                   \"key\": \"Machine: Startup, Driving\","
      "                   \"value\": {"
      "                       \"object[]\": ["
      "                           {"
      "                               \"key\": \"depends\","
      "                               \"value\": {"
      "                                   \"string[]\": [ \"Calculator\", \"DiagnosticManager\", \"UnknownDep\" ]"
      "                               },"
      "                               \"checksum\": 2301152528"
      "                           },"
      "                           {"
      "                               \"key\": \"machine_states\","
      "                               \"value\": {"
      "                                   \"string[]\": [ \"Startup\", \"Driving\" ]"
      "                               },"
      "                               \"checksum\": 2301152528"
      "                           },"
      "                           {"
      "                               \"key\": \"function_group_states\","
      "                               \"value\": {"
      "                                   \"string[]\": [ ]"
      "                               },"
      "                               \"checksum\": 2301152528"
      "                           },"
      "                           {"
      "                               \"key\": \"resource_group\","
      "                               \"value\": {"
      "                                   \"string\": \"Resource group 1\""
      "                               },"
      "                               \"checksum\": 2301152528"
      "                           },"
      "                           {"
      "                               \"key\": \"scheduling_policy\","
      "                               \"value\": {"
      "                                   \"string\": \"Round Robin\""
      "                               },"
      "                               \"checksum\": 2301152528"
      "                           },"
      "                           {"
      "                              \"key\": \"scheduling_priority\","
      "                               \"value\": {"
      "                                   \"uint32\": 12"
      "                               },"
      "                               \"checksum\": 2301152528"
      "                           },"
      "                           {"
      "                               \"key\": \"arguments\","
      "                               \"value\": {"
      "                                   \"string[]\": [ ]"
      "                               },"
      "                               \"checksum\": 2301152528"
      "                           }"
      "                       ]"
      "                   },"
      "                   \"checksum\": 870226170"
      "               }"
      "           ]"
      "       },"
      "       \"checksum\": 870226170"
      "   }"
      "]";

const String sample_application_manifest_no_dep
    = "["
      "   {"
      "       \"key\": \"platform_application\","
      "       \"value\": {"
      "           \"bool\": true"
      "       },"
      "       \"checksum\": 1639441683"
      "   },"
      "   {"
      "       \"key\": \"application_states\","
      "       \"value\": {"
      "           \"string[]\": [ ]"
      "       },"
      "       \"checksum\": 1639441683"
      "   },"
      "   {"
      "       \"key\": \"timer_resolution_ns\","
      "       \"value\": {"
      "           \"uint32\": 1000"
      "       },"
      "       \"checksum\": 1405615546"
      "   },"
      "   {"
      "       \"key\": \"startup_configs\","
      "       \"value\": {"
      "           \"object[]\": ["
      "               {"
      "                   \"key\": \"Machine: Startup, Driving\","
      "                   \"value\": {"
      "                       \"object[]\": ["
      "                           {"
      "                               \"key\": \"depends\","
      "                               \"value\": {"
      "                                   \"string[]\": [ ]"
      "                               },"
      "                               \"checksum\": 2301152528"
      "                           },"
      "                           {"
      "                               \"key\": \"machine_states\","
      "                               \"value\": {"
      "                                   \"string[]\": [ \"Startup\", \"Driving\" ]"
      "                               },"
      "                               \"checksum\": 2301152528"
      "                           },"
      "                           {"
      "                               \"key\": \"function_group_states\","
      "                               \"value\": {"
      "                                   \"string[]\": [ ]"
      "                               },"
      "                               \"checksum\": 2301152528"
      "                           },"
      "                           {"
      "                               \"key\": \"resource_group\","
      "                               \"value\": {"
      "                                   \"string\": \"Resource group 1\""
      "                               },"
      "                               \"checksum\": 2301152528"
      "                           },"
      "                           {"
      "                               \"key\": \"scheduling_policy\","
      "                               \"value\": {"
      "                                   \"string\": \"Round Robin\""
      "                               },"
      "                               \"checksum\": 2301152528"
      "                           },"
      "                           {"
      "                              \"key\": \"scheduling_priority\","
      "                               \"value\": {"
      "                                   \"uint32\": 12"
      "                               },"
      "                               \"checksum\": 2301152528"
      "                           },"
      "                           {"
      "                               \"key\": \"arguments\","
      "                               \"value\": {"
      "                                   \"string[]\": [ ]"
      "                               },"
      "                               \"checksum\": 2301152528"
      "                           }"
      "                       ]"
      "                   },"
      "                   \"checksum\": 870226170"
      "               }"
      "           ]"
      "       },"
      "       \"checksum\": 870226170"
      "   }"
      "]";

const String sample_swpackage_manifest
    = "{  "
      "  \"name\": \"/swcls/swcl0\","
      "  \"path\": \"TestApplUCM\","
      "  \"requestType\": 1,"
      "  \"version\": \"1.0\","
      "  \"checksum\": 1639441683,"
      "  \"uncompressedSize\":123"
      "}";

const String sample_processes_list
    = "["
      "    {"
      "        \"key\": \"processes\","
      "        \"value\": {"
      "            \"string[]\": ["
      "                \"/var/lib/apd_ucm/sample/swclSample/applicationSample/exec1\""
      "            ]"
      "        },"
      "        \"checksum\": 870226170"
      "    }"
      "]";

ptree CreateSoftwarePackageManifest(const String& action)
{
    const String manifest = R"({
    "actionType": ")"
        + action + R"(",
    "activationAction": "ApplicationRestart",
    "category": "examplePACKAGE",
    "compressedSoftwarePackageSize": "524288",
    "diagnosticAddress": "17473",
    "isDeltaPackage": "false",
    "minUCMSupportedVersion": "1.0.0_0",
    "maxUCMSupportedVersion": "1.0.0_0",
    "packagerID": "1347111236",
    "shortName": "SWP_TEST1_INS",
    "typeApproval": "None",
    "ucmIdentifier": "16722",
    "uncompressedSoftwarePackageSize": "655360",
    "uuid": "079af322-1570-11e9-8e05-e79fcc9bd29c"
})";

    ptree manifest_tree;
    std::stringstream stream(manifest.c_str());
    boost::property_tree::read_json(stream, manifest_tree);

    return manifest_tree;
}

SoftwarePackageManifest CreateSoftwarePackageManifestObject(const String& action)
{
    ptree manifestTree = CreateSoftwarePackageManifest(action);

    SoftwarePackageManifestParser parser;
    return *(parser.Parse(manifestTree));
}

String ElementsAsCommaSeparatedString(ara::core::Vector<String> elements)
{
    String elementsAsCommaSeparatedString;
    if (elements.size() > 0) {
        for (int i = 0; i < static_cast<int>(elements.size() - 1); i++) {
            elementsAsCommaSeparatedString += "\"";
            elementsAsCommaSeparatedString += elements[i];
            elementsAsCommaSeparatedString += "\"";
            elementsAsCommaSeparatedString += ", ";
        }
        elementsAsCommaSeparatedString += "\"";
        elementsAsCommaSeparatedString += elements[elements.size() - 1];
        elementsAsCommaSeparatedString += "\"";
    }
    return elementsAsCommaSeparatedString;
}

ptree CreateSoftwareClusterManifest(const String& swclName, const String& version)
{
    const String manifest = R"({
    "category": "exampleSWCL",
    "changes": "Initial release",
    "conflicts": [
        {
            "operator": ">=",
            "shortName": "SWCL_TEST1_VENDOR_B",
            "version": "0.0.0_0"
        }
    ],
    "depends": [
        {
            "operator": ">=",
            "shortName": "SWCL_BASE",
            "version": "1.0.0_0"
        }
    ],
    "license": "AUTOSAR",
    "previousVersion": "0.0.0_0",
    "shortName": ")"
        + swclName + R"(",
    "uuid": "0925f048-1570-11e9-8000-4f62f9ef99ce",
    "vendorID": "1447315780",
    "version": ")"
        + version + R"("
})";
    ptree manifest_tree;
    std::stringstream stream(manifest.c_str());
    boost::property_tree::read_json(stream, manifest_tree);

    return manifest_tree;
}

SoftwareClusterManifest CreateSoftwareClusterManifestObject(const String& swclName, const String& version)
{
    ptree manifestTree = CreateSoftwareClusterManifest(swclName, version);
    SoftwareClusterManifestParser parser;
    return *(parser.Parse(manifestTree));
}

}  // namespace testdata
}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
