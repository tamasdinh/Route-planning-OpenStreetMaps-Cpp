#include <optional>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <io2d.h>
#include "route_model.h"
#include "render.h"
#include "route_planner.h"

using namespace std::experimental;

static std::optional<std::vector<std::byte>> ReadFile(const std::string &path)
{   
    std::ifstream is{path, std::ios::binary | std::ios::ate}; // opened in binary mode & ate = at the end
    if( !is )
        return std::nullopt;
    
    auto size = is.tellg();                 // because the input stream is at the end of the file, tellg() tells size
    std::vector<std::byte> contents(size);  // vector of bytes same size as the file is created
    
    is.seekg(0);                            // seeking back to beginning of file
    is.read((char*)contents.data(), size);  // entire file is read into the contents vector

    if( contents.empty() )
        return std::nullopt;
    return std::move(contents);
}

int main(int argc, const char **argv)       // argc os arg count, argv is array of arguments passed
{    
    std::string osm_data_file = "";         // searches for "-f" in argument array and then...
    if( argc > 1 ) {
        for( int i = 1; i < argc; ++i )
            if( std::string_view{argv[i]} == "-f" && ++i < argc )
                osm_data_file = argv[i];    // ... stores the file name into osm_data_file
    }
    else {
        std::cout << "Usage: [executable] [-f filename.osm]" << std::endl;    
    }
    
    std::vector<std::byte> osm_data;
 
    if( osm_data.empty() && !osm_data_file.empty() ) {
        std::cout << "Reading OpenStreetMap data from the following file: " <<  osm_data_file << std::endl;
        auto data = ReadFile(osm_data_file);
        if( !data )
            std::cout << "Failed to read." << std::endl;
        else
            osm_data = std::move(*data);
    }
    
    // TODO: Declare floats `start_x`, `start_y`, `end_x`, and `end_y` and get
    // user input for these values using std::cin. Pass the user input to the
    // RoutePlanner object below.

    // Build Model.
    RouteModel model{osm_data};

    // Perform search and render results.
    RoutePlanner route_planner{model, 10, 10, 90, 90};
    RoutePlanner::AStarSearch(route_planner);
    std::cout << "Distance: " << route_planner.GetDistance() << " meters" << std::endl;
    Render render{model};

    auto display = io2d::output_surface{400, 400, io2d::format::argb32, io2d::scaling::none, io2d::refresh_style::fixed, 30};
    display.size_change_callback([](io2d::output_surface& surface){
        surface.dimensions(surface.display_dimensions());
    });
    display.draw_callback([&](io2d::output_surface& surface){
        render.Display(surface);
    });
    display.begin_show();
}
