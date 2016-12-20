#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "student.pb-c.h"

static int malloc_student_info(Student *stu)
{
  stu->id = (char *)malloc(ID_LEN)
}
