#include "commandOfFigure.h"
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <iostream>
#include "iofmtguard.h"
void cmdArea(const std::vector< Polygon >& polygons, std::istream& input, std::ostream& output)
{
    using namespace std::placeholders;
    std::map< std::string, std::function< void(const std::vector< Polygon >&, std::ostream&) > > cmdsArea;
    cmdsArea["EVEN"] = std::bind(evenArea, _1, _2);
    cmdsArea["ODD"] = std::bind(oddArea, _1, _2);
    cmdsArea["MEAN"] = std::bind(meanArea, _1, _2);
    auto warningInvCom = std::bind(warning, _1, "<INVALID COMMAND>\n");
    std::string areaType;
    input >> areaType;
    try
    {
        cmdsArea.at(areaType)(polygons, output);
    }
    catch (const std::out_of_range& e)
    {
        if (std::isdigit(areaType[0]))
        {
            size_t num = std::stoull(areaType);
            if (num < 3)
            {
                warningInvCom(output);
            }
            else
            {
                vertexArea(num, polygons, output);
            }
        }
        else
        {
            warningInvCom(output);
        }
    }
}

void evenArea(const std::vector< Polygon >& polygons, std::ostream& output)
{
    std::vector< Polygon > evenPolygons;
    std::copy_if
            (
                    polygons.begin(),
                    polygons.end(),
                    std::back_inserter(evenPolygons),
                    [](const Polygon& pol) { return pol.points_.size() % 2 == 0; }
            );
    double sumArea = std::accumulate(evenPolygons.begin(), evenPolygons.end(), 0.0, plusArea);
    output << std::fixed << std::setprecision(1) << sumArea << "\n";
}

void oddArea(const std::vector< Polygon >& polygons, std::ostream& output)
{
    std::vector< Polygon > oddPolygons;
    std::copy_if
            (
                    polygons.begin(),
                    polygons.end(),
                    std::back_inserter(oddPolygons),
                    [](const Polygon& pol) { return pol.points_.size() % 2 != 0; }
            );
    double sumArea = std::accumulate(oddPolygons.begin(), oddPolygons.end(), 0.0, plusArea);
    output << std::fixed << std::setprecision(1) << sumArea << "\n";
}

void meanArea(const std::vector< Polygon >& polygons, std::ostream& output)
{
    auto warningInvCom = std::bind(warning, std::placeholders::_1, "<INVALID COMMAND>\n");
    if(polygons.empty())
    {
        warningInvCom(output);
    }
    else
    {
        double sumArea = std::accumulate(polygons.begin(), polygons.end(), 0.0, plusArea) / polygons.size();
        output << std::fixed << std::setprecision(1) << sumArea << "\n";
    }
}

void vertexArea(size_t num, const std::vector< Polygon >& polygons, std::ostream& output)
{
    std::vector< Polygon > needPolygons;
    std::copy_if
            (
                    polygons.begin(),
                    polygons.end(),
                    std::back_inserter(needPolygons),
                    [num](const Polygon& pol) { return pol.points_.size() == num; }
            );
    double sumArea = std::accumulate(needPolygons.begin(), needPolygons.end(), 0.0, plusArea);
    output << std::fixed << std::setprecision(1) << sumArea << "\n";
}

double plusArea(double sum, const Polygon& polygon)
{
    sum += calculatePolygonArea(polygon);
    return sum;
}

double calculatePolygonAreaRec(const Polygon& polygon, size_t i, double area)
{
    const size_t numpoints_ = polygon.points_.size();
    if (i >= numpoints_)
    {
        return area;
    }
    const Point& p1 = polygon.points_[i];
    const Point& p2 = polygon.points_[(i + 1) % numpoints_];
    return calculatePolygonAreaRec(polygon, i + 1, area + (p1.x * p2.y - p2.x * p1.y));
}

double calculatePolygonArea(const Polygon& polygon)
{
    return 0.5 * std::abs(calculatePolygonAreaRec(polygon, 0, 0.0));
}

void cmdMax(const std::vector< Polygon >& polygons, std::istream& input, std::ostream& output)
{
    using namespace std::placeholders;
    std::map< std::string, std::function< void(const std::vector< Polygon >&, std::ostream&) > > cmdsMax;
    cmdsMax["AREA"] = std::bind(findMaxArea, _1, _2);
    cmdsMax["VERTEXES"] = std::bind(findMaxVertexes, _1, _2);
    auto warningInvCom = std::bind(warning, _1, "<INVALID COMMAND>\n");
    std::string maxType;
    input >> maxType;
    try
    {
        if(polygons.empty())
        {
            warningInvCom(output);
        }
        else
        {
            cmdsMax.at(maxType)(polygons, output);
        }
    }
    catch (const std::out_of_range& e)
    {
        warningInvCom(output);
    }
}

void findMaxArea(const std::vector< Polygon >& polygons, std::ostream& output)
{
    std::vector< double > areasOfPolygons;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areasOfPolygons), calculatePolygonArea);
    std::sort(areasOfPolygons.begin(), areasOfPolygons.end());
    output << std::fixed << std::setprecision(1) << areasOfPolygons[areasOfPolygons.size() - 1] << "\n";
}

void findMaxVertexes(const std::vector< Polygon >& polygons, std::ostream& output)
{
    std::vector< size_t > vertexes;
    std::transform
            (
                    polygons.begin(),
                    polygons.end(),
                    std::back_inserter(vertexes),
                    [](const Polygon& pol) { return pol.points_.size(); }
            );
    std::sort(vertexes.begin(), vertexes.end());
    output << vertexes[vertexes.size() - 1] << "\n";
}

void cmdMin(const std::vector< Polygon >& polygons, std::istream& input, std::ostream& output)
{
    using namespace std::placeholders;
    std::map< std::string, std::function< void(const std::vector< Polygon >&, std::ostream&) > > cmdsMin;
    cmdsMin["AREA"] = std::bind(findMinArea, _1, _2);
    cmdsMin["VERTEXES"] = std::bind(findMinVertexes, _1, _2);
    auto warningInvCom = std::bind(warning, _1, "<INVALID COMMAND>\n");
    std::string minType;
    input >> minType;
    try
    {
        if(polygons.empty())
        {
            warningInvCom(output);
        }
        cmdsMin.at(minType)(polygons, output);
    }
    catch (const std::out_of_range& e)
    {
        warningInvCom(output);
    }
}

void findMinArea(const std::vector< Polygon >& polygons, std::ostream& output)
{
    std::vector< double > areasOfPolygons;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areasOfPolygons), calculatePolygonArea);
    std::sort(areasOfPolygons.begin(), areasOfPolygons.end());
    output << std::fixed << std::setprecision(1) << areasOfPolygons[0];
}

void findMinVertexes(const std::vector< Polygon >& polygons, std::ostream& output)
{
    std::vector< size_t > vertexes;
    std::transform
            (
                    polygons.begin(),
                    polygons.end(),
                    std::back_inserter(vertexes),
                    [](const Polygon& pol) { return pol.points_.size(); }
            );
    std::sort(vertexes.begin(), vertexes.end());
    output << vertexes[0];
}

void cmdCount(const std::vector< Polygon >& polygons, std::istream& input, std::ostream& output)
{
    using namespace std::placeholders;
    std::map< std::string, std::function< void(const std::vector< Polygon >&, std::ostream&) > > cmdsCount;
    cmdsCount["EVEN"] = std::bind(countEven, _1, _2);
    cmdsCount["ODD"] = std::bind(countOdd, _1, _2);
    auto warningInvCom = std::bind(warning, _1, "<INVALID COMMAND>\n");
    std::string countType;
    input >> countType;
    try
    {
        cmdsCount.at(countType)(polygons, output);
    }
    catch (const std::out_of_range& e)
    {
        if (std::isdigit(countType[0]))
        {
            size_t num = std::stoull(countType);
            if (num < 3)
            {
                warningInvCom(output);
            }
            else
            {
                vertexCount(num, polygons, output);
            }
        }
        else
        {
            throw std::invalid_argument("<INVALID COMMAND>\n");
        }
    }
}

void countEven(const std::vector< Polygon >& polygons, std::ostream& output)
{
    size_t result = std::count_if
            (
                    polygons.begin(),
                    polygons.end(),
                    [](const Polygon& pol) { return pol.points_.size() % 2 == 0; }
            );
    output << result << "\n";
}

void countOdd(const std::vector< Polygon >& polygons, std::ostream& output)
{
    size_t result = std::count_if
            (
                    polygons.begin(),
                    polygons.end(),
                    [](const Polygon& pol) { return pol.points_.size() % 2 != 0; }
            );
    output << result << "\n";
}

void vertexCount(size_t num, const std::vector< Polygon >& polygons, std::ostream& output)
{
    size_t result = std::count_if
            (
                    polygons.begin(),
                    polygons.end(),
                    [num](const Polygon& pol) { return pol.points_.size() == num; }
            );
    output << result << "\n";
}

void warning(std::ostream& output, const std::string& mes)
{
    output << mes;
}

void intersections(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
    using namespace std::placeholders;
    Polygon polygon;
    in >> polygon;
    try {
        if (polygons.empty() or (in.peek() != '\n' or !in))
        {
            throw std::invalid_argument("");
        }
    } catch (std::invalid_argument& e) {
        std::cerr << "<INVALID COMMAND>\n";
    }
    auto intersectPredicate = std::bind(isIntersectionChecks, std::cref(polygon), _1);
    out << std::count_if(polygons.cbegin(), polygons.cend(), intersectPredicate);
}
void rmecho(std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
    using namespace std::placeholders;
    Polygon figure;
    in >> figure;
    if (polygons.empty()  or !in || in.peek() != '\n')
    {
        in.setstate(std::ios::failbit);
    }
    auto identical = std::bind(EqualFigures {figure}, _1, _2);
    auto last = std::unique(polygons.begin(), polygons.end(), identical);
    size_t removeFigure = std::distance(last, polygons.end());
    polygons.erase(last, polygons.end());
    out << removeFigure;
}
