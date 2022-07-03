#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "node.h"
#include "heap.h"

// 허프만 트리를 순회하며 허프만 코드를 생성하여 codes에 저장
// leaf 노드에서만 코드를 생성
// strdup 함수 사용
// make_huffman_code 함수에서 호출
static void traverse_tree( tNode *root, char *code, int depth, char *codes[]){
	tNode* left = root->left;
	tNode* right = root->right;

	if (left == NULL && right == NULL){
		char temp[256] = {0, };
		for (int i = 0; i < depth; i++) temp[i] = code[i];
		// printf("%c, %d, %s\n", root->data, root->data, temp);
		codes[root->data] = strdup(temp);
	}

	if (left != NULL){
		code[depth] = '0';
		traverse_tree(left, code, depth+1, codes);
	}

	if (right != NULL){
		code[depth] = '1';
		traverse_tree(right, code, depth+1, codes);
	}
}

// 새로운 노드를 생성
// 좌/우 subtree가 NULL(0)이고 문자(data)와 빈도값(freq)이 저장됨
// make_huffman_tree 함수에서 호출
// return value : 노드의 포인터
static tNode *newNode(unsigned char data, int freq){
	tNode *temp = malloc(sizeof(tNode));
	temp->data = data;
	temp->freq = freq;
	temp->left = NULL;
	temp->right = NULL;
	return temp;
}

////////////////////////////////////////////////////////////////////////////////
// 허프만 코드를 화면에 출력
void print_huffman_code( char *codes[])
{
	int i;
	
	for (i = 0; i < 256; i++)
	{
		printf( "%d\t%s\n", i, codes[i]);
	}
}

////////////////////////////////////////////////////////////////////////////////
// 허프만 트리로부터 허프만 코드를 생성
// traverse_tree 함수 호출
void make_huffman_code( tNode *root, char *codes[])
{
	char code[256];
	
	traverse_tree( root, code, 0, codes);
}

////////////////////////////////////////////////////////////////////////////////
// 파일에 속한 각 문자(바이트)의 빈도 저장
// return value : 파일에서 읽은 바이트 수
int read_chars( FILE *fp, int ch_freq[]){
    // 문자별 빈도
    int buffer;
    while (1) {
        buffer = fgetc(fp);
		if (feof(fp)) break;
        ch_freq[buffer] += 1;
    }
    // 파일 바이트 수 리턴
    fseek(fp, 0, SEEK_END);
    return ftell(fp);
}

////////////////////////////////////////////////////////////////////////////////
// 파일로부터 문자별 빈도(256개)를 읽어서 ch_freq에 저장
void get_char_freq( FILE *fp, int ch_freq[]){
    fread(ch_freq, sizeof(int), 256, fp);
}

////////////////////////////////////////////////////////////////////////////////
// 허프만 코드에 대한 메모리 해제
void free_huffman_code( char *codes[]){
	for (int i = 0; i < 256; i++){
		free(codes[i]);
	}
}

////////////////////////////////////////////////////////////////////////////////
// 허프만 트리를 생성
// 1. capacity 256 짜리 빈(empty) 힙 생성 // HEAP *heap = heapCreate( 256);
// 2. 개별 알파벳에 대한 노드 생성
// 3. 힙에 삽입 (minheap 구성) // heapInsert( heap, node);
// 4. 2개의 최소값을 갖는 트리 추출 // tNode *left = heapDelete( heap); tNode *right = heapDelete( heap);
// 5. 두 트리를 결합 후 새 노드에 추가
// 6. 새 트리를 힙에 삽입
// 7. 힙에 한개의 노드가 남을 때까지 반복
// return value: 트리의 root 노드의 포인터
tNode *make_huffman_tree( int ch_freq[]){
	HEAP *heap = heapCreate(256);

	for (int i = 0; i < 256; i++){
		tNode *newnode = newNode(i, ch_freq[i]);
		heapInsert(heap, newnode);
	}

	while (heap->last > 0) {
		tNode *left = heapDelete(heap);
		tNode *right = heapDelete(heap);
		// printf("heapDelete: %d, %d\n", left->data, right->data);
		tNode *group = newNode(0, left->freq + right->freq);
		group->left = left;
		group->right = right;
		heapInsert(heap, group);
	}

	tNode *ret = heapDelete(heap);
	heapDestroy(heap);
	return ret;
	
}

////////////////////////////////////////////////////////////////////////////////
// 허프만 트리 메모리 해제
void destroyTree( tNode *root){
	//Postorder traverse
	tNode* left = root->left;
	tNode* right = root->right;
	
	if (left != NULL) destroyTree(left);
	if (right != NULL) destroyTree(right);
	free(root);
}

////////////////////////////////////////////////////////////////////////////////
// 입력 텍스트 파일(infp)을 허프만 코드를 이용하여 출력 파일(outfp)로 인코딩
// return value : 인코딩된 텍스트의 바이트 수 (파일 크기와는 다름)
int encoding( char *codes[], int ch_freq[], FILE *infp, FILE *outfp){
	// 문자별 빈도를 출력 파일에 저장
	fwrite(ch_freq, sizeof(int), 256, outfp);

	// 1바이트 마다 허프만 코드를 파일에 작성
	int compressed_byte = 0;
	int buffer;
	char printch = 0;
	int bit = 0;
    while (1) {
		// 문자 입력
        buffer = fgetc(infp);
		//printf("buffer: %c(%d)\n", buffer, buffer);
		if (feof(infp)) break;
		// 문자에 대응되는 허프만 코드 저장
		char code[256];
		strcpy(code, codes[buffer]);
		// 8비트 마다 파일에 기록
		for (int i = 0; i < strlen(code); i++){
			printch <<= 1;
			if (code[i] == '1') printch |= 1;
			bit++;
			if (bit == 8){
				fputc(printch, outfp);
				printch = 0;
				bit = 0;
				compressed_byte++;
			}
		}
    }
	// 비트가 남았을 경우
	if (bit) {
		printch <<= 8 - bit;
		fputc(printch, outfp);
		compressed_byte++;
	}

	// 파일 맨 끝에 읽어야 할 비트 수 저장
	fwrite(&bit, sizeof(int), 1, outfp);
	
	return compressed_byte;
}

////////////////////////////////////////////////////////////////////////////////
// 입력 파일(infp)을 허프만 트리를 이용하여 텍스트 파일(outfp)로 디코딩
void decoding( tNode *root, FILE *infp, FILE *outfp){
	// 파일 맨 끝에서 읽어야 할 비트 수 불러오기
	fseek(infp, -sizeof(int), SEEK_END);
	int end_bit;
	fread(&end_bit, sizeof(int), 1, infp);
	if (end_bit == 0) end_bit = 8;

	// 입력 파일 사이즈 계산
	fseek(infp, 0, SEEK_END);
	int size_infp = ftell(infp) - sizeof(int)*257;
	
	// 입력 파일 처음으로 다시 돌려오기
	fseek(infp, sizeof(int) * 256, SEEK_SET);

	// 허프만 트리 순회하며 파일 출력
	tNode *ptr = root;
	char byte;
	int bit;
	for (int j = 0; j < size_infp; j++){
		fread(&byte, sizeof(char), 1, infp);
		
		int maxbit = 8;
		// 맨 마지막에는 맨 끝에서 읽어야 할 비트 수 만큼만 반복
		if (j == size_infp-1) maxbit = end_bit;

		for (int i = 0; i < maxbit; i++){
			// 비트 추출
			bit = byte & (1 << (7-i));
			// 트리 순회
			if (bit) ptr = ptr->right;
			else ptr = ptr->left;
			// 리프 노드에 도착했을 경우
			if (ptr->left == NULL && ptr->right == NULL){
				fwrite(&(ptr->data), sizeof(char), 1, outfp);
				ptr = root;
			}
		}
	}
}