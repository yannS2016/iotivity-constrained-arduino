#include <stdlib.h>
#include "mather.h"
#include "cppmather.h"
 
struct mather {
    void *obj;
};
 
mather_t *mather_create(int start)
{
    mather_t *m;
    CPPMather *obj;
 
    m      = (typeof(m))malloc(sizeof(*m));
    obj    = new CPPMather(start);
    m->obj = obj;
 
    return m;
}
 
void mather_destroy(mather_t *m)
{
    if (m == NULL)
        return;
    delete static_cast<CPPMather *>(m->obj);
    free(m);
}
 
void mather_add(mather_t *m, int val)
{
    CPPMather *obj;
 
    if (m == NULL)
        return;
 
    obj = static_cast<CPPMather *>(m->obj);
    obj->add(val);
}
 
int mather_val(mather_t *m)
{
    CPPMather *obj;
 
    if (m == NULL)
        return 0;
 
    obj = static_cast<CPPMather *>(m->obj);
    return obj->val();
}