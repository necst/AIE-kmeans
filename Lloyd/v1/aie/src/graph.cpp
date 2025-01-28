#include "graph.h"

using namespace adf;

my_graph aie_graph;

int main(int argc, char **argv)
{
	aie_graph.init();
	aie_graph.run(1);
	aie_graph.end();

	return 0;
}