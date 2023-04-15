#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 500

typedef struct Node {
    char* name;
    char* company;
    char* address;
    char* zip;
    char* phone;
    char* email;
    struct Node* left;
    struct Node* right;
} Node;

Node* root = NULL; // ����Ž��Ʈ���� ��Ʈ ���


// ����Ž��Ʈ������ ���ο� ��带 �߰��ϴ� �Լ�
Node* insertNode(Node* node, char* name, char* company, char* address, char* zip, char* phone, char* email) {
    if (node == NULL) { // ��尡 NULL�� ��� ���ο� ��带 �����ϰ� ��ȯ
        Node* new_node = (Node*)malloc(sizeof(Node));
        new_node->name = _strdup(name);
        new_node->company = _strdup(company);
        new_node->address = _strdup(address);
        new_node->zip = _strdup(zip);
        new_node->phone = _strdup(phone);
        new_node->email = _strdup(email);
        new_node->left = NULL;
        new_node->right = NULL;
        return new_node;
    }

    // ��尡 NULL�� �ƴ� ��� ����Ž��Ʈ���� ������ ���� ������ ��ġ�� ����
    int cmp = strcmp(name, node->name);
    if (cmp < 0) node->left = insertNode(node->left, name, company, address, zip, phone, email);
    else if (cmp > 0) node->right = insertNode(node->right, name, company, address, zip, phone, email);
    return node;
}

Node* searchNode(Node* root, const char* name) {
    // ��Ʈ�� NULL�̰ų� ã�� ��带 ã���� ���
    if (root == NULL || strcmp(root->name, name) == 0) {
        return root;
    }
    // ã�� ��尡 ��Ʈ���� ���� ��� ���� ����Ʈ�� Ž��
    if (strcmp(name, root->name) < 0) {
        return searchNode(root->left, name);
    }
    // ã�� ��尡 ��Ʈ���� ū ��� ������ ����Ʈ�� Ž��
    else {
        return searchNode(root->right, name);
    }
}

Node* traceNode(Node* root, const char* name) {

    // ��Ʈ�� NULL�̰ų� ã�� ��带 ã���� ���
    if (root == NULL || strcmp(root->name, name) == 0) {
        return root;
    }
    printf("%s\n", root->name);
    // ã�� ��尡 ��Ʈ���� ���� ��� ���� ����Ʈ�� Ž��
    if (strcmp(name, root->name) < 0) {
        return traceNode(root->left, name);
    }
    // ã�� ��尡 ��Ʈ���� ū ��� ������ ����Ʈ�� Ž��
    else {
        return traceNode(root->right, name);
    }
}

Node* findMinNode(Node* node) {
    if (node == NULL) {
        return NULL;
    }
    else if (node->left == NULL) {
        return node;
    }
    else {
        return findMinNode(node->left);
    }
}

Node* deleteNode(Node* node, const char* name) {
    if (node == NULL) {
        return NULL;
    }

    if (strcmp(node->name, name) == 0) {
        if (node->left == NULL && node->right == NULL) {
            free(node->name);
            free(node->company);
            free(node->address);
            free(node->zip);
            free(node->phone);
            free(node->email);
            free(node);
            return NULL;
        }
        if (node->left == NULL) {
            Node* rightChild = node->right;
            free(node->name);
            free(node->company);
            free(node->address);
            free(node->zip);
            free(node->phone);
            free(node->email);
            free(node);
            return rightChild;
        }
        if (node->right == NULL) {
            Node* leftChild = node->left;
            free(node->name);
            free(node->company);
            free(node->address);
            free(node->zip);
            free(node->phone);
            free(node->email);
            free(node);
            return leftChild;
        }

        Node* minRightNode = findMinNode(node->right);
        strcpy(node->name, minRightNode->name);
        node->right = deleteNode(node->right, minRightNode->name);
    }
    else if (strcmp(name, node->name) < 0) {
        node->left = deleteNode(node->left, name);
    }
    else {
        node->right = deleteNode(node->right, name);
    }

    return node;
}


void readFromFile(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: Failed to open file %s\n", filename);
        return;
    }
    char line[MAX];
    fgets(line, MAX, fp); // ù ���� �ʵ� �̸��̹Ƿ� �о ������
    while (fgets(line, MAX, fp) != NULL) {
        char* name = strtok(line, "\t\n");
        char* company = strtok(NULL, "\t\n");
        char* address = strtok(NULL, "\t\n");
        char* zip = strtok(NULL, "\t\n");
        char* phone = strtok(NULL, "\t\n");
        char* email = strtok(NULL, "\t\n");
        root = insertNode(root, name, company, address, zip, phone, email);
    }
    fclose(fp);
}

void printInorder(Node* node) {
    if (node == NULL) {
        return;
    }
    printInorder(node->left);
    printf("%s\n\tCompany: %s\n\tAddress: %s\n\tZipcode: %s\n\tPhones: %s\n\tEmail: %s\n", node->name, node->company, node->address, node->zip, node->phone, node->email);
    printInorder(node->right);
}
void save_helper(Node* root, FILE* fp) {
    if (root == NULL) {
        return;
    }

    save_helper(root->left, fp);

    fprintf(fp, "%s\t%s\t%s\t%s\t%s\t%s\n", root->name, root->company, root->address,
        root->zip, root->phone, root->email);

    save_helper(root->right, fp);
}

void save(Node* root, char* filename) {
    FILE* fp = fopen(filename, "w");

    if (fp == NULL) {
        printf("Failed to open file for saving.\n");
        return;
    }

    fprintf(fp, "name\tcompany\taddress\tzip\tphone\temail\n");

    save_helper(root, fp);

    fclose(fp);
}



int main(void) {

    char input[50];
    char* token;
    while (1)
    {
        printf("$ ");
        gets_s(input, 50);
        token = strtok(input, " ");

        if (strcmp(input, "exit") == 0)//����
            exit(0);

        else if (strcmp(input, "read") == 0)//����
            readFromFile("address_book2020.tsv");

        else if (strcmp(input, "list") == 0) // Ʈ���� ���� ��ȸ�Ͽ� ��� ��� ���(����)
            printInorder(root);

        else if (strcmp(input, "find") == 0)// Ʈ������ �Է��� �̸� �˻�(����)
        {
            Node* q;
            token = strtok(NULL, " ");
            q = searchNode(root, token);
            printf("%s\n\tCompany: %s\n\tAddress: %s\n\tZipcode: %s\n\tPhones: %s\n\tEmail: %s\n", q->name, q->company, q->address, q->zip, q->phone, q->email);
        }

        else if (strcmp(input, "add") == 0) // ����
        {
            char company[50], address[50], zip[50], phone[50], email[50];
            printf("Company? ");
            scanf("%s", company);
            getchar();
            printf("Address? ");
            scanf("%s", address);
            getchar();
            printf("Zipcode? ");
            scanf("%s", zip);
            getchar();
            printf("Phones? ");
            scanf("%s", phone);
            getchar();
            printf("Email? ");
            scanf("%s", email);
            getchar();
            token = strtok(NULL, " ");
            insertNode(root, token, company, address, zip, phone, email);
            printf("%s was successfully added.\n", token);
        }

        else if (strcmp(input, "trace") == 0)
        {
            token = strtok(NULL, " ");
            traceNode(root, token);
        }

        else if (strcmp(input, "delete") == 0)// Ʈ������ token�̸� ����(����)
        {
            token = strtok(NULL, " ");
            deleteNode(root, token);
        }

        else if (strcmp(input, "save") == 0)
        {
            token = strtok(NULL, " ");
            save(root, token);
        }

    }

    return 0;
}