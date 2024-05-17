#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Product {
    int product_id;
    char name[100];
    char category[50];
    float price;
    int quantity;
} Product;

typedef struct TreeNode {
    Product product;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

typedef struct ProductInventory {
    TreeNode* root;
} ProductInventory;

typedef struct Order {
    int order_id;
    char customer_name[100];
    Product* products;
    int num_products;
    float total_price;
} Order;

typedef struct StackNode {
    Order order;
    struct StackNode* next;
} StackNode;

typedef struct OrderStack {
    StackNode* top;
} OrderStack;

ProductInventory inventory = { .root = NULL };
OrderStack stack = { .top = NULL };

TreeNode* create_tree_node(Product product);
void add_product_to_tree(TreeNode** root, Product product);
Product* find_product_by_id(TreeNode* root, int product_id);
void inorder_traversal(TreeNode* root);
void push_order(Order order, OrderStack* stack);
Order pop_order(OrderStack* stack);
int is_empty(OrderStack* stack);
void process_order(Order order, ProductInventory* inventory, OrderStack* stack);
void ship_orders(OrderStack* stack);
void input_product_details(Product *product);
void input_order_details(Order *order);

int main() {

    int num_products,choice;

    Order order;
    
    do{
        printf("\n----------PRODUCT FULFILLMENT----------\n");
        printf("1. Add details about product \n2. Place an order\n3. Check inventory\n4. Exit\n" );
        printf("Enter your choice: ");
        scanf("%d",&choice);

        switch(choice){
            case 1:
                printf("\n----------ADD PRODUCT DETAILS----------\n");
                printf("Enter the number of products: ");
                scanf("%d", &num_products);

                printf("Enter product details for each product:\n");
                for (int i = 0; i < num_products; i++) {
                    printf("Product %d:\n", i + 1);
                    Product product;
                    input_product_details(&product);
                    add_product_to_tree(&(inventory.root), product);
                }
                break;
            case 2:
                printf("\n----------PLACE AN ORDER----------\n");
                input_order_details(&order);
                process_order(order, &inventory, &stack);
                ship_orders(&stack);
                break;
            case 3:
            printf("\n----------THE INVENTORY----------\n");
                inorder_traversal(inventory.root);
                break;
            case 4:
                break;
            default:
                printf("\nInvalid choice!!");
                break;    
        }
    }while(choice != 4);

    return 0;
}

// Create a new tree node
TreeNode* create_tree_node(Product product) {
    TreeNode* new_node = (TreeNode*)malloc(sizeof(TreeNode));
    if (new_node != NULL) {
        new_node->product = product;
        new_node->left = NULL;
        new_node->right = NULL;
    }
    return new_node;
}

// Add a product to the binary search tree
void add_product_to_tree(TreeNode** root, Product product) {
    if (*root == NULL) {
        *root = create_tree_node(product);
    } else {
        if (product.product_id < (*root)->product.product_id) {
            add_product_to_tree(&((*root)->left), product);
        } else {
            add_product_to_tree(&((*root)->right), product);
        }
    }
}

// Find a product by ID in the binary search tree
Product* find_product_by_id(TreeNode* root, int product_id) {
    if (root == NULL) {
        return NULL;
    } else if (root->product.product_id == product_id) {
        return &(root->product);
    } else if (product_id < root->product.product_id) {
        return find_product_by_id(root->left, product_id);
    } else {
        return find_product_by_id(root->right, product_id);
    }
}

// Function to perform an in-order traversal of the binary search tree
void inorder_traversal(TreeNode* root) {
    if (root != NULL) {
        inorder_traversal(root->left);
        printf("Product ID: %d, Name: %s, Category: %s, Price: %.2f, Quantity: %d\n",
               root->product.product_id, root->product.name, root->product.category,
               root->product.price, root->product.quantity);
        inorder_traversal(root->right);
    }else{
        // printf("\nThe inventory is empty!\n");
    }
}

// Push an order onto the order stack
void push_order(Order order, OrderStack* stack) {
    StackNode* new_node = (StackNode*)malloc(sizeof(StackNode));
    if (new_node != NULL) {
        new_node->order = order;
        new_node->next = stack->top;
        stack->top = new_node;
    }
}

// Pop an order from the order stack
Order pop_order(OrderStack* stack) {
    Order order;
    if (stack->top != NULL) {
        StackNode* temp = stack->top;
        order = temp->order;
        stack->top = temp->next;
        free(temp);
    }
    return order;
}

// Check if the order stack is empty
int is_empty(OrderStack* stack) {
    return stack->top == NULL;
}

// Process an order
void process_order(Order order, ProductInventory* inventory, OrderStack* stack) {
    float total_price = 0;
    for (int i = 0; i < order.num_products; i++) {
        Product* product = find_product_by_id(inventory->root, order.products[i].product_id);
        if (product != NULL && product->quantity >= order.products[i].quantity) {
            product->quantity -= order.products[i].quantity;
            total_price += order.products[i].price * order.products[i].quantity;
        } else {
            printf("Product with ID %d not found or insufficient quantity\n", order.products[i].product_id);
            return;
        }
    }
    order.total_price = total_price;
    push_order(order, stack);
}

// Ship orders
void ship_orders(OrderStack* stack) {
    while (!is_empty(stack)) {
        Order order = pop_order(stack);
        printf("Shipping order %d to %s\n", order.order_id, order.customer_name);
        // Additional shipping logic can be added here
    }
}

// Function to input product details
void input_product_details(Product *product) {

    bool valid_input = false;

    do {
        printf("ID: ");
        if (scanf("%d", &(product->product_id)) == 0) {
            printf("Error: Invalid input. Please enter a valid integer for ID.\n");
            // Clear input buffer
            while (getchar() != '\n');
        } else {
            valid_input = true;
        }
    } while (!valid_input);
    printf("Name: ");
    scanf("%s", product->name);
    printf("Category: ");
    scanf("%s", product->category);
    valid_input = false;
    do {
        printf("Price: ");
        if (scanf("%f", &(product->price)) == 0) {
            printf("Error: Invalid input. Please enter a valid floating-point number for Price.\n");
            // Clear input buffer
            while (getchar() != '\n');
        } else {
            valid_input = true;
        }
    } while (!valid_input);
    valid_input = false;
    do {
        printf("Quantity: ");
        if (scanf("%d", &(product->quantity)) == 0) {
            printf("Error: Invalid input. Please enter a valid integer for Quantity.\n");
            // Clear input buffer
            while (getchar() != '\n');
        } else {
            valid_input = true;
        }
    } while (!valid_input);
}

// Function to input order details
void input_order_details(Order *order) {
    printf("Order ID: ");
    scanf("%d", &(order->order_id));
    printf("Customer Name: ");
    scanf("%s", order->customer_name);
    printf("Number of products: ");
    scanf("%d", &(order->num_products));

    order->products = (Product*)malloc(order->num_products * sizeof(Product));
    if (order->products == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    for (int i = 0; i < order->num_products; i++) {
        printf("Product %d:\n", i + 1);
        input_product_details(&(order->products[i]));
    }
}
