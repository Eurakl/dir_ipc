/*
 * df_frame.h
 *
 * Created on 2023年5月23日
 * Author wangyuxin
 *
 * Copyright (c) zhejiang univerisity
 */
//#include "../ipc_interrupt.h"
#ifndef DF_FRAME_H
#define DF_FRAME_H

typedef unsigned char UINT8;
typedef unsigned short int UINT16;
typedef unsigned int UINT32;
typedef unsigned long long  UINT64;
#define MAX_NODE_NUM	32


//宏定义
#define DATA_SIZE_400		50
#define DATA_SIZE_4000		500
#define SEND_DATA_SIZE		(1?DATA_SIZE_400:DATA_SIZE_4000)
#define RECV_DATA_SIZE		(1?DATA_SIZE_400:DATA_SIZE_4000)

//帧类型
typedef enum
{
	FIN,    
	DNET_RTS,
	ALIGN,
	D_RTS,
	D_CTS,
	QUICK_ALIGN,
	D_DATA,
	D_ACK,
}Frame_Type;

//FIN帧
typedef struct 
{
	UINT8 src;						//5bit
	UINT8 SubnetID;					//2bit
	UINT8 finish;					//2bit
	UINT8 ant_index[MAX_NODE_NUM];  //64bit
	UINT8 dir_online_node[MAX_NODE_NUM];		//32bit
}FIN_frame;

//DNET_RTS帧
typedef struct 
{
	UINT8 src;						//5bit
	UINT8 dest;						//5bit
	UINT8 SubnetID;					//2bit
	UINT8 send_dir;					//1bit
	UINT8 potential_neigh[MAX_NODE_NUM];		//32bit
}DNET_RTS_frame;

//DNET_START帧 定向核告知全向核
// typedef struct 
// {
// 	UINT8 cluster;						//5bit
// 	UINT8 dest;						//5bit
// 	UINT8 pos_record[MAX_NODE_NUM][6];		//4608bit
// 	UINT32 dist_record[MAX_NODE_NUM];		//640bit
// }DNET_START_frame;

//ROUTE_START帧 定向核告知路由核
// typedef struct 
// {
// 	UINT8 online_num;						//6bit
// 	UINT8 ant_index_mat[MAX_NODE_NUM][MAX_NODE_NUM];		//3072bit
// }ROUTE_START_frame;

//ALIGN帧
typedef struct 
{
	UINT8 src;						//5bit
}ALIGN_frame;
//D_RTS帧
typedef struct 
{
	UINT8 src;						//5bit
}D_RTS_frame;
//D_CTS帧
typedef struct 
{
	UINT8 src;						//5bit
}D_CTS_frame;
//QUICK_ALIGN帧
typedef struct 
{
	UINT8 src;						//5bit
}QUICK_ALIGN_frame;
//D_DATA帧
typedef struct 
{
	UINT8 src;						//5bit
	UINT8 next_hop;					//5bit
	UINT8 dest;						//5bit
	UINT8 pk_id;					//3bit
	UINT8 pk_num;					//3bit
	UINT8 priority;					//2bit
	UINT8 rate;						//1bit
	UINT8 data_buf[DATA_SIZE_4000];	//3bit
}D_DATA_frame;
//D_ACK帧
typedef struct 
{
	UINT8 src;						//5bit
}D_ACK_frame;



//普通帧内容块
typedef struct 
{
	Frame_Type frame_type;
	FIN_frame fin_part;
	DNET_RTS_frame dnet_rts_part;
	ALIGN_frame align_part;
	D_RTS_frame d_rts_part;
	D_CTS_frame d_cts_part;
	QUICK_ALIGN_frame quick_align_part;
	D_DATA_frame d_data_part;
	D_ACK_frame d_ack_part;
}Frame_Content_Block;

extern volatile Frame_Content_Block far frame_send_block;
extern volatile Frame_Content_Block far frame_recv_block;

//function
void framing_FIN(UINT8 *ptr);
void resolve_FIN(UINT8 *ptr);
void framing_DNET_RTS(UINT8 *ptr);
void resolve_DNET_RTS(UINT8 *ptr);
void framing_ALIGN(UINT8 *ptr);
void resolve_ALIGN(UINT8 *ptr);
void framing_D_RTS(UINT8 *ptr);
void resolve_D_RTS(UINT8 *ptr);
void framing_D_CTS(UINT8 *ptr);
void resolve_D_CTS(UINT8 *ptr);
void framing_QUICK_ALIGN(UINT8 *ptr);
void resolve_QUICK_ALIGN(UINT8 *ptr);
void framing_D_DATA(UINT8 *ptr);
void resolve_D_DATA(UINT8 *ptr);
void framing_D_ACK(UINT8 *ptr);
void resolve_D_ACK(UINT8 *ptr);

#endif
