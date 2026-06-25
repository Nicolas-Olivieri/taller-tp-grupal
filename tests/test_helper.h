#ifndef TEST_HELPER_H
#define TEST_HELPER_H
#include <vector>

#include "common/dto/snapshot/map/asset_info.h"


class TestHelper {
public:
    TestHelper();

    std::vector<AssetInfoDTO> mock_asset_vector(int size);

    bool equals(const std::vector<AssetInfoDTO>& a, const std::vector<AssetInfoDTO>& b);
};


#endif  // TEST_HELPER_H
