#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <math.h>
#include <TH1D.h>
#include <TGraph.h>

int test(){

    uint32_t pixdata;
    uint32_t previous_pixdata;
    uint32_t previous_fpga;
    int8_t   retval;
    int64_t   count;
    int64_t   count_extended;

    uint32_t i=0;
    uint8_t c1,c2;
    uint32_t extended_cnt;
    double old_cnt = 0.0;
    double tdc_cnt=0.0;
    double fpga_cnt = 0.0;
    double real_time =0.0;
    double prev_time =0.0;
    double start_time = 0.0;
    double t_diff = 0.0;
    double fpga_current_time = 0.0;
    uint32_t edge_bit;
    double t0 = 0.0;

    // correlation
    double t_scint1;
    double t_scint2;
    double t_scint3;
    double t_mcp;

    int fpga_count = 0;
    int ch0_count = 0;
    int ch1_count = 0;
    int ch2_count = 0;
    int ch3_count = 0;
    int ch4_count = 0;

    vector<double> ch0_vect;
    vector<double> ch1_vect;
    vector<double> ch2_vect;
    vector<double> ch3_vect;
    vector<double> ch4_vect;
    vector<double> t1_vect;
    vector<double> t2_vect;
    vector<double> t_scint1_vect;
    vector<double> t_scint2_vect;
    vector<double> t_scint3_vect;        
    vector<double> t_mcp_vect;

    FILE *fp = fopen("Run3019.dat", "r");

    count = -1;
    while (!feof(fp)){
        retval = fread( &pixdata, sizeof(uint32_t), 1, fp);
        count = count + 1;

        if (retval == 1) {
            // to make sure the file is opened


            if((count&1)==0){
                // every second packet contains data for the extended timestamps, this is every odd timestamp (so first one in the datafile should be fpga)
                c1 = (pixdata>>28)&0xF;
                extended_cnt = (pixdata&0x0FFFFFF8);

            } else {
                // this should be the picotdc packet
                c2      = (pixdata>>27)&0xF;
		        edge_bit = (pixdata>>26)&0x1;
                tdc_cnt = (pixdata&0x03FFFFFF)*3.0517578125;
		        fpga_cnt = (extended_cnt)*1024*25;
                real_time = (extended_cnt)*1024*25+tdc_cnt/1000;

		        printf("Before subtracting t0\n");
		        printf("pixdata: %X \n", pixdata);
		        printf("ch : %u \n", c2);
		        printf("fpga_cnt : %f\n" , fpga_cnt);
		        printf("tdc_cnt : %f ns \n", tdc_cnt);
		        printf("real_time : %f ns \n", real_time);
		        printf("Edge : %u \n", edge_bit);

                if (c2 == 0 && abs(real_time - t0) > 1e3){
                    // "t0" is connected to channel 0
                    printf("Setting t0\n");
                    t0 = real_time;
                }


                // subtract the t0 time
                real_time = real_time - t0;

		        fpga_current_time = (extended_cnt)*1024*25;

		        if (prev_time>fpga_current_time){
		            printf("-----------------------------FLAG--------------------------------\n");
		            real_time = real_time + pow(2,28)*1024*25;
		            fpga_count++;
		            //printf("fpga, ch %u: %u nanoseconds\t %u\n", c2, (extended_cnt)*1024*25, extended_cnt);
                    //printf("pico, ch %u: %f nanoseconds\t0x%.8X\n",c2, tdc_cnt/1000, pixdata);
                    //printf("real time, ch %u: %f nanoseconds\n\n",c2, real_time);
		            //break;
		        }

                if (previous_pixdata ==  0xF0000000){
                    printf("found wrong pico packet, skipping\n\n");
                    continue;
                }

		        //real_time = real_time - t0;

		        printf("After subtracting t0\n");
                printf("fpga, ch %u: %u nanoseconds\t %u\n", c2, (extended_cnt)*1024*25, extended_cnt);
                printf("pico, ch %u: %f nanoseconds\t0x%.8X\n",c2, tdc_cnt/1000, pixdata);
                printf("real time, ch %u: %f nanoseconds\n\n",c2, real_time);

                prev_time = fpga_current_time;

                // Calculate signal width of MCP
		        if (c2==4){
		            if (edge_bit==1){
			            t1_vect.push_back(tdc_cnt/1000);
			            printf("RISING EDGE");
		            }else{
			            t2_vect.push_back(tdc_cnt/1000);
			            printf("FALLING EDGE");
                    }
		        }	

                // Calculate correlation between scintillators and MCP
                if (c2==1){

                    t_scint1 = tdc_cnt/1000;
                    t_scint1_vect.push_back(t_scint1);
                }	

                if (c2==2){

                    t_scint2 = tdc_cnt/1000;
                    t_scint2_vect.push_back(t_scint2);
                }	

                if (c2==3){

                    t_scint3 = tdc_cnt/1000;
                    t_scint3_vect.push_back(t_scint3);
                }	

                if(c2==4){
                    t_mcp = tdc_cnt/1000;
                    t_mcp_vect.push_back(t_mcp);
                }

                // Counting each channel
                if (c2 == 0){
                   ch0_count++;
                   ch0_vect.push_back(real_time);
                   ch1_vect.push_back(0);
                   ch2_vect.push_back(0);
                   ch3_vect.push_back(0);
                   ch4_vect.push_back(0);
                }
		        if (c2 == 1){
		            ch1_count++;
		            ch1_vect.push_back(real_time);
                   ch0_vect.push_back(0);
		            ch2_vect.push_back(0);
                   ch3_vect.push_back(0);
                   ch4_vect.push_back(0);
		        }
		        if (c2 == 2){
		           ch2_count++;
		           ch2_vect.push_back(real_time);
                   ch0_vect.push_back(0);
                   ch1_vect.push_back(0);
                   ch3_vect.push_back(0);
                   ch4_vect.push_back(0);

	            }
		        if (c2 == 3){
		           ch3_count++;
		           ch3_vect.push_back(real_time);
                   ch0_vect.push_back(0);
                   ch1_vect.push_back(0);
                   ch2_vect.push_back(0);
                   ch4_vect.push_back(0);

		        }
                if (c2 == 4){
                   ch4_count++;
                   ch4_vect.push_back(real_time);
                   ch0_vect.push_back(0);
                   ch1_vect.push_back(0);
                   ch2_vect.push_back(0);
                   ch3_vect.push_back(0);

                }

            }

        previous_pixdata = pixdata;
        }

        //if (count > 20){return 1;}
    }

   // Plotting the counters for each channel

   printf("Ch0 count = %d\n",ch0_count);
   printf("Ch1 count = %d\n",ch1_count);
   printf("Ch2 count = %d\n",ch2_count);
   printf("Ch3 count = %d\n",ch3_count);
   printf("Ch4 count = %d\n",ch4_count);

   printf("fpga wrapping over %d times\n", fpga_count);

   const Int_t nch = 5;

   string ch_X[nch] = {"Ch0","Ch1","Ch2","Ch3","Ch4"}; 
   int ch_counts[nch] = {ch0_count,ch1_count,ch2_count,ch3_count,ch4_count};

   auto cb = new TCanvas("cb","cb",600,400);
   cb->SetGrid();

   gStyle->SetHistMinimumZero();
   auto h_ch = new TH1F("h_ch","Channels couting",nch,0,nch);
   h_ch->SetFillColor(4);
   h_ch->SetBarWidth(0.4);
   h_ch->SetBarOffset(0.1);
   h_ch->SetStats(0);

   for (i=1; i<=nch; i++) {
      h_ch->SetBinContent(i, ch_counts[i-1]);
      h_ch->GetXaxis()->SetBinLabel(i,ch_X[i-1].c_str());
   }
 
   h_ch->Draw("b");

   cb->SaveAs("Counting_channels.pdf");

   // Saving time stamps for Ch0,1,2,3,4

   ofstream f_ch0;
   f_ch0.open ("channel0_times.txt");

   ofstream f_ch1;
   f_ch1.open ("channel1_times.txt");

   ofstream f_ch2;
   f_ch2.open ("channel2_times.txt");

   ofstream f_ch3;
   f_ch3.open ("channel3_times.txt");

   ofstream f_ch4;
   f_ch4.open ("channel4_times.txt");

   for (double x : ch0_vect){
        f_ch0 << x << endl;
   }
   f_ch0.close();

   for (double x : ch1_vect){
   	f_ch1 << x << endl;
   }
   f_ch1.close();

   for (float x : ch2_vect){
        f_ch2 << x << endl;
   }
   f_ch2.close();

   for (float x : ch3_vect){
        f_ch3 << x << endl;
   }
   f_ch3.close();

   for (float x : ch4_vect){
        f_ch4 << x << endl;
   }
   f_ch4.close();


   // Saving rising and falling edges of MCP

   ofstream f_t1;
   f_t1.open ("t1_times.txt");

   ofstream f_t2;
   f_t2.open ("t2_times.txt");

   for (float x : t1_vect){
        f_t1 << x << endl;
   }
   f_t1.close();

   for (float x : t2_vect){
        f_t2 << x << endl;
   }
   f_t2.close();


   // Saving Scintillators and MCP timestamps

   ofstream f_t_scint1;
   f_t_scint1.open ("t_scint1.txt");

   ofstream f_t_scint2;
   f_t_scint2.open ("t_scint2.txt");

   ofstream f_t_scint3;
   f_t_scint3.open ("t_scint3.txt");

   ofstream f_t_mcp;
   f_t_mcp.open ("t_mcp.txt");

   for (float x : t_scint1_vect){
        f_t_scint1 << x << endl;
   }
   f_t_scint1.close();

   for (float x : t_scint2_vect){
        f_t_scint2 << x << endl;
   }
   f_t_scint2.close();


   for (float x : t_scint3_vect){
        f_t_scint3 << x << endl;
   }
   f_t_scint3.close();

   for (float x : t_mcp_vect){
        f_t_mcp << x << endl;
   }
   f_t_mcp.close();
 
   return 1;
}
