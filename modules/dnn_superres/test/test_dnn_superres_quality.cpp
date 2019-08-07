// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "test_precomp.hpp"

namespace opencv_test
{
    namespace
    {
        class CV_DnnSuperResQualityTest : public cvtest::BaseTest
        {
        public:
            CV_DnnSuperResQualityTest();

        protected:
            virtual void run(int);
        };

        void CV_DnnSuperResQualityTest::run(int)
        {

        }

        TEST(CV_DnnSuperResQualityTest, accuracy)
        {
            CV_DnnSuperResQualityTest test;
            test.safe_run();
        }
    }
}