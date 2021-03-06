#include "Track.hpp"
// Rad to Deg -> * 57.2957795f

Track::Track(b2World* world, Controller* controller) : world(world), controller(controller) {

	// Set the width and height of a single sector.
	float width = 50.0f;
	float height = 200.0f;
	float length = sqrt(((width / 2) * (width / 2)) + ((height / 2) * (height / 2)));
	float heightLengthAngle = acos((height / 2) / length);
	
	// TODO: Possibly read track composition from a file?
	// TODO: The track must always begin with a "straight", add a check for this
	std::vector<std::string> track = {
		"straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "left", "straight", "straight", "straight", "straight", "straight", "straight", "right", "straight", "straight", "straight", "straight", "straight", "straight", "left", "straight", "straight", "straight", "straight", "straight", "straight", "left", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "left", "left", "straight", "straight", "straight", "right", "straight", "straight", "straight", "right", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "right", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "left", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "left", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "left", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight", "straight"
	};

	// Initialize an empty vector to hold the graphical representation of the circuit.
	//std::vector<sf::VertexArray> sectors;
	//std::vector<sf::ConvexShape> kerbs;
	float angle = 0.0f;
	b2Vec2 midPoint = b2Vec2(0, 0);
	sf::Vector2f lastLeftCorner;
	sf::Vector2f lastRightCorner;
	sf::Vector2f kerbBegin;
	sf::Vector2f kerbEnd;
	
	for (auto & element : track) {
		if (element == "straight") {
			newSector(width, height, angle, midPoint, element);
			
			sf::VertexArray sector = drawSector(length, heightLengthAngle, angle, midPoint);
			lastLeftCorner = sector[1].position;
			lastRightCorner = sector[0].position;
			sectors.push_back(sector);
			
			midPoint.x += cos(angle * DEGTORAD) * width;
			midPoint.y += cos((angle + 90.0f) * DEGTORAD) * width;
		} else if (element == "left") {
			sf::ConvexShape kerb;
			kerb.setPointCount(11);
			
			kerbBegin = lastLeftCorner;
			for (unsigned i = 0; i < 10; i++) {
				newSector(width, height, angle, midPoint, element);
				
				sf::VertexArray sector = drawSector(length, heightLengthAngle, angle, midPoint);
				kerb.setPoint(i, lastLeftCorner);
				lastLeftCorner = sector[1].position;
				lastRightCorner = sector[0].position;
				sectors.push_back(sector);

				if (i < 9) {
					angle += 10.0f;
				}
				float xChange = cos(angle * DEGTORAD) * (width / 2);
				float yChange = sin(angle * DEGTORAD) * (width / 2);
				midPoint.x = lastRightCorner.x;
				midPoint.y = lastRightCorner.y;
				midPoint.x += xChange;
				midPoint.y -= yChange;
				midPoint.x -= sin(angle * DEGTORAD) * (height / 2);
				midPoint.y -= cos(angle * DEGTORAD) * (height / 2);
			}
			kerbEnd = lastLeftCorner;
			
			b2BodyDef body;
			body.position.Set((kerbBegin.x + kerbEnd.x) / 2, (kerbBegin.y + kerbEnd.y) / 2);
			
			b2Body* trackPart = world->CreateBody(&body);
			
			b2EdgeShape shape;
			b2FixtureDef walls;
			walls.shape = &shape;
			
			// Kerb
			shape.Set(b2Vec2(-(body.position.x - kerbBegin.x), -(body.position.y - kerbBegin.y)), b2Vec2(kerbEnd.x - body.position.x, kerbEnd.y - body.position.y));
			trackPart->CreateFixture(&walls);
			
			kerb.setPoint(10, lastLeftCorner);
			kerbs.push_back(kerb);
		} else if (element == "right") {
			sf::ConvexShape kerb;
			kerb.setPointCount(11);
			
			kerbBegin = lastRightCorner;
			for (unsigned i = 0; i < 10; i++) {
				newSector(width, height, angle, midPoint, element);
				
				sf::VertexArray sector = drawSector(length, heightLengthAngle, angle, midPoint);
				kerb.setPoint(i, lastRightCorner);
				lastLeftCorner = sector[1].position;
				lastRightCorner = sector[0].position;
				sectors.push_back(sector);

				if (i < 9) {
					angle -= 10.0f;
				}
				float xChange = cos(angle * DEGTORAD) * (width / 2);
				float yChange = sin(angle * DEGTORAD) * (width / 2);
				midPoint.x = lastLeftCorner.x;
				midPoint.y = lastLeftCorner.y;
				midPoint.x += xChange;
				midPoint.y -= yChange;
				midPoint.x += sin(angle * DEGTORAD) * (height / 2);
				midPoint.y += cos(angle * DEGTORAD) * (height / 2);
			}
			kerbEnd = lastRightCorner;
			
			b2BodyDef body;
			body.position.Set((kerbBegin.x + kerbEnd.x) / 2, (kerbBegin.y + kerbEnd.y) / 2);
			
			b2Body* trackPart = world->CreateBody(&body);
			
			b2EdgeShape shape;
			b2FixtureDef walls;
			walls.shape = &shape;
			
			// Kerb
			shape.Set(b2Vec2(-(body.position.x - kerbBegin.x), -(body.position.y - kerbBegin.y)), b2Vec2(kerbEnd.x - body.position.x, kerbEnd.y - body.position.y));
			trackPart->CreateFixture(&walls);
			
			kerb.setPoint(10, lastRightCorner);
			kerbs.push_back(kerb);
		} else {
			throw "Corrupted track file.";
		}
	}
	
	drawTextures();
	
}

Track::~Track() {

}

// Creates a track part that the physics engine can utilize. The method takes the width, height, angle and middle point of the track part as parameters.
void Track::newSector(float width, float height, float angle, b2Vec2 middlePoint, std::string direction) {
	
	b2BodyDef body;
	body.position.Set(middlePoint.x, middlePoint.y);
	
	b2Body* trackPart = world->CreateBody(&body);
	
	b2EdgeShape shape;
	b2FixtureDef checkpoints;
	checkpoints.shape = &shape;
	checkpoints.isSensor = true;
	
	// End of sector (right vertical edge)
	shape.Set(b2Vec2(width / 2, -height / 2), b2Vec2(width / 2, height / 2));
	trackPart->CreateFixture(&checkpoints);
	
	b2FixtureDef walls;
	walls.shape = &shape;
	
	// Left barrier (top horizontal edge)
	if (direction != "left") {
		shape.Set(b2Vec2(-width / 2, -height / 2), b2Vec2(width / 2, -height / 2));
		trackPart->CreateFixture(&walls);
	}
	
	// Right barrier (bottom horizontal edge)
	if (direction != "right") {
		shape.Set(b2Vec2(-width / 2, height / 2), b2Vec2(width / 2, height / 2));
		trackPart->CreateFixture(&walls);
	}
	
	trackPart->SetTransform(middlePoint, -angle * DEGTORAD);
	
}

// Creates a VertexArray that SFML uses to draw the track part in GUI
sf::VertexArray Track::drawSector(float length, float heightLengthAngle, float angle, b2Vec2 midPoint) {
	
	float bottomLeftYOffset = cos(angle * DEGTORAD - heightLengthAngle) * length;
	float bottomLeftXOffset = sin(angle * DEGTORAD - heightLengthAngle) * length;
	float topRightYOffset = -bottomLeftYOffset;
	float topRightXOffset = -bottomLeftXOffset;
	float topLeftYOffset = sin((90.0f - angle) * DEGTORAD - heightLengthAngle) * length;
	float topLeftXOffset = cos((90.0f - angle) * DEGTORAD - heightLengthAngle) * length;
	float bottomRightYOffset = -topLeftYOffset;
	float bottomRightXOffset = -topLeftXOffset;
	
	sf::VertexArray sector(sf::LinesStrip, 5);
	sector[0].position = sf::Vector2f(midPoint.x + topLeftXOffset, midPoint.y + topLeftYOffset);
	sector[1].position = sf::Vector2f(midPoint.x + topRightXOffset, midPoint.y + topRightYOffset);
	sector[2].position = sf::Vector2f(midPoint.x + bottomRightXOffset, midPoint.y + bottomRightYOffset);
	sector[3].position = sf::Vector2f(midPoint.x + bottomLeftXOffset, midPoint.y + bottomLeftYOffset);
	sector[4].position = sf::Vector2f(midPoint.x + topLeftXOffset, midPoint.y + topLeftYOffset);
	sector[0].color = sf::Color::Blue;
	sector[1].color = sf::Color::Blue;
	sector[2].color = sf::Color::Blue;
	sector[3].color = sf::Color::Blue;
	sector[4].color = sf::Color::Blue;
	return sector;
	
}

void Track::drawTextures() {
	// Create texture for finish line
	finishLine.setOrigin(-12.5, 100);
	unsigned int x = 25;
	unsigned int y = 200;
	checkered.create(x, y);
	sf::Uint8* pixels = new sf::Uint8[x * y * 4];
	int column = 0;
	int row = 0;
	int color = 255;
	for (unsigned i = 0; i < x * y * 4; i += 4) {
		pixels[i] = color;
		pixels[i + 1] = color;
		pixels[i + 2] = color;
		pixels[i + 3] = 255;
		
		column++;
		if (column >= 5) {
			column = 0;
			row++;
			if (!(row % 5 != 0) != (row % 50 != 0)) {
				if (color == 255) {
					color = 0;
				} else {
					color = 255;
				}
			}
		}
	}
	checkered.update(pixels);
	finishLine.setTexture(checkered);
	
	// Create top-right->bottom-left texture for kerb
	x = 120;
	y = 120;
	kerb1.create(x, y);
	pixels = new sf::Uint8[x * y * 4];
	column = 0;
	row = 0;
	for (unsigned i = 0; i < x * y * 4; i += 4) {
		if ((column - row) % 20 < 10) {
			pixels[i] = 255;
			pixels[i + 1] = 0;
			pixels[i + 2] = 0;
			pixels[i + 3] = 255;
		} else {
			pixels[i] = 255;
			pixels[i + 1] = 255;
			pixels[i + 2] = 255;
			pixels[i + 3] = 255;
		}
		column++;
		if (column % 120 == 0) {
			row++;
		}
	}
	kerb1.update(pixels);
	
	// Create top-left->bottom-right texture for kerb
	x = 120;
	y = 120;
	kerb2.create(x, y);
	pixels = new sf::Uint8[x * y * 4];
	column = 0;
	for (unsigned i = 0; i < x * y * 4; i += 4) {
		if (column % 20 < 10) {
			pixels[i] = 255;
			pixels[i + 1] = 0;
			pixels[i + 2] = 0;
			pixels[i + 3] = 255;
		} else {
			pixels[i] = 255;
			pixels[i + 1] = 255;
			pixels[i + 2] = 255;
			pixels[i + 3] = 255;
		}
		column++;
		if (column % 120 == 0) {
			column++;
		}
	}
	kerb2.update(pixels);
	
	// Set the correct texture for each kerb
	for (auto & kerb : kerbs) {
		sf::Vector2f begin = kerb.getPoint(0);
		sf::Vector2f end = kerb.getPoint(10);
		if ((begin.x < end.x && begin.y < end.y) || (begin.x > end.x && begin.y > end.y)) {
			kerb.setTexture(&kerb2);
		} else {
			kerb.setTexture(&kerb1);
		}
	}
}

void Track::render(sf::RenderWindow& window) {
	for (auto sector : sectors) {
		window.draw(sector);
	}
	for (auto kerb : kerbs) {
		window.draw(kerb);
	}
	window.draw(finishLine);
}
