#include<iostream>
#include<stdlib.h>
#include<string>
#define MAX 50
FILE* outfile;



//For the items of each tree Name
class itemNode
{
public:
	char name[MAX];
	int count;
	itemNode* left, * right;
	itemNode()
	{
		name[0] = '\0';
		count = 0;
		left = NULL;
		right = NULL;
	}
	itemNode(char itemName[], int population)
	{
		strcpy(name, itemName);
		count = population;
		left = NULL;
		right = NULL;
	}

};

//For Tree Names
class treeNameNode
{
public:
	char name[50];
	treeNameNode* left, * right;
	itemNode* theTree;
	treeNameNode()
	{
		name[0] = '\0';
		theTree = NULL;
		left = NULL;
		right = NULL;
	}
	treeNameNode(char treeName[])
	{
		strcpy(name, treeName);
		theTree = NULL;
		left = NULL;
		right = NULL;
	}

};

// PROTOTYPES
treeNameNode* insertTreeNameNode(treeNameNode* root, char name[MAX]);
itemNode* insertItemNode(itemNode* root, char name[MAX], int pop);
template<typename T>
char* inorder(T* root, static char[]);
treeNameNode* searchNameNode(treeNameNode* root, char treeName[MAX]);
void traverse_in_traverse(treeNameNode* root);
treeNameNode* buildNameTree();
bool search(treeNameNode* tree, itemNode* root, char name[]);
int countNodeNum(itemNode* root);
int itemBefore(itemNode *root, char name[]);
void heightBal(treeNameNode *root);
int count(itemNode *root);



treeNameNode* insertTreeNameNode(treeNameNode *root, char name[MAX]) {
	
	if (root == NULL) {
		treeNameNode* newNode = new treeNameNode(name);
		return newNode;
	}

	if (strcmp(name, root->name) < 0) {
		root->left = insertTreeNameNode(root->left, name);
	}
	else if (strcmp(name, root->name) > 0) {
		root->right = insertTreeNameNode(root->right, name);
	}

	return root;
}


itemNode* insertItemNode(itemNode* root, char name[MAX], int pop) {

	if (root == NULL) {
		itemNode* newNode = new itemNode(name, pop);
		return newNode;
	}
	else if (strcmp(name, root->name) < 0) {
		root->left = insertItemNode(root->left, name, pop);
	}
	else if (strcmp(name, root->name) > 0) {
		root->right = insertItemNode(root->right, name, pop);
	}

	return root;
}


treeNameNode* buildNameTree() {
	
	
	FILE* infile = fopen("in.txt", "r");
	if (infile == NULL)
		std::cout << "Error opening file" << std::endl;

	int treeNameCount, itemCount, queryCount;
	fscanf(infile, "%d %d %d", &treeNameCount, &itemCount, &queryCount);

	// set the first entry as root node
	char line[MAX];
	fscanf(infile, "%s", &line);
	treeNameNode *tempRoot = new treeNameNode(line);
	// create the rest of the nodes
	for (int i = 1; i < treeNameCount; i++) {
		fscanf(infile, "%s", &line);
		insertTreeNameNode(tempRoot, line);
	}
	
	for (int i = 0; i < itemCount; i++) {
		char treeName[MAX], itemName[MAX];
		int population;

		fscanf(infile, "%s %s %d", &treeName, &itemName, &population);
		treeNameNode* referencedNode = searchNameNode(tempRoot, treeName);
		referencedNode->theTree = insertItemNode(referencedNode->theTree, itemName, population);
	}
	fclose(infile);
	return tempRoot;
}


template<typename T>
char* inorder(T* root, static char result[]) {
	if (root != NULL) {
		inorder(root->left, result);
		strcat(result, root->name);
		strcat(result, " ");
		inorder(root->right, result);
	}
	return result;
}


treeNameNode* searchNameNode(treeNameNode* root, char treeName[MAX]) {
	if (root == NULL) {
		return NULL;
	}
	if (strcmp(treeName, root->name) == 0) {
		return root;
	}
	else if (strcmp(treeName, root->name) < 0) {
		searchNameNode(root->left, treeName);
	}
	else if (strcmp(treeName, root->name) > 0) {
		searchNameNode(root->right, treeName);
	}
}


bool search(treeNameNode *tree, itemNode *root, char name[]) {
	if (root == NULL) {
		std::cout << name << " not found in " << tree->name << std::endl;
		fprintf(outfile, "%s not found in %s\n", name, tree->name);
		return false;
	}
	
	else if (strcmp(root->name, name) == 0) {
		std::cout << root->count << " " << root->name << " found in " << tree->name << std::endl;
		fprintf(outfile, "%d %s found in %s\n", root->count, root->name, tree->name);
		return true;
	}
	else if (strcmp(name, root->name) < 0) {
		search(tree, root->left, name);
	}
	else if (strcmp(name, root->name) > 0) {
		search(tree, root->right, name);
	}
}


int countNodeNum(itemNode* root) {
	if (root == NULL)
		return 0;
	else
		return countNodeNum(root->left) + countNodeNum(root->right) + 1;
}


int itemBefore(itemNode *root, char name[]) {
	static int sum = 0;

	if (root == NULL) return 0;

	else if (strcmp(root->name, name) == 0) {
		sum += countNodeNum(root->left);
		// now that static sum is not needed, reset it for next time function is called
		int tempVal = sum;
		sum = 0;
		return tempVal;
	}
	else if (strcmp(name, root->name) < 0) {
		itemBefore(root->left, name);
	}
	else if (strcmp(name, root->name) > 0) {
		sum += (countNodeNum(root->left) + 1);
		itemBefore(root->right, name);
	}
}


int height(itemNode* node) {
	if (node == NULL) return 0;
	else
		return 1 + std::max(height(node->left), height(node->right));
}

void heightBal(treeNameNode *root) {
	int lheight = height(root->theTree->left);
	int rheight = height(root->theTree->right);
	int dif = abs(rheight - lheight);

	bool isBal = dif <= 1;

	if (isBal) {
		std::cout << "left height " << lheight << ", right height " << rheight << ", difference " << dif << ", balanced" << std::endl;
		fprintf(outfile, "left height %d, right height %d, difference %d, balanced\n", lheight, rheight, dif);
	}
	else {
		std::cout << "left height " << lheight << ", right height " << rheight << ", difference " << dif << ", not balanced" << std::endl;
		fprintf(outfile, "left height %d, right height %d, difference %d, not balanced\n", lheight, rheight, dif);
	}
	return;
}


int count(itemNode *root) {
	if (root == NULL)
		return 0;
	else
		return count(root->left) + count(root->right) + root->count;
}


void traverse_in_traverse(treeNameNode* root) {
	//~~~~~~~~~~~~ step 1: print the name tree ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static char result[MAX];
	inorder<treeNameNode>(root, result);

	std::cout << result << std::endl;
	fprintf(outfile,"%s\n", result);

	//~~~~~~~~~~~ step 2: for each name just printed, print the title then its own tree inorder ~~~~~~~~~~~~~~~~~~~~~~~~
	char* word[sizeof(strtok(result, " "))];
	
	word[0] = strtok(result, " "); // Splits spaces between words in str
	for (int i = 1; i < sizeof(strtok(result, " ")); i++)
	{
		word[i] = strtok(NULL, " ,.-");
	}

	// open the file to get the number of lines you need to read
	// used later to work through queries
	FILE* infile = fopen("in.txt", "r");
	if (infile == NULL)
		std::cout << "Error opening file" << std::endl;
	int treeNameCount, itemCount, queryCount;
	fscanf(infile, "%d %d %d", &treeNameCount, &itemCount, &queryCount);

	// loop through each node recently printed
	// print inorder of each tree attached to the treeNameNode
	for (int i = 0; i < treeNameCount; i++) {
		std::cout << "**" << word[i] << "**" << std::endl;
		fprintf(outfile, "**%s**\n", word[i]);

		static char arr[MAX];
		inorder<itemNode>(searchNameNode(root, word[i])->theTree, arr);
		std::cout << arr << std::endl;
		fprintf(outfile, "%s\n", arr);
		strcpy(arr, "");
	}

	fclose(infile);
	
}


int main() {
	
	outfile = fopen("out.txt", "w");

	treeNameNode *root = buildNameTree();
	
	traverse_in_traverse(root);

	FILE* infile = fopen("in.txt", "r");
	if (infile == NULL)
		std::cout << "Error opening file" << std::endl;

	int treeNameCount, itemCount, queryCount;
	fscanf(infile, "%d %d %d", &treeNameCount, &itemCount, &queryCount);

	// read through the file until we are at the start of the queries
	// dont need to save the data we are reading through
	for (int i = 0; i <= (treeNameCount + itemCount); i++) {
		char bin[MAX];
		fgets(bin, sizeof(bin), infile);
	}

	// ~~~~~~~~~~ work through all the queries ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	for (int i = 0; i < queryCount; i++) {
		char command[MAX], treeName[MAX], param[MAX];
		fscanf(infile, "%s %s", &command, &treeName);

		if (strcmp(command, "search") == 0) {
			fscanf(infile, "%s", &param);
			treeNameNode* nodeReff = searchNameNode(root, treeName);

			if (nodeReff == NULL) {
				std::cout << treeName << " does not exist" << std::endl;
				fprintf(outfile, "%s does not exist\n", treeName);
			}
			else search(nodeReff, nodeReff->theTree, param);
		}

		else if (strcmp(command, "item_before") == 0) {
			fscanf(infile, "%s", &param);
			int res = itemBefore(searchNameNode(root, treeName)->theTree, param);
			std::cout << "item before " << param << ": " << res << std::endl;
			fprintf(outfile, "item before %s: %d\n", param, res);
		}

		else if (strcmp(command, "height_balance") == 0) {
			heightBal(searchNameNode(root, treeName));
		}

		else if (strcmp(command, "count") == 0) {
			int sum = count(searchNameNode(root, treeName)->theTree);
			std::cout << treeName << " count " << sum << std::endl;
			fprintf(outfile, "%s count %d\n", treeName, sum);
		}
	}

	fclose(infile);
	fclose(outfile);
}