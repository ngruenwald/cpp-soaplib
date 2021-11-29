#include "unit_tests.hpp"

#include "dws/DispatcherService.hpp"
#include "dws/types/FileMetadataDCResponse.hpp"
#include "dws/types/FileMetadataGetListDCResponseItem.hpp"

admin::dws::FileMetadata_GetListResponse GetList()
{
    const std::string fixture{FIXTURES_PATH "/FileMetadata_GetListResponse.xml"};

    auto response = soaplib::xml::Document::ParseFile(fixture.c_str());
    auto envelope = response.GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("FileMetadata_GetListResponse");
    return admin::dws::FileMetadata_GetListResponseFromXml(operation);
}

TEST_CASE("FileMetadata_GetListResponse", "[.all][fixtures]")
{
    auto result = GetList();

    REQUIRE(result.FileMetadata_GetListResult);
    REQUIRE(result.FileMetadata_GetListResult->List);
    REQUIRE(result.FileMetadata_GetListResult->List->ReturnEntity.empty() == false);

    for (const auto& entry : result.FileMetadata_GetListResult->List->ReturnEntity)
    {
        REQUIRE(entry.Value);
        REQUIRE(entry.Value->Value);

        auto fmd = std::dynamic_pointer_cast<admin::dws::FileMetadataGetListDCResponseItem>(entry.Value->Value);
        REQUIRE(fmd);
        REQUIRE(fmd->FileMetadataDetails);

        REQUIRE(fmd->FileMetadataDetails->UniversalIdentifier);
        REQUIRE(fmd->FileMetadataDetails->Version);
    }
}
