#include<cmath>
#include<limits>
#include<iostream>
#include<list>
#include<queue>
#include<cstdlib>
using namespace std;

//delcare
class Event;
//const int MAXBUFFER = numeric_limits<int>::max();
const int MAXBUFFER = 1; 
enum EventType {ARRIVAL, DEPARTURE};
list<Event> GEL;
queue<Event> WAIT;
//global time, service rate, arrival rate
double gtime, srate, arate, mean_length, busy_time;
int length, packet_drop;
double m_distr_time(double);
void init(double, double);
void run();
void process_arrival(Event*);
void process_departure(Event*);
void insert_GEL(Event*);

class Event{
    public:
    Event(double ti, EventType t, double st = 0) {
        time = ti;
        type = t;
        service_time = st;
    }
    double time;
    //the transmission time for packet
    double service_time;
    EventType type;
};


double m_distr_time(double rate) {
    double u;
    u = drand48();
    return ((-1/rate)*log(1-u));
}

void init(double service_rate, double arrival_rate) {
    length = 0;
    gtime = 0;
    srate = service_rate;
    arate = arrival_rate;
    //todo init GEL
    double arrive_time = m_distr_time(arate);
    double service_time = m_distr_time(srate);
    packet_drop = 0;
    mean_length = 0;
    busy_time = 0;
    GEL.push_front(Event(arrive_time, ARRIVAL, service_time));
}

void insert_GEL(Event* event) {
    list<Event>:: iterator it;
    for(it = GEL.begin(); it != GEL.end(); it++) {
        if(event->time < it->time) {
            GEL.insert(it,*event);
            break;  
        }
    }
    if(it == GEL.end()) {
        GEL.push_back(*event);
    }
}

void process_arrival(Event* event) {
    //cout<<"arrival "<<gtime<<endl;
    double pre_time = gtime;
    gtime = event->time;
    //insert new arrival event
    double next_time = m_distr_time(arate) + gtime;
    double service_time = m_distr_time(srate);
    Event new_event(next_time, ARRIVAL, service_time);
    insert_GEL(&new_event);
    //process current event
    if(length == 0) {
        //if server is free, schedule departure event
        double stime = event->service_time;
        Event devent(gtime+stime, DEPARTURE);
        insert_GEL(&devent); 
        WAIT.push(*event);
        length ++;
    } else {
       //server is busy, put into queue
       if(length-1 < MAXBUFFER) {
           WAIT.push(*event);
           //cout<<length<<" "<<gtime-pre_time<<"  "<<gtime<<endl;
           mean_length = (mean_length*pre_time + length*(gtime-pre_time))/gtime;
           length ++;
       } else {
           packet_drop++;
       }
       busy_time = busy_time + gtime - pre_time;
    }

}

void process_departure(Event* event) {
    //cout<<"departure "<<gtime<<endl;
    double pre_time = gtime;
    gtime = event->time; 
    mean_length = (mean_length*pre_time + length*(gtime-pre_time))/gtime;
    //process current event
    if(length > 0) {
       //server is busy, de queue
       Event new_event = WAIT.front();
       WAIT.pop();
       Event depart_event(gtime+new_event.service_time,DEPARTURE);
       insert_GEL(&depart_event);
       length --;
       busy_time = busy_time + gtime - pre_time;
    }
}

void run() {
    Event* event = new Event(0,ARRIVAL,0);
    for( int i = 0; i<100000; i++) {
        if(GEL.empty()) {
            cout<<"i is "<<i<<endl;
            break;
        }
        *event = GEL.front();
        GEL.pop_front();
        if(event->type == ARRIVAL) {
           process_arrival(event);
        } else {
           process_departure(event);
        }
    }
}

int main(){
    double mu,lambda;
    cout<<"set mu and lambda"<<endl;
    cin>>mu>>lambda;
    init(mu, lambda);
    run();
    double uti = busy_time/gtime;
    cout<<"mean queue length is "<<mean_length<<endl;
    cout<<"server utilization "<<uti<<endl;
    cout<<"number of dropped packages is "<<packet_drop<<endl;


}
