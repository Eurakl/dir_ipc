/*
 * df_frame.c
 *
 * Created on 2023年5月23日
 * Author wangyuxin
 *
 * Copyright (c) zhejiang univerisity
 */
#include "../inc/DF/df_frame.h"
//#include "../ipc_interrupt.h"

//extern Frame_Content_Block frame_send_block;               //发送帧
//extern Frame_Content_Block frame_recv_block;               //接收帧

/*---------------functions--------------*/


/*---------------functions--------------*/
//1 
//name:framing_FIN
//function: 组建FIN帧
//by: wyx
void framing_FIN(UINT8 *ptr)
{
	int i;
	ptr[0] = ((UINT8)frame_send_block.frame_type << 3) + frame_send_block.fin_part.SubnetID;
	ptr[1] = ((UINT8)(frame_send_block.fin_part.src) << 3) + (frame_send_block.fin_part.finish);

    for(i=2;i<10;i++)
        ptr[i] = 0;
    for(i=0;i<4;i++)
        ptr[2] += (UINT8)(frame_send_block.fin_part.ant_index[i] << 2*i);
    for(i=0;i<4;i++)
        ptr[3] += (UINT8)(frame_send_block.fin_part.ant_index[i+4] << 2*i);
    for(i=0;i<4;i++)
        ptr[4] += (UINT8)(frame_send_block.fin_part.ant_index[i+8] << 2*i);
    for(i=0;i<4;i++)
        ptr[5] += (UINT8)(frame_send_block.fin_part.ant_index[i+12] << 2*i);
    for(i=0;i<4;i++)
        ptr[6] += (UINT8)(frame_send_block.fin_part.ant_index[i+16] << 2*i);
    for(i=0;i<4;i++)
        ptr[7] += (UINT8)(frame_send_block.fin_part.ant_index[i+20] << 2*i);
    for(i=0;i<4;i++)
        ptr[8] += (UINT8)(frame_send_block.fin_part.ant_index[i+24] << 2*i);
    for(i=0;i<4;i++)
        ptr[9] += (UINT8)(frame_send_block.fin_part.ant_index[i+28] << 2*i);

    for(i=10;i<14;i++)
        ptr[i] = 0;
	for (i = 0; i < 8; i++) 
		ptr[10] += (UINT8)(frame_send_block.fin_part.dir_online_node[i] << i);
	for (i = 0; i < 8; i++) 
		ptr[11] += (UINT8)(frame_send_block.fin_part.dir_online_node[i+8] << i);
	for (i = 0; i < 8; i++) 
		ptr[12] += (UINT8)(frame_send_block.fin_part.dir_online_node[i+16] << i);
	for (i = 0; i < 8; i++) 
		ptr[13] += (UINT8)(frame_send_block.fin_part.dir_online_node[i+24] << i);
}

//2
//name:resolve_FIN 
//function: 解FIN帧
//by: wyx
void resolve_FIN(UINT8 *ptr)
{
	frame_recv_block.fin_part.SubnetID = (ptr[0] & 0x3);
	frame_recv_block.fin_part.src = (ptr[1] & 0xF8) >> 3;
	frame_recv_block.fin_part.finish = (ptr[1] & 0x3);

	frame_recv_block.fin_part.ant_index[0] = (ptr[2] & 0x3);
	frame_recv_block.fin_part.ant_index[1] = (ptr[2] & 0xC) >> 2;
	frame_recv_block.fin_part.ant_index[2] = (ptr[2] & 0x30) >> 4;
	frame_recv_block.fin_part.ant_index[3] = (ptr[2] & 0xC0) >> 6;
	frame_recv_block.fin_part.ant_index[4] = (ptr[3] & 0x3) ;
	frame_recv_block.fin_part.ant_index[5] = (ptr[3] & 0xC) >> 2;
	frame_recv_block.fin_part.ant_index[6] = (ptr[3] & 0x30) >> 4;
	frame_recv_block.fin_part.ant_index[7] = (ptr[3] & 0xC0) >> 6;
	frame_recv_block.fin_part.ant_index[8] = (ptr[4] & 0x3);
	frame_recv_block.fin_part.ant_index[9] = (ptr[4] & 0xC) >> 2;
	frame_recv_block.fin_part.ant_index[10] = (ptr[4] & 0x30) >> 4;
	frame_recv_block.fin_part.ant_index[11] = (ptr[4] & 0xC0) >> 6;
	frame_recv_block.fin_part.ant_index[12] = (ptr[5] & 0x3);
	frame_recv_block.fin_part.ant_index[13] = (ptr[5] & 0xC) >> 2;
	frame_recv_block.fin_part.ant_index[14] = (ptr[5] & 0x30) >> 4;
	frame_recv_block.fin_part.ant_index[15] = (ptr[5] & 0xC0) >> 6;
	frame_recv_block.fin_part.ant_index[16] = (ptr[6] & 0x3);
	frame_recv_block.fin_part.ant_index[17] = (ptr[6] & 0xC) >> 2;
	frame_recv_block.fin_part.ant_index[18] = (ptr[6] & 0x30) >> 4;
	frame_recv_block.fin_part.ant_index[19] = (ptr[6] & 0xC0) >> 6;
	frame_recv_block.fin_part.ant_index[20] = (ptr[7] & 0x3);
	frame_recv_block.fin_part.ant_index[21] = (ptr[7] & 0xC) >> 2;
	frame_recv_block.fin_part.ant_index[22] = (ptr[7] & 0x30) >> 4;
	frame_recv_block.fin_part.ant_index[23] = (ptr[7] & 0xC0) >> 6;
	frame_recv_block.fin_part.ant_index[24] = (ptr[8] & 0x3);
	frame_recv_block.fin_part.ant_index[25] = (ptr[8] & 0xC) >> 2;
	frame_recv_block.fin_part.ant_index[26] = (ptr[8] & 0x30) >> 4;
	frame_recv_block.fin_part.ant_index[27] = (ptr[8] & 0xC0) >> 6;
	frame_recv_block.fin_part.ant_index[28] = (ptr[9] & 0x3);
	frame_recv_block.fin_part.ant_index[29] = (ptr[9] & 0xC) >> 2;
	frame_recv_block.fin_part.ant_index[30] = (ptr[9] & 0x30) >> 4;
	frame_recv_block.fin_part.ant_index[31] = (ptr[9] & 0xC0) >> 6;

	frame_recv_block.fin_part.dir_online_node[0] = (ptr[10] & 0x1);
	frame_recv_block.fin_part.dir_online_node[1] = (ptr[10] & 0x2) >> 1;
	frame_recv_block.fin_part.dir_online_node[2] = (ptr[10] & 0x4) >> 2;
	frame_recv_block.fin_part.dir_online_node[3] = (ptr[10] & 0x8) >> 3;
	frame_recv_block.fin_part.dir_online_node[4] = (ptr[10] & 0x10) >> 4;
	frame_recv_block.fin_part.dir_online_node[5] = (ptr[10] & 0x20) >> 5;
	frame_recv_block.fin_part.dir_online_node[6] = (ptr[10] & 0x40) >> 6;
    frame_recv_block.fin_part.dir_online_node[7] = (ptr[10] & 0x80) >> 7;
	frame_recv_block.fin_part.dir_online_node[8] = (ptr[11] & 0x1);
	frame_recv_block.fin_part.dir_online_node[9] = (ptr[11] & 0x2) >> 1;
	frame_recv_block.fin_part.dir_online_node[10] = (ptr[11] & 0x4) >> 2;
	frame_recv_block.fin_part.dir_online_node[11] = (ptr[11] & 0x8) >> 3;
	frame_recv_block.fin_part.dir_online_node[12] = (ptr[11] & 0x10) >> 4;
	frame_recv_block.fin_part.dir_online_node[13] = (ptr[11] & 0x20) >> 5;
	frame_recv_block.fin_part.dir_online_node[14] = (ptr[11] & 0x40) >> 6;
    frame_recv_block.fin_part.dir_online_node[15] = (ptr[11] & 0x80) >> 7;
	frame_recv_block.fin_part.dir_online_node[16] = (ptr[12] & 0x1);
	frame_recv_block.fin_part.dir_online_node[17] = (ptr[12] & 0x2) >> 1;
	frame_recv_block.fin_part.dir_online_node[18] = (ptr[12] & 0x4) >> 2;
	frame_recv_block.fin_part.dir_online_node[19] = (ptr[12] & 0x8) >> 3;
	frame_recv_block.fin_part.dir_online_node[20] = (ptr[12] & 0x10) >> 4;
	frame_recv_block.fin_part.dir_online_node[21] = (ptr[12] & 0x20) >> 5;
	frame_recv_block.fin_part.dir_online_node[22] = (ptr[12] & 0x40) >> 6;
    frame_recv_block.fin_part.dir_online_node[23] = (ptr[12] & 0x80) >> 7;
	frame_recv_block.fin_part.dir_online_node[24] = (ptr[13] & 0x1);
	frame_recv_block.fin_part.dir_online_node[25] = (ptr[13] & 0x2) >> 1;
	frame_recv_block.fin_part.dir_online_node[26] = (ptr[13] & 0x4) >> 2;
	frame_recv_block.fin_part.dir_online_node[27] = (ptr[13] & 0x8) >> 3;
	frame_recv_block.fin_part.dir_online_node[28] = (ptr[13] & 0x10) >> 4;
	frame_recv_block.fin_part.dir_online_node[29] = (ptr[13] & 0x20) >> 5;
	frame_recv_block.fin_part.dir_online_node[30] = (ptr[13] & 0x40) >> 6;
    frame_recv_block.fin_part.dir_online_node[31] = (ptr[13] & 0x80) >> 7;
}

//3
//name:framing_DNET_RTS
//function: 组建DNET_RTS帧
//by: wyx
void framing_DNET_RTS(UINT8 *ptr)
{
	int i;
	ptr[0] = ((UINT8)frame_send_block.frame_type << 3) + frame_send_block.dnet_rts_part.SubnetID;
	ptr[1] = (UINT8)(frame_send_block.dnet_rts_part.src);
	ptr[2] = ((UINT8)frame_send_block.dnet_rts_part.dest<<3)+ frame_send_block.dnet_rts_part.send_dir;
	for(i=3;i<7;i++)
		ptr[i] = 0;
	for(i=0;i<8;i++)
		ptr[3] += (UINT8)(frame_send_block.dnet_rts_part.potential_neigh[i] << i);
	for(i=0;i<8;i++)
		ptr[4] += (UINT8)(frame_send_block.dnet_rts_part.potential_neigh[i+8] << i);
	for(i=0;i<8;i++)
		ptr[5] += (UINT8)(frame_send_block.dnet_rts_part.potential_neigh[i+16] << i);
	for(i=0;i<8;i++)
		ptr[6] += (UINT8)(frame_send_block.dnet_rts_part.potential_neigh[i+24] << i);
}

//4
//name:resolve_DNET_RTS
//function: 解DNET_RTS帧
//by: wyx
void resolve_DNET_RTS(UINT8 *ptr)
{
	frame_recv_block.dnet_rts_part.SubnetID = (ptr[0] & 0x3);
	frame_recv_block.dnet_rts_part.src = (ptr[1] & 0x1F);
	frame_recv_block.dnet_rts_part.dest = (ptr[2] & 0xF8);
	frame_recv_block.dnet_rts_part.send_dir = (ptr[2] & 0x1);

	frame_recv_block.dnet_rts_part.potential_neigh[0] = (ptr[3] & 0x1);
	frame_recv_block.dnet_rts_part.potential_neigh[1] = (ptr[3] & 0x2) >> 1;
	frame_recv_block.dnet_rts_part.potential_neigh[2] = (ptr[3] & 0x4) >> 2;
	frame_recv_block.dnet_rts_part.potential_neigh[3] = (ptr[3] & 0x8) >> 3;
	frame_recv_block.dnet_rts_part.potential_neigh[4] = (ptr[3] & 0x10) >> 4;
	frame_recv_block.dnet_rts_part.potential_neigh[5] = (ptr[3] & 0x20) >> 5;
	frame_recv_block.dnet_rts_part.potential_neigh[6] = (ptr[3] & 0x40) >> 6;
    frame_recv_block.dnet_rts_part.potential_neigh[7] = (ptr[3] & 0x80) >> 7;
	frame_recv_block.dnet_rts_part.potential_neigh[8] = (ptr[4] & 0x1);
	frame_recv_block.dnet_rts_part.potential_neigh[9] = (ptr[4] & 0x2) >> 1;
	frame_recv_block.dnet_rts_part.potential_neigh[10] = (ptr[4] & 0x4) >> 2;
	frame_recv_block.dnet_rts_part.potential_neigh[11] = (ptr[4] & 0x8) >> 3;
	frame_recv_block.dnet_rts_part.potential_neigh[12] = (ptr[4] & 0x10) >> 4;
	frame_recv_block.dnet_rts_part.potential_neigh[13] = (ptr[4] & 0x20) >> 5;
	frame_recv_block.dnet_rts_part.potential_neigh[14] = (ptr[4] & 0x40) >> 6;
    frame_recv_block.dnet_rts_part.potential_neigh[15] = (ptr[4] & 0x80) >> 7;
	frame_recv_block.dnet_rts_part.potential_neigh[16] = (ptr[5] & 0x1);
	frame_recv_block.dnet_rts_part.potential_neigh[17] = (ptr[5] & 0x2) >> 1;
	frame_recv_block.dnet_rts_part.potential_neigh[18] = (ptr[5] & 0x4) >> 2;
	frame_recv_block.dnet_rts_part.potential_neigh[19] = (ptr[5] & 0x8) >> 3;
	frame_recv_block.dnet_rts_part.potential_neigh[20] = (ptr[5] & 0x10) >> 4;
	frame_recv_block.dnet_rts_part.potential_neigh[21] = (ptr[5] & 0x20) >> 5;
	frame_recv_block.dnet_rts_part.potential_neigh[22] = (ptr[5] & 0x40) >> 6;
    frame_recv_block.dnet_rts_part.potential_neigh[23] = (ptr[5] & 0x80) >> 7;
	frame_recv_block.dnet_rts_part.potential_neigh[24] = (ptr[6] & 0x1);
	frame_recv_block.dnet_rts_part.potential_neigh[25] = (ptr[6] & 0x2) >> 1;
	frame_recv_block.dnet_rts_part.potential_neigh[26] = (ptr[6] & 0x4) >> 2;
	frame_recv_block.dnet_rts_part.potential_neigh[27] = (ptr[6] & 0x8) >> 3;
	frame_recv_block.dnet_rts_part.potential_neigh[28] = (ptr[6] & 0x10) >> 4;
	frame_recv_block.dnet_rts_part.potential_neigh[29] = (ptr[6] & 0x20) >> 5;
	frame_recv_block.dnet_rts_part.potential_neigh[30] = (ptr[6] & 0x40) >> 6;
    frame_recv_block.dnet_rts_part.potential_neigh[31] = (ptr[6] & 0x80) >> 7;
}

//5
//name:framing_ALIGN
//function: 组建ALIGN帧
//by: wyx
void framing_ALIGN(UINT8 *ptr)
{
	ptr[0] = ((UINT8)frame_send_block.frame_type << 3);
	ptr[1] = (UINT8)(frame_send_block.align_part.src);
}

//6
//name:resolve_ALIGN
//function: 解ALIGN帧
//by: wyx
void resolve_ALIGN(UINT8 *ptr)
{
	frame_recv_block.align_part.src = (ptr[1] & 0x1F);
}

//7
//name:framing_D_RTS
//function: 组建D_RTS帧
//by: wyx
void framing_D_RTS(UINT8 *ptr)
{
	ptr[0] = ((UINT8)frame_send_block.frame_type << 3);
	ptr[1] = (UINT8)(frame_send_block.d_rts_part.src);
}

//8
//name:resolve_D_RTS
//function: 解D_RTS帧
//by: wyx
void resolve_D_RTS(UINT8 *ptr)
{
	frame_recv_block.d_rts_part.src = (ptr[1] & 0x1F);
}

//9
//name:framing_D_CTS
//function: 组建D_CTS帧
//by: wyx
void framing_D_CTS(UINT8 *ptr)
{
	ptr[0] = ((UINT8)frame_send_block.frame_type << 3);
	ptr[1] = (UINT8)(frame_send_block.d_cts_part.src);
}

//10
//name:resolve_D_CTS
//function: 解D_CTS帧
//by: wyx
void resolve_D_CTS(UINT8 *ptr)
{
	frame_recv_block.d_cts_part.src = (ptr[1] & 0x1F);
}

//11
//name:framing_QUICK_ALIGN
//function: 组建QUICK_ALIGN帧
//by: wyx
void framing_D_QUICK_ALIGN(UINT8 *ptr)
{
	ptr[0] = ((UINT8)frame_send_block.frame_type << 3);
	ptr[1] = (UINT8)(frame_send_block.quick_align_part.src);
}

//12
//name:resolve_QUICK_ALIGN
//function: 解QUICK_ALIGN帧
//by: wyx
void resolve_D_QUICK_ALIGN(UINT8 *ptr)
{
	frame_recv_block.quick_align_part.src = (ptr[1] & 0x1F);
}

//13 
//name:framing_D_DATA
//function: 组建D_DATA帧
//by: wyx
void framing_D_DATA(UINT8 *ptr)
{
	ptr[0] = ((UINT8)frame_send_block.frame_type << 3);
	ptr[1] = (UINT8)(frame_send_block.d_data_part.src);
	ptr[2] = (UINT8)(frame_send_block.d_data_part.next_hop);
	ptr[3] = (UINT8)(frame_send_block.d_data_part.dest);
	ptr[4] = ((UINT8)frame_send_block.d_data_part.pk_id << 3)+frame_send_block.d_data_part.pk_num;
	ptr[5] = ((UINT8)frame_send_block.d_data_part.priority << 1)+frame_send_block.d_data_part.rate;

	memcpy(&ptr[6], &(frame_send_block.d_data_part.data_buf[0]), sizeof(UINT8)*SEND_DATA_SIZE);
}

//14 
//name:resolve_D_DATA
//function: 解D_DATA帧
//by: wyx
void resolve_D_DATA(UINT8 *ptr)
{
	frame_recv_block.d_data_part.src = (ptr[1]& 0x1F);
	frame_recv_block.d_data_part.next_hop = (ptr[2] & 0x1F);
	frame_recv_block.d_data_part.dest = (ptr[3] & 0x1F);
	frame_recv_block.d_data_part.pk_id = (ptr[4] & 0x38) >> 3;
	frame_recv_block.d_data_part.pk_num = (ptr[4] & 0x7);
	frame_recv_block.d_data_part.priority = (ptr[5] & 0x6) >> 1;
	frame_recv_block.d_data_part.rate = (ptr[5] & 0x1);

	memcpy(&(frame_recv_block.d_data_part.data_buf[0]), &ptr[6], sizeof(UINT8)*RECV_DATA_SIZE);
}

//15
//name:framing_D_ACK
//function: 组建D_ACK帧
//by: wyx
void framing_D_ACK(UINT8 *ptr)
{
	ptr[0] = ((UINT8)frame_send_block.frame_type << 3);
	ptr[1] = (UINT8)(frame_send_block.d_ack_part.src);
}

//16
//name:resolve_D_ACK
//function: 解D_ACK帧
//by: wyx
void resolve_D_ACK(UINT8 *ptr)
{
	ptr[1] = (UINT8)(frame_recv_block.d_ack_part.src);
}
