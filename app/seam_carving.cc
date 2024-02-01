#include <iostream>
#include <string>
#include "header/seam_carver.h"
using namespace std;

using seam_carving::SeamCarver;

void PrintUsage() {
  cout << "USAGE:" << endl << endl;
  cout << "./build/app/seam_carving <input_image> <direction> <number of seams>" << endl;
  cout << "<direction> has to be like 'h' or 'v'" << endl << endl;
  cout << " OR " << endl << endl;
  cout << "./build/app/seam_carving <input_image> <direction> <number of seams> "
      "<mode> <x> <y> <w> <h>" << endl;
  cout << "<mode> should be 'r' or 'p'" << endl <<"'r' = removal, 'p' = protection" << endl;
  cout << "<x> <y> <w> <h> specifies the region of interest." << endl;
}

int main(int argc, char **argv) {
  if (argc != 4 && argc != 9) {
    cout << "Wrong arguments! Please check how to use the executable! ;-)" << endl;
    PrintUsage();
    return 1;
  }
  std::string input_img_path(argv[1]);
  std::string carve_direction(argv[2]);
  const int num_seams = std::stoi(std::string(argv[3]));

  cv::Rect roi_rect;
  std::string mode;
  if (argc == 9) {
    mode = std::string(argv[4]);
    roi_rect = cv::Rect(std::stoi(std::string(argv[5])),
        std::stoi(std::string(argv[6])),
        std::stoi(std::string(argv[7])),
        std::stoi(std::string(argv[8])));
  }

  cv::Mat input_img;
  input_img = cv::imread(input_img_path);

#ifdef VIZ_DEBUG
  cv::imshow("Input Image", input_img);
  std::cout << "In progress... meanwhile you can press any key to look at the live process!" << endl;
  cv::waitKey(0);
#endif

  SeamCarver seam_carver(input_img);
  if (mode == "r") {
    seam_carver.SetRemovalMaskByRect(roi_rect);
  }
  if (mode == "p") {
    seam_carver.SetProtectionMaskByRect(roi_rect);
  }
  if (carve_direction == "h") {
    seam_carver.HorizontalCarving(num_seams);
  } else if (carve_direction == "v") {
    seam_carver.VerticalCarving(num_seams);
  } else {
    cout << "Unknown direction: " << carve_direction << endl;
  }

  cv::imshow("Carved image: ", seam_carver.carved_image());
  cv::waitKey(0);

  return 0;
}

