#include "stdafx.h"
#include "CppUnitTest.h"
#include "../include/image_process.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(ImageProcessTest)
	{
	public:
		
		TEST_METHOD(AutoAdjustImageTextTest)
		{
			// TODO: Your test code here
			Logger::WriteMessage(L"Testing AutoAdjustImageText()");
			ImageProcess image_process;

			image_process.autoAdjustImageText()

		}

	};
}