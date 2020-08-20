#define _CRT_SECURE_NO_WARNINGS
#include "wave.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//出典	http://hooktail.org/computer/index.php?Wave%A5%D5%A5%A1%A5%A4%A5%EB%A4%F2%C6%FE%BD%D0%CE%CF%A4%B7%A4%C6%A4%DF%A4%EB

/********************************************************/
/*														*/
/* Read_Wave関数										*/
/*														*/
/* 引数													*/
/*	char* filename	 読み込むファイル名を示すポインタ	*/
/*														*/
/*	戻り値	Sound*	（構造体）ポインタ					*/
/*														*/
/*	Waveファイルを読み込んでメモリ上に展開する			*/
/********************************************************/

Sound *Read_Wave(char *filename)
{
	unsigned int i;
	unsigned char header_buf[20]; //フォーマットチャンクのサイズまでのヘッダ情報を取り込む
	FILE *fp;
	Sound *snd;
	unsigned long datasize;		   //波形データのバイト数
	unsigned short fmtid;		   //fmtのIDを格納する
	unsigned short channelnum;	   //チャンネル数
	unsigned long samplingrate;	   //サンプリング周波数
	unsigned short bit_per_sample; //量子化ビット数
	unsigned char *buf;			   //フォーマットチャンクIDから拡張部分までのデータを取り込む
	unsigned long fmtsize;

	//ファイルオープン（バイナリ読み込みモード）
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		//ファイルオープンに失敗したらエラー表示してNULLを返す
		fprintf(stderr, "Error: %s could not read.", filename);
		return NULL;
	}

	fread(header_buf, sizeof(unsigned char), 20, fp); //フォーマットチャンクサイズまでのヘッダ部分を取り込む

	//ファイルがRIFF形式であるか
	if (strncmp(header_buf, "RIFF", 4))
	{
		//ファイルタイプが"RIFF"でないならばエラー表示してファイルを閉じ、NULLを返す
		fprintf(stderr, "Error: %s is not RIFF.", filename);
		fclose(fp);
		return NULL;
	}

	//ファイルがWAVEファイルであるか
	if (strncmp(header_buf + 8, "WAVE", 4))
	{
		//RIFFタイプが"WAVE"でないならばエラー表示してファイルを閉じ、NULLを返す
		fprintf(stderr, "Error: %s is not WAVE.", filename);
		fclose(fp);
		return NULL;
	}

	//fmt のチェック
	//RIFFヘッダ(12バイト固定)の直後はfmtチャンクのはず
	//FormatチャンクIDが"fmt "であることを確認
	if (strncmp(header_buf + 12, "fmt ", 4))
	{
		//FormatチャンクIDが"fmt "でないならばエラー表示してファイルを閉じ、NULLを返す
		fprintf(stderr, "Error: %s fmt not found.", filename);
		fclose(fp);
		return NULL;
	}

	//Formatチャンク直後の4バイト（fmtチャンクサイズ）を取得
	//注意：unsigned long型が4バイトであることを想定した実装なので望ましい書き方ではない
	memcpy(&fmtsize, header_buf + 16, sizeof(fmtsize));

	//fmtチャンクの残りの部分を取り込むメモリ領域を確保して、先頭ポインタをbufに代入
	if ((buf = (unsigned char *)malloc(sizeof(unsigned char) * fmtsize)) == NULL)
	{
		fprintf(stderr, "Allocation error\n");
		fclose(fp);
		return NULL;
	}

	//フォーマットIDから拡張部分までのヘッダ部分を取り込む
	fread(buf, sizeof(unsigned char), fmtsize, fp);

	//フォーマットIDを取得
	//注意：unsigned short型が2バイトであることを想定した実装なので望ましい書き方ではない
	memcpy(&fmtid, buf, sizeof(fmtid)); //LinearPCMファイルならば1が入る

	if (fmtid != 1)
	{
		//リニアPCM以外は対応できないのでエラー表示してファイルを閉じ、NULLを返す
		fprintf(stderr, "Error: %s is not LinearPCM.", filename);
		free(buf); //mallocした領域を解放しないのは行儀が悪いので追加
		fclose(fp);
		return NULL;
	}

	//チャンネル数・サンプリング周波数・量子化ビット数を取得
	//注意：unsigned short型が2バイト／unsigned long型が4バイトであることを
	//想定した実装なので望ましい書き方ではない
	memcpy(&channelnum, buf + 2, sizeof(channelnum));		   //チャンネル数を取得
	memcpy(&samplingrate, buf + 4, sizeof(samplingrate));	   //サンプリング周波数を取得
	memcpy(&bit_per_sample, buf + 14, sizeof(bit_per_sample)); //量子化ビット数を取得

	//次のチャンクのIDとサイズを取得
	fread(buf, sizeof(unsigned char), 8, fp); //factもしくはdataのIDとサイズを取得8バイト

	//factチャンク（12バイト）が続いていたら次のチャンクまで読み飛ばす
	if (!strncmp(buf, "fact", 4))
	{
		fread(buf, sizeof(unsigned char), 4, fp); //factチャンクの残りを読んで
		fread(buf, sizeof(unsigned char), 8, fp); //次のチャンクのIDとサイズを取得
	}

	//これでdataチャンクにたどり着いているはず
	//チャンクIDが"data"であることを確認
	if (strncmp(buf, "data", 4))
	{
		//チャンクIDが"data"でないならばエラー表示してファイルを閉じ、NULLを返す
		fprintf(stderr, "Error: %s data part not found.", filename);
		free(buf); //mallocした領域を解放しないのは行儀が悪いので追加
		fclose(fp);
		return NULL;
	}

	//波形データのサイズの取得
	memcpy(&datasize, buf + 4, sizeof(datasize));

	//Sound構造体（および波形保存領域）を生成
	if ((snd = Create_Sound(channelnum, samplingrate, bit_per_sample, datasize)) == NULL)
	{
		//メモリ確保に失敗したら終了
		fclose(fp);
		return NULL;
	}

	//フォーマットに応じて波形情報を取り込む
	if (channelnum == 1 && bit_per_sample == 8)
	{
		//モノラル8bitの場合
		fread(snd->monaural8, sizeof(unsigned char), snd->datanum, fp); //データ部分を全て取り込む
	}
	else if (channelnum == 1 && bit_per_sample == 16)
	{
		//モノラル16bitの場合
		fread(snd->monaural16, sizeof(signed short), snd->datanum, fp);
	}
	else if (channelnum == 2 && bit_per_sample == 8)
	{
		//ステレオ8bitの場合
		for (i = 0; i < snd->datanum; i++)
		{
			fread(&(snd->stereo8[i].l), sizeof(unsigned char), 1, fp);
			fread(&(snd->stereo8[i].r), sizeof(unsigned char), 1, fp);
		}
	}
	else if (channelnum == 2 && bit_per_sample == 16)
	{
		//ステレオ16bitの場合
		for (i = 0; i < snd->datanum; i++)
		{
			fread(&(snd->stereo16[i].l), sizeof(signed short), 1, fp);
			fread(&(snd->stereo16[i].r), sizeof(signed short), 1, fp);
		}
	}
	else
	{
		//それ以外は未対応なので処理を終了する
		fprintf(stderr, "Header is destroyed.");
		free(buf); //mallocした領域を解放しないのは行儀が悪いので追加
		fclose(fp);
		Free_Sound(snd);
	}

	//データの格納が終了したらSound構造体をリターン
	return snd;
}

/********************************************************/
/*														*/
/* Write_Wave関数										*/
/*														*/
/* 引数													*/
/*	char* filename	 読み込むファイル名を示すポインタ	*/
/*	Sound*	snd		（構造体）ポインタ					*/
/*														*/
/*	戻り値	int		処理結果（0:正常終了)				*/
/*														*/
/*	メモリ上の波形をWaveファイルとして保存する			*/
/********************************************************/

int Write_Wave(char *filename, Sound *snd)
{
	int i;
	FILE *fp;
	unsigned char header_buf[HEADERSIZE]; //ヘッダを格納する
	unsigned long fswrh;				  //リフヘッダ以外のファイルサイズ
	unsigned long fmtchunksize;			  //fmtチャンクのサイズ
	unsigned long dataspeed;			  //データ速度
	unsigned short blocksize;			  //1ブロックあたりのバイト数
	unsigned long datasize;				  //周波数データのバイト数
	unsigned short fmtid;				  //フォーマットID

	//ファイルオープン（バイナリ書き込みモード）
	if ((fp = fopen(filename, "wb")) == NULL)
	{
		//ファイルオープンに失敗したらエラー表示して1を返す
		fprintf(stderr, "Error: %s could not open.", filename);
		return 1;
	}

	//各種情報の生成
	fmtchunksize = 16;
	blocksize = snd->channelnum * (snd->bit_per_sample / 8);
	dataspeed = snd->samplingrate * blocksize;
	datasize = snd->datanum * blocksize;
	fswrh = datasize + HEADERSIZE - 8;
	fmtid = 1;

	//RIFFヘッダの生成
	header_buf[0] = 'R';
	header_buf[1] = 'I';
	header_buf[2] = 'F';
	header_buf[3] = 'F';
	memcpy(header_buf + 4, &fswrh, sizeof(fswrh));
	header_buf[8] = 'W';
	header_buf[9] = 'A';
	header_buf[10] = 'V';
	header_buf[11] = 'E';
	//fmtチャンクの生成
	header_buf[12] = 'f';
	header_buf[13] = 'm';
	header_buf[14] = 't';
	header_buf[15] = ' ';
	memcpy(header_buf + 16, &fmtchunksize, sizeof(fmtchunksize));
	memcpy(header_buf + 20, &fmtid, sizeof(fmtid));
	memcpy(header_buf + 22, &(snd->channelnum), sizeof(snd->channelnum));
	memcpy(header_buf + 24, &(snd->samplingrate), sizeof(snd->samplingrate));
	memcpy(header_buf + 28, &dataspeed, sizeof(dataspeed));
	memcpy(header_buf + 32, &blocksize, sizeof(blocksize));
	memcpy(header_buf + 34, &(snd->bit_per_sample), sizeof(snd->bit_per_sample));

	//dataチャンクの生成
	header_buf[36] = 'd';
	header_buf[37] = 'a';
	header_buf[38] = 't';
	header_buf[39] = 'a';
	memcpy(header_buf + 40, &datasize, sizeof(datasize));

	//メモリ上のヘッダ情報をファイルに書き込む
	fwrite(header_buf, sizeof(unsigned char), HEADERSIZE, fp);

	//波形部分をファイルに書き込む
	if (snd->channelnum == 1 && snd->bit_per_sample == 8)
	{
		//モノラル8bitの場合
		fwrite(snd->monaural8, sizeof(unsigned char), snd->datanum, fp); //データ部分を全て書き込む
	}
	else if (snd->channelnum == 1 && snd->bit_per_sample == 16)
	{
		//モノラル16bitの場合
		fwrite(snd->monaural16, sizeof(signed short), snd->datanum, fp);
	}
	else if (snd->channelnum == 2 && snd->bit_per_sample == 8)
	{
		//ステレオ8bitの場合
		for (i = 0; i < snd->datanum; i++)
		{
			fwrite(&(snd->stereo8[i].l), sizeof(unsigned char), 1, fp);
			fwrite(&(snd->stereo8[i].r), sizeof(unsigned char), 1, fp);
		}
	}
	else
	{
		//ステレオ16bitの場合
		for (i = 0; i < snd->datanum; i++)
		{
			fwrite(&(snd->stereo16[i].l), sizeof(signed short), 1, fp);
			fwrite(&(snd->stereo16[i].r), sizeof(signed short), 1, fp);
		}
	}

	//ファイルクローズ
	fclose(fp);

	//保存完了したら0をリターン(正常終了)
	return 0;
}

/********************************************************/
/*														*/
/* Create_Sound関数										*/
/*														*/
/* 引数													*/
/*	ushort	channelnum		チャンネル数				*/
/*	ulong	samplingrate	サンプリングレート			*/
/*	ushort	bit_per\sample	ビット速度					*/
/*	ulong	datasize		データサイズ				*/
/*														*/
/*	戻り値	Sound*	（構造体）ポインタ					*/
/*														*/
/*	メモリ上にSound*構造体と波形格納用の				*/
/*	メモリ領域を確保し、Sound*構造体を返す				*/
/********************************************************/

Sound *Create_Sound(unsigned short channelnum, unsigned long samplingrate, unsigned short bit_per_sample, unsigned long datasize)
{
	Sound *snd;

	//Sound構造体のメモリ領域を確保
	if ((snd = (Sound *)malloc(sizeof(Sound))) == NULL)
	{
		//メモリ確保に失敗したら終了
		fprintf(stderr, "Allocation error\n");
		return NULL;
	}

	//Sound構造体に各データを格納
	snd->channelnum = channelnum;
	snd->samplingrate = samplingrate;
	snd->bit_per_sample = bit_per_sample;
	snd->datanum = datasize / (channelnum * (bit_per_sample / 8));

	//データ格納ポインタを一度NULLでリセット
	snd->monaural8 = NULL;
	snd->monaural16 = NULL;
	snd->stereo8 = NULL;
	snd->stereo16 = NULL;

	//波形格納用領域の確保
	if (channelnum == 1 && bit_per_sample == 8)
	{
		//モノラル8bitの場合
		if ((snd->monaural8 = (unsigned char *)malloc(datasize)) == NULL)
		{
			//メモリ確保に失敗したら終了
			fprintf(stderr, "Allocation error\n");
			free(snd);
			return NULL;
		}
	}
	else if (channelnum == 1 && bit_per_sample == 16)
	{
		//モノラル16bitの場合
		if ((snd->monaural16 = (signed short *)malloc(sizeof(signed short) * snd->datanum)) == NULL)
		{
			fprintf(stderr, "Allocation error\n");
			//メモリ確保に失敗したら終了
			free(snd);
			return NULL;
		}
	}
	else if (channelnum == 2 && bit_per_sample == 8)
	{
		//ステレオ8bitの場合
		if ((snd->stereo8 = (Soundsample8 *)malloc(sizeof(Soundsample8) * snd->datanum)) == NULL)
		{
			fprintf(stderr, "Allocation error\n");
			//メモリ確保に失敗したら終了
			free(snd);
			return NULL;
		}
	}
	else if (channelnum == 2 && bit_per_sample == 16)
	{
		//ステレオ16bitの場合
		if ((snd->stereo16 = (Soundsample16 *)malloc(sizeof(Soundsample16) * snd->datanum)) == NULL)
		{
			fprintf(stderr, "Allocation error\n");
			//メモリ確保に失敗したら終了
			free(snd);
			return NULL;
		}
	}
	else
	{
		//想定外のちゃんる数・ビットレートの場合は終了
		fprintf(stderr, "Channelnum or Bit/Sample unknown");
		free(snd);
		return NULL;
	}

	//確保完了したらSound構造体のポインタをリターン
	return snd;
}

/********************************************************/
/*														*/
/* Free_Sound関数										*/
/*														*/
/* 引数													*/
/*	Sound*	snd		（構造体）ポインタ					*/
/*														*/
/*	戻り値	void										*/
/*														*/
/*	メモリ上にSound*構造体と波形格納用の				*/
/*	メモリ領域を解放する								*/
/********************************************************/

void Free_Sound(Sound *snd)
{
	if (snd->channelnum == 1 && snd->bit_per_sample == 8)
	{
		free(snd->monaural8);
	}
	else if (snd->channelnum == 1 && snd->bit_per_sample == 16)
	{
		free(snd->monaural16);
	}
	else if (snd->channelnum == 2 && snd->bit_per_sample == 8)
	{
		free(snd->stereo8);
	}
	else
	{
		free(snd->stereo16);
	}

	free(snd);
}
