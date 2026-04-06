#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX 10

// Node structure
struct Node {
    int vertex;
    int weight;
    struct Node* next;
};

struct Node* graph[MAX];
int nodes;

// Create node
struct Node* createNode(int v, int w) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->vertex = v;
    newNode->weight = w;
    newNode->next = NULL;
    return newNode;
}

// Add edge
void addEdge(int u, int v, int w) {
    struct Node* n1 = createNode(v, w);
    n1->next = graph[u];
    graph[u] = n1;

    struct Node* n2 = createNode(u, w);
    n2->next = graph[v];
    graph[v] = n2;
}

// Dijkstra Algorithm
void dijkstra(int start, int dist[], int parent[]) {
    int visited[MAX] = {0};

    for (int i = 0; i < nodes; i++) {
        dist[i] = INT_MAX;
        parent[i] = -1;
    }

    dist[start] = 0;

    for (int count = 0; count < nodes - 1; count++) {
        int min = INT_MAX, u = -1;

        for (int i = 0; i < nodes; i++) {
            if (!visited[i] && dist[i] < min) {
                min = dist[i];
                u = i;
            }
        }

        if (u == -1) break;
        visited[u] = 1;

        struct Node* temp = graph[u];
        while (temp != NULL) {
            int v = temp->vertex;
            int w = temp->weight;

            if (!visited[v] && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
            }
            temp = temp->next;
        }
    }
}

// Print path
void printPath(int parent[], int j) {
    if (j == -1) return;
    printPath(parent, parent[j]);
    printf("%d ", j);
}

// Find nearest ambulance
int findNearest(int patient, int ambulances[], int count) {
    int dist[MAX], parent[MAX];
    dijkstra(patient, dist, parent);

    int min = INT_MAX, nearest = -1;

    for (int i = 0; i < count; i++) {
        int a = ambulances[i];
        if (dist[a] < min) {
            min = dist[a];
            nearest = a;
        }
    }

    printf("\nNearest Ambulance Node: %d\n", nearest);
    printf("Distance to Patient: %d\n", min);

    printf("Path (Ambulance -> Patient): ");
    printPath(parent, nearest);
    printf("\n");

    return nearest;
}

int main() {
    int edges;

    printf("Enter number of locations: ");
    scanf("%d", &nodes);

    for (int i = 0; i < MAX; i++)
        graph[i] = NULL;

    printf("Enter number of roads: ");
    scanf("%d", &edges);

    printf("Enter roads (u v distance):\n");
    for (int i = 0; i < edges; i++) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        addEdge(u, v, w);
    }

    int patient, hospital, ambCount, ambulances[MAX];

    printf("Enter patient location: ");
    scanf("%d", &patient);

    printf("Enter number of ambulances: ");
    scanf("%d", &ambCount);

    printf("Enter ambulance locations:\n");
    for (int i = 0; i < ambCount; i++) {
        scanf("%d", &ambulances[i]);
    }

    printf("Enter hospital location: ");
    scanf("%d", &hospital);

    // Find nearest ambulance
    int nearest = findNearest(patient, ambulances, ambCount);

    // Ambulance -> Patient
    int dist1[MAX], parent1[MAX];
    dijkstra(nearest, dist1, parent1);

    printf("\nRoute Ambulance -> Patient:\n");
    printPath(parent1, patient);
    printf("\nDistance: %d\n", dist1[patient]);

    // Patient -> Hospital
    int dist2[MAX], parent2[MAX];
    dijkstra(patient, dist2, parent2);

    printf("\nRoute Patient -> Hospital:\n");
    printPath(parent2, hospital);
    printf("\nDistance: %d\n", dist2[hospital]);

    printf("\nTotal Distance: %d\n", dist1[patient] + dist2[hospital]);

    return 0;
}