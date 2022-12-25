#include "slowlib.h"
#include <filesystem>
#include "slowlib.base/archive/slArchive.h"
#include "slowlib.base/gs/slImage.h"

SL_LINK_LIBRARY("slowlib.base")

void to_h(const char* inFile, const char* outFile)
{
	FILE* inF = 0;
	fopen_s(&inF, inFile, "rb");
	if (inF)
	{
		FILE* outF = 0;
		fopen_s(&outF, outFile, "wb");
		if (outF)
		{
			std::uintmax_t inFileSz = std::filesystem::file_size(inFile);
			if (inFileSz)
			{
				uint8_t* fileData = (uint8_t*)malloc((size_t)inFileSz);
				if (fileData)
				{
					fread(fileData, (size_t)inFileSz, 1, inF);

					int byte_counter = 0;
					for (uint32_t i = 0; i < inFileSz; ++i)
					{
						if (!byte_counter)
							fprintf(outF, "\t");

						fprintf(outF, "0x%02X, ", ((uint8_t*)fileData)[i]);

						++byte_counter;
						if (byte_counter == 8)
						{
							fprintf(outF, "\r\n");
							byte_counter = 0;
						}
					}
				}
			}
			fclose(outF);
		}

		fclose(inF);
	}
}

class FrameworkCallback : public slFrameworkCallback
{
public:
	FrameworkCallback() {}
	virtual ~FrameworkCallback() {}
	virtual void OnMessage() {};
};

int main()
{
	std::filesystem::create_directory("../src/data/");
	FrameworkCallback fcb;
	slFramework::Start(&fcb);
	
	to_h("../data/fonts/font.png", "../src/data/font.inl");
	to_h("../data/fonts/defaultIcons.png", "../src/data/defaultIcons.inl");
	
	slFramework::Stop();

	return 0;
}