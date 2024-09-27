import unittest
import cppephem as ce

class TestCEAngle(unittest.TestCase):

    def setUp(self):
        self.base_ = ce.CEAngle(1.57079632679489661923132169163975144)

    def test_construct(self):
        
        # Default constructor
        test1 = ce.CEAngle()
        self.assertEqual(test1.Rad(), 0.0)

        # Copy constructor (CEAngle)
        test2 = ce.CEAngle(self.base_);
        self.assertEqual(test2.Rad(), self.base_.Rad())

        # // Copy-assignment operator (CEAngle)
        test3 = self.base_
        self.assertEqual(test3.Rad(), self.base_.Rad());

        # // Copy-assignment operator (double)
        # double angle_test = M_PI;
        # CEAngle test4;
        # test4 = angle_test;
        # test_double(test4, angle_test, __func__, __LINE__);

        # // Copy-assignment operator (CEAngle)
        # CEAngle test5;
        # test5 = base_;
        # test_double(test5, base_, __func__, __LINE__);

        # // Make sure support methods work
        # test_string(base_.ClassName(), "CEAngle", __func__, __LINE__);
        # test_greaterthan(base_.describe().size(), 0, __func__, __LINE__);

        return