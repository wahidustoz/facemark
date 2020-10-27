/*
Download jsoncpp library from the link below
https://github.com/open-source-parsers/jsoncpp
*/

class Parson {
public:
    Landmark* read(string name)
    {
        Landmark* Lm = new Landmark;
        Json::Value root;
        Json::Reader reader;
        ifstream json(name, ifstream::binary);

        reader.parse(json, root);
        for (int i = 0; i < 68; i++)
        {
            Lm->all[i].x = root[to_string(i)][0].asInt();
            Lm->all[i].y = root[to_string(i)][1].asInt();
        }

        Lm->divideMark();
        return Lm;
    }
     void write(int score, int asy)
    {
        Json::Value root;
        Json::Value string_value;

        root["score"] = score;
        root["asy?"] = true;

        ofstream outFile("result.json", ios::out);
        outFile << root;
    }
};
