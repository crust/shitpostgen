#include <Magick++.h>

#include <cassert>

#include "json.h"

#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <sstream>

using namespace Magick;

std::string command_line(const char *command)
{
    FILE *pipe = popen(command, "r");
    assert(pipe);

    char buffer[1024];
    std::string result;

    while (auto count = std::fread(buffer, 1, sizeof(buffer), pipe))
    {
        result.insert(result.end(), buffer, buffer + count);
    }

    pclose(pipe);
    return result;
}

std::string fetch_frinkiac_random_json()
{
    return command_line("curl -s https://frinkiac.com/api/random");
}

struct Frame
{
    explicit Frame(std::string const &frame_json)
    {
        auto value = json::parse(frame_json).get_object();
        const auto &frame = value.at("Frame").get_object();
        const auto &episode = frame.at("Episode").get_string();
        const uint64_t timestamp = frame.at("Timestamp").get_number();
        std::stringstream ss;
        ss << "https://frinkiac.com/img/" << episode << '/' << timestamp << ".jpg";
        url = std::move(ss).str();
        ss = std::stringstream();
        ss << episode << '-' << timestamp;
        id = std::move(ss).str();
    }

    std::string id;
    std::string url;
};

Image download_and_open_image(Frame const &frame)
{
    auto path = "tmp/" + frame.id + ".jpg";
    std::stringstream command;
    command << "curl -s " << frame.url << " > " << path;
    command_line(command.str().c_str());
    return Image(path);
}

int main()
{
    Image top_image("guys-want-one-thing.png");

    auto frame_json = fetch_frinkiac_random_json();
    assert(frame_json.size());

    Frame frame(frame_json);

    auto bottom_image = download_and_open_image(frame);

    Geometry new_size(top_image.columns() + 10, bottom_image.rows() * 0.9);
    new_size.aspect(true);
    bottom_image.adaptiveResize(new_size);

    Image result(Geometry(
        top_image.columns(),
        top_image.rows() + bottom_image.rows()
    ), "white");

    result.composite(top_image, 0, 0, OverCompositeOp);
    result.composite(bottom_image, -5, top_image.rows(), OverCompositeOp);

    auto result_path = "img/" + frame.id + ".jpg";
    result.write(result_path);

    command_line(std::string("rm tmp/" + frame.id + ".jpg").c_str());

    std::cout << result_path << '\n';
    return 0;
}
