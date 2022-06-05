#include <stdlib.h> // atoi, rand, malloc, realloc
#include <stdio.h>
#include <time.h> //time

#define RANGE 10000

typedef struct
{
	int x;
	int y;
} t_point;

typedef struct
{
	t_point from;
	t_point to;
} t_line;

////////////////////////////////////////////////////////////////////////////////
void print_header( char *filename)
{
	printf( "#! /usr/bin/env Rscript\n");
	printf( "png(\"%s\", width=700, height=700)\n", filename);
	
	printf( "plot(1:%d, 1:%d, type=\"n\")\n", RANGE, RANGE);
}
////////////////////////////////////////////////////////////////////////////////
void print_footer( void)
{
	printf( "dev.off()\n");
}

////////////////////////////////////////////////////////////////////////////////
/*
#points
points(2247,7459)
points(616,2904)
points(5976,6539)
points(1246,8191)
*/
void print_points( t_point *points, int num_point);	//배열, 개수

/*
#line segments
segments(7107,2909,7107,2909)
segments(43,8,5,38)
segments(43,8,329,2)
segments(5047,8014,5047,8014)
*/
void print_line_segments( t_line *lines, int num_line);

// [input] points : set of points
// [input] num_point : number of points
// [output] num_line : number of line segments that forms the convex hull
// return value : set of line segments that forms the convex hull
t_line *convex_hull( t_point *points, int num_point, int *num_line); //num_line: 포인터 사용해야 함수 끝나도 저장 가능 / 배열로 리턴하기 위해 포인터로 리턴함.

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	int x, y;
	int num_point; // number of points
	int num_line; // number of lines
	
	if (argc != 2)
	{
		printf( "%s number_of_points\n", argv[0]);
		return 0;
	}

	num_point = atoi( argv[1]);
	if (num_point <= 0)
	{
		printf( "The number of points should be a positive integer!\n");
		return 0;
	}

	t_point *points = (t_point *) malloc( num_point * sizeof( t_point));
		
	t_line *lines;

	// making n points
	srand( time(NULL));
	for (int i = 0; i < num_point; i++)
	{
		x = rand() % RANGE + 1; // 1 ~ RANGE random number
		y = rand() % RANGE + 1;
		
		points[i].x = x;
		points[i].y = y;
 	}

	fprintf( stderr, "%d points created!\n", num_point);

	print_header( "convex.png");
	
	print_points( points, num_point);
	
	lines = convex_hull( points, num_point, &num_line);

	fprintf( stderr, "%d lines created!\n", num_line);

	print_line_segments( lines, num_line);
	
	print_footer();
	
	free( points);
	free( lines);
	
	//system("leaks a.out > leaks_result; cat leaks_result | grep leaked && rm -rf leaks_result");
	
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

void print_points( t_point *points, int num_point){
	for (int i = 0; i < num_point; i++){
		printf("points(%d,%d)\n", (points+i)->x, (points+i)->y);
	}
}

void print_line_segments( t_line *lines, int num_line){
	for (int i = 0; i < num_line; i++){
		printf("segments(%d,%d,%d,%d)\n", (lines+i)->from.x, (lines+i)->from.y, (lines+i)->to.x, (lines+i)->to.y);
	}
}

void copy_arr2to1(t_line *arr1, t_line *arr2, int num2){
	for (int i = 0; i < num2; i++){
		arr1->from.x = arr2->from.x;
		arr1->from.y = arr2->from.y;
		arr1->to.x = arr2->to.x;
		arr1->to.y = arr2->to.y;
		arr1++, arr2++;
	}
}

t_line *convex_hull( t_point *points, int num_point, int *num_line){
	int lineCapacity = 10;
	*num_line = 0;
	t_line *lines = (t_line *)malloc(lineCapacity*sizeof(t_line));
	int pre = 0;
	int curr;
	for (int i = 0; i < num_point; i++){
		int x1 = (points+i)->x, y1 = (points+i)->y;
		for (int j = i+1; j < num_point; j++){
			int x2 = (points+j)->x, y2 = (points+j)->y;
			int coef[3] = {y2-y1, x1-x2, x1*y2-y1*x2};
			curr = 0;
			for (int k = 0; k < num_point; k++){
				if (k==i || k==j) continue;
				pre = curr;
				int flag = coef[0]*(points+k)->x + coef[1]*(points+k)->y - coef[2];
				if (flag > 0) curr = 1;
				else if (flag < 0) curr = -1;
				else curr = 0;
				if (pre*curr < 0) {
					curr = -2;
					break;
				}
			}
			if (curr != -2) {
				if (*num_line >= lineCapacity){
					lineCapacity *= 2;
					t_line *newlines = (t_line *)malloc(lineCapacity*sizeof(t_line));
					copy_arr2to1(newlines, lines, *num_line);
					free(lines);
					lines = newlines;
				}
				lines[*num_line].from.x = x1; 
				lines[*num_line].from.y = y1; 
				lines[*num_line].to.x = x2; 
				lines[*num_line].to.y = y2;
				*num_line += 1;
			}
		}
	}

	return lines;
}
