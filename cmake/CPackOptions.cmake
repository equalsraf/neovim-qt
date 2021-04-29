
# Name packages as neovim-qt-installer
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-installer")

# WIX cannot handle a license file without a supported extension
set(PACKAGE_LICENSE_TXT "${PROJECT_BINARY_DIR}/LICENSE.txt")
file(COPY ${PROJECT_SOURCE_DIR}/LICENSE DESTINATION ${PROJECT_BINARY_DIR})
file(RENAME "${PROJECT_BINARY_DIR}/LICENSE" ${PACKAGE_LICENSE_TXT})
set(CPACK_RESOURCE_FILE_LICENSE ${PACKAGE_LICENSE_TXT})
# Used by Wix installer to detect upgrades
set(CPACK_WIX_UPGRADE_GUID "b3357595-f87b-4706-85b0-e2630d989979")
# Pack Wix settings with additional options
set(CPACK_WIX_PATCH_FILE "${PROJECT_SOURCE_DIR}/contrib/wix_patch.xml")
set(CPACK_WIX_PRODUCT_ICON "${PROJECT_SOURCE_DIR}/third-party/neovim.ico")

include(CPack)
