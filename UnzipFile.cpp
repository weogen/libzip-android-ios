#include <zip.h>
#include<stream>
#include <stdio.h> //remove
#include<fstream>


bool UnzipFile(string read_path,string write_path)
{
    //打开zip文件
    int open_err = 0;
    string zip_file_path=read_path+"/resource.zip";//压缩包路径
    struct zip *z_archive = zip_open(zip_file_path.c_str(), 0, &open_err);
  
    if (!z_archive)
    {
        zip_error_t init_err;
        zip_error_init_with_code(&init_err, open_err);
        std::cerr << "Can't open zip file '" << zip_file_path << "' : " << zip_error_strerror(&init_err)
        << " (err_code: " << open_err << ")" << std::endl;
        return false;
    }
    //设置密码
    string password="123456";
    if (zip_set_default_password(z_archive, password.c_str()) != 0)
    {
        std::cerr << "Error setting zip file password "<<password<<"..." << std::endl;
        return false;
    }
    
    //依次获取buffer并写到目录
    vector<string> vec_unzip_file_path;
    
    zip_uint64_t file_numb = static_cast<zip_uint64_t>(zip_get_num_entries(z_archive, 0));
    for (zip_uint64_t i = 0; i < file_numb; ++i)
    {
        struct zip_stat z_stat;
        if (zip_stat_index(z_archive, i, 0, &z_stat) == 0)
        {
            //文件名,原始大小,压缩后大小,压缩方式,加密方式
            cout << "file_info:" << z_stat.name << ", "<< z_stat.size<<","<<z_stat.comp_size<<","<<z_stat.comp_method<<","<<z_stat.encryption_method<< endl;
            
            if (z_stat.name[strlen(z_stat.name) - 1] == '/')//路径为文件夹,创建文件夹
            {
                //mkdir(z_stat.name,S_IRWXU);//
            }
            else
            {
                char *buffer = new char[z_stat.size];
                string file_out_path=write_path+"/"+z_stat.name;
                vec_unzip_file_path.push_back(file_out_path);
                
                ofstream ofs_dst_file(file_out_path,ios::out | ios::binary);
                struct zip_file *z_file = zip_fopen_index(z_archive, i, ZIP_FL_UNCHANGED);//0
              
                if (z_file)
                {
                    zip_int64_t fr_numb=zip_fread(z_file, buffer,z_stat.size);
                    ofs_dst_file.write(buffer,z_stat.size);
                    //防止文件过大多次写buffer
//                    zip_uint64_t sum = 0;
//                    while (sum != z_stat.size)
//                    {
//                        char buffer[2048];
//                        //cout<<"sum:"<<sum<<endl;
//                        zip_int64_t len = zip_fread(z_file, static_cast<void*>(buffer), sizeof(buffer));
//                        if (len > 0)
//                        {
//                            //printf("buffer:%s",buffer);
//                            ofs_dst_file.write(buffer,sizeof(buffer));
//                            sum += static_cast<zip_uint64_t>(len);
//                        }
//                        else
//                        {
//                            std::cerr << "Error reading encrypted file..." << std::endl;
//                            break;
//                        }
//                    }
                    delete []buffer;
                    ofs_dst_file.close();
                    zip_fclose(z_file);
                    
                }
                else
                {
                    std::cerr << "Error opening encrypted file.. (wrong password?)" << std::endl;
                    return false;
                }
            }
        }
        else
        {
            std::cerr << "Error getting information on file with index: " << i << std::endl;
            return false;
        }
    }
		//解压后删除
		for(size_t i=0;i<vec_unzip_file_path.size();i++)
    {

        if(remove(vec_unzip_file_path[i].c_str())!=0)
        {
            cout<<"delete file:"<<vec_unzip_file_path[i]<<" failed.."<<endl;
            return false;
        }
        
    }
		return true;

}
