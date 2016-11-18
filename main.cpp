#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdlib>


int main()
{
    srand(time(0));
    int r = 10;
    // Create the main window
    sf::RenderWindow app(sf::VideoMode(800, 600), "SFML window");
    std::vector<sf::Vertex*> lines;
    sf::Vertex lines_array[200][2];
    std::vector<int> values(190, 1);
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

    std::vector<point> points;
    std::vector<sf::CircleShape> circles;
    bool Q[100];
    bool S[100];
    for(int i =0; i<10; ++i){
        for(int g =0; g<10; ++g){
            sf::CircleShape circle;
            circle.setRadius(r);
            circle.setPosition(r * g * 4 + 30, r * i * 4 + 30);
            circle.setOutlineColor(sf::Color::Red);
            circle.setFillColor(sf::Color::White);
            circles.push_back(circle);
            int iter = i * 10 + g;
            point new_point =  { iter , 99, -1, 999, 999, 999, 999 };
            points.push_back(new_point);
            Q[iter] = true;
            S[i] = false;
        }
    }

    for( int i =1; i<100; ++i ){
        if ( i%10 != 0  && rand()%9 > 3 ){
            points[i-1].right = 1;
            points[i].left = 1;
            lines_array[i][0] = sf::Vertex(sf::Vector2f( circles[i-1].getPosition().x + r, circles[i-1].getPosition().y + r ));
            lines_array[i][1] = sf::Vertex(sf::Vector2f( circles[i].getPosition().x + r, circles[i].getPosition().y + r));
        }
        else {
            points[i-1].right = 999;
            points[i].left = 999;
            values[i] = 999;
        }
    }
    for( int i =0; i<90; ++i ){
        if ( rand()%9 > 3 ){
            points[i].bot = 1;
            points[i + 10].top = 1;
            lines_array[i + 100][0] = sf::Vertex(sf::Vector2f( circles[i].getPosition().x + r, circles[i].getPosition().y + r ), sf::Color::White);
            lines_array[i + 100][1] = sf::Vertex(sf::Vector2f( circles[i + 10].getPosition().x + r, circles[i + 10].getPosition().y + r), sf::Color::White);
        }
        else{
            points[i].bot = 999;
            points[i + 10].top = 999;
            values [i +100] = 999;
        }
    }
    points[0].weight = 0;
    for( auto i : points){
        point bufor;
        bufor.weight = 9999999;
        for(auto p : points){
            if (bufor.weight > p.weight && Q[p.id])
                bufor = p;
        }
        Q[bufor.id] = false;
        S[bufor.id] = true;
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
    int point_id = 0;

    while (app.isOpen())
    {
        // Process events
        sf::Event event;
        while (app.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                app.close();
        }
        // Clear screen
        app.clear();
        // Draw the sprite
        for(auto line : lines_array){
            app.draw(line, 2, sf::Lines);
        }
        for(auto circle : circles){
            app.draw(circle);
        }
        // Pick finish circle
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
        }
        for(int i =0; i < circles.size(); ++i){
            circles[i].setFillColor(sf::Color::White);
        }
        while( point_id > -1){
            circles[point_id].setFillColor(sf::Color::Red);
            point_id = points[point_id].related_id;
        }

        app.display();
    }
    return EXIT_SUCCESS;
}
