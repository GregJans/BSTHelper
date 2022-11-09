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
void inorder(T* root, static char[]);

treeNameNode* searchNameNode(treeNameNode* root, char treeName[MAX]);
void traverse_in_traverse(treeNameNode* root);
treeNameNode* buildNameTree(FILE* infile, int treeNameCount, int itemCount);
void handleQueries(FILE* infile, treeNameNode* root, int queryCount);

bool search(treeNameNode* tree, itemNode* root, char name[]);
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


treeNameNode* buildNameTree(FILE* infile, int treeNameCount, int itemCount) {
	
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
	return tempRoot;
}


template<typename T>
void inorder(T* root, static char result[]) {
	if (root != NULL) {
		inorder(root->left, result);
		strcat(result, root->name);
		strcat(result, " ");
		inorder(root->right, result);
	}
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


void traverse_in_traverse(treeNameNode* root) {

	if (root != NULL) {
		traverse_in_traverse(root->left);
		// print the name
		std::cout << "**" << root->name << "**" << std::endl;
		fprintf(outfile, "**%s**\n", root->name);

		//print the tree
		static char result[MAX];
		strcpy(result, "");
		inorder<itemNode>(root->theTree, result);
		std::cout << result << std::endl;
		fprintf(outfile, "%s\n", result);

		traverse_in_traverse(root->right);
	}
}


bool search(treeNameNode* tree, itemNode* root, char name[]) {
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


int itemBefore(itemNode* root, char name[]) {
	int sum = 0;

	if (root != NULL) {
		sum += itemBefore(root->left, name);

		if (strcmp(name, root->name) == 0) 
			return sum;
		else if (strcmp(name, root->name) > 0)
			sum++;

		sum += itemBefore(root->right, name);
	}
	return sum;
}


int height(itemNode* node) {
	if (node == NULL) return 0;
	else
		return 1 + std::max(height(node->left), height(node->right));
}


void heightBal(treeNameNode* root) {
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


int count(itemNode* root) {
	if (root == NULL)
		return 0;
	else
		return count(root->left) + count(root->right) + root->count;
}


void handleQueries(FILE* infile, treeNameNode* root, int queryCount) {
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
}


int main() {
	
	outfile = fopen("out.txt", "w");
	FILE* infile = fopen("in.txt", "r");
	if (infile == NULL)
		std::cout << "Error opening file" << std::endl;

	int treeNameCount, itemCount, queryCount;
	fscanf(infile, "%d %d %d", &treeNameCount, &itemCount, &queryCount);

	treeNameNode *root = buildNameTree(infile, treeNameCount, itemCount);

	static char nameTree[MAX];
	inorder<treeNameNode>(root, nameTree);
	std::cout << nameTree << std::endl;
	fprintf(outfile, "%s\n", nameTree);

	traverse_in_traverse(root);

	handleQueries(infile, root, queryCount);
	

	fclose(infile);
	fclose(outfile);
}
