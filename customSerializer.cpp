#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <unordered_map>

using SerializeType = std::variant<
                                    int,
                                    float,
                                    std::string,
                                    std::vector<std::string>,
                                    std::vector<int>,
                                    std::vector<float>,
                                    std::unordered_map<std::string, std::string>,
                                    std::unordered_map<std::string, int>,
                                    std::unordered_map<std::string, float>
>;

class CustomSerializer {
public:
    template<class T>
    void serialize(const std::string& filename, std::vector<T>& objList) {
        std::ofstream out(filename, std::ios::binary);
        if (!out) throw std::runtime_error("Unable to open file for writing");

        for (auto& obj : objList){
            std::unordered_map<std::string, SerializeType> data = obj.toSerilizeType();

            for (const auto& [key, value] : data) {
                writeString(out, key);   
                writeData(out, value);     
            }

            out.put('&');
        }
    }

    
    template<class T>
    std::vector<T> deserialize(const std::string& filename) {
        std::vector<T> obj_list;

        std::ifstream in(filename, std::ios::binary);
        if (!in) throw std::runtime_error("Unable to open file for reading");

        while (in.peek() != EOF) {
            T obj_deserialized;

            std::unordered_map<std::string, SerializeType> data;
            while (in.peek() != '&') {
                std::string key = readString(in);   
                auto value = readData(in);      
                data[key] = value;
            }

            in.get();
            obj_deserialized.fromSerilizeType(data);

            obj_list.push_back(obj_deserialized);
        }

        return obj_list;
    }

private:
    void writeString(std::ofstream& out, const std::string& str) {
        int length = str.size();
        out.write(reinterpret_cast<const char*>(&length), sizeof(length));  
        out.write(str.c_str(), length);                                    
    }

    std::string readString(std::ifstream& in) {
        int length;
        in.read(reinterpret_cast<char*>(&length), sizeof(length)); 
        std::string str(length, '\0');
        in.read(&str[0], length);       
        return str;
    }


    void writeData(std::ofstream& out, const SerializeType& data) {
        if (std::holds_alternative<int>(data)) {
            out.put('I'); 
            int value = std::get<int>(data);
            out.write(reinterpret_cast<const char*>(&value), sizeof(value));

        } else if (std::holds_alternative<float>(data)) {
            out.put('F');
            float value = std::get<float>(data);
            out.write(reinterpret_cast<const char*>(&value), sizeof(value));

        } else if (std::holds_alternative<std::string>(data)) {
            out.put('S');
            writeString(out, std::get<std::string>(data));

        } else if (std::holds_alternative<std::vector<std::string>>(data)) {
            out.put('L');
            const auto& list = std::get<std::vector<std::string>>(data);
            int length = list.size();
            out.write(reinterpret_cast<const char*>(&length), sizeof(length));
            for (const auto& item : list) {
                writeData(out, item);
            }

        } else if (std::holds_alternative<std::unordered_map<std::string, std::string>>(data)) {
            out.put('D');
            const auto& map = std::get<std::unordered_map<std::string, std::string>>(data);
            int length = map.size();
            out.write(reinterpret_cast<const char*>(&length), sizeof(length));
            for (const auto& [key, value] : map) {
                writeString(out, key);
                writeData(out, value);
            }
        }
    }


    SerializeType readData(std::ifstream& in) {
        char type = in.get();
        if (type == 'I') {
            int value;
            in.read(reinterpret_cast<char*>(&value), sizeof(value));
            return value;

        } else if (type == 'F') {
            float value;
            in.read(reinterpret_cast<char*>(&value), sizeof(value));
            return value;

        } else if (type == 'S') {
            return readString(in);

        } else if (type == 'L') {
            int length;
            in.read(reinterpret_cast<char*>(&length), sizeof(length));
            std::vector<std::string> list;
            for (int i = 0; i < length; ++i) {
                std::string res = std::get<std::string>(readData(in));
                list.push_back(res);
            }
            return list;

        } else if (type == 'D') {
            int length;
            in.read(reinterpret_cast<char*>(&length), sizeof(length));
            std::unordered_map<std::string, std::string> map;
            for (int i = 0; i < length; ++i) {
                std::string key = readString(in); 
                std::string value = std::get<std::string>(readData(in));
                map[key] = value;
            }
            return map;

        } else {
            throw std::runtime_error("Unknown data type marker");
        }
    }
};
