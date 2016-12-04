#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <limits>

struct point{
    short id;
    int weight;
    short related_id;
    int top;
    int right;
    int bot;
    int left;
    int get_direction(int id){
        switch(id){
            case 0:
                return top;
            case 1:
                return right;
            case 2:
                return bot;
            case 3:
                return left;
        }
    }
};

void set_params(std::vector<point> &, std::vector<int> &, std::vector<sf::CircleShape> &, sf::Vertex lines_array[][2], int r , bool *, int start_point, int big_val);
void fill_points_and_circles(std::vector<point>& , std::vector<sf::CircleShape>& , int , bool * );
void connecting_circles_horizontaly(std::vector<point>&, std::vector<int> &, std::vector<sf::CircleShape>&, sf::Vertex lines_array[][2], int r);
void connecting_circles_verticaly(std::vector<point>&, std::vector<int>&, std::vector<sf::CircleShape>&, sf::Vertex lines_array[][2], int r );
void dijkstra_alg(std::vector<point> &, std::vector<int> &, std::vector<sf::CircleShape> &, sf::Vertex lines_array[][2], int r , bool *, int start_point);
void render_app(std::vector<point> &, std::vector<int> &, std::vector<sf::CircleShape> &, sf::Vertex lines_array[][2], int , bool *, sf::RenderWindow&, int &);
void reset_points(std::vector<point> &);

int main(){
    srand(time(0));
    const int r = 10;
    sf::RenderWindow app(sf::VideoMode(600, 600), "SFML window");
    sf::Vertex lines_array[200][2];
    std::vector<int> values(190, 1);
    std::vector<point> points;
    std::vector<sf::CircleShape> circles;
    bool Q[100];
    int point_id = 0;
    int start_point = 0;
    const int big_val = 999;
    set_params(points, values, circles, lines_array, r, Q, start_point, big_val);
    render_app(points, values, circles, lines_array, r, Q, app, point_id);
    return EXIT_SUCCESS;
}


void fill_points_and_circles(std::vector<point> &points, std::vector<sf::CircleShape> &circles, int r, bool *Q, int big_val){
    for(int i =0; i<10; ++i){
        for(int g =0; g<10; ++g){
            sf::CircleShape circle;
            circle.setRadius(r);
            circle.setPosition(r * g * 4 + 30, r * i * 4 + 30);
            circle.setOutlineColor(sf::Color::Red);
            circle.setFillColor(sf::Color::White);
            circles.push_back(circle);
            short int iter = i * 10 + g;
            point new_point =  { iter , big_val, -1, big_val, big_val , big_val , big_val };
            points.push_back(new_point);
            Q[iter] = true;
        }
    }
}


void connecting_circles_horizontaly(std::vector<point> &points, std::vector<int> &values, std::vector<sf::CircleShape> &circles, sf::Vertex lines_array[][2], int r, int big_val){
    for( int i =1; i<100; ++i ){
        if ( i%10 != 0  && rand()%9 > 3 ){
            points[i-1].right=1;
            points[i].left = 1;
            lines_array[i][0] = sf::Vertex(sf::Vector2f( circles[i-1].getPosition().x + r, circles[i-1].getPosition().y + r ));
            lines_array[i][1] = sf::Vertex(sf::Vector2f( circles[i].getPosition().x + r, circles[i].getPosition().y + r));
        }
        else {
            points[i-1].right = big_val;
            points[i].left = big_val;
            values[i] = big_val;
        }
    }
}

void connecting_circles_verticaly(std::vector<point> &points, std::vector<int> &values, std::vector<sf::CircleShape> &circles, sf::Vertex lines_array[][2], int r, int big_val){
    for( int i =0; i<90; ++i ){
        if ( rand()%9 > 3 ){
            points[i].bot = 1;
            points[i + 10].top = 1;
            lines_array[i + 100][0] = sf::Vertex(sf::Vector2f( circles[i].getPosition().x + r, circles[i].getPosition().y + r ), sf::Color::White);
            lines_array[i + 100][1] = sf::Vertex(sf::Vector2f( circles[i + 10].getPosition().x + r, circles[i + 10].getPosition().y + r), sf::Color::White);
        }
        else{
            points[i].bot = big_val;
            points[i + 10].top = big_val;
            values [i +100] = big_val;
        }
    }
}

void dijkstra_alg(std::vector<point> &points, std::vector<int> &values, std::vector<sf::CircleShape> &circles, sf::Vertex lines_array[][2], int r, bool *Q, int start_point){
    points[start_point].weight = 0;
    for( auto i : points){
        point bufor;
        bufor.weight = std::numeric_limits<int>::max();
        for(auto p : points){
            if (bufor.weight > p.weight && Q[p.id])
                bufor = p;
        }
        Q[bufor.id] = false;
        int cords[4] = { 10, -1, -10, 1 };
        for(int j =0; j < 4; ++j){
            if( bufor.id -cords[j] < 100 && bufor.id - cords[j] >= 0 && Q[bufor.id -cords[j]] ){
                if( points[bufor.id - cords[j]].weight > bufor.weight + bufor.get_direction(j) ){
                    points[bufor.id - cords[j]].weight = bufor.weight + bufor.get_direction(j) ;
                    points[bufor.id - cords[j]].related_id = bufor.id;
                }
            }
        }
    }
}

void set_params(std::vector<point> &points, std::vector<int> &values, std::vector<sf::CircleShape> &circles, sf::Vertex lines_array[][2], int r, bool *Q, int start_point, int big_val){
    fill_points_and_circles(points, circles, r, Q, big_val);
    connecting_circles_horizontaly(points, values, circles, lines_array, r, big_val);
    connecting_circles_verticaly(points, values, circles, lines_array, r, big_val );
    dijkstra_alg(points, values, circles, lines_array, r, Q, start_point);
}
void render_app(std::vector<point> &points, std::vector<int> &values, std::vector<sf::CircleShape> &circles, sf::Vertex lines_array[][2], int r, bool *Q, sf::RenderWindow &app, int &point_id){
    while (app.isOpen())
    {
        sf::Event event;
        while (app.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                app.close();
        }
        app.clear();
        for(int i = 0 ; i < 200; ++i){
            app.draw(lines_array[i], 2, sf::Lines);
        }
        for(auto circle : circles){
            app.draw(circle);
        }
        if (event.type == sf::Event::MouseButtonPressed){
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                for(int i=0; i < circles.size(); ++i){
                    if(circles[i].getPosition().x <= event.mouseButton.x && circles[i].getPosition().x + 20 >= event.mouseButton.x){
                        if(circles[i].getPosition().y <= event.mouseButton.y && circles[i].getPosition().y + 20 >= event.mouseButton.y){
                            point_id = i;
                        }
                    }
                }
            }
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                for(int i=0; i < circles.size(); ++i){
                    if(circles[i].getPosition().x <= event.mouseButton.x && circles[i].getPosition().x + 20 >= event.mouseButton.x){
                        if(circles[i].getPosition().y <= event.mouseButton.y && circles[i].getPosition().y + 20 >= event.mouseButton.y){
                            for( int j = 0; j < 100; j++ ){
                                points[j].weight = 999;
                                points[j].related_id = -1;
                                Q[j] = true;
                            }

                            dijkstra_alg(points, values, circles, lines_array, r, Q, i);
                        }
                    }
                }
            }
        }
        for(int i =0; i < circles.size(); ++i){
            circles[i].setFillColor(sf::Color::White);
        }
        while( point_id > -1){
            circles[point_id].setFillColor(sf::Color::Red);
            point_id = points[point_id].related_id;
        }
        for( auto p : points){
            if (p.weight == 0)
                circles[p.id].setFillColor(sf::Color::Red);
        }
        app.display();
    }
}

