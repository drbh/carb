#include <iostream>
#include <vector>
#include <hiredis/hiredis.h>

#include <iomanip>
#include <ctime>

#include <nlohmann/json.hpp>
// for convenience
using json = nlohmann::json;
using namespace std;

static redisContext *redisConnection;

void *setRedisImg(char* value) {
    printf("Start to make connection\n");
    // unsigned int j;
    redisContext *c;
    // redisReply *reply;
    const char *hostname = "127.0.0.1";
    int port = 6379;

    struct timeval timeout = { 1, 500000 }; // 1.5 seconds
    c = redisConnectWithTimeout(hostname, port, timeout);
    if (c == NULL || c->err) {
        if (c) {
            printf("Connection error: %s\n", c->errstr);
            redisFree(c);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
        exit(1);
    }


    char buffer[10];
    printf("\nImage added");
    redisCommand(c,"LPUSH images %s", value);
    redisCommand(c,"LTRIM images 0 10");
    redisFree(c);

    return 0;
}

class Triangle 
{ 
    // Access specifier 
    public: 
  
    // Data Members 
    string uuid; 
    string txpath;

    string nodeA; 
    string nodeB; 
    string nodeC;

    string nodeTimeA; 
    string nodeTimeB; 
    string nodeTimeC;

    float edgeAtoB;
    float edgeBtoC;
    float edgeAtoC;

    float inefficiency;
    

  
  	int walkEdges() 
  	{
    	// float inefficiency;
    	// A -> B -> C 
  		// A -> C
  		float ac = edgeAtoC / ( edgeAtoB * edgeBtoC );

  		// A -> C -> B 
  		// A -> B
  		float ab = edgeAtoB / ( edgeAtoC * ( 1/edgeBtoC ) );

  		if (ab == ac)
  		{
  			cout << "\nWoah, equalibrium\n";
        txpath = "A";
  			inefficiency = ab;
  		};
  		if (ab < ac)
  		{
  			cout << "\nShould go ACBA\n";
        txpath = "ACBA";
  			inefficiency = ac;
  		};
  		if (ab > ac) 
  		{
  			cout << "\nShould go ABCA\n";
        txpath = "ABCA";
  			inefficiency = ab;
  		};
  		return 0;
  	}
    // Member Functions() 
    void printname() 
    { 
       cout << "Triangle Name is: " << uuid; 
    } 

    // Member Functions() 
    void printnodes() 
    { 
       cout << "\nNode A: " << nodeA; 
       cout << "\nNode B: " << nodeB; 
       cout << "\nNode C: " << nodeC; 
       cout << "\n";
       cout << "\nEdge  : " << 1 << " " << nodeA << " " << (1/edgeAtoB) << " " << nodeB;
       cout << "\nEdge  : " << 1 << " " << nodeB << " " << (1/edgeBtoC) << " " << nodeC;
       cout << "\nEdge  : " << 1 << " " << nodeA << " " << (1/edgeAtoC) << " " << nodeC;
    } 

}; 

vector<string> split(const string& str, const string& delim)
{
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}

json buildTri(const string s)
{

  vector<string> args = split(s,",");

  string delimiter = " ";
   // Declare an object of class geeks 
  Triangle obj1; 
  
    // accessing data member 
  obj1.uuid = "My Triangle"; 
  obj1.nodeA = args[0]; 
  obj1.nodeB = args[1];
  obj1.nodeC = args[2];  

    // accessing data member 
  obj1.nodeTimeA = args[6]; 
  obj1.nodeTimeB = args[7];
  obj1.nodeTimeC = args[8];

  string orbits1 = args[3];
  string::size_type sz1;     
  float eab = stof (orbits1,&sz1);

  string orbits2 = args[4];
  string::size_type sz2;     
  float ebc = stof (orbits2,&sz2);

  string orbits3 = args[5];
  string::size_type sz3;     
  float eac = stof (orbits3,&sz3);

  obj1.edgeAtoB = eab;
  obj1.edgeBtoC = ebc;  
  obj1.edgeAtoC = eac;

  obj1.printnodes(); 
  
  // cout << "\n";
  // cout << 
  obj1.walkEdges();
  cout << "\n";


timeval curTime;
gettimeofday(&curTime, NULL);
int milli = curTime.tv_usec / 1000;

char buffer [80];
strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&curTime.tv_sec));

char currentTime[84] = "";
sprintf(currentTime, "%s:%d", buffer, milli);
// printf("current time: %s \n", currentTime);
  // time_t rawtime;
  // struct tm * timeinfo;
  // char buffer[80];

  // time (&rawtime);
  // timeinfo = localtime(&rawtime);

  // strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S.000",timeinfo);
  // string str(buffer);

    // instead, you could also write (which looks very similar to the JSON above)
    json j2 = {
      {"currenttime", currentTime },
      // {"txpath", obj1.txpath},
      // {"inefficiency", obj1.inefficiency},
      // {"nodes", {
      //   obj1.nodeA,
      //   obj1.nodeB,
      //   obj1.nodeC
      // }},
      {"compute", {
        {"inefficiency", obj1.inefficiency},
        {"txpath", obj1.txpath},
      }},

      {"nodes", {
        {"A", obj1.nodeA},
        {"B", obj1.nodeB},
        {"C", obj1.nodeC},
      }},

      {"edges", {
        {"AB", obj1.edgeAtoB},
        {"BC", obj1.edgeBtoC},
        {"CA", obj1.edgeAtoC},
      }},
      {"origins", {
        {"A", obj1.nodeTimeA},
        {"B", obj1.nodeTimeB},
        {"C", obj1.nodeTimeC},
      }},
    };

  return j2;
}

void *getRedisImg() {
    // printf("Start to make connection\n");
    // unsigned int j;
    redisContext *c;
    redisReply *reply;
    const char *hostname = "127.0.0.1";
    int port = 6379;

    struct timeval timeout = { 1, 500000 }; // 1.5 seconds
    c = redisConnectWithTimeout(hostname, port, timeout);
    if (c == NULL || c->err) {
        if (c) {
            printf("Connection error: %s\n", c->errstr);
            redisFree(c);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
        exit(1);
    }


    char buffer[10];
    // printf("\nImage added");

    reply = (redisReply *) redisCommand(c,"RPOP images");
    // cout << ("%s", reply);
    // printf("PING: %s\n", reply->str);


    if(reply->str){
     // printf("PING: %s\n", reply->str); 
     json pay = buildTri(reply->str);
    // serialization with pretty printing
    // pass in the amount of spaces to indent
    cout << pay.dump(4) << endl;



    
    string cmd = "LPUSH tri ";
    string result = cmd + pay.dump().c_str(); 
    // cout << result;
     redisCommand(c, result.c_str());
     redisCommand(c,"RPOP images");
    }

    freeReplyObject(reply);
    redisFree(c);

    return 0;
}

int main(int argc, char** argv) 
{ 	

  if(argc>1){
    string s = argv[1];
  }
	
  // buildTri(s);
  // char *p = "USD YEN GBP 0.00893655 142.857142857 1.282051282"; // valid and safe in either C or C++.
  // char *p = "GBP USD JPY 0.7801711695546002 0.008894344086595334 0.006938902959442112"; // valid and safe in either C or C++.
  // setRedisImg(p); 
  while (true)
  {
    getRedisImg();
  };
    // cout << "\nHello, World!\n";
    return 0;
}