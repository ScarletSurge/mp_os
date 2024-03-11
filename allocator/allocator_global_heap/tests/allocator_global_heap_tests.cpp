#include <gtest/gtest.h>
#include <iostream>
#include <allocator_global_heap.h>
#include <client_logger_builder.h>
#include <logger.h>
#include <logger_builder.h>

TEST(allocatorGlobalHeapTests, test1)
{
    logger_builder *logger_builder_instance = new client_logger_builder;
    
    logger *logger_instance = logger_builder_instance
        ->add_file_stream("gh_alc_test1_logs.txt", logger::severity::debug)
        ->build();
    delete logger_builder_instance;
    
    allocator *allocator_instance = new allocator_global_heap(logger_instance);
    auto block = reinterpret_cast<int *>(allocator_instance->allocate(sizeof(unsigned char), 0));
    delete allocator_instance;
    allocator *allocator_another_instance = new allocator_global_heap(logger_instance);
    allocator_another_instance->deallocate(block);
    delete allocator_another_instance;
    
    delete logger_instance;
}

TEST(allocatorGlobalHeapTests, test2)
{
    
    logger_builder *logger_builder_instance = new client_logger_builder;
    
    logger *logger_instance = logger_builder_instance
        ->add_file_stream("gh_alc_test2_logs.txt", logger::severity::debug)
        ->build();
    delete logger_builder_instance;
    
    allocator *allocator_instance = new allocator_global_heap;
    
    auto first_block = reinterpret_cast<char *>(allocator_instance->allocate(sizeof(char), 11));
    
    strcpy(first_block, "0123456789");
    
    allocator_instance->deallocate(first_block);
    delete allocator_instance;
    delete logger_instance;
}

TEST(allocatorGlobalHeapTests, test3)
{
    int const values_to_allocate_count = 30;
    
    allocator *allocator_instance = new allocator_global_heap(nullptr);
    
    auto first_block = reinterpret_cast<unsigned int *>(allocator_instance->allocate(sizeof(unsigned int), values_to_allocate_count));
    auto second_block = reinterpret_cast<unsigned int *>(allocator_instance->allocate(sizeof(unsigned long int), values_to_allocate_count));
    auto third_block = reinterpret_cast<double *>(allocator_instance->allocate(sizeof(double), values_to_allocate_count));
    
    for (int i = 0; i < values_to_allocate_count; i++)
    {
        first_block[i] = i;
        second_block[i] = i * i;
        third_block[i] = i / 2;
    }
    
    allocator_instance->deallocate(third_block);
    allocator_instance->deallocate(first_block);
    allocator_instance->deallocate(second_block);
    
    delete allocator_instance;
}

TEST(allocatorGlobalHeapTests, test4)
{
    int const values_to_allocate_count = 75;
    
    allocator *allocator_instance = new allocator_global_heap;
    
    auto first_block = reinterpret_cast<short *>(allocator_instance->allocate(sizeof(short), values_to_allocate_count));
    auto second_block = reinterpret_cast<float *>(allocator_instance->allocate(sizeof(float), values_to_allocate_count));
    
    for (int i = 0; i < values_to_allocate_count; i++)
    {
        second_block[i] = first_block[i] = (i & 1)
            ? i / 2.0
            : i;
    }
    
    allocator_instance->deallocate(first_block);
    allocator_instance->deallocate(second_block);
    
    delete allocator_instance;
}

TEST(allocatorGlobalHeapTests, test5)
{
    struct struct_metainfo
    {
        int _int_value;
        
        std::string _string_value;
        
        struct_metainfo(
            int int_value,
            std::string const &string_value):
            _int_value(int_value),
            _string_value(string_value)
        {
        
        }
    };
    
    allocator *allocator_instance = new allocator_global_heap;
    
    struct_metainfo *metainfo_instance = reinterpret_cast<struct_metainfo *>(allocator_instance->allocate(sizeof(struct_metainfo), 1));
    allocator::construct<struct_metainfo>(metainfo_instance, 10, std::string("string"));
    struct_metainfo new_obj = std::move(*metainfo_instance);
    allocator::destruct<struct_metainfo>(metainfo_instance);
    allocator_instance->deallocate(metainfo_instance);
    
    delete allocator_instance;
}

class A final
{

private:
    
    std::string _value;
    
    char *_value2;

public:
    
    A(
        std::string const &value,
        char const *value2):
        _value(value)
    {
        _value2 = new char[strlen(value2) + 1];
        strcpy(_value2, value2);
    }
    
    A(
        A const &obj):
        _value(obj._value)
    {
        _value2 = new char[strlen(obj._value2) + 1];
        strcpy(_value2, obj._value2);
    }
    
    A &operator=(
        A const &obj)
    {
        if (this != &obj)
        {
            _value = obj._value;
            
            delete[] _value2;
            _value2 = new char[strlen(obj._value2) + 1];
            strcpy(_value2, obj._value2);
        }
        
        return *this;
    }
    
    ~A()
    {
        if (_value2 != nullptr)
        {
            delete[] _value2;
        }
    }
    
    A(
        A &&obj)
    {
        _value = std::move(obj._value);
        _value2 = obj._value2;
        obj._value2 = nullptr;
    }
    
    A &operator=(
        A &&obj)
    {
        if (this != &obj)
        {
            delete[] _value2;
            _value2 = obj._value2;
            obj._value2 = nullptr;
            
            _value = std::move(obj._value);
        }
        
        return *this;
    }
    
};

int main(
    int argc,
    char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    
    return RUN_ALL_TESTS();
}