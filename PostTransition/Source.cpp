#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STRING_LENGTH 6

struct package
{
	char* id;
	int weight;
};

typedef struct package package;

struct post_office
{
	int min_weight;
	int max_weight;
	package* packages;
	int packages_count;
};

typedef struct post_office post_office;

struct town
{
	char* name;
	post_office* offices;
	int offices_count;
};

typedef struct town town;



void print_all_packages(town t) {
	printf("%s:\n", t.name);
	int i, j;
	for (i = 0; i < t.offices_count; i++) {
		printf("\t%d:\n", i);
		for (j = 0; j < t.offices[i].packages_count; j++) {
			printf("\t\t%s\n", t.offices[i].packages[j].id);
		}
	}
}

void send_all_acceptable_packages(town* source, int source_office_index, town* target, int target_office_index) {
	int i, nsent = 0, w, min_w, max_w;
	package nullpack = { NULL, -1 };

	min_w = target->offices[target_office_index].min_weight;
	max_w = target->offices[target_office_index].max_weight;
	for (i = 0; i < source->offices[source_office_index].packages_count; i++) {
		w = source->offices[source_office_index].packages[i].weight;
		if (w >= min_w && w <= max_w) {
			target->offices[target_office_index].packages_count++;
			target->offices[target_office_index].packages = (package*)realloc(
				target->offices[target_office_index].packages,
				target->offices[target_office_index].packages_count * sizeof(package));

			target->offices[target_office_index].packages[
				target->offices[target_office_index].packages_count - 1] = source->offices[source_office_index].packages[i];
				source->offices[source_office_index].packages[i] = nullpack; /*Lazy delet*/

				nsent++;

		}
	}

	int j;
	source->offices[source_office_index].packages_count -= nsent;
	for (i = 0, j = 0; i < source->offices[source_office_index].packages_count; j++) {
		if (source->offices[source_office_index].packages[j].id != NULL) {
			if(j>i)
				source->offices[source_office_index].packages[i] = source->offices[source_office_index].packages[j];
			i++;
		}
	}

}

town town_with_most_packages(town* towns, int towns_count) {
	int i, j, k;
	int *packages_counts = (int *)calloc(towns_count, sizeof(int));

	for (k = 0; k < towns_count; k++) {
		for (i = 0; i < towns[k].offices_count; i++) {
			packages_counts[k] += towns[k].offices[i].packages_count;
		}
	}

	j = 0;
	for (k = 1; k < towns_count; k++) {
		if (packages_counts[k] > packages_counts[j])
			j = k;
	}

	free(packages_counts);

	return towns[j];
}

town* find_town(town* towns, int towns_count, char* name) {
	int j = -1;
	int i;
	for (i = 0; i < towns_count && j == -1; i++) {
		if (strcmp(towns[i].name, name) == 0)
			j = i;
	}

	return (towns + j);
}

int main()
{
	int towns_count;
	scanf("%d", &towns_count);
	town* towns = (town*)malloc(sizeof(town)*towns_count);
	for (int i = 0; i < towns_count; i++) {
		towns[i].name = (char*)malloc(sizeof(char) * MAX_STRING_LENGTH);
		scanf("%s", towns[i].name);
		scanf("%d", &towns[i].offices_count);
		towns[i].offices = (post_office*)malloc(sizeof(post_office)*towns[i].offices_count);
		for (int j = 0; j < towns[i].offices_count; j++) {
			scanf("%d%d%d", &towns[i].offices[j].packages_count, &towns[i].offices[j].min_weight, &towns[i].offices[j].max_weight);
			towns[i].offices[j].packages = (package*)malloc(sizeof(package)*towns[i].offices[j].packages_count);
			for (int k = 0; k < towns[i].offices[j].packages_count; k++) {
				towns[i].offices[j].packages[k].id = (char*)malloc(sizeof(char) * MAX_STRING_LENGTH);
				scanf("%s", towns[i].offices[j].packages[k].id);
				scanf("%d", &towns[i].offices[j].packages[k].weight);
			}
		}
	}
	int queries;
	scanf("%d", &queries);
	char town_name[MAX_STRING_LENGTH];
	while (queries--) {
		int type;
		scanf("%d", &type);
		switch (type) {
		case 1:
			scanf("%s", town_name);
			town* t = find_town(towns, towns_count, town_name);
			print_all_packages(*t);
			break;
		case 2:
			scanf("%s", town_name);
			town* source = find_town(towns, towns_count, town_name);
			int source_index;
			scanf("%d", &source_index);
			scanf("%s", town_name);
			town* target = find_town(towns, towns_count, town_name);
			int target_index;
			scanf("%d", &target_index);
			send_all_acceptable_packages(source, source_index, target, target_index);
			break;
		case 3:
			printf("Town with the most number of packages is %s\n", town_with_most_packages(towns, towns_count).name);
			break;
		}
	}
	return 0;
}