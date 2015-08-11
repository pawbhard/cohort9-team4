#include "../include/multicast_db.h"

multicast_group* find_multicast_group (multicast_group* head, ipv4_address group) {
    multicast_group* cur = head;
    while (cur) {
        if (compare_ipv4_address(cur->group_addr, group)) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

int add_multicast_group (multicast_group* head, ipv4_address group) {
    multicast_group* cur = head;
    multicast_group* node = (multicast_group*)malloc(sizeof(multicast_group));
    if (!node) {
        return FALSE;
    }
    node->next = NULL;
    node->client_list = NULL;
    node->group_addr = group;
    if (!cur) {
        cur = node;
        return TRUE;
    }
    while (cur->next) {
        cur = cur->next;
    }
    cur->next = node;
    return TRUE;
}

int remove_client_list (client_node* head) {
    client_node* cur;
    client_node* node;
    if (!head) {
        return FALSE;
    }
    cur = head;
    while (cur) {
        node = cur;
        cur = cur->next;
        node->next = NULL;
        free (node);
    }
    return TRUE;
}

int remove_multicast_group (multicast_group* head, ipv4_address group) {
    multicast_group* cur = find_multicast_group (head, group);
    multicast_group* prev = head;
    multicast_group* node;
    
    if (!cur) {
        return FALSE;
    }
    if (cur == head) {
        head = head->next;
        remove_client_list (cur->client_list);
        free (cur);
        return TRUE;        
    }
    while (prev->next != cur) {
        prev = prev->next;
    }
    node = cur;
    prev->next = cur->next;
    node->next = NULL;
    remove_client_list (node->client_list);
    free (node);
    return TRUE;
}

int add_client_to_multicast_group (multicast_group* head, ipv4_address group, ipv4_address client, int id, char* message) {
    multicast_group* cur_group = find_multicast_group (head, group);
    client_node* cur_client;
    client_node* node;
    client_node* check;
    if (!cur_group) {
        return FALSE;
    }
    cur_client = cur_group->client_list; 
    check = cur_client;
    while (check) {
        if (compare_ipv4_address(check->client_addr, client)) {
            return add_message_to_list (check->msg, message);
        }
        check = check->next;
    }
    
    node = (client_node*)malloc(sizeof(client_node));
    if (!node) {
        return FALSE;
    }
    node->client_addr = client;
    node->id = id;
    add_message_to_list (node->msg, message);
    node->next = NULL;

    if (!cur_client) {
        cur_client = node;
        return TRUE;
    }
    while (cur_client->next) {
        cur_client = cur_client->next;
    }
    cur_client->next = node;
    return TRUE;
}

int remove_client_from_multicast_group (multicast_group* head, ipv4_address group, int id) {
    multicast_group* cur_group = find_multicast_group (head, group);
    client_node* cur_client;
    client_node* prev_client;
    client_node* node;
    if (!cur_group) {
        return FALSE;
    }
    prev_client = cur_group->client_list;
    if (!prev_client) {
        return FALSE;
    }
    if (prev_client->id == id) {
        node = prev_client;
        prev_client = prev_client->next;
        node->next = NULL;
        free (node);
        return TRUE;
    }
    cur_client = prev_client->next;
    while (cur_client) {
        if (cur_client->id == id) {
            node = cur_client;
            prev_client->next = cur_client->next;
            node->next = NULL;
            free (node);
            return TRUE;
        }
        prev_client = cur_client;
        cur_client = cur_client->next;
    }
    return FALSE;
}
