// ------------------------------------------------------------ structs for messages ----------------------------------------------------


struct messages
{
    int pid;
    char buf[100];
}msg;
 
struct stream 
{
    // structure within structure
    struct msg in;
    struct msg out;
    {
    	
    };
};



//---------------------------------------------------------------------------------------------------------------------------------------------




struct student_college_detail
{
    int college_id;
    char college_name[50];
};
 
struct student_detail 
{
    int id;
    char name[20];
    float percentage;
    // structure within structure
    struct student_college_detail clg_data;
}stu_data;
//---------------------------------------------------------------------------------------------------------