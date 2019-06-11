#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <vector>

#define MAX_SIZE 900000
#define SIZE sizeof(uint64_t)

struct Info
{
    std::ifstream reading;
    std::ofstream writing;
    std::string name;
    uint64_t size_ = 0;
};

uint64_t Numbers_in_file(const std::string &name)
{
    std::streampos begin,end;
    std::ifstream file (name, std::ios_base::binary);
    begin = file.tellg();
    file.seekg (0, std::ios_base::end);
    end = file.tellg();
    return (end - begin) / SIZE;
}

void Print(const std::string &name)
{
    std::unique_ptr<FILE, int(*)(FILE*)> f(fopen(name.c_str(), "rb"), &fclose);
    uint64_t x;
    for (size_t i = 0; i < Numbers_in_file(name); i++)
    {
        fread(&x, sizeof(x), 1, f.get());
        std::cout << x << '\n';
    }
}

Info& Merge_Files(Info& left, Info& right, uint64_t mark)
{
    std::string name = "temp" + std::to_string(mark) + ".bin";
    std::ofstream write_data(name, std::ios_base::binary);
    left.reading.open(left.name, std::ios_base::binary);
    right.reading.open(right.name, std::ios_base::binary);

    uint64_t left_num;
    uint64_t right_num;
    uint64_t leftLen = left.size_;
    uint64_t rightLen = right.size_;
    uint64_t allLen = leftLen + rightLen;

    left.reading.read((char*)&left_num, SIZE);
    right.reading.read((char*)&right_num, SIZE);

    while (leftLen && rightLen)
    {
        if (left_num <= right_num)
        {
            write_data.write((char*)&left_num, SIZE);
            left.reading.read((char*)&left_num, SIZE);
            leftLen--;
        }
        else
        {
            write_data.write((char*)&right_num, SIZE);
            right.reading.read((char*)&right_num, SIZE);
            rightLen--;
        }
    }

    while (leftLen)
    {
        left.reading.read((char*)&left_num, SIZE);
		write_data.write((char*)&left_num, SIZE);
		leftLen--;
	}

	while (rightLen)
	{
        right.reading.read((char*)&right_num, SIZE);
        write_data.write((char*)&right_num, SIZE);
        rightLen--;
	}

	left.reading.close();
	right.reading.close();
	write_data.close();

	std::ifstream readBox(name, std::ios_base::binary);
	left.writing.open(left.name, std::ios_base::binary);

	while (allLen)
    {
        readBox.read((char*)&left_num, SIZE);
        left.writing.write((char*)&left_num, SIZE);
        allLen--;
    }

    readBox.close();
    left.writing.close();
    left.size_ += right.size_;
    remove(name.c_str());
    return left;
}

Info& Sort_Files(std::vector<Info>& files, uint64_t l, uint64_t r, const uint64_t mark) {
    if (l < r)
    {
	uint64_t m = l + (r - l) / 2;
	Info& left = Sort_Files(files, l, m, mark);
	Info& right = Sort_Files(files, m + 1, r, mark);
	return Merge_Files(left, right, mark);
	}
    return files[l];
}

void Merge(std::vector<uint64_t>& A, uint64_t l, uint64_t m, uint64_t r)
{
    uint64_t i, j, k;
    uint64_t left_len = m - l + 1;
    uint64_t right_len = r - m;
    std::vector<uint64_t> L(left_len);
    std::vector<uint64_t> R(right_len);

    for (i = 0; i < left_len; i++)
    {
        L[i] = A[l + i];
    }
    for (j = 0; j < right_len; j++)
    {
	    R[j] = A[m + 1 + j];
    }

    i = 0;
    j = 0;
    k = l;

    while (i < left_len && j < right_len)
    {
        if (L[i] <= R[j])
        {
            A[k++] = L[i++];
	    }
    else
    {
        A[k++] = R[j++];
    }
    }

    while (i < left_len)
    {
	    A[k++] = L[i++];
    }

    while (j < right_len)
    {
	    A[k++] = R[j++];
    }
}

void MergeSort(std::vector<uint64_t>& A, uint64_t l, uint64_t r)
{
    if (l < r)
    {
        uint64_t m = l + (r - l) / 2;
        MergeSort(A, l, m);
        MergeSort(A, m + 1, r);
        Merge(A, l, m, r);
    }
}

std::vector<Info> make_files(uint64_t file_num)
{
    std::ifstream fin("input.bin", std::ios_base::binary);
    std::vector<uint64_t> data(MAX_SIZE);
    std::vector<Info> files(file_num);

    for (size_t i = 0; i < file_num; i++)
    {
        while (!fin.eof() && files[i].size_ < MAX_SIZE)
        {
            fin.read((char*)&data[files[i].size_], SIZE);
            files[i].size_++;
        }
        files[i].name = "file" + std::to_string(i) + ".bin";
        MergeSort(data, 0, files[i].size_ - 1);
        files[i].writing.open(files[i].name, std::ios_base::binary);
        for (size_t j = 0; j < files[i].size_; j++)
            files[i].writing.write((char*)&data[j], SIZE);
        files[i].writing.close();
    }
    return files;
}

int main(void)
{
    uint64_t size_ = Numbers_in_file("input.bin");
    uint64_t file_num = size_ / MAX_SIZE;
    if (size_ % MAX_SIZE)
        file_num++;
    std::vector<Info> files = make_files(file_num);
    if (file_num != 1)
        {
            uint64_t m = file_num / 2 - 1;
            std::thread left(Sort_Files, std::ref(files), 0, m, 1);
            std::thread right(Sort_Files, std::ref(files), m + 1, file_num - 1, 2);
            left.join();
            right.join();
            Merge_Files(files[0], files[m + 1], 0);
        }
    Print("file0.bin");
    return 0;
}
