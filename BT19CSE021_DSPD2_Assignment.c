#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define SIZE 100
#define SIZE_AQI 10


typedef struct sensor_node_tag
{
	int sensor_id;
	char sensor_type[SIZE];
	char sensor_location[SIZE];
	int duration;
	int distance_between_stations;
	struct sensor_node_tag* next;
	struct sensor_node_tag* prev;
}sensor_node;


typedef struct record_node_tag
{
	int sensor_id;
	//time at which last data was recived
	int time;
	//date at which last data was recived 
	int day, month, year;
	int data;
	struct record_node_tag* next;
	struct record_node_tag* prev;
}record_node;


typedef struct AQI_sensor_node_tag
{
	int sensor_id;
	char sensor_location[SIZE];
	char health_status[SIZE];
	int distance_between_stations;
	//here i am assuming that SIZE_AQI amount of max different type and duration can be stored
	int duration[SIZE_AQI];
	char sensor_type[SIZE_AQI][SIZE];
	//each entity has different AQI
	int AQI[SIZE_AQI];
	struct sensor_node_tag* next;
	struct sensor_node_tag* prev;
}AQI_sensor_node;


typedef struct AQI_record_node_tag
{
	int sensor_id;
	//date at which last data was recived 
	int day, month, year;
	//same as sensor only SIZE_AQI amount of data required to be collected
	int data[SIZE_AQI];
	//time at which last data was recived
	//for each different sensor having different duration
	//so again SIZE_AQI amount required
	int time[SIZE_AQI];
	struct record_node_tag* next;
	struct record_node_tag* prev;
}AQI_record_node;


sensor_node* install_new_sensor(sensor_node* lptr);
sensor_node* delete_sensor(sensor_node* lptr, int sen_id);
record_node* delete_record(record_node* lptr);
record_node* delete_idle(record_node* lptr, sensor_node** npptr);
sensor_node* make_sensor();
void print_sensor(sensor_node* lptr);
void create_sensor_list(sensor_node* new_node);
record_node* make_record();
void central_repository(record_node* new_record);
void Retrieve_info(sensor_node* lptr, record_node* nptr);
sensor_node* insert_end_node(sensor_node* lptr, sensor_node* nptr);
record_node* insert_end_record(record_node* lptr, record_node* nptr);
sensor_node* find_communicating_neighbours(sensor_node* lptr);
AQI_sensor_node* make_sensor_AQI();
AQI_record_node* make_record_AQI();
void AQI_create_sensor_list(AQI_sensor_node* new_node);
AQI_sensor_node* insert_end_AQI_node(AQI_sensor_node* lptr, AQI_sensor_node* nptr);
AQI_record_node* insert_end_AQI_record(AQI_record_node* lptr, AQI_record_node* nptr);
AQI_sensor_node* install_new_AQI_sensor(AQI_sensor_node* lptr);
AQI_sensor_node* delete_AQI_sensor(AQI_sensor_node* lptr, int sen_id);
AQI_record_node* delete_AQI_record(AQI_record_node* lptr);
AQI_record_node* delete_idle_AQI(AQI_record_node* lptr, AQI_sensor_node** npptr);
AQI_sensor_node* find_communicating_AQI_neighbours(AQI_sensor_node* lptr);
void display_max_AQI(AQI_record_node* lptr, AQI_sensor_node* nptr);
void display_health_status(AQI_sensor_node* lptr, AQI_record_node* nptr);
void display_hazardous_status(AQI_sensor_node* lptr, AQI_record_node* nptr);


int main()
{

	int i, n;
	sensor_node* sptr = make_sensor();
	create_sensor_list(sptr);
	record_node* rptr = make_record();
	central_repository(rptr);
	AQI_sensor_node* AQI_sptr = make_sensor_AQI();
	AQI_record_node* AQI_rptr = make_record_AQI();

	printf("Enter no of sensors to be input: \n");
	scanf("%d", &n);

	if (n > 100 || n == 0)
	{
		n = SIZE;
	}
	else
	{
		sensor_node* new_node = make_sensor();
		printf("Enter details of sensor database:\n");
		for (i = 0; i < n; i++)
		{
			sptr = insert_end_node(sptr, new_node);
		}

		record_node* new_rec = make_record();
		printf("Enter details of central database:\n");
		for (i = 0; i < n; i++)
		{
			rptr = insert_end_record(rptr, new_rec);
		}
	}

	sptr = install_new_sensor(sptr);
	Retrieve_info(sptr, rptr);
	rptr = delete_idle(rptr, &sptr);

	//new list for making group of neighbours
	sensor_node* gptr = make_sensor();
	create_sensor_list(gptr);

	gptr = find_communicating_neighbours(sptr);

	AQI_sensor_node* new_node_AQI = make_sensor_AQI();
	printf("Enter details of sensor database:\n");
	for (i = 0; i < n; i++)
	{
		AQI_sptr = insert_end_AQI_node(AQI_sptr, new_node_AQI);
	}

	AQI_record_node* new_rec_AQI = make_record_AQI();
	printf("Enter details of central database:\n");
	for (i = 0; i < n; i++)
	{
		AQI_rptr = insert_end_AQI_record(AQI_rptr, new_rec_AQI);
	}

	display_max_AQI(AQI_rptr, AQI_sptr);
	display_health_status(AQI_sptr, AQI_rptr);
	display_hazardous_status(AQI_sptr, AQI_rptr);

	return 0;
}


sensor_node* install_new_sensor(sensor_node* lptr)
{
	if (lptr == NULL)
	{
		printf("list is empty!\n");
		return lptr;
	}

	int id1;
	sensor_node* n, * p, * ptr;
	n = NULL;
	p = NULL;

	printf("Enter the sensor id of the sensor between which you want to install a new sensor:\n");
	scanf("%d", &id1);

	ptr = lptr;
	while (ptr != NULL)
	{
		if (ptr->sensor_id == id1)
		{
			p = ptr;
		}
		if (p->next != NULL)
		{
			n = p->next;
		}
		ptr = ptr->next;
	}

	if (p == NULL || n == NULL)
	{
		printf("One of the node doesn't exist,please give correct input!\n");
		free(ptr);
		return lptr;
	}

	if (abs(p->distance_between_stations - n->distance_between_stations) > 10)
	{
		printf("distance is more than 10km, new node cant be installed\n");
		free(ptr);
		return lptr;
	}

	sensor_node* new_node = make_sensor();

	ptr = lptr;
	while (ptr != NULL)
	{
		//scans all sensors at location p
		if (ptr->sensor_location == p->sensor_location)
		{
			if (!strcmp(ptr->sensor_type, new_node->sensor_type))
			{
				printf("type already exists, can't install new sensor\n");
				free(ptr);
				return lptr;
			}
		}
		//scans all sensors at location q
		if (ptr->sensor_location == n->sensor_location)
		{
			if (!strcmp(ptr->sensor_type, new_node->sensor_type))
			{
				printf("type already exists, can't install new sensor\n");
				free(ptr);
				return lptr;
			}
		}
		ptr = ptr->next;
	}

	p->next = new_node;
	new_node->prev = p;
	new_node->next = n;
	n->prev = new_node;

	free(ptr);
	return lptr;
}


sensor_node* delete_sensor(sensor_node* lptr, int sen_id)
{
	if (lptr == NULL)
	{
		return lptr;
	}

	sensor_node* prev_node, * next_node, * ptr, * nptr;

	ptr = lptr;
	while (ptr != NULL)
	{
		if (ptr->sensor_id == sen_id)
		{
			nptr = ptr;
		}
		ptr = ptr->next;
	}

	prev_node = nptr->prev;
	next_node = nptr->next;
	if (prev_node != NULL)
	{
		prev_node->next = next_node;
	}
	if (next_node != NULL)
	{
		next_node->prev = prev_node;
	}
	free(nptr);

	return lptr;
}


record_node* delete_record(record_node* lptr)
{
	if (lptr == NULL)
	{
		return lptr;
	}
	record_node* prev_node, * next_node;

	prev_node = lptr->prev;
	next_node = lptr->next;
	if (prev_node != NULL)
	{
		prev_node->next = next_node;
	}
	if (next_node != NULL)
	{
		next_node->prev = prev_node;
	}

	return 0;
}


record_node* delete_idle(record_node* lptr, sensor_node** npptr)
{
	if (lptr == NULL)
	{
		return lptr;
	}
	
	record_node* ptr;
	//for day month and year inputs respectively
	int dy, mt, yr;

	printf("enter todays date:dd mm yyyy:\n");
	scanf("%d%d%d", &dy, &mt, &yr);
	
	ptr = lptr;
	while (ptr != NULL)
	{
		if (ptr->year > yr )
		{
			*npptr = delete_sensor(*npptr, ptr->sensor_id);
			delete_record(ptr);
			return lptr;
		}
		else if (ptr->month != mt && ptr->month > (mt - 2) )
		{
			//here we assume that if mt is 1 then -1 referes to 12 of prev year
			*npptr = delete_sensor(*npptr, ptr->sensor_id);
			delete_record(ptr);
			return lptr;
		}
		ptr = ptr->next;
	}

	return lptr;
}


sensor_node* make_sensor()
{
	//takes all values as input to generate a node;
	sensor_node* nptr;
	nptr = (sensor_node*)malloc(sizeof(sensor_node));

	nptr->next = NULL;
	nptr->prev = NULL;

	printf("enter sensor id:\n");
	scanf("%d", &(nptr->sensor_id));
	printf("enter sensor type:\n");
	scanf("%s", &(nptr->sensor_type));
	printf("enter sensor location:\n");
	scanf("%s", &(nptr->sensor_location));
	printf("enter duration:\n");
	scanf("%d", &(nptr->duration));
	printf("enter neighbour distance:\n");
	scanf("%d", &(nptr->distance_between_stations));

	return nptr;
}


void print_sensor(sensor_node* lptr)
{
	printf("sensor id is: %d\n",lptr->sensor_id);
	printf("sensor type is: %s\n",lptr->sensor_location);
	printf("sensor location is: %s\n",lptr->sensor_location);
	printf("duration is: %d\n",lptr->duration);
	printf("neighbour distance is: %d\n",lptr->distance_between_stations);
}


void create_sensor_list(sensor_node* new_node)
{
	//here we are assuming that the new_node is given  so we only need to 
	//set the initial value to 0 if by chance it was set to someting else 
	
	(new_node->distance_between_stations) = 0;
}


record_node* make_record()
{
	//takes all values as input to generate a node;
	record_node* nptr;
	nptr = (record_node*)malloc(sizeof(record_node));

	nptr->next = NULL;
	nptr->prev = NULL;

	printf("enter sensor id:\n");
	scanf("%d", &(nptr->sensor_id));
	printf("enter time of entering:\n");
	scanf("%d", &(nptr->time));
	printf("enter day of entering:\n");
	scanf("%d", &(nptr->day));
	printf("enter month of entering:\n");
	scanf("%d", &(nptr->month));
	printf("enter year of entering:\n");
	scanf("%d", &(nptr->year));
	printf("enter data:\n");
	scanf("%d", &(nptr->data));

	return nptr;
}


void central_repository(record_node* new_record)
{
	//set the initial value of time to 0 indicating it is start of day
	new_record->time = 0;

}


void Retrieve_info(sensor_node* lptr, record_node* nptr)
{
	int ip, d, d2, m, y, dur;
	char type[SIZE];
	sensor_node* ptr1;
	record_node* ptr2;

	printf("Enter the mode to retriev info:\n");
	printf("1)Depending on sensor type\n");
	printf("2)Depending on specified date for specific sensor type\n");
	printf("3)Depending on specific time interval for specific sensor type for specified date\n");
	printf("4)Depending on specific time interval for specific sensor type for specified date range\n");
	printf("write 1,2,3 or 4\t");
	scanf("%d", &ip);

	if (ip == 1)
	{
		ptr1 = lptr;
		printf("enter type of sensor \t");
		scanf("%s", &type);
		while(ptr1 != NULL)
		{
			if (!strcmp(ptr1->sensor_type, type))
			{
				print_sensor(ptr1);
			}
			ptr1 = ptr1->next;
		}

	}

	if (ip == 2)
	{
		ptr1 = lptr;
		ptr2 = nptr;
		printf("enter type of sensor \t");
		scanf("%s", &type);
		printf("Enter date as dd mm yyyy\t");
		scanf("%d%d%d", &d, &m, &y);

		while (ptr1 != NULL)
		{
			if (!strcmp(ptr1->sensor_type, type))
			{
				while (ptr2 != NULL)
				{
					if ((d == ptr2->day) && (m == ptr2->month) && (y == ptr2->year))
					{
						print_sensor(ptr1);
					}
					ptr2 = ptr2->next;
				}
			}
			ptr1 = ptr1->next;
		}
		
	}

	if (ip == 3)
	{
		ptr1 = lptr;
		ptr2 = nptr;
		printf("enter type of sensor \t");
		scanf("%s", &type);
		printf("enter duration of sensor \t");
		scanf("%d", &dur);
		printf("Enter date dd mm yyyy\t");
		scanf("%d%d%d", &d, &m, &y);

		while (ptr1 != NULL)
		{
			if (!strcmp(ptr1->sensor_type, type) && ptr1->duration == dur)
			{
				while (ptr2 != NULL)
				{
					if ((d == ptr2->day) && (m == ptr2->month) && (y == ptr2->year))
					{
						print_sensor(ptr1);
					}
					ptr2 = ptr2->next;
				}
			}
			ptr1 = ptr1->next;
		}
	}

	if (ip == 4)
	{
		ptr1 = lptr;
		ptr2 = nptr;
		printf("enter type of sensor \t");
		scanf("%s", &type);
		printf("enter duration of sensor \t");
		scanf("%d", &dur);
		printf("Enter date1 and date2 as dd1 dd2 mm yyyy\t");
		scanf("%d%d%d%d", &d, &d2, &m, &y);
		
		while (ptr1 != NULL)
		{
			if (!strcmp(ptr1->sensor_type, type) && ptr1->duration == dur)
			{
				while (ptr2 != NULL)
				{
					if ((m == ptr2->month) && (y == ptr2->year))
					{
						while (ptr2->day == d && d < d2)
						{
							print_sensor(ptr1);
							d++;
						}
					}
					ptr2 = ptr2->next;
				}
			}
			ptr1 = ptr1->next;
		}
	}
}


sensor_node* insert_end_node(sensor_node* lptr, sensor_node* nptr)
{
	sensor_node* ptr;
	if (lptr == NULL)
	{
		lptr = nptr;
		nptr->next = NULL;
		nptr->prev = NULL;
	}
	else
	{
		ptr = lptr;
		while (ptr->next != NULL)
		{
			ptr = ptr->next;
		}
		ptr->next = nptr;
		nptr->next = NULL;
		nptr->prev = ptr;
	}
	free(ptr);

	return lptr;
}


record_node* insert_end_record(record_node* lptr, record_node* nptr)
{
	record_node* ptr;
	if (lptr == NULL)
	{
		lptr = nptr;
		nptr->next = NULL;
		nptr->prev = NULL;
	}
	else
	{
		ptr = lptr;
		while (ptr->next != NULL)
		{
			ptr = ptr->next;
		}
		ptr->next = nptr;
		nptr->next = NULL;
		nptr->prev = ptr;
	}
	free(ptr);

	return lptr;
}


sensor_node* find_communicating_neighbours(sensor_node* lptr)
{
	sensor_node* ptr1, * ptr2;
	sensor_node* new_node = (sensor_node*)malloc(sizeof(sensor_node));
	new_node->next = NULL;
	new_node->prev = NULL;
	ptr1 = lptr;
	ptr2 = lptr;
	while (ptr1 != NULL)
	{
		new_node = insert_end_node(new_node, ptr1);
		while (ptr2 != NULL)
		{
			if (abs(ptr1->distance_between_stations - ptr2->distance_between_stations) > 10)
			{
				new_node = insert_end_node(new_node, ptr2);
			}
			ptr2 = ptr2->next;
		}
		ptr1 = ptr1->next;
	}

	return new_node;
}


AQI_sensor_node* make_sensor_AQI()
{
	//takes all values as input to generate a node;
	AQI_sensor_node* nptr;
	int i;
	nptr = (AQI_sensor_node*)malloc(sizeof(AQI_sensor_node));

	nptr->next = NULL;
	nptr->prev = NULL;

	printf("enter sensor id:\n");
	scanf("%d", &(nptr->sensor_id));
	printf("enter sensor location:\n");
	scanf("%s", &(nptr->sensor_location));
	printf("enter neighbour distance:\n");
	scanf("%d", &(nptr->distance_between_stations));
	printf("enter health status:\n");
	scanf("%s", &(nptr->health_status));

	for (i = 0; i < SIZE_AQI; i++)
	{
		printf("enter sensor type:\n");
		scanf("%s", &(nptr->sensor_type[i]));
		printf("enter duration:\n");
		scanf("%d", &(nptr->duration[i]));
		printf("enter AQI:\n");
		scanf("%d", &(nptr->AQI[i]));
	}

	return nptr;
}


AQI_record_node* make_record_AQI()
{
	//takes all values as input to generate a node;
	AQI_record_node* nptr;
	int i;
	nptr = (AQI_record_node*)malloc(sizeof(AQI_record_node));

	nptr->next = NULL;
	nptr->prev = NULL;

	printf("enter sensor id:\n");
	scanf("%d", &(nptr->sensor_id));
	printf("enter day of entering:\n");
	scanf("%d", &(nptr->day));
	printf("enter month of entering:\n");
	scanf("%d", &(nptr->month));
	printf("enter year of entering:\n");
	scanf("%d", &(nptr->year));

	for (i = 0; i < SIZE_AQI; i++)
	{
		printf("enter data:\n");
		scanf("%d", &(nptr->data[i]));
		printf("enter time of entering:\n");
		scanf("%d", &(nptr->time[i]));
	}

	return nptr;
}


void AQI_create_sensor_list(AQI_sensor_node* new_node)
{
	//here we are assuming that the new_node is given  so we only need to 
	//set the initial value to 0 if by chance it was set to someting else 

	(new_node->distance_between_stations) = 0;
}


AQI_sensor_node* insert_end_AQI_node(AQI_sensor_node* lptr, AQI_sensor_node* nptr)
{
	AQI_sensor_node* ptr;
	if (lptr == NULL)
	{
		lptr = nptr;
		nptr->next = NULL;
		nptr->prev = NULL;
	}
	else
	{
		ptr = lptr;
		while (ptr->next != NULL)
		{
			ptr = ptr->next;
		}
		ptr->next = nptr;
		nptr->next = NULL;
		nptr->prev = ptr;
	}
	free(ptr);

	return lptr;
}


AQI_record_node* insert_end_AQI_record(AQI_record_node* lptr, AQI_record_node* nptr)
{
	AQI_record_node* ptr;
	if (lptr == NULL)
	{
		lptr = nptr;
		nptr->next = NULL;
		nptr->prev = NULL;
	}
	else
	{
		ptr = lptr;
		while (ptr->next != NULL)
		{
			ptr = ptr->next;
		}
		ptr->next = nptr;
		nptr->next = NULL;
		nptr->prev = ptr;
	}
	free(ptr);

	return lptr;
}


AQI_sensor_node* install_new_AQI_sensor(AQI_sensor_node* lptr)
{
	if (lptr == NULL)
	{
		printf("list is empty!\n");
		return lptr;
	}

	int id1,i;
	AQI_sensor_node* n, * p, * ptr;
	n = NULL;
	p = NULL;

	printf("Enter the sensor id of the sensor between which you want to install a new sensor:\n");
	scanf("%d", &id1);

	ptr = lptr;
	while (ptr != NULL)
	{
		if (ptr->sensor_id == id1)
		{
			p = ptr;
		}
		if (p->next != NULL)
		{
			n = p->next;
		}
		ptr = ptr->next;
	}

	if (p == NULL || n == NULL)
	{
		printf("One of the node doesn't exist,please give correct input!\n");
		free(ptr);
		return lptr;
	}

	if (abs(p->distance_between_stations - n->distance_between_stations) > 10)
	{
		printf("distance is more than 10km, new node cant be installed\n");
		free(ptr);
		return lptr;
	}

	AQI_sensor_node* new_node = make_sensor_AQI();

	ptr = lptr;
	while (ptr != NULL)
	{
		for (i = 0; i < SIZE_AQI; i++)
		{
			//scans all sensors at location p
			if (ptr->sensor_location == p->sensor_location)
			{
				if (!strcmp(ptr->sensor_type[i], new_node->sensor_type[i]))
				{
					printf("type already exists, can't install new sensor\n");
					free(ptr);
					return lptr;
				}
			}
			//scans all sensors at location q
			if (ptr->sensor_location == n->sensor_location)
			{
				if (!strcmp(ptr->sensor_type[i], new_node->sensor_type[i]))
				{
					printf("type already exists, can't install new sensor\n");
					free(ptr);
					return lptr;
				}
			}
		}
		ptr = ptr->next;
	}

	p->next = new_node;
	new_node->prev = p;
	new_node->next = n;
	n->prev = new_node;

	free(ptr);
	return lptr;
}


AQI_sensor_node* delete_AQI_sensor(AQI_sensor_node* lptr, int sen_id)
{
	if (lptr == NULL)
	{
		return lptr;
	}

	AQI_sensor_node* prev_node, * next_node, * ptr, * nptr;

	ptr = lptr;
	while (ptr != NULL)
	{
		if (ptr->sensor_id == sen_id)
		{
			nptr = ptr;
		}
		ptr = ptr->next;
	}

	prev_node = nptr->prev;
	next_node = nptr->next;
	if (prev_node != NULL)
	{
		prev_node->next = next_node;
	}
	if (next_node != NULL)
	{
		next_node->prev = prev_node;
	}
	free(nptr);

	return lptr;
}


AQI_record_node* delete_AQI_record(AQI_record_node* lptr)
{
	if (lptr == NULL)
	{
		return lptr;
	}
	AQI_record_node* prev_node, * next_node;

	prev_node = lptr->prev;
	next_node = lptr->next;
	if (prev_node != NULL)
	{
		prev_node->next = next_node;
	}
	if (next_node != NULL)
	{
		next_node->prev = prev_node;
	}

	return 0;
}


AQI_record_node* delete_idle_AQI(AQI_record_node* lptr, AQI_sensor_node** npptr)
{
	if (lptr == NULL)
	{
		return lptr;
	}

	AQI_sensor_node* prev_node, * next_node;
	AQI_record_node* ptr;
	//for day month and year inputs respectively
	int dy, mt, yr;
	int id1;

	printf("enter todays date:dd mm yyyy:\n");
	scanf("%d%d%d", &dy, &mt, &yr);

	ptr = lptr;
	while (ptr != NULL)
	{
		if (ptr->year > yr)
		{
			*npptr = delete_AQI_sensor(*npptr, ptr->sensor_id);
			delete_AQI_record(ptr);
			return lptr;
		}
		else if (ptr->month != mt && ptr->month > (mt - 2))
		{
			//here we assume that if mt is 1 then -1 referes to 12 of prev year
			*npptr = delete_AQI_sensor(*npptr, ptr->sensor_id);
			delete_AQI_record(ptr);
			return lptr;
		}
		ptr = ptr->next;
	}
}


AQI_sensor_node* find_communicating_AQI_neighbours(AQI_sensor_node* lptr)
{
	AQI_sensor_node* ptr1, * ptr2;
	AQI_sensor_node* new_node = (AQI_sensor_node*)malloc(sizeof(AQI_sensor_node));
	new_node->next = NULL;
	new_node->prev = NULL;
	ptr1 = lptr;
	ptr2 = lptr;
	while (ptr1 != NULL)
	{
		new_node = insert_end_node(new_node, ptr1);
		while (ptr2 != NULL)
		{
			if (abs(ptr1->distance_between_stations - ptr2->distance_between_stations) > 10)
			{
				new_node = insert_end_AQI_node(new_node, ptr2);
			}
			ptr2 = ptr2->next;
		}
		ptr1 = ptr1->next;
	}

	return new_node;
}


void display_max_AQI(AQI_record_node* lptr, AQI_sensor_node* nptr)
{
	int i, max_aqi = nptr->AQI[0], sen_id;
	AQI_sensor_node* ptr;
	AQI_record_node* rptr;

	ptr = nptr;
	while(ptr != NULL)
	{
		for (i = 0; i < SIZE_AQI; i++)
		{
			if (max_aqi < ptr->AQI[i])
			{
				max_aqi = ptr->AQI[i];
				sen_id = ptr->sensor_id;
			}
		}
		ptr = ptr->next;
	}

	rptr = lptr;
	while(rptr != NULL)
	{
		if (sen_id == rptr->sensor_id)
		{
			printf("max aqi in month %d", rptr->month);
		}
		rptr = rptr->next;
	}
}


void display_health_status(AQI_sensor_node* lptr, AQI_record_node* nptr)
{
	char ip_stat[SIZE],loc[SIZE];
	AQI_sensor_node* ptr;
	AQI_record_node* rptr;

	printf("enter the health status \n");
	scanf("%s", ip_stat);
	printf("enter the station \n");
	scanf("%s", loc);

	if (ip_stat == "Good")
	{
		ptr = lptr;
		rptr = nptr;
		while(ptr != NULL)
		{
			if ( !strcmp(ptr->health_status ,"Good") && !strcmp(ptr->sensor_location, loc))
			{
				while (rptr != NULL)
				{
					if (rptr->sensor_id == nptr->sensor_id)
					{
						printf(" date is %d/%d/%d\n", rptr->day, rptr->month, rptr->year);
					}
					rptr = rptr->next;
				}
			}
			ptr = ptr->next;
		}

	}
	else if (ip_stat == "Satisfactory")
	{
		ptr = lptr;
		rptr = nptr;
		while (ptr != NULL)
		{
			if (!strcmp(ptr->health_status, "Satisfactory") && !strcmp(ptr->sensor_location, loc))
			{
				while (rptr != NULL)
				{
					if (rptr->sensor_id == nptr->sensor_id)
					{
						printf(" date is %d/%d/%d\n", rptr->day, rptr->month, rptr->year);
					}
					rptr = rptr->next;
				}
			}
			ptr = ptr->next;
		}
	}
	else if (ip_stat == "Moderately polluted")
	{
		ptr = lptr;
		rptr = nptr;
		while (ptr != NULL)
		{
			if (!strcmp(ptr->health_status, "Moderately polluted") && !strcmp(ptr->sensor_location, loc))
			{
				while (rptr != NULL)
				{
					if (rptr->sensor_id == nptr->sensor_id)
					{
						printf(" date is %d/%d/%d\n", rptr->day, rptr->month, rptr->year);
					}
					rptr = rptr->next;
				}
			}
			ptr = ptr->next;
		}
	}
	else if (ip_stat == "poor")
	{
		ptr = lptr;
		rptr = nptr;
		while (ptr != NULL)
		{
			if (!strcmp(ptr->health_status, "poor") && !strcmp(ptr->sensor_location, loc))
			{
				while (rptr != NULL)
				{
					if (rptr->sensor_id == nptr->sensor_id)
					{
						printf(" date is %d/%d/%d\n", rptr->day, rptr->month, rptr->year);
					}
					rptr = rptr->next;
				}
			}
			ptr = ptr->next;
		}
	}
	else if (ip_stat == "May cause respiratory illness")
	{
		ptr = lptr;
		rptr = nptr;
		while (ptr != NULL)
		{
			if (!strcmp(ptr->health_status, "May cause respiratory illness") && !strcmp(ptr->sensor_location, loc))
			{
				while (rptr != NULL)
				{
					if (rptr->sensor_id == nptr->sensor_id)
					{
						printf(" date is %d/%d/%d\n", rptr->day, rptr->month, rptr->year);
					}
					rptr = rptr->next;
				}
			}
			ptr = ptr->next;
		}
	}
	else if (ip_stat == "Severe")
	{
		ptr = lptr;
		rptr = nptr;
		while (ptr != NULL)
		{
			if (!strcmp(ptr->health_status, "Severe") && !strcmp(ptr->sensor_location, loc))
			{
				while (rptr != NULL)
				{
					if (rptr->sensor_id == nptr->sensor_id)
					{
						printf(" date is %d/%d/%d\n", rptr->day, rptr->month, rptr->year);
					}
					rptr = rptr->next;
				}
			}
			ptr = ptr->next;
		}
	}
	else if (ip_stat == "Hazardous")
	{
		ptr = lptr;
		rptr = nptr;
		while (ptr != NULL)
		{
			if (!strcmp(ptr->health_status, "Hazardous") && !strcmp(ptr->sensor_location, loc))
			{
				while (rptr != NULL)
				{
					if (rptr->sensor_id == nptr->sensor_id)
					{
						printf(" date is %d/%d/%d\n", rptr->day, rptr->month, rptr->year);
					}
					rptr = rptr->next;
				}
			}
			ptr = ptr->next;
		}
	}
}


void display_hazardous_status(AQI_sensor_node* lptr, AQI_record_node* nptr)
{
	AQI_sensor_node* ptr;
	AQI_record_node* rptr;

	ptr = lptr;
	rptr = nptr;
	
	while (ptr != NULL)
	{
		if (!strcmp(ptr->health_status, "Hazardous"))
		{
			while (rptr != NULL)
			{
				if (rptr->sensor_id == nptr->sensor_id)
				{
					printf(" date is %d/%d/%d\n", rptr->day, rptr->month, rptr->year);
				}
				rptr = rptr->next;
			}
		}
		ptr = ptr->next;
	}

}
