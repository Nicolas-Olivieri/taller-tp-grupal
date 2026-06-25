#include "test_helper.h"

TestHelper::TestHelper() {}

std::vector<AssetInfoDTO> TestHelper::mock_asset_vector(const int size) {
    std::vector<AssetInfoDTO> result;
    for (int i = 0; i < size; i++) {
        AssetInfoDTO asset(i, i, i);
        result.push_back(asset);
    }
    return result;
}


bool TestHelper::equals(const std::vector<AssetInfoDTO>& a, const std::vector<AssetInfoDTO>& b) {
    if (a.size() != b.size())
        return false;
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i].id != b[i].id || a[i].x != b[i].x || a[i].y != b[i].y) {
            return false;
        }
    }
    return true;
}
