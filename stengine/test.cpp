#include <stengine/engine.h>
#include <stengine/fixedgrid.h>
#include <fstream>
#include <ncurses.h>
#include <thread>
#include <chrono>

using namespace stengine;

class TestRenderer : public Renderer {
public:
	static const ComponentID ID;

	TestRenderer( GameObject* parent ) : Renderer( parent ) {}

	ComponentID getID() {
		return ID;
	}

	void update() {}
	void render() {
		mvaddch(parent->position.y, parent->position.x, 'C');
	}
};
const ComponentID TestRenderer::ID = nextComponentID();

class TestComponent : public Component {
public:
	static const ComponentID ID;

	TestComponent( GameObject* parent ) : Component( parent ) {}

	ComponentID getID() {
		return ID;
	}

	void update() {
		parent->position.x++;
	}

	void onCollision( const Collider* col ) {
		debug::log << "Collision Happened" << debug::endl;
	}
};
const ComponentID TestComponent::ID = nextComponentID();

int main() {
	int loops = 0;

	std::ofstream logOutput( "debug.log" );
	DebugChannel::defaultOutput = &logOutput;

	GameManager::init();

	GameObject* testObject = new GameObject();
	testObject->addComponent<TestComponent>();
	testObject->addComponent<GridSpaceCollider>();
	testObject->addComponent<TestRenderer>();
	GameManager::addObject( testObject );

	GameObject* obstacle = new GameObject();
	obstacle->addComponent<GridSpaceCollider>();
	obstacle->position.x = 2;
	GameManager::addObject( obstacle );

	if( auto* j = testObject->getComponent<TestRenderer>() )
		debug::log << "Got Component of ID " << j->getID() << "Should be: " << TestRenderer::ID << debug::endl;
	else
		debug::log << "Returned NULL" << debug::endl;


	while( loops++ < 8 ) {
		GameManager::update();
		GameManager::render();
		std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
	}

	GameManager::end();

	return 0;
}