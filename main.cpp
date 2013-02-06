#include<limits>
#include<iostream>
#include<list>
#include<queue>
#include<cstdlib>
using namespace std;

//delcare
class Event;
const int MAXBUFFER = numeric_limits<int>::max();
enum EventType = {ARRIVAL, DEPARTURE};
list<Event> GEL;
queue<Event> wait;
//global time, service rate, arrival rate
int length, gtime, srate, arate;
double m_distr_time(double);
void init(int, int);
void run();
void process_arrival(Event*);
void process_departure(Event*);


class Event{
    public:
    Event(int time, EventType t, int service_time = 0) {
        this.time = time;
        this.type = t;
        this.service_time = service_time;
    }
    int time;
    //the transmission time for packet
    int service_time;
    EventType type;
}


double m_distr_time(double rate) {
    double u;
    u = drand48();
    return ((-1/rate)*log(1-u));
}

void init(int service_rate, int arrival_rate) {
    length = 0;
    gtime = 0;
    srate = service_rate;
    arate = arrival_rate;
    //todo init GEL
    GEL.push_front(new Event(m_distr_time(arate),EventType::ARRIVAL));
}

void process_arrival(Event* event) {
    gtime = event->time;
    int next_time = m_distr_time(arate) + gtime;
    int service_time = m_distr_time(srate);





}
void run() {
    Event* event;
    for( int i = 0; i<100000; i++) {
       *event = GEL.pop_front();
       if(event->type == EventType::ARRIVAL) {
           process_arrival(event);
       } else {
           process_departure(event);
       }
    }
}


