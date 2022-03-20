#include"ec_map.h"
#include"utils.h"
#include"plate_reco.h"
#include<cstdlib>
using namespace std;
using namespace Tsolpr;

int Testfunction(const char* image_adress);

int main() {
    std::shared_ptr<Tsolpr::ec_map>ec_map(new Tsolpr::ec_map);
    ec_map->load("text/ec_map.txt");
    int t;
    t = Testfunction("E:/personal_file/Tsolpr/Tsolpr/Tsolpr/The_system_of_license_plate_recognition/resources/test_file");
    system("pause");
    return 0;
}

int Testfunction(const char* image_adress) {
    auto files = Tsolpr::utils::get_files(image_adress);
    if (files.size() == 0) {
    	cout << "files open error."<<endl;
    	return 0;
    }
    cout << "test start.\n";
    int files_size = files.size();
    PlateRecognize Plate_reco;
    Plate_reco.LoadANN("E:/personal_file/Tsolpr/Tsolpr/Tsolpr/The_system_of_license_plate_recognition/model/ann.xml");
    Plate_reco.LoadSVM("E:/personal_file/Tsolpr/Tsolpr/Tsolpr/The_system_of_license_plate_recognition/model/svm_hist.xml");
    Plate_reco.LoadChAnn("E:/personal_file/Tsolpr/Tsolpr/Tsolpr/The_system_of_license_plate_recognition/model/annCh.xml");
    Plate_reco.LoadChMap("E:/personal_file/Tsolpr/Tsolpr/Tsolpr/The_system_of_license_plate_recognition/text/province_mapping.txt");
    cout << "Begin to recognize plate!" << endl;
    for (int i = 0; i < files_size; i++) {
    	cout << "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\"<<"\n";
    	string file = files[i].c_str();
    	Mat src = imread(file);
    	string original_name = Tsolpr::utils::get_file_name(file);
    	vector<Plate> plateVec;
    	int result = Plate_reco.plateRecognize(src, plateVec, i);
    	cout << "Ô­³µÅÆ"<< ":" << original_name << endl;
    	if (plateVec.size() != 0) {
    		for (int j = 0; j < plateVec.size(); j++) {
    			string license = plateVec[j].getPlateStr();
    			cout << license << endl;
    		}
    	}
    	else {
    		cout << "this image do not have plate." << endl;
    	}
        
    
    }
   
    return 0;
}

