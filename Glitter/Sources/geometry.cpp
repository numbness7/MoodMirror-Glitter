#include <geometry.hpp>

bool isInSquare(float x, float y, float centerX, float centerY, float length){
    float dx = abs(centerX - x);
    float dy = abs(centerY - y);
    return length >= dx && length >= dy;
}

bool isInSquare(glm::vec2 point, glm::vec2 center,  float length){
    return isInSquare(point.x, point.y, center.x, center.y, length);
}

void generateDistancesFromBorder(std::vector<float>& distances, int width, int height, const std::vector<glm::vec2>& borderVertices){
        std::vector<std::tuple<float,float,float>> borderLines{}; 
        glm::vec2 prevVertex  = borderVertices[borderVertices.size()-1];
        std::vector<glm::vec2> borderVerticesN{};
        glm::vec2 centerN = glm::vec2(
                boundTransformation(0, -0.5f, 0.5f, 0, width),
                boundTransformation(0, -0.5f, 0.5f, 0, height));
        for(int borderVertexIndex = 0; borderVertexIndex < borderVertices.size(); borderVertexIndex++){
            glm::vec2 curVertex;
            if(borderVertexIndex < borderVertices.size() - 1)
                curVertex = borderVertices[borderVertexIndex];
            else
                curVertex = borderVertices[borderVertices.size()-1];
            float  preVertexNormX = boundTransformation(prevVertex.x, -0.5f, 0.5f, 0, width);
            float  preVertexNormY = boundTransformation(prevVertex.y, -0.5f, 0.5f, 0, height);
            glm::vec2 preVertexNorm(preVertexNormX,preVertexNormY);
            float  curVertexNormX = boundTransformation(curVertex.x, -0.5f, 0.5f, 0, width);
            float  curVertexNormY = boundTransformation(curVertex.y, -0.5f, 0.5f, 0, height);
            glm::vec2 curVertexNorm(curVertexNormX,curVertexNormY);
            borderVerticesN.push_back(curVertexNorm);
            auto line = getLineFromVertices(preVertexNorm.x, curVertexNorm.x, preVertexNorm.y, curVertexNorm.y);
            borderLines.push_back(line);
            prevVertex = curVertex;
        }
    

    for(int rowI = 0; rowI < height; rowI++){
        for(int colI = 0; colI < width; colI++){
            float row = (float) rowI;
            float col = (float) colI;
            float gridY = row;
            float gridX = col;
            glm::vec2 tileCenter(0.5,0.5);
            glm::vec2 gridVertex(gridX + tileCenter.x, gridY + tileCenter.y);

            float minDistance = std::numeric_limits<float>::max();
            glm::vec2 prevVertex  = borderVerticesN[borderVerticesN.size()-1];
            auto curVertex = borderVerticesN[0];
            glm::vec2 leftVertex = dir3D::LEFT*(float)width*10.0f;
            glm::vec2 rightVertex = dir3D::RIGHT*(float)width*10.0f;
            auto [allRight, allLeft, allDown, allUp] = std::tuple<bool,bool,bool,bool>{true,true,true,true};
            for(int borderLineIndex = 0; borderLineIndex < borderLines.size(); borderLineIndex++){
                curVertex = borderVerticesN[borderLineIndex];
                if(curVertex.x < gridVertex.x)
                    allRight = false;
                if(curVertex.x > gridVertex.x)
                    allLeft = false;
                if(curVertex.y < gridVertex.y)
                    allUp = false;
                if(curVertex.y > gridVertex.y)
                    allDown = false;
                if(curVertex.x > leftVertex.x && curVertex.x < gridVertex.x){
                    leftVertex = curVertex;
                }
                if(curVertex.x < rightVertex.x && curVertex.x > gridVertex.x){
                    rightVertex = curVertex;
                }
            }


            glm::vec2  endRayPoint;
            endRayPoint = ((glm::vec2(leftVertex.x, height*10.0f) + glm::vec2(rightVertex.x, height*10.0f))/2.0f);

            int intersections = 0;
            bool onBorder = false;
            for(int borderLineIndex = 0; borderLineIndex < borderLines.size(); borderLineIndex++){
                curVertex = borderVerticesN[borderLineIndex];
                auto line = borderLines[borderLineIndex];
                glm::vec2 trash;
                if(tryGetIntersection(trash,{gridVertex, endRayPoint}, {prevVertex, curVertex}, false)){
                    intersections++;
                }
                float distance = distFromLine(line, gridVertex);
                if(glm::abs(distance) < glm::abs(minDistance)){
                    minDistance = distance;
                }
                prevVertex = curVertex;
            }
            

            bool isInsideShape = (intersections % 2 == 1 && !(allDown || allLeft || allRight || allUp)) || onBorder;
            
            if(isInsideShape)
                distances.push_back(minDistance);
            else
                distances.push_back(-minDistance);
        }
    }
    float maxDistance = maxM(distances);
    float minDistance = minM(distances);
    float normDistance = (glm::abs(maxDistance) > glm::abs(minDistance)) ? glm::abs(maxDistance) : glm::abs(minDistance);
    for(int i = 0; i < distances.size(); i++){
        distances[i]/=normDistance;
    }
}
unsigned int boundTransformation(unsigned int x, unsigned int min, unsigned int max, unsigned int minT, unsigned int maxT){
    unsigned int slope = (maxT - minT)/(max - min);
    unsigned int intercept = minT - slope*min;
    return slope*x + intercept;
}

float boundTransformation(float x, float min, float max, float minT, float maxT){
    assert(max != min);
    float slope = (maxT - minT)/(max - min);
    float intercept = minT - slope*min;
    return slope*x + intercept;
}
glm::vec2 boundTransformation(glm::vec2 pos, glm::vec2 min, glm::vec2 max, glm::vec2 minT, glm::vec2 maxT){
    assert(max != min);
    float xT = boundTransformation(pos.x, min.x, max.x, minT.x, maxT.x);
    float yT = boundTransformation(pos.y, min.y, max.y, minT.y, maxT.y);
    return glm::vec2(xT, yT);
}

std::tuple<float,float> linearTransformation(float xf, float xi, float yf, float yi){
    if(xf - xi == 0){
        throw(std::runtime_error("Error: No horizontal distance"));
    }
    float slope = (yf - yi)/(xf - xi);
    float intercept = yi - slope*xi;
    return std::tuple<float,float>(slope,intercept);
}

std::tuple<float,float,float> getLineFromVertices(const float xi, const float xf, const float yi, const float yf){
    const float A = yf - yi;
    const float B = xi - xf;
    const float C = xf*yi - xi*yf;
    return {A,B,C};
}

std::tuple<float,float,float> getLineFromVertices(const glm::vec2 vi, const glm::vec2 vf){
    return getLineFromVertices(vi.x, vf.x, vi.y, vf.y);
}

float boundTransformation(float x, std::tuple<float,float> line){
    return x*std::get<0>(line)+ std::get<1>(line);
}

float boundTransformation(float x, float slope, float intercept){
    return x*slope + intercept;
}
bool hasIntersection(const std::tuple<float, float, float> line1, const std::tuple<float, float, float> line2){
    const auto [A1, B1, C1] = line1;
    const auto [A2, B2, C2] = line2;
    return A1 != A2 || B1 != B2;
}

bool hasIntersection(const std::tuple<glm::vec2,glm::vec2> lineSegment1, const std::tuple<glm::vec2, glm::vec2> lineSegment2, const glm::vec2 lineIntersection){
    const glm::vec2 startPoint1 = std::get<0>(lineSegment1);
    const glm::vec2 endPoint1 = std::get<1>(lineSegment1);
    
    const glm::vec2 startPoint2 = std::get<0>(lineSegment2);
    const glm::vec2 endPoint2 = std::get<1>(lineSegment2);

    const float minX1 = glm::min(startPoint1.x, endPoint1.x);
    const float maxX1 = glm::max(startPoint1.x, endPoint1.x); 
    const float minY1 = glm::min(startPoint1.y, endPoint1.y);
    const float maxY1 = glm::max(startPoint1.y, endPoint1.y); 

    const float minX2 = glm::min(startPoint2.x, endPoint2.x);
    const float maxX2 = glm::max(startPoint2.x, endPoint2.x); 
    const float minY2 = glm::min(startPoint2.y, endPoint2.y);
    const float maxY2 = glm::max(startPoint2.y, endPoint2.y); 

    const float minX = glm::max(minX1, minX2);
    const float maxX = glm::min(maxX1, maxX2);
    const float minY = glm::max(minY1, minY2);
    const float maxY = glm::min(maxY1, maxY2);

    return lineIntersection.x >= minX && lineIntersection.x <= maxX && lineIntersection.y >= minY && lineIntersection.y <= maxY;
}


bool tryGetIntersection(glm::vec2& intersection, const std::tuple<float, float, float> line1, const std::tuple<float, float, float> line2){
    const auto [A1, B1, C1] = line1;
    const auto [A2, B2, C2] = line2;

    if(!hasIntersection(line1, line2))
        return false;

    const auto x = (B1*C2 - B2*C1)/(A1*B2-A2*B1);
    const auto y = (A1*C2 - A2*C1)/(B1*A2-B2*A1);
    intersection = glm::vec2(x,y);
    return true;
}

bool tryGetIntersection(glm::vec2& intersection, const std::tuple<glm::vec2,glm::vec2> lineSegment1, const std::tuple<glm::vec2, glm::vec2> lineSegment2, bool lineSegmentsDefineLines){
    const auto line1= getLineFromVertices(std::get<0>(lineSegment1), std::get<1>(lineSegment1));
    const auto line2= getLineFromVertices(std::get<0>(lineSegment2), std::get<1>(lineSegment2));
    if(!tryGetIntersection(intersection, line1, line2))
        return false;
    return lineSegmentsDefineLines || hasIntersection(lineSegment1, lineSegment2, intersection);
}

bool hasIntersection(const std::tuple<glm::vec2,glm::vec2> lineSegment1, const std::tuple<glm::vec2, glm::vec2> lineSegment2, bool lineSegmentsDefineLines){
    glm::vec2 intersection;
    const auto line1= getLineFromVertices(std::get<0>(lineSegment1), std::get<1>(lineSegment1));
    const auto line2= getLineFromVertices(std::get<0>(lineSegment2), std::get<1>(lineSegment2));
    if(!tryGetIntersection(intersection, line1, line2))
        return false;
    return lineSegmentsDefineLines || hasIntersection(lineSegment1, lineSegment2, intersection);
}
glm::vec2 rotate(const glm::vec2& vec2, const float& radians){
    float originalAngle = glm::atan(vec2.y,vec2.x);
    glm::vec2 rotatedN = glm::vec2(glm::cos(originalAngle +  radians), glm::sin( originalAngle + radians));
    return glm::length(vec2) * rotatedN;
}
float distFromLine(std::tuple<float,float,float> line, glm::vec2 point){
    const auto [A, B, C] = line;
    assert(A != 0 || B != 0);
    auto lineValue = A*point.x + B*point.y + C;
    lineValue = glm::abs(lineValue);
    auto lineNormalizer = glm::sqrt(glm::pow(A,2) + glm::pow(B,2));
    return lineValue/lineNormalizer;
}

bool isOnLine(std::tuple<float, float, float> line, glm::vec2 point)
{
    const auto& [A, B, C] = line;
    float actualValue = A*point.x + B*point.y + C;
    return isInRange(actualValue, -SMALL_FLOAT, SMALL_FLOAT);
}

glm::mat3 glm::rotate(glm::mat3 mat3, float angle)
{
    glm::mat3 rotMat3(0.0f);
    rotMat3[0][0] = glm::cos(angle);
    rotMat3[1][0] = -glm::sin(angle);
    rotMat3[0][1] = glm::sin(angle);
    rotMat3[1][1] = glm::cos(angle);
    rotMat3[2][2] = 1;
    return rotMat3 * mat3;
}

glm::mat3 glm::scale(glm::mat3 mat3, glm::vec2 vec2)
{
    for(int i = 0; i < 2; i++){
        mat3[i][i] *= vec2[i];
    }
    return mat3;
}

glm::mat3 glm::translate(glm::mat3 mat3, glm::vec2 vec2)
{
    for(int i = 0; i < 2; i++){
        mat3[2][i] = vec2[i];
    }
    return mat3;
}

glm::vec2 glm::vec3ToVec2(glm::vec3 vec3)
{
    return glm::vec2(vec3.x,vec3.y);
}


glm::vec2 glm::operator*(glm::mat3 mat3, glm::vec2 vec2)
{
    glm::vec3 vec3(vec2, 1.0f);
    glm::vec2 newVec(0.0f);
    for(int row = 0; row < 2; row++){
        for(int col = 1; col < 3; col++){
            newVec[row] += mat3[col][row]*vec3[col];
        }
    }
    return newVec;
}