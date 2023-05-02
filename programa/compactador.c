#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct huffman{
	unsigned char caractere;
	long int freq;
	struct huffman *next;
	struct huffman *left;
	struct huffman *right;
}node;

typedef struct hash_table{
	long h;
	unsigned short map;
}ht;

ht *caracteres[256];

/* adiciona um caractere a uma lista encadeada de nós, que será usada posteriormente na 
criação da árvore. Se o caractere já existe na lista, sua frequência é incrementada em uma unidade
Caso contrário, um novo nó é criado e adicionado ao início da lista.*/

node *add(node *head, unsigned char caractere){
	node *aux = head, *new;
	while(aux != NULL && aux->caractere != caractere){
		aux = aux->next;
	}
	if(aux == NULL){
		new = malloc(sizeof(node));
		new->caractere = caractere;
		new->freq = 1;
		new->next = head;
		new->left = NULL;
		new->right = NULL;
		return new;
	} else {
		aux->freq++;
		return head;
	}
}

/*r um novo nó a partir de dois nós com as menores frequências. 
Esse novo nó é inserido na lista encadeada em ordem crescente de frequência.*/

node *add_arvore(node *head){
	node *new_head = head->next->next, *new;
	new = malloc(sizeof(node));
	new->freq = head->freq + head->next->freq;
	new->left = head;
	new->right = head->next;
	new->left->next = NULL;
	new->right->next = NULL;
	new->caractere = '*';
	new->next = NULL;

	if(new_head == NULL) return new;
	else{
		node *aux = new_head, *anterior = NULL;
		while(aux != NULL && aux->freq < new->freq){
			anterior = aux;
			aux = aux->next;
		}
		if(anterior == NULL){
			new->next = new_head;
			return new;
		} else {
			anterior->next = new;
			new->next = aux;
			return new_head;
		}
	}
}

//ordena a lista encadeada de forma crescente em mergesort
node* merge(node* left, node* right) {
    node* result = NULL;
    if (left == NULL)
        return right;
    else if (right == NULL)
        return left;
    if (left->freq <= right->freq) {
        result = left;
        result->next = merge(left->next, right);
    } else {
        result = right;
        result->next = merge(left, right->next);
    }
    return result;
}

node* ordenar(node* head) {
    if (head == NULL || head->next == NULL)
        return head;
    node* slow = head;
    node* fast = head->next;
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    node* left = head;
    node* right = slow->next;
    slow->next = NULL;
    left = ordenar(left);
    right = ordenar(right);
    return merge(left, right);
}

/* responsável por imprimir a árvore de Huffman em um arquivo de saída. 
Ela percorre a árvore em pré-ordem e, para cada nó folha, imprime seu caractere correspondente.
Quando o caractere é um asterisco ou uma barra invertida, a função imprime uma barra invertida 
antes dele. */

void printar(node *head, FILE *new){
	if(head != NULL){
		if(head->right == NULL && head->left == NULL && (head->caractere == '*' || head->caractere == '\\')){
			fprintf(new, "\\");
		}
		fprintf(new,"%c",head->caractere);
		printar(head->left,new);
		printar(head->right,new);
	}
}

/* inicializa um vetor de ponteiros para a tabela de hash que 
será utilizada posteriormente na compressão.*/
void cria_ht(){
	int i;
	for(i = 0; i < 256; i++){
		caracteres[i] = NULL;
	}
}

/*percorre a árvore em pré-ordem e preenche a tabela de hash com os valores 
correspondentes a cada caractere. Ela retorna o número de nós folha da árvore.*/
int map_arvore(node *head, int height, unsigned short direction, int cont){
    if(head == NULL) return 0;
	if(head != NULL){
		if(head->right == NULL && head->left == NULL && (head->caractere == '\\' || head->caractere == '*')) cont++;
		if(head->right == NULL && head->left == NULL){
			int i = (int)head->caractere;
			caracteres[i] = malloc(sizeof(ht));
			caracteres[i]->h = height;
			caracteres[i]->map = direction;
		}
		cont = 1 + map_arvore(head->left, height+1, direction<<1, cont);
		cont = cont + map_arvore(head->right, height+1, (direction<<1)+1, cont);
	}
    return cont;
}

/* cria um arquivo de saída contendo a tabela de hash. Cada linha do arquivo contém um 
caractere, sua altura na árvore e seu código na árvore de Huffman.*/
void create_file_ht() {
    FILE *file = fopen("hash.txt", "w+b");
    if (file == NULL) {
        fprintf(stderr, "Error opening file ¯\\_(ツ)_/¯");
        return;
    }

    int i;
    for (i = 0; i < 256; i++) {
        if (caracteres[i] != NULL) {
            fprintf(file, "%c %d %d\n", i, caracteres[i]->h, caracteres[i]->map);
        }
    }

    fclose(file);
}


struct new_map{
    unsigned char byte;
    struct new_map *next;
};
typedef struct new_map map;

long bit_is_set(unsigned short c, int i){
    unsigned char comp = 1;
    c = c>>i;
    return comp & c;
}

map *add_byte(map *tail){
    map *new = malloc(sizeof(map));
    tail->next = new;
    new->next = NULL;
    return new;
}
unsigned char set_bit(unsigned char c){
	unsigned char comp = 1;
    c = c<<1;
    return c|comp;
}

/* é responsável por percorrer o arquivo de entrada, criar uma lista encadeada de bytes 
correspondentes ao arquivo comprimido e preencher a tabela de hash. A cada caractere lido, 
a função verifica seu código na tabela de hash e adiciona os bits correspondentes à lista 
encadeada de bytes.*/
long map_b(char s[], map *map_head){
    FILE *file = fopen(s,"rb");

    map *tail = map_head;

    long int cont = 0, pos, h, i;

    unsigned char c;
    while(fscanf(file,"%c", &c) != EOF){
        pos = (long int)c;
        h = caracteres[pos]->h;
        for(i = h-1 ; i >= 0 ; i--){
			if(cont == 8){
				tail = add_byte(tail);
				cont = 0;
			}
			if(bit_is_set(caracteres[pos]->map,i)){
				tail->byte = set_bit(tail->byte);
			} else {
				tail->byte = tail->byte<<1;
			}
			cont ++;
		}
    }
	tail->byte = tail->byte<<(8-cont);
    fclose(file);
    return 8-cont;
}

unsigned short trash(unsigned short lixo){
	lixo = lixo<<12;
	return lixo;
}


unsigned short tree(unsigned short inf, unsigned short tam){
	return inf|tam;
}


int main(){
	unsigned char caractere;
    char s[100];
	node *head = NULL;
	unsigned short tam_arv, lixo;
	map *map_head = malloc(sizeof(map));
	map_head->next = NULL;

    FILE *file,*new;
	printf("Nome do Arquivo que sera compactado:\n");
    scanf(" %[^\n]",s);
    file = fopen(s,"r+b");
	
	if (file == NULL){
		system("cls || clear");
		printf("\n\n\n\n\n            Arquivo nao encontrado. :( \n\n\n\n\n");
	}
	else{
        while(fscanf(file,"%c",&caractere) != EOF){
            head = add(head,caractere);
        }
    }
	
	
    fclose(file);
	head = ordenar(head);
    
	while(head->next != NULL){
        head = add_arvore(head);
    }
    cria_ht();

    tam_arv = map_arvore(head,0,0,0);
	lixo = map_b(s,map_head);
	char n[100];
	printf("Digite o nome do arquivo.huff :\n");
	scanf(" %s",n);
	new = fopen(n,"wb");

	if(new != NULL){
		unsigned long int info = trash(lixo);
		info = tree(info,tam_arv);
		unsigned char first_byte, second_byte;
		first_byte = (unsigned char)(info>>7);
		second_byte = (unsigned char)info;
		fprintf(file,"%c%c",first_byte,second_byte);
		printar(head, new);
		while(map_head != NULL){
			fprintf(file,"%c",map_head->byte);
			map_head = map_head->next;
		}
	}
    fclose(new);
	system("cls || clear");
	printf("\n\n\n\n\n            Arquivo compactado com sucesso. :)\n\n\n\n\n. "); 
	return 0;
}
