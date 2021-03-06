// VegCipherBracker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <regex>
#include <algorithm>
#include <map>
#include <numeric>
#include <set>
using namespace std;

#define MAXKEY 12

void TrigramsTest(string dataLow)
{
	map <uint32_t, uint32_t> distances;

	for (size_t i = 0; i < dataLow.size(); i++)
	{
		auto keyStr = dataLow.substr(i, 3);
		auto pos = dataLow.find(keyStr, i);
		auto prevPos = pos;
		while (true)
		{
			pos = dataLow.find(keyStr, ++pos);
			if (pos != string::npos)
			{
				//cout << prevPos << keyStr << pos << '\n';
				//cout << pos-prevPos << keyStr <<  '\n';
				distances[pos - prevPos] = distances[pos - prevPos]++;
				prevPos = pos;
			}
			else
			{
				break;
			}
		}
	}

	map <uint32_t, set<uint32_t>> distDivs;
	map <uint32_t, uint32_t> distHits;

	for_each(distances.begin(), distances.end(), [&](auto p) {
		// Note that this loop runs till square root
		auto n = p.first;
		for (int i = 2; i < MAXKEY; i++)
		{
			if (p.first%i == 0)
			{
					distHits[i] = distHits[i]+p.second;
			}
		}
	});
	if (distHits.size() > 2)
	{
		vector <pair<uint32_t, uint32_t>> ordDistHits;

		for_each(distHits.begin(), distHits.end(),
			[&ordDistHits](auto a)
		{
			ordDistHits.push_back(make_pair(a.first, a.second));
		});

		sort(ordDistHits.begin(), ordDistHits.end(),
			[](auto &a, auto &b) -> bool
		{
			return a.second > b.second;
		});
		if (ordDistHits.size() >= 5)
		{
			for_each(ordDistHits.begin(), ordDistHits.begin() + 5, [](auto tmp) {
				cout << tmp.first << " " << tmp.second << '\n';
			});
		}
		else
		{
			for_each(ordDistHits.begin(), ordDistHits.end(), [](auto tmp) {
				cout << tmp.first << " " << tmp.second << '\n';
			});
		}

	}
	else
	{
		cout << "Shift failed" << '\n';
	}

	cout << '\n';
}

void AutoCorrelation(string dataLow)
{
	auto dataRot = dataLow;
	auto pureSize = dataLow.size();
	vector <pair<uint32_t, double_t>> ordDistHits;
	for (auto keyLen = 0; keyLen <= 12; keyLen++)
	{
		int counter = 0;
		for (auto i = 0; i < dataLow.size(); i++)
		{
			if (dataLow[i] == dataRot[i])
				counter++;
		}
		rotate(dataRot.begin(), dataRot.begin() + 1, dataRot.end());
		//cout << counter << "\t" << keyLen << "\t" << counter /(double)pureSize << endl;
		ordDistHits.push_back(make_pair(keyLen, counter / (double)pureSize));
	}

	sort(ordDistHits.begin(), ordDistHits.end(),
		[](auto &a, auto &b) -> bool
	{
		return a.second > b.second;
	});
	if (ordDistHits.size() >= 5)
	{
		for_each(ordDistHits.begin() + 1, ordDistHits.begin() + 6, [](auto tmp) {
			cout << tmp.first << " " << tmp.second << '\n';
		});
	}
	else
	{
		for_each(ordDistHits.begin() + 1, ordDistHits.end(), [](auto tmp) {
			cout << tmp.first << " " << tmp.second << '\n';
		});
	}
	cout << '\n';
}

void CiCountTest(string dataLow)
{
	vector <pair<uint32_t, double_t>> ordDistHits;
	for (int keyLen = 1; keyLen < 12; keyLen++)
	{
		double ciSum = 0;

		string tmp;
		for (auto i = dataLow.begin();;)
		{
			//cout << *i;
			tmp += *i;
			if (i < dataLow.end() - keyLen)
				i += keyLen;
			else
				break;
		}
		//cout << endl;
		auto elementSize = tmp.size();
		for (char curChar = 'a'; curChar <= 'z'; curChar++)
		{
			auto n = count(tmp.begin(), tmp.end(), curChar);
			double cn = (n * (n - 1));
			double celementSize = (elementSize * (elementSize - 1));
			double ci = cn / celementSize;
			ciSum += ci;
			//cout << curChar << " : " << n << " : " << ci << endl;
		}
		//cout << "Size\t" << elementSize << "\t";
		//cout << keyLen << "\tciSum\t" << ciSum << endl;
		ordDistHits.push_back(make_pair(keyLen, ciSum));
	}

	sort(ordDistHits.begin(), ordDistHits.end(),
		[](auto &a, auto &b) -> bool
	{
		return a.second > b.second;
	});
	if (ordDistHits.size() >= 5)
	{
		for_each(ordDistHits.begin(), ordDistHits.begin() + 5, [](auto tmp) {
			cout << tmp.first << " " << tmp.second << '\n';
		});
	}
	else
	{
		for_each(ordDistHits.begin() + 1, ordDistHits.end(), [](auto tmp) {
			cout << tmp.first << " " << tmp.second << '\n';
		});
	}
}

int main()
{
	/*SYSTEM*/string input = "\
		LFWKI MJCLP SISWK HJOGL KMVGU RAGKM KMXMA MJCVX WUYLG GIISW\
		ALXAE YCXMF KMKBQ BDCLA EFLFW KIMJC GUZUG SKECZ GBWYM OACFV\
		MQKYF WXTWM LAIDO YQBWF GKSDI ULQGV SYHJA VEFWB LAEFL FWKIM\
		JCFHS NNGGN WPWDA VMQFA AXWFZ CXBVE LKWML AVGKY EDEMJ XHUXD\
		AVYXL. ";
	//string input = "LIOMWGFEGGDVWGHHCQUCRHRWAGWIOWQLKGZETKKMEVLW\
		PCZVG'TH VTSGXQOVGCSVETQLTJSUMVWVEUVLXEWSLGFZMVVWLGYHCUSWX\
		QH - KVGSHEEVFLCFDGVSUMPHKIRZDMPHHBVWVWJWIXGFWLTSHGJOUEEH\
		H- VUCFVGOWICQLIJSUXGLW";
	///*BOY*/string input = "NWAIW EBBRF QFOCJ PUGDO JVBGW SPTWR Z";
	//string input = "UTPDHUG NYH USVKCG МУСЕ FXL KQIB. WX RKU GI TZN, RLS BHZLXMSNP KDKS; СЕВ Ш HKEWIBA, YYM SRB PER SBS, JV UPL О UVADGR HRRWXF. JV ZTVOOV УН ZCQU У UKWGEB, PL UQFB Р FOUKCG, TBF RQ VHCF R KPG, 0U КЕТ ZCQU MAW QKKW ZGSY, ЕР PGM QKETK UQEB DER EZRN, MCYE, MG UCTESVA, WP КЕТ ZCQU MAW KOIJS, LCOV NTHDNV JPNUJVB Ш GGV RWX ONKCGTHKFL XG VKD, ZJM VG CCI MVGD JPNUJ, RLS EWVKJT ASGUCS MVGD; DDK VG NYH PWUV CCHIIY RD DBQN RWTH PFRWBBI VTTK VCGNTGSF FL IAWU XJDUS, HFP VHSF, RR LAWEY QDFS RVMEES FZB СНН JRTT MVGZP UBZN FD ATIIYRTK WP КЕТ HIVJCI; TBF BLDPWPX RWTH ULAW TG VYCHX KQLJS US DCGCW OPPUPR, VG KFDNUJK GI JIKKC PL KGCJ lAOV KFTR GJFSAW KTZLZES WG RWXWT VWTL WP XPXGG, CJ EPOS VYC BTZCUW XG ZGJQ PMHTRAIBJG WMGEG. JZQ DPB JVYGM ZCLEWXR:CEB lAOV NYH JIKKC TGCWXE UHE JZK. WX VCULD YTTKETK WPKCGVCWIQT PWVY QEBFKKQ, QNH NZTTWIREL IAS VERPE ODJRXGSPTC EKWPTGEES, GMCG TTVVPLTEEJ; YCW WV NYH TZYRWH LOKU MU AWO, KEPM VG BLTP VQN RD DSGG AWKWUKKPL KGCJ, XY GPP KPG ONZTT ICUJCHLSE KET DBQHJTWUG. DYN MVCK ZT MEWCW HTWE ED JL, GPU YAE CH LQ! PGR UE, YH MWPP RXE CDJCGOSE, XMS UZGJQJL, SXVPN HBG!";
	///*required*/string input = "klym qj e erwu wtrwv kicjtrxh klqn krr ev yiyl ks gvvypm zxhiejiz tpdjwum nisp zx. yn qj rrk ed cbvvdksh wtrwv rrt xwvw qfx flwmmgv eds ww xkv jkhkkmreebcbp eq zxuliksu zw urxvgwvh ji przh.klym jrwh tpqma frop thidzhhj wegm dipsih nggiv, nlywp zr irgj uzv rrk vukczvhu xe vm gvhjidn qe eqp mjyzrxri xojm(zxhiejiz kcsvw xudv rr jtuwqwmf dicvmi vhhyylmdiqkw), rob klhp qyapk fh lwuzcc, wlegu npvc gvjyhm klh dicvmiw qvityl wsu klu xmwexcx ynmiewfv_jlizxv tpqma kipgpqnm ks jvruliki wyi qjxissimqnm zrvkednqrxlfr qobfqdkmsutcc(deh ioky mqjxqhbzewzsd ca iitlmhyl ks ev zqfqu jri ebf qkiurxel bpthj).";

	auto origSize = input.size();
	input = regex_replace(input, regex("\\s+"), "");
	auto whitespaceSize = input.size();
	input = regex_replace(input, regex("\\d+"), "");
	auto numberSize = input.size();
	input = regex_replace(input, regex("\\W+"), "");
	auto pureSize = input.size();
	cout << input<< endl;
	cout << "origSize "<<origSize<<endl
		<< "whitespace "<<origSize - whitespaceSize<<endl
		<< "numbers "<<whitespaceSize - numberSize<<endl
		<< "restOfSumbols " << numberSize - pureSize << endl
		<< "pureSize " << pureSize << endl;
	cout << '\n';
	auto dataLow = input;
	transform(dataLow.begin(), dataLow.end(), dataLow.begin(), ::tolower);

	AutoCorrelation(dataLow);
	TrigramsTest(dataLow);
	CiCountTest(dataLow);
	
	


	

	//cin >> a;
    return 0;
}

