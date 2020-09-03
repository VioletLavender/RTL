/*******************************************************************
 * Device     : RF
 *
 * File Name  : KERNEL.C
 * Description: This file provides the kernel functions including system
 *              resource initialization, buffer allocation & deallocation,
 *              timer handling, message sending & receiving, and
 *              mailbox scheduling.
 *
 * $Log:  $
 *
 *
 *
 *******************************************************************
 *
 *      Copyright (c) 2020, All Right Reserved
 *      Rafael Microelectronics Co. Ltd.
 *      Taiwan, R.O.C.
 *
 *******************************************************************/
#pragma push
//#pragma Otime
#pragma Ospace

/*******************************************************************
 *      Include List
 *******************************************************************/
#include "knl_pblc.h"



/*******************************************************************
 *      Function Prototype Decelaration
 *******************************************************************/


/*******************************************************************
 *      External Function Decelaration
 *******************************************************************/

/*******************************************************************
 *      Global Variable Defines
 *******************************************************************/

#pragma Otime
/*F******************************************************************
 *
 * Knl_MemCpy - Memory copy
 * Description:
 * Arguments:
 *      *pDst: pointer of destination
 *      *pSrc: pointer of source
 *      len: data length to copy
 * Return:
 *      None
 *
 *******************************************************************/
void Knl_MemCpy(Uint8 *pDst, Uint8 *pSrc, Uint8 len)
{
    do
    {
        len--;
        *(pDst + len) = *(pSrc + len);
    }
    while (len != 0);
}


void Knl_MemCpy_Fwd(Uint8 *pDst, Uint8 *pSrc, Uint8 len)
{
    Uint8 i;

    for (i = 0; i < len; i++)
    {
        *(pDst + i) = *(pSrc + i);
    }
}


void Knl_MemCpyInv(Uint8 *pDst, Uint8 *pSrc, Uint8 len)
{
    Uint8 i;

    i = 0;
    do
    {
        len--;
        *(pDst + len) = *(pSrc + i);
        i++;
    }
    while (len != 0);
}


void Knl_CodeCpy(Uint8 *pDst, Uint8 const *pSrc, Uint8 len)
{
    do
    {
        len--;
        *(pDst + len) = *(pSrc + len);
    }
    while (len != 0);
}


void Knl_CodeCpyInv(Uint8 *pDst, Uint8 const *pSrc, Uint8 len)
{
    Uint8 i;

    i = 0;
    do
    {
        len--;
        *(pDst + len) = *(pSrc + i);
        i++;
    }
    while (len != 0);
}


Uint8 Knl_MemComp(Uint8 *pDst, Uint8 *pSrc, Uint8 len)
{
    while (len != 0)
    {
        len--;
        if (*(pSrc + len) != *(pDst + len))
        {
            break;
        }
    }
    return len;     //SUCCESS: 0
}


Uint8 Knl_CodeComp(Uint8 *pDst, Uint8 const *pSrc, Uint8 len)
{
    while (len != 0)
    {
        len--;
        if (*(pSrc + len) != *(pDst + len))
        {
            break;
        }
    }
    return len;     //SUCCESS: 0
}


void Knl_MemCpy_Isr(Uint8 *pDst, Uint8 *pSrc, Uint8 len)
{
    do
    {
        len--;
        *(pDst + len) = *(pSrc + len);
    }
    while (len != 0);
}


void Knl_MemCpyInv_Isr(Uint8 *pDst, Uint8 *pSrc, Uint8 len)
{
    Uint8 i;

    i = 0;
    do
    {
        len--;
        *(pDst + len) = *(pSrc + i);
        i++;
    }
    while (len != 0);
}


void Knl_CodeCpy_Isr(Uint8 *pDst, Uint8 const *pSrc, Uint8 len)
{
    do
    {
        len--;
        *(pDst + len) = *(pSrc + len);
    }
    while (len != 0);
}


void Knl_CodeCpyInv_Isr(Uint8 *pDst, Uint8 const *pSrc, Uint8 len)
{
    Uint8 i;

    i = 0;
    do
    {
        len--;
        *(pDst + len) = *(pSrc + i);
        i++;
    }
    while (len != 0);
}


Uint8 Knl_MemComp_Isr(Uint8 *pDst, Uint8 *pSrc, Uint8 len)
{
    while (len != 0)
    {
        len--;
        if (*(pSrc + len) != *(pDst + len))
        {
            break;
        }
    }
    return len;     //SUCCESS: 0
}


Uint8 Knl_CodeComp_Isr(Uint8 *pDst, Uint8 const *pSrc, Uint8 len)
{
    while (len != 0)
    {
        len--;
        if (*(pSrc + len) != *(pDst + len))
        {
            break;
        }
    }
    return len;     //SUCCESS: 0
}



#pragma Ospace

#pragma Otime
void ErrorEntry(Uint32 errID)
{
    Uint32 i;
    InterruptDisable();
    i = errID;
    if (i)
    {
        while (1);
    }
    InterruptEnable();
}



#pragma pop

