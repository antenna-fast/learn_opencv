#include "load_yaml.h"


bool read_yaml(string yaml_path, vector<double>& disto, vector<double> &intri)
{

    ifstream in_file;
    in_file.open(yaml_path);

    if(in_file.fail())
    {
        cout << "failed to open file: " << yaml_path << endl;
        return false;
    }

    disto.resize(4);
    intri.resize(4);

    string line;
    int row_counter = 0; 

    while (!in_file.eof())
    {
        row_counter ++;
        getline(in_file, line);

        if(row_counter == 4)
        {
            istringstream str_line(line);
            string str;

            // std::cout << "str_line: " << line << std::endl;
            
            size_t pos = line.find("[");
            line = line.substr(pos+1, line.size() - pos -2); 
            // cout << "line: " << line << endl;

            for (size_t i = 0; i < 3; i++)
            {
                pos = line.find(", ");
                string p0 = line.substr(0, pos);
                line = line.substr(pos+2);

                disto.at(i) = stod(p0);
                // std::cout << "disto: " << disto.at(i) << std::endl;
            }
            disto.at(3) = stod(line);
            // std::cout << "disto: " << disto.at(3) << std::endl;
        }

        if(row_counter == 6)
        {
            istringstream str_line(line);
            string str; 

            size_t pos = line.find("[");
            line = line.substr(pos+1, line.size() - pos -2); 
            // cout << "line: " << line << endl;

            for (size_t i = 0; i < 3; i++)
            {
                pos = line.find(", ");
                string p0 = line.substr(0, pos);
                line = line.substr(pos+2);

                intri.at(i) = stod(p0);
                // std::cout << "intri: " << intri.at(i) << std::endl;
            }
            intri.at(3) = stod(line);
            // std::cout << "intri: " << intri.at(3) << std::endl;
        }        
    }

    return true;
}
