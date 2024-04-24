#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H
#include <list>
#include <optional>
#include <stack>
#include <vector>
#include <logger.h>
#include <logger_guardant.h>
#include <allocator.h>
#include <allocator_guardant.h>
#include <not_implemented.h>
#include <search_tree.h>

template<
        typename tkey,
        typename tvalue>
class binary_search_tree:
        public search_tree<tkey, tvalue>
{

    friend class template_method_basics;
protected:

    struct node
    {

    public:

        tkey key;

        tvalue value;

        node *left_subtree;

        node *right_subtree;

    public:

        explicit node(
                tkey const &key,
                tvalue const &value);

        explicit node(
                tkey const &key,
                tvalue &&value);

    public:

        virtual ~node() noexcept = default;

    };

public:

    // region iterators definition

    class prefix_iterator;
    class prefix_const_iterator;
    class prefix_reverse_iterator;
    class prefix_reverse_const_iterator;

    class infix_iterator;
    class infix_const_iterator;
    class infix_reverse_iterator;
    class infix_const_reverse_iterator;

    class postfix_iterator;
    class postfix_const_iterator;
    class postfix_reverse_iterator;
    class postfix_const_reverse_iterator;

    struct iterator_data
    {

        friend class prefix_iterator;
        friend class prefix_const_iterator;
        friend class prefix_reverse_iterator;
        friend class prefix_reverse_const_iterator;

        friend class infix_iterator;
        friend class infix_const_iterator;
        friend class infix_reverse_iterator;
        friend class infix_const_reverse_iterator;

        friend class postfix_iterator;
        friend class postfix_const_iterator;
        friend class postfix_reverse_iterator;
        friend class postfix_const_reverse_iterator;

    public:

        unsigned int depth;

    protected:

        tkey *_key;

        tvalue *_value;

    protected:

        bool _is_state_initialized;

    public:

        inline tkey const &get_key() const noexcept
        {
            if (is_state_initialized())
            {
                return *_key;
            }
            else
            {
                throw std::logic_error("get_key: key is not initialized");
            }
        }

        inline tvalue const &get_value() const noexcept
        {
            if (is_state_initialized())
            {
                return *_value;
            }
            else
            {
                throw std::logic_error("get_value: value is not initialized");
            }
        }

        inline bool is_state_initialized() const noexcept
        {
            return _is_state_initialized;
        }

    public:

        iterator_data():
                _key(reinterpret_cast<tkey *>(::operator new(sizeof(tkey)))),
                _value(reinterpret_cast<tvalue *>(::operator new(sizeof(tvalue)))),
                _is_state_initialized(false)
        {

        }

        iterator_data(
                unsigned int depth,
                tkey const &key,
                tvalue const &value):
                depth(depth),
                _key(reinterpret_cast<tkey *>(::operator new(sizeof(tkey)))),
                _value(reinterpret_cast<tvalue *>(::operator new(sizeof(tvalue)))),
                _is_state_initialized(true)
        {
            allocator::construct(_key, key);
            allocator::construct(_value, value);
        }

    public:

        iterator_data(iterator_data const &other)
        : iterator_data(other.depth, other.get_key(), other.get_value())
        {
        }

        iterator_data &operator=(iterator_data const &other)
        {
            if (this != &other)
            {
                if (is_state_initialized())
                {
                    _key = other.key;
                    _value = other.value;
                }
                else
                {
                    allocator::construct(_key, other._key);
                    allocator::construct(_value, other._value);
                    _is_state_initialized = true;
                }
                depth = other.depth;
            }

            return *this;
        }

        iterator_data(iterator_data &&other) noexcept
        : iterator_data(other.depth, other.get_key(), other.get_value())
        {
            allocator::destruct(other._key);
            allocator::destruct(other._value);
            other._is_state_initialized = false;
            _is_state_initialized = true;
        }

        iterator_data &operator=(iterator_data &&other) noexcept
        {
            if (this != &other)
            {
                if (_is_state_initialized)
                {
                    _key(std::move(other.key));
                    _value(std::move(other.value));
                }
                else
                {
                    allocator::construct(_key, std::move(other._key));
                    allocator::construct(_value, std::move(other._value));
                    _is_state_initialized = true;
                }
                depth = other.depth;
                other.is_state_initialized = false;
            }

            return *this;
        }

        virtual ~iterator_data() noexcept
        {
            if (_is_state_initialized)
            {
                allocator::destruct(_key);
                allocator::destruct(_value);
            }

            ::operator delete(_key);
            _key = nullptr;
            ::operator delete(_value);
            _value = nullptr;

            _is_state_initialized = false;
        };

    };

    class prefix_iterator final
    {

        friend class binary_search_tree<tkey, tvalue>;

    private:

        iterator_data *_data;
        std::stack<node *> _state;
        binary_search_tree<tkey, tvalue> *_holder;

    public:
        explicit prefix_iterator(binary_search_tree<tkey, tvalue> const *holder, iterator_data *data)
        : prefix_iterator(holder, holder->_root, data)
        {

        }

        explicit prefix_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                typename binary_search_tree<tkey, tvalue>::node *subtree_root,
                iterator_data *data)
                :  _holder(const_cast<binary_search_tree<tkey, tvalue> *>(holder)),
                                       _data(data)
        {
            _state.push(subtree_root);

            if (_state.empty())
            {
                return;
            }
            allocator::construct(_data->_key, _state.top()->key);
            allocator::construct(_data->_value, _state.top()->value);
            _data->_is_state_initialized = true;
            _data->depth = 0;
        }

        explicit prefix_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node *> const &state,
                iterator_data *data):
                _holder(const_cast<binary_search_tree<tkey, tvalue> *>(holder)),
                _state(state),
                _data(data)
        {
            if (state.empty())
            {
                return;
            }

            allocator::construct(_data->_key, _state.top()->key);
            allocator::construct(_data->_value, _state.top()->value);
            _data->_is_state_initialized = true;
        }

        ~prefix_iterator()
        {
            delete _data;
        }

        prefix_iterator(prefix_iterator const &other)
        : _data(iterator_data(other._data->depth, other._data->_key, other._data->_value)), _holder(other._holder), _state(other._state)
        {
        }

        prefix_iterator &operator=(prefix_iterator const &other)
        {
            if (this != &other)
            {
                if (_data->is_state_initialized)
                {
                    *(_data->_key) = other._data->key;
                    *(_data->_value) = other._data->value;
                }
                else
                {
                    allocator::construct(_data->_key, other._data->key);
                    allocator::construct(_data->_value, other._data->_value);
                    _data->_is_state_initialized = true;
                }
                _data->depth = other._data->depth;
                _holder = other._holder;
                _state = other._state;

            }

            return *this;
        }

        prefix_iterator(prefix_iterator &&other) noexcept
        : _holder(other._holder)
        {
            _data = iterator_data(other._data->depth, other._data->get_key(), other._data->get_value());
            allocator::destruct(other._data->_key);
            allocator::destruct(other._data->_value);

            other._holder = nullptr;
            other._data->_is_state_initialized = false;

            while (!other._state.empty())
            {
                node* current = other._state.top();
                other._state.pop();
                _state.push(current);
            }
        }

        prefix_iterator &operator=(prefix_iterator &&other) noexcept
        {
            if (this != &other)
            {
                if (_data->is_state_initialized())
                {
                    *(_data->_key) = std::move(other._data->key);
                    *(_data->_value) = std::move(other._data->value);
                }
                else
                {
                    allocator::construct(_data->_key, std::move(other._data->key));
                    allocator::construct(_data->_value, std::move(other._data->_value));
                    _data->_is_state_initialized = true;
                }
                _data->depth = other._data->depth;
                this->_holder = other._holder;
                other._holder = nullptr;

                other._data->_is_state_initialized = false;

                _data->_is_state_initialized = true;

                while (!other._state.empty())
                {
                    node *current = other._state.pop();
                    _state.push(current);
                }
                other = nullptr;
            }

            return *this;
        }

    public:

        bool operator==(prefix_iterator const &other) const noexcept;

        bool operator!=(prefix_iterator const &other) const noexcept;

        prefix_iterator &operator++();

        prefix_iterator const operator++(int not_used);

        iterator_data *operator*() const;

    private:

        virtual void assign_data()
        {
            if (_data->is_state_initialized())
            {
                *(_data->_key) = _state.top()->key;
                *(_data->_value) = _state.top()->value;
            }
            else
            {
                allocator::construct(_data->_key, _state.top()->key);
                allocator::construct(_data->_value, _state.top()->value);
                _data->_is_state_initialized = true;
            }

            _data->depth = _state.size() - 1;
            _holder->inject_additional_data(_data, _state.top());
        }

    };

    class prefix_const_iterator final
    {

        friend class binary_search_tree<tkey, tvalue>;

    private:

        iterator_data *_data;
        std::stack<node *> _state;
        binary_search_tree<tkey, tvalue> *_holder;

    public:
        explicit prefix_const_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                iterator_data *data)
                : prefix_const_iterator(holder, holder->_root, data)
        {

        }

        explicit prefix_const_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                typename binary_search_tree<tkey, tvalue>::node *subtree_root,
                iterator_data *data)
                : _holder(const_cast<binary_search_tree<tkey, tvalue> *>(holder)), _data(data)
        {
            _state.push(subtree_root);

            if (_state.empty())
            {
                return;
            }

            allocator::construct(_data->_key, _state.top()->key);
            allocator::construct(_data->_value, _state.top()->value);
            _data->depth = 0;
            _data->_is_state_initialized = true;
        }

        explicit prefix_const_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node *> const &state,
                iterator_data *data):
                _holder(const_cast<binary_search_tree<tkey, tvalue> *>(holder)),
                _state(state),
                _data(data)
        {
            if (state.empty())
            {
                return;
            }

            allocator::construct(_data->_key, _state.top()->key);
            allocator::construct(_data->_value, _state.top()->value);
            _data->_is_state_initialized = true;
        }
    public:

        ~prefix_const_iterator()
        {
            delete _data;
        }

        prefix_const_iterator(prefix_iterator const &other)
        : _data(iterator_data(other._data->depth, other._data->_key, other._data->_value)), _holder(other._holder), _state(other._state)
        {
        }

        prefix_const_iterator &operator=(prefix_const_iterator const &other)
        {
            if (this != &other)
            {
                if (_data->is_state_initialized)
                {
                    *(_data->_key) = other._data->key;
                    *(_data->_value) = other._data->value;
                }
                else
                {
                    allocator::construct(_data->_key, other._data->key);
                    allocator::construct(_data->_value, other._data->_value);
                    _data->_is_state_initialized = true;
                }
                _data->depth = other._data->depth;
                _holder = other._holder;
                _state = other._state;
            }

            return *this;
        }

        prefix_const_iterator(prefix_const_iterator &&other) noexcept
        : _holder(other._holder)
        {
            _data = new iterator_data(other._data->depth, other._data->get_key(), other._data->get_value());
            allocator::destruct(other._data->_key);
            allocator::destruct(other._data->_value);

            other._holder = nullptr;

            other._data->_is_state_initialized = false;

            while (!other._state.empty())
            {
                node *current = other._state.top();
                other._state.pop();
                _state.push(current);
            }
        }

        prefix_const_iterator &operator=(prefix_const_iterator &&other) noexcept
        {
            if (this != &other)
            {
                if (_data->is_state_initialized())
                {
                    *(_data->_key) = std::move(other._data->key);
                    *(_data->_value) = std::move(other._data->value);
                }
                else
                {
                    allocator::construct(_data->_key, std::move(other._data->key));
                    allocator::construct(_data->_value, std::move(other._data->_value));
                    _data->_is_state_initialized = true;
                }
                _data->depth = other._data->depth;
                this->_holder = other._holder;
                other._holder = nullptr;

                other._data->_is_state_initialized = false;

                _data->_is_state_initialized = true;

                while (!other._state.empty())
                {
                    node *current = other._state.pop();
                    _state.push(current);
                }
                other = nullptr;
            }

            return *this;
        }

    public:

        bool operator==(prefix_const_iterator const &other) const noexcept;

        bool operator!=(prefix_const_iterator const &other) const noexcept;

        prefix_const_iterator &operator++();

        prefix_const_iterator const operator++(int not_used);

        iterator_data const *operator*() const;

    private:

        void assign_data()
        {
            if (_data->is_state_initialized())
            {
                *(_data->_key) = _state.top()->key;
                *(_data->_value) = _state.top()->value;
            }
            else
            {
                allocator::construct(_data->_key, _state.top()->key);
                allocator::construct(_data->_value, _state.top()->value);
                _data->_is_state_initialized = true;
            }

            _data->depth = _state.size() - 1;
            _holder->inject_additional_data(_data, _state.top());
        }

    };

    class prefix_reverse_iterator final
    {

        friend class binary_search_tree<tkey, tvalue>;

    private:
        iterator_data *_data;
        std::stack<node *> _state;
        binary_search_tree<tkey, tvalue> *_holder;

    public:
        explicit prefix_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                iterator_data *data)
                : prefix_reverse_iterator(holder, holder->_root, data)
        {
        }

        explicit prefix_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                typename binary_search_tree<tkey, tvalue>::node *subtree_root,
                iterator_data *data): _holder(const_cast<binary_search_tree<tkey, tvalue> *>(holder)), _data(data)
        {
            while (subtree_root != nullptr)
            {
                _state.push(subtree_root);
                subtree_root = subtree_root->right_subtree;
            }
            if (_state.empty())
            {
                return;
            }
            while(_state.top()->right_subtree != nullptr || _state.top()->left_subtree != nullptr)
            {
                if (_state.top()->left_subtree != nullptr)
                {
                    _state.push(_state.top()->left_subtree);
                    while (_state.top()->right_subtree != nullptr)
                    {
                        _state.push(_state.top()->right_subtree);
                    }
                }
            }
            allocator::construct(_data->_key, _state.top()->key);
            allocator::construct(_data->_value, _state.top()->value);
            _data->_is_state_initialized = true;
            _data->depth = _state.size() - 1;
        }

        explicit prefix_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node *> const &state,
                iterator_data *data) : _holder(const_cast<binary_search_tree<tkey, tvalue> *>(holder)),
                                       _state(state),
                                       _data(data)
        {
            if (state.empty())
            {
                return;
            }

            allocator::construct(_data->_key, _state.top()->key);
            allocator::construct(_data->_value, _state.top()->value);
            _data->_is_state_initialized = true;
        }


        ~prefix_reverse_iterator()
        {
            delete _data;
        }

        prefix_reverse_iterator(
                prefix_iterator const &other) : _data(iterator_data(other._data->depth, other._data->_key, other._data->_value)), _holder(other._holder), _state(other._state)
        {
        }

        prefix_reverse_iterator &operator=(
                prefix_reverse_iterator const &other)
        {
            if (this != &other)
            {
                if (_data->is_state_initialized)
                {
                    *(_data->_key) = other._data->key;
                    *(_data->_value) = other._data->value;
                }
                else
                {
                    allocator::construct(_data->_key, other._data->key);
                    allocator::construct(_data->_value, other._data->_value);
                    _data->_is_state_initialized = true;
                }
                _data->depth = other._data->depth;
                _holder = other._holder;
                _state = other._state;
            }

            return *this;
        }

        prefix_reverse_iterator(
                prefix_reverse_iterator &&other) noexcept : _holder(other._holder)
        {
            _data = new iterator_data(other._data->depth, other._data->_key, other._data->_value);

            allocator::destruct(other._data->_key);
            allocator::destruct(other._data->_value);

            other._holder = nullptr;

            other._data->_is_state_initialized = false;

            while (!other._state.empty())
            {
                node *current = other._state.pop();
                _state.push(current);
            }
        }

        prefix_reverse_iterator &operator=(
                prefix_reverse_iterator &&other) noexcept
        {
            if (this != &other)
            {
                if (_data->is_state_initialized())
                {
                    *(_data->_key) = std::move(other._data->key);
                    *(_data->_value) = std::move(other._data->value);
                }
                else
                {
                    allocator::construct(_data->_key, std::move(other._data->key));
                    allocator::construct(_data->_value, std::move(other._data->_value));
                    _data->_is_state_initialized = true;
                }
                _data->depth = other._data->depth;
                this->_holder = other._holder;
                other._holder = nullptr;

                other._data->_is_state_initialized = false;

                _data->_is_state_initialized = true;

                while (!other._state.empty())
                {
                    node *current = other._state.pop();
                    _state.push(current);
                }
                other = nullptr;
            }

            return *this;
        }

    public:

        bool operator==(
                prefix_reverse_iterator const &other) const noexcept;

        bool operator!=(
                prefix_reverse_iterator const &other) const noexcept;

        prefix_reverse_iterator &operator++();

        prefix_reverse_iterator const operator++(
                int not_used);

        iterator_data *operator*() const;

    private:

        void assign_data()
        {
            if (_data->is_state_initialized())
            {
                *(_data->_key) = _state.top()->key;
                *(_data->_value) = _state.top()->value;
            }
            else
            {
                allocator::construct(_data->_key, _state.top()->key);
                allocator::construct(_data->_value, _state.top()->value);
                _data->_is_state_initialized = true;
            }

            _data->depth = _state.size() - 1;
            _holder->inject_additional_data(_data, _state.top());
        }


    };

    class prefix_const_reverse_iterator final
    {

        friend class binary_search_tree<tkey, tvalue>;

    private:
        const iterator_data *_data;
        std::stack<const node *> _state;
        const binary_search_tree<tkey, tvalue> *_holder;

    public:
        explicit prefix_const_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                iterator_data *data) : prefix_const_reverse_iterator(holder, holder->_root, data)
        {
        }

        explicit prefix_const_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                typename binary_search_tree<tkey, tvalue>::node *subtree_root,
                iterator_data *data)
        {
            while (subtree_root != nullptr)
            {
                _state.push(subtree_root);
                subtree_root = subtree_root->right_subtree;
            }

            if (_state.empty())
            {
                return;
            }

            while(_state.top()->right_subtree != nullptr || _state.top()->left_subtree != nullptr)
            {
                if (_state.top()->left_subtree != nullptr)
                {
                    _state.push(_state.top()->left_subtree);
                    while (_state.top()->right_subtree != nullptr)
                    {
                        _state.push(_state.top()->right_subtree);
                    }
                }
            }

            allocator::construct(_data->_key, _state.top()->key);
            allocator::construct(_data->_value, _state.top()->value);
            _data->_is_state_initialized = true;
            _data->depth = _state.size() - 1;
        }

        explicit prefix_const_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node *> const &state,
                iterator_data *data) : _holder(const_cast<binary_search_tree<tkey, tvalue> *>(holder)),
                                       _state(state),
                                       _data(data)
        {
            if (state.empty())
            {
                return;
            }

            allocator::construct(_data->_key, _state.top()->key);
            allocator::construct(_data->_value, _state.top()->value);
            _data->_is_state_initialized = true;
        }


        ~prefix_const_reverse_iterator()
        {
            delete _data;
        }

        prefix_const_reverse_iterator(
                prefix_iterator const &other) : _data(iterator_data(other._data->depth, other._data->_key, other._data->_value)), _holder(other._holder), _state(other._state)
        {
        }

        prefix_const_reverse_iterator &operator=(
                prefix_const_reverse_iterator const &other)
        {
            if (this != &other)
            {
                if (_data->is_state_initialized)
                {
                    *(_data->_key) = other._data->key;
                    *(_data->_value) = other._data->value;
                }
                else
                {
                    allocator::construct(_data->_key, other._data->key);
                    allocator::construct(_data->_value, other._data->_value);
                    _data->_is_state_initialized = true;
                }
                _data->depth = other._data->depth;
                _holder = other._holder;
                _state = other._state;
            }

            return *this;
        }

        prefix_const_reverse_iterator(
                prefix_const_reverse_iterator &&other) noexcept : _holder(other._holder)
        {
            _data = new iterator_data(other._data->depth, std::move(other._data->_key), std::move(other._data->_value));

            other._holder = nullptr;

            other._data->_is_state_initialized = false;

            while (!other._state.empty())
            {
                node *current = other._state.pop();
                _state.push(current);
            }
        }

        prefix_const_reverse_iterator &operator=(
                prefix_const_reverse_iterator &&other) noexcept
        {
            if (this != &other)
            {
                if (_data->is_state_initialized())
                {
                    *(_data->_key) = std::move(other._data->key);
                    *(_data->_value) = std::move(other._data->value);
                }
                else
                {
                    allocator::construct(_data->_key, std::move(other._data->key));
                    allocator::construct(_data->_value, std::move(other._data->_value));
                    _data->_is_state_initialized = true;
                }
                _data->depth = other._data->depth;
                this->_holder = other._holder;
                other._holder = nullptr;

                other._data->_is_state_initialized = false;

                _data->_is_state_initialized = true;

                while (!other._state.empty())
                {
                    node *current = other._state.pop();
                    _state.push(current);
                }
                other = nullptr;
            }

            return *this;
        }

    public:

        bool operator==(
                prefix_const_reverse_iterator const &other) const noexcept;

        bool operator!=(
                prefix_const_reverse_iterator const &other) const noexcept;

        prefix_const_reverse_iterator &operator++();

        prefix_const_reverse_iterator const operator++(
                int not_used);

        iterator_data const *operator*() const;

    private:

        void assign_data()
        {
            if (_data->is_state_initialized())
            {
                *(_data->_key) = _state.top()->key;
                *(_data->_value) = _state.top()->value;
            }
            else
            {
                allocator::construct(_data->_key, _state.top()->key);
                allocator::construct(_data->_value, _state.top()->value);
                _data->_is_state_initialized = true;
            }

            _data->depth = _state.size() - 1;
            _holder->inject_additional_data(_data, _state.top());
        }

    };

    class infix_iterator final
    {

        friend class binary_search_tree<tkey, tvalue>;

    private:

        iterator_data *_data;
        std::stack<node *> _state;
        binary_search_tree<tkey, tvalue> *_holder;

    public:
        explicit infix_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                iterator_data *data) : infix_iterator(holder, holder->_root, data)
        {

        }

        explicit infix_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                typename binary_search_tree<tkey, tvalue>::node *subtree_root,
                iterator_data *data):  _holder(const_cast<binary_search_tree<tkey, tvalue> *>(holder)),
                                       _data(data)
        {
            while (subtree_root != nullptr)
            {
                _state.push(subtree_root);
                subtree_root = subtree_root->left_subtree;
            }

            if (_state.empty())
            {
                return;
            }
            allocator::construct(_data->_key, _state.top()->key);
            allocator::construct(_data->_value, _state.top()->value);
            _data->_is_state_initialized = true;
            _data->depth = _state.size() - 1;
        }

        explicit infix_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node *> const &state,
                iterator_data *data):
                _holder(const_cast<binary_search_tree<tkey, tvalue> *>(holder)),
                _state(state),
                _data(data)
        {
            if (state.empty())
            {
                return;
            }

            allocator::construct(_data->_key, _state.top()->key);
            allocator::construct(_data->_value, _state.top()->value);
            data->_is_state_initialized = true;
            data->depth = _state.size() - 1;
        }

    public:

        ~infix_iterator()
        {
            delete _data;
        }

        infix_iterator(infix_iterator const &other)
        {
            _data = iterator_data(other._data->depth, other._data->_key, other._data->_value);
            _holder = other._holder;
            _state = other._state;
        }

        infix_iterator &operator=(
                infix_iterator const &other)
        {
            if (this != &other)
            {
                if (_data->is_state_initialized)
                {
                    *(_data->_key) = other._data->key;
                    *(_data->_value) = other._data->value;
                }
                else
                {
                    allocator::construct(_data->_key, other._data->key);
                    allocator::construct(_data->_value, other._data->_value);
                    _data->_is_state_initialized = true;
                }
                _data->depth = other._data->depth;
                _holder = other._holder;
                _state = other._state;

            }

            return *this;
        }

        infix_iterator(
                infix_iterator &&other) noexcept  //pointer to _data ????????????
        {
            _data = new iterator_data(other._data->depth, other._data->get_key(), other._data->get_value());

            this->_holder = other._holder;
            other._holder = nullptr;

            allocator::destruct(other._data->_key);
            allocator::destruct(other._data->_value);

            other._data->_is_state_initialized = false;

            _data->_is_state_initialized = true;


            while (!other._state.empty())
            {
                node* current = other._state.top();
                other._state.top();
                _state.push(current);
            }
        }

        infix_iterator &operator=(
                infix_iterator &&other) noexcept
        {
            if (this != &other)
            {
                if (_data->is_state_initialized())
                {
                    *(_data->_key) = std::move(other._data->key);
                    *(_data->_value) = std::move(other._data->value);
                }
                else
                {
                    allocator::construct(_data->_key, std::move(other._data->key));
                    allocator::construct(_data->_value, std::move(other._data->_value));
                    _data->_is_state_initialized = true;
                }
                _data->depth = other._data->depth;
                this->_holder = other._holder;
                other._holder = nullptr;

                other._data->_is_state_initialized = false;

                _data->_is_state_initialized = true;

                while (!other._state.empty())
                {
                    node *current = other._state.pop();
                    _state.push(current);
                }
                other = nullptr;
            }

            return *this;
        }

    public:

        bool operator==(
                infix_iterator const &other) const noexcept;

        bool operator!=(
                infix_iterator const &other) const noexcept;

        infix_iterator &operator++();

        infix_iterator const operator++(
                int not_used);

        iterator_data *operator*() const;

    private:

        virtual void assign_data()
        {
            if (_data->is_state_initialized())
            {
                *(_data->_key) = _state.top()->key;
                *(_data->_value) = _state.top()->value;
            }
            else
            {
                allocator::construct(_data->_key, _state.top()->key);
                allocator::construct(_data->_value, _state.top()->value);
                _data->_is_state_initialized = true;
            }
            _data->depth = _state.size() - 1;
            _holder->inject_additional_data(_data, _state.top());
        }

    };

    class infix_const_iterator final
    {

        friend class binary_search_tree<tkey, tvalue>;

    private:
        iterator_data *_data;
        std::stack<node *> _state;
        binary_search_tree<tkey, tvalue> *_holder;

    public:
        explicit infix_const_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                iterator_data *data) : infix_const_iterator(holder, holder->_root, data)
        {

        }
        explicit infix_const_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                typename binary_search_tree<tkey, tvalue>::node *subtree_root,
                iterator_data *data) :  _holder(const_cast<binary_search_tree<tkey, tvalue> *>(holder)),
                                        _data(data)
        {
            while (subtree_root != nullptr)
            {
                _state.push(subtree_root);
                subtree_root = subtree_root->left_subtree;
            }

            if (_state.empty())
            {
                return;
            }
            allocator::construct(_data->_key, _state.top()->key);
            allocator::construct(_data->_value, _state.top()->value);
            _data->_is_state_initialized = true;
            _data->depth = _state.size() - 1;

        }

        explicit infix_const_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node *> const &state,
                iterator_data *data) : _holder(const_cast<binary_search_tree<tkey, tvalue> *>(holder)),
                                       _state(state),
                                       _data(data)
        {
            if (state.empty())
            {
                return;
            }
            if (_data->_is_state_initialized)
            {
                *(_data->_key) = _state.top()->key;
                *(_data->_value) = _state.top()->value;
            }
            else
            {
                allocator::construct(_data->_key, _state.top()->key);
                allocator::construct(_data->_value, _state.top()->value);
                _data->_is_state_initialized = true;
            }
        }

    public:

        ~infix_const_iterator()
        {
            delete _data;
        }

        infix_const_iterator(
                infix_iterator const &other)
        {
            _data = iterator_data(other._data->depth, other._data->_key, other._data->_value);
            _holder = other._holder;
            _state = other._state;
        }

        infix_const_iterator &operator=(
                infix_const_iterator const &other)
        {
            if (this != &other)
            {
                if (_data->is_state_initialized)
                {
                    *(_data->_key) = other._data->key;
                    *(_data->_value) = other._data->value;
                }
                else
                {
                    allocator::construct(_data->_key, other._data->key);
                    allocator::construct(_data->_value, other._data->_value);
                    _data->_is_state_initialized = true;
                }
                _data->depth = other._data->depth;
                _holder = other._holder;
                _state = other._state;
            }

            return *this;
        }

        infix_const_iterator(
                infix_const_iterator &&other) noexcept
        {
            _data = new iterator_data(other._data->depth, other._data->get_key(), other._data->get_value());

            this->_holder = other._holder;
            other._holder = nullptr;

            allocator::destruct(other._data->_key);
            allocator::destruct(other._data->_value);

            other._data->_is_state_initialized = false;

            _data->_is_state_initialized = true;


            while (!other._state.empty())
            {
                node* current = other._state.top();
                other._state.pop();
                _state.push(current);
            }
        }

        infix_const_iterator &operator=(
                infix_const_iterator &&other) noexcept
        {
            if (this != &other)
            {
                if (_data->is_state_initialized())
                {
                    *(_data->_key) = other._data->key;
                    *(_data->_value) = other._data->value;
                }
                else
                {
                    allocator::construct(_data->_key, other._data->key);
                    allocator::construct(_data->_value, other._data->_value);
                    _data->_is_state_initialized = true;
                }
                _data->depth = other._data->depth;
                this->_holder = other._holder;
                other._holder = nullptr;

                allocator::destruct(other._data->_key);
                allocator::destruct(other._data->_value);
                other._data->_is_state_initialized = false;

                other._data = nullptr;

                _data->_is_state_initialized = true;

                while (!other._state.empty())
                {
                    node *current = other._state.pop();
                    _state.push(current);
                }
                other = nullptr;
            }

            return *this;
        }

    public:

        bool operator==(
                infix_const_iterator const &other) const noexcept;

        bool operator!=(
                infix_const_iterator const &other) const noexcept;

        infix_const_iterator &operator++();

        infix_const_iterator const operator++(
                int not_used);

        iterator_data const *operator*() const;

    private:

        void assign_data()
        {
            if (_data->is_state_initialized())
            {
                *(_data->_key) = _state.top()->key;
                *(_data->_value) = _state.top()->value;
            }
            else
            {
                allocator::construct(_data->_key, _state.top()->key);
                allocator::construct(_data->_value, _state.top()->value);
                _data->_is_state_initialized = true;
            }

            _data->depth = _state.size() - 1;
            _holder->inject_additional_data(_data, _state.top());
        }
    };

    class infix_reverse_iterator final
    {

        friend class binary_search_tree<tkey, tvalue>;

    private:

        iterator_data *_data;
        std::stack<node *> _state;
        binary_search_tree<tkey, tvalue> *_holder;

    public:
        explicit infix_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                iterator_data *data) : infix_reverse_iterator(holder, holder->_root, data)
        {
        }

        explicit infix_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                typename binary_search_tree<tkey, tvalue>::node *subtree_root,
                iterator_data *data) :  _holder(const_cast<binary_search_tree<tkey, tvalue> *>(holder)),
                                        _data(data)
        {
            while (subtree_root != nullptr)
            {
                _state.push(subtree_root);
                subtree_root = subtree_root->right_subtree;
            }

            if (_state.empty())
            {
                return;
            }
            allocator::construct(_data->_key, _state.top()->key);
            allocator::construct(_data->_value, _state.top()->value);
            _data->_is_state_initialized = true;
            _data->depth = _state.size() - 1;
        }

        explicit infix_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node *> const &state,
                iterator_data *data) : _holder(const_cast<binary_search_tree<tkey, tvalue> *>(holder)),
                                       _state(state),
                                       _data(data)
        {
            if (state.empty())
            {
                return;
            }

            allocator::construct(_data->_key, _state.top()->key);
            allocator::construct(_data->_value, _state.top()->value);
            _data->_is_state_initialized = true;
        }

    public:

        ~infix_reverse_iterator()
        {
            delete _data;
        }

        infix_reverse_iterator(
                infix_reverse_iterator const &other)
        {
            _data = iterator_data(other._data->depth, other._data->_key, other._data->_value);
            _holder = other._holder;
            _state = other._state;
        }

        infix_reverse_iterator &operator=(
                infix_reverse_iterator const &other)
        {
            if (this != &other)
            {
                if (_data->is_state_initialized)
                {
                    *(_data->_key) = other._data->key;
                    *(_data->_value) = other._data->value;
                }
                else
                {
                    allocator::construct(_data->_key, other._data->key);
                    allocator::construct(_data->_value, other._data->_value);
                    _data->_is_state_initialized = true;
                }
                _data->depth = other._data->depth;
                _holder = other._holder;
                _state = other._state;

            }

            return *this;
        }

        infix_reverse_iterator(
                infix_reverse_iterator &&other) noexcept  //pointer to _data
        {
            _data = new iterator_data(other._data->depth, other._data->get_key(), other._data->get_value());

            allocator::destruct(other._data->_key);
            allocator::destruct(other._data->_value);

            this->_holder = other._holder;
            other._holder = nullptr;

            other._data->_is_state_initialized = false;

            _data->_is_state_initialized = true;


            while (!other._state.empty())
            {
                node* current = other._state.top();
                other._state.pop();
                _state.push(current);
            }
        }

        infix_reverse_iterator &operator=(
                infix_reverse_iterator &&other) noexcept
        {
            if (this != &other)
            {
                if (_data->is_state_initialized())
                {
                    *(_data->_key) = std::move(other._data->key);
                    *(_data->_value) = std::move(other._data->value);
                }
                else
                {
                    allocator::construct(_data->_key, std::move(other._data->key));
                    allocator::construct(_data->_value, std::move(other._data->_value));
                    _data->_is_state_initialized = true;
                }
                _data->depth = other._data->depth;
                this->_holder = other._holder;
                other._holder = nullptr;

                other._data->_is_state_initialized = false;

                _data->_is_state_initialized = true;

                while (!other._state.empty())
                {
                    node *current = other._state.pop();
                    _state.push(current);
                }
                other = nullptr;
            }

            return *this;
        }

    public:

        bool operator==(
                infix_reverse_iterator const &other) const noexcept;

        bool operator!=(
                infix_reverse_iterator const &other) const noexcept;

        infix_reverse_iterator &operator++();

        infix_reverse_iterator const operator++(
                int not_used);

        iterator_data *operator*() const;

    private:

        void assign_data()
        {
            if (_data->is_state_initialized())
            {
                *(_data->_key) = _state.top()->key;
                *(_data->_value) = _state.top()->value;
            }
            else
            {
                allocator::construct(_data->_key, _state.top()->key);
                allocator::construct(_data->_value, _state.top()->value);
                _data->_is_state_initialized = true;
            }

            _data->depth = _state.size() - 1;
            _holder->inject_additional_data(_data, _state.top());
        }

    };

    class infix_const_reverse_iterator final
    {

        friend class binary_search_tree<tkey, tvalue>;


    private:

        iterator_data *_data;
        std::stack<node *> _state;
        binary_search_tree<tkey, tvalue> *_holder;

    public:
        explicit infix_const_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                iterator_data *data) : infix_const_reverse_iterator(holder, holder->_root, data)
        {
        }

        explicit infix_const_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                typename binary_search_tree<tkey, tvalue>::node *subtree_root,
                iterator_data *data):  _holder(const_cast<binary_search_tree<tkey, tvalue> *>(holder)),
                                       _data(data)
        {
            while (subtree_root != nullptr)
            {
                _state.push(subtree_root);
                subtree_root = subtree_root->right_subtree;
            }

            if (_state.empty())
            {
                return;
            }
            allocator::construct(_data->_key, _state.top()->key);
            allocator::construct(_data->_value, _state.top()->value);
            _data->_is_state_initialized = true;
            _data->depth = _state.size() - 1;
        }

        explicit infix_const_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node *> const &state,
                iterator_data *data) : _holder(const_cast<binary_search_tree<tkey, tvalue> *>(holder)),
                                       _state(state),
                                       _data(data)
        {
            if (state.empty())
            {
                return;
            }

            allocator::construct(_data->_key, _state.top()->key);
            allocator::construct(_data->_value, _state.top()->value);
            _data->_is_state_initialized = true;
        }

    public:

        ~infix_const_reverse_iterator()
        {
            delete _data;
        }

        infix_const_reverse_iterator(
                infix_const_reverse_iterator const &other)
        {
            _data = iterator_data(other._data->depth, other._data->_key, other._data->_value);
            _holder = other._holder;
            _state = other._state;
        }

        infix_const_reverse_iterator &operator=(
                infix_const_reverse_iterator const &other)
        {
            if (this != &other)
            {
                if (_data->is_state_initialized)
                {
                    *(_data->_key) = other._data->key;
                    *(_data->_value) = other._data->value;
                }
                else
                {
                    allocator::construct(_data->_key, other._data->key);
                    allocator::construct(_data->_value, other._data->_value);
                    _data->_is_state_initialized = true;
                }
                _data->depth = other._data->depth;
                _holder = other._holder;
                _state = other._state;

            }

            return *this;
        }

        infix_const_reverse_iterator(
                infix_const_reverse_iterator &&other) noexcept
        {
            _data = new iterator_data(other._data->depth, other._data->get_key(), other._data->get_value());
            allocator::destruct(other._data->_key);
            allocator::destruct(other._data->_value);

            this->_holder = other._holder;
            other._holder = nullptr;

            other._data->_is_state_initialized = false;

            _data->_is_state_initialized = true;


            while (!other._state.empty())
            {
                node* current = other._state.top();
                other._state.pop();
                _state.push(current);
            }
        }

        infix_const_reverse_iterator &operator=(
                infix_const_reverse_iterator &&other) noexcept
        {
            if (this != &other)
            {
                if (_data->is_state_initialized())
                {
                    *(_data->_key) = std::move(other._data->key);
                    *(_data->_value) = std::move(other._data->value);
                }
                else
                {
                    allocator::construct(_data->_key, std::move(other._data->key));
                    allocator::construct(_data->_value, std::move(other._data->_value));
                    _data->_is_state_initialized = true;
                }
                _data->depth = other._data->depth;
                this->_holder = other._holder;
                other._holder = nullptr;

                other._data->_is_state_initialized = false;

                _data->_is_state_initialized = true;

                while (!other._state.empty())
                {
                    node *current = other._state.pop();
                    _state.push(current);
                }
                other = nullptr;
            }

            return *this;
        }

    public:

        bool operator==(
                infix_const_reverse_iterator const &other) const noexcept;

        bool operator!=(
                infix_const_reverse_iterator const &other) const noexcept;

        infix_const_reverse_iterator &operator++();

        infix_const_reverse_iterator const operator++(
                int not_used);

        iterator_data const *operator*() const;

    private:

        void assign_data()
        {
            if (_data->is_state_initialized())
            {
                *(_data->_key) = _state.top()->key;
                *(_data->_value) = _state.top()->value;
            }
            else
            {
                allocator::construct(_data->_key, _state.top()->key);
                allocator::construct(_data->_value, _state.top()->value);
                _data->_is_state_initialized = true;
            }

            _data->depth = _state.size() - 1;
            _holder->inject_additional_data(_data, _state.top());
        }

    };

    class postfix_iterator final
    {

        friend class binary_search_tree<tkey, tvalue>;

    private:

        iterator_data *_data;
        std::stack<node *> _state;
        binary_search_tree<tkey, tvalue> *_holder;

    public:
        explicit postfix_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                iterator_data *data) : postfix_iterator(holder, holder->_root, data)
        {
        }

        explicit postfix_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                typename binary_search_tree<tkey, tvalue>::node *subtree_root,
                iterator_data *data):  _holder(const_cast<binary_search_tree<tkey, tvalue> *>(holder)),
                                       _data(data)
        {
            while (subtree_root != nullptr)
            {
                _state.push(subtree_root);
                subtree_root = subtree_root->left_subtree;
            }

            if (_state.empty())
            {
                return;
            }
            while(_state.top()->left_subtree != nullptr || _state.top()->right_subtree != nullptr)
            {
                if (_state.top()->right_subtree != nullptr)
                {
                    _state.push(_state.top()->right_subtree);
                    while (_state.top()->left_subtree != nullptr)
                    {
                        _state.push(_state.top()->left_subtree);
                    }
                }
            }
            allocator::construct(_data->_key, _state.top()->key);
            allocator::construct(_data->_value, _state.top()->value);
            _data->_is_state_initialized = true;
            _data->depth = _state.size() - 1;
        }

        explicit postfix_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node *> const &state,
                iterator_data *data) : _holder(const_cast<binary_search_tree<tkey, tvalue> *>(holder)),
                                       _state(state),
                                       _data(data)
        {
            if (state.empty())
            {
                return;
            }

            allocator::construct(_data->_key, _state.top()->key);
            allocator::construct(_data->_value, _state.top()->value);
        }


    public:
        ~postfix_iterator()
        {
            delete _data;
        }

        postfix_iterator(
                postfix_iterator const &other)
        {
            _data = iterator_data(other._data->depth, other._data->_key, other._data->_value);
            _holder = other._holder;
            _state = other._state;
        }

        postfix_iterator &operator=(
                postfix_iterator const &other)
        {
            if (this != &other)
            {
                if (_data->is_state_initialized)
                {
                    *(_data->_key) = other._data->key;
                    *(_data->_value) = other._data->value;
                }
                else
                {
                    allocator::construct(_data->_key, other._data->key);
                    allocator::construct(_data->_value, other._data->_value);
                    _data->_is_state_initialized = true;
                }
                _data->depth = other._data->depth;
                _holder = other._holder;
                _state = other._state;
            }

            return *this;
        }

        postfix_iterator(
                postfix_iterator &&other) noexcept
        {
            _data = new iterator_data(other._data->depth, other._data->get_key(), other._data->get_value());
            allocator::destruct(other._data->_key);
            allocator::destruct(other._data->_value);

            this->_holder = other._holder;
            other._holder = nullptr;

            other._data->_is_state_initialized = false;

            _data->_is_state_initialized = true;

            while (!other._state.empty())
            {
                node *current = other._state.top();
                other._state.pop();
                _state.push(current);
            }
        }

        postfix_iterator &operator=(
                postfix_iterator &&other) noexcept
        {
            if (this != &other)
            {
                if (_data->is_state_initialized())
                {
                    *(_data->_key) = std::move(other._data->key);
                    *(_data->_value) = std::move(other._data->value);
                }
                else
                {
                    allocator::construct(_data->_key, std::move(other._data->key));
                    allocator::construct(_data->_value, std::move(other._data->_value));
                    _data->_is_state_initialized = true;
                }
                _data->depth = other._data->depth;
                this->_holder = other._holder;
                other._holder = nullptr;

                other._data->_is_state_initialized = false;

                _data->_is_state_initialized = true;

                while (!other._state.empty())
                {
                    node *current = other._state.pop();
                    _state.push(current);
                }
                other = nullptr;
            }

            return *this;
        }

    public:

        bool operator==(
                postfix_iterator const &other) const noexcept;

        bool operator!=(
                postfix_iterator const &other) const noexcept;

        postfix_iterator &operator++();

        postfix_iterator const operator++(
                int not_used);

        iterator_data *operator*() const;


    private:

        void assign_data()
        {
            if (_data->is_state_initialized())
            {
                *(_data->_key) = _state.top()->key;
                *(_data->_value) = _state.top()->value;
            }
            else
            {
                allocator::construct(_data->_key, _state.top()->key);
                allocator::construct(_data->_value, _state.top()->value);
                _data->_is_state_initialized = true;
            }

            _data->depth = _state.size() - 1;
            _holder->inject_additional_data(_data, _state.top());
        }
    };

    class postfix_const_iterator final
    {

        friend class binary_search_tree<tkey, tvalue>;

    private:
        iterator_data *_data;
        std::stack<node *> _state;
        binary_search_tree<tkey, tvalue> *_holder;

    public:
        explicit postfix_const_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                iterator_data *data) : postfix_const_iterator(holder, holder->_root, data)
        {
        }

        explicit postfix_const_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                typename binary_search_tree<tkey, tvalue>::node *subtree_root,
                iterator_data *data) : _holder(const_cast<binary_search_tree<tkey, tvalue> *>(holder)),
                                       _data(data)
        {
            while (subtree_root != nullptr)
            {
                _state.push(subtree_root);
                subtree_root = subtree_root->left_subtree;
            }

            while(_state.top()->left_subtree != nullptr || _state.top()->right_subtree != nullptr)
            {
                if (_state.top()->right_subtree != nullptr)
                {
                    _state.push(_state.top()->right_subtree);
                    while (_state.top()->left_subtree != nullptr)
                    {
                        _state.push(_state.top()->left_subtree);
                    }
                }
            }
            allocator::construct(_data->_key, _state.top()->key);
            allocator::construct(_data->_value, _state.top()->value);
            _data->_is_state_initialized = true;
            _data->depth = _state.size() - 1;
        }

        explicit postfix_const_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node *> const &state,
                iterator_data *data) : _holder(const_cast<binary_search_tree<tkey, tvalue> *>(holder)),
                                       _state(state),
                                       _data(data)
        {
            if (state.empty())
            {
                return;
            }

            allocator::construct(_data->_key, _state.top()->key);
            allocator::construct(_data->_value, _state.top()->value);
            _data->_is_state_initialized = true;
        }

    public:
        ~postfix_const_iterator()
        {
            delete _data;
        }

        postfix_const_iterator(
                postfix_const_iterator const &other)
        {
            _data = iterator_data(other._data->depth, other._data->_key, other._data->_value);
            _holder = other._holder;
            _state = other._state;
        }

        postfix_const_iterator &operator=(
                postfix_const_iterator const &other)
        {
            if (this != &other)
            {
                if (_data->is_state_initialized)
                {
                    *(_data->_key) = other._data->key;
                    *(_data->_value) = other._data->value;
                }
                else
                {
                    allocator::construct(_data->_key, other._data->key);
                    allocator::construct(_data->_value, other._data->_value);
                    _data->_is_state_initialized = true;
                }
                _data->depth = other._data->depth;
                _holder = other._holder;
                _state = other._state;
            }

            return *this;
        }

        postfix_const_iterator(
                postfix_const_iterator &&other) noexcept
        {
            _data = new iterator_data(other._data->depth, other._data->get_key(), other._data->get_value());
            allocator::destruct(other._data->_key);
            allocator::destruct(other._data->_value);

            this->_holder = other._holder;
            other._holder = nullptr;

            other._data->_is_state_initialized = false;

            _data->_is_state_initialized = true;

            while (!other._state.empty())
            {
                node *current = other._state.top();
                other._state.pop();
                _state.push(current);
            }
        }

        postfix_const_iterator &operator=(
                postfix_const_iterator &&other) noexcept
        {
            if (this != &other)
            {
                if (_data->is_state_initialized())
                {
                    *(_data->_key) = std::move(other._data->key);
                    *(_data->_value) = std::move(other._data->value);
                }
                else
                {
                    allocator::construct(_data->_key, std::move(other._data->key));
                    allocator::construct(_data->_value, std::move(other._data->_value));
                    _data->_is_state_initialized = true;
                }
                _data->depth = other._data->depth;
                this->_holder = other._holder;
                other._holder = nullptr;

                other._data->_is_state_initialized = false;

                _data->_is_state_initialized = true;

                while (!other._state.empty())
                {
                    node *current = other._state.pop();
                    _state.push(current);
                }
                other = nullptr;
            }

            return *this;
        }

    public:

        bool operator==(
                postfix_const_iterator const &other) const noexcept;

        bool operator!=(
                postfix_const_iterator const &other) const noexcept;

        postfix_const_iterator &operator++();

        postfix_const_iterator const operator++(
                int not_used);

        iterator_data const *operator*() const;

    private:
        void assign_data()
        {
            if (_data->is_state_initialized())
            {
                *(_data->_key) = _state.top()->key;
                *(_data->_value) = _state.top()->value;
            }
            else
            {
                allocator::construct(_data->_key, _state.top()->key);
                allocator::construct(_data->_value, _state.top()->value);
                _data->_is_state_initialized = true;
            }

            _data->depth = _state.size() - 1;
            _holder->inject_additional_data(_data, _state.top());
        }
    };

    class postfix_reverse_iterator final
    {

        friend class binary_search_tree<tkey, tvalue>;

    private:

        iterator_data *_data;
        std::stack<node *> _state;
        binary_search_tree<tkey, tvalue> *_holder;

    public:
        explicit postfix_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                iterator_data *data) : postfix_reverse_iterator(holder, holder->_root, data)
        {
        }

        explicit postfix_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                typename binary_search_tree<tkey, tvalue>::node *subtree_root,
                iterator_data *data):  _holder(const_cast<binary_search_tree<tkey, tvalue> *>(holder)),
                                       _data(data)
        {
            _state.push(subtree_root);

            if (_state.empty())
            {
                return;
            }

            allocator::construct(_data->_key, _state.top()->key);
            allocator::construct(_data->_value, _state.top()->value);
            _data->_is_state_initialized = true;
            _data->depth = _state.size() - 1;
        }

        explicit postfix_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node *> const &state,
                iterator_data *data) : _holder(const_cast<binary_search_tree<tkey, tvalue> *>(holder)),
                                       _state(state),
                                       _data(data)
        {
            if (state.empty())
            {
                return;
            }

            allocator::construct(_data->_key, _state.top()->key);
            allocator::construct(_data->_value, _state.top()->value);
            _data->_is_state_initialized = true;
        }


    public:
        ~postfix_reverse_iterator()
        {
            delete _data;
        }

        postfix_reverse_iterator(
                postfix_reverse_iterator const &other)
        {
            _data = iterator_data(other._data->depth, other._data->_key, other._data->_value);
            _holder = other._holder;
            _state = other._state;
        }

        postfix_reverse_iterator &operator=(
                postfix_reverse_iterator const &other)
        {
            if (this != &other)
            {
                if (_data->is_state_initialized)
                {
                    *(_data->_key) = other._data->key;
                    *(_data->_value) = other._data->value;
                }
                else
                {
                    allocator::construct(_data->_key, other._data->key);
                    allocator::construct(_data->_value, other._data->_value);
                    _data->_is_state_initialized = true;
                }
                _data->depth = other._data->depth;
                _holder = other._holder;
                _state = other._state;
            }

            return *this;
        }

        postfix_reverse_iterator(
                postfix_reverse_iterator &&other) noexcept
        {
            _data = new iterator_data(other._data->depth, other._data->get_key(), other._data->get_value());
            allocator::destruct(other._data->_key);
            allocator::destruct(other._data->_value);

            this->_holder = other._holder;
            other._holder = nullptr;

            other._data->_is_state_initialized = false;

            _data->_is_state_initialized = true;

            while (!other._state.empty())
            {
                node *current = other._state.top();
                other._state.pop();
                _state.push(current);
            }
        }

        postfix_reverse_iterator &operator=(
                postfix_reverse_iterator &&other) noexcept
        {
            if (this != &other)
            {
                if (_data->is_state_initialized())
                {
                    *(_data->_key) = std::move(other._data->key);
                    *(_data->_value) = std::move(other._data->value);
                }
                else
                {
                    allocator::construct(_data->_key, std::move(other._data->key));
                    allocator::construct(_data->_value, std::move(other._data->_value));
                    _data->_is_state_initialized = true;
                }
                _data->depth = other._data->depth;
                this->_holder = other._holder;
                other._holder = nullptr;

                other._data->_is_state_initialized = false;

                _data->_is_state_initialized = true;

                while (!other._state.empty())
                {
                    node *current = other._state.pop();
                    _state.push(current);
                }
            }

            return *this;
        }

    public:

        bool operator==(
                postfix_reverse_iterator const &other) const noexcept;

        bool operator!=(
                postfix_reverse_iterator const &other) const noexcept;

        postfix_reverse_iterator &operator++();

        postfix_reverse_iterator const operator++(
                int not_used);

        iterator_data *operator*() const;


    private:

        void assign_data()
        {
            if (_data->is_state_initialized())
            {
                *(_data->_key) = _state.top()->key;
                *(_data->_value) = _state.top()->value;
            }
            else
            {
                allocator::construct(_data->_key, _state.top()->key);
                allocator::construct(_data->_value, _state.top()->value);
                _data->_is_state_initialized = true;
            }

            _data->depth = _state.size() - 1;
            _holder->inject_additional_data(_data, _state.top());
        }

    };

    class postfix_const_reverse_iterator final
    {

        friend class binary_search_tree<tkey, tvalue>;

    private:

        iterator_data *_data;
        std::stack<node *> _state;
        binary_search_tree<tkey, tvalue> *_holder;

    public:
        explicit postfix_const_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                iterator_data *data) : postfix_const_reverse_iterator(holder, holder->_root, data)
        {
        }

        explicit postfix_const_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                typename binary_search_tree<tkey, tvalue>::node *subtree_root,
                iterator_data *data):  _holder(const_cast<binary_search_tree<tkey, tvalue> *>(holder)),
                                       _data(data)
        {
            _state.push(subtree_root);

            if (_state.empty())
            {
                return;
            }

            allocator::construct(_data->_key, _state.top()->key);
            allocator::construct(_data->_value, _state.top()->value);
            _data->_is_state_initialized = true;
            _data->depth = _state.size() - 1;
        }

        explicit postfix_const_reverse_iterator(
                binary_search_tree<tkey, tvalue> const *holder,
                std::stack<node *> const &state,
                iterator_data *data) : _holder(const_cast<binary_search_tree<tkey, tvalue> *>(holder)),
                                       _state(state),
                                       _data(data)
        {
            if (state.empty())
            {
                return;
            }

            allocator::construct(_data->_key, _state.top()->key);
            allocator::construct(_data->_value, _state.top()->value);
            _data->_is_state_initialized = true;
        }


    public:
        ~postfix_const_reverse_iterator()
        {
            delete _data;
        }

        postfix_const_reverse_iterator(
                postfix_const_reverse_iterator const &other)
        {
            _data = iterator_data(other._data->depth, other._data->_key, other._data->_value);
            _holder = other._holder;
            _state = other._state;
        }

        postfix_const_reverse_iterator &operator=(
                postfix_const_reverse_iterator const &other)
        {
            if (this != &other)
            {
                if (_data->is_state_initialized)
                {
                    *(_data->_key) = other._data->key;
                    *(_data->_value) = other._data->value;
                }
                else
                {
                    allocator::construct(_data->_key, other._data->key);
                    allocator::construct(_data->_value, other._data->_value);
                    _data->_is_state_initialized = true;
                }
                _data->depth = other._data->depth;
                _holder = other._holder;
                _state = other._state;
            }

            return *this;
        }

        postfix_const_reverse_iterator(
                postfix_const_reverse_iterator &&other) noexcept
        {
            _data = new iterator_data(other._data->depth, other._data->get_key(), other._data->get_value());
            allocator::destruct(other._data->_key);
            allocator::destruct(other._data->_value);

            this->_holder = other._holder;
            other._holder = nullptr;

            other._data->_is_state_initialized = false;

            _data->_is_state_initialized = true;

            while (!other._state.empty())
            {
                node *current = other._state.top();
                other._state.pop();
                _state.push(current);
            }
        }

        postfix_const_reverse_iterator &operator=(
                postfix_const_reverse_iterator &&other) noexcept
        {
            if (this != &other)
            {
                if (_data->is_state_initialized())
                {
                    *(_data->_key) = std::move(other._data->key);
                    *(_data->_value) = std::move(other._data->value);
                }
                else
                {
                    allocator::construct(_data->_key, std::move(other._data->key));
                    allocator::construct(_data->_value, std::move(other._data->_value));
                    _data->_is_state_initialized = true;
                }
                _data->depth = other._data->depth;
                this->_holder = other._holder;
                other._holder = nullptr;

                other._data->_is_state_initialized = false;

                _data->_is_state_initialized = true;

                while (!other._state.empty())
                {
                    node *current = other._state.pop();
                    _state.push(current);
                }
                other = nullptr;
            }

            return *this;
        }
    public:

        bool operator==(
                postfix_const_reverse_iterator const &other) const noexcept;

        bool operator!=(
                postfix_const_reverse_iterator const &other) const noexcept;

        postfix_const_reverse_iterator &operator++();

        postfix_const_reverse_iterator const operator++(
                int not_used);

        iterator_data const *operator*() const;
    private:

        void assign_data()
        {
            if (_data->is_state_initialized())
            {
                *(_data->_key) = _state.top()->key;
                *(_data->_value) = _state.top()->value;
            }
            else
            {
                allocator::construct(_data->_key, _state.top()->key);
                allocator::construct(_data->_value, _state.top()->value);
                _data->_is_state_initialized = true;
            }

            _data->depth = _state.size() - 1;
            _holder->inject_additional_data(_data, _state.top());
        }
    };

    // endregion iterators definition

public:

    // region target operations strategies definition

    enum class insertion_of_existent_key_attempt_strategy
    {
        update_value,
        throw_an_exception
    };

    enum class disposal_of_nonexistent_key_attempt_strategy
    {
        do_nothing,
        throw_an_exception
    };

    // endregion target operations strategies definition

    // region target operations associated exception types

    class insertion_of_existent_key_attempt_exception final:
            public std::logic_error
    {

    private:

        tkey _key;

    public:

        explicit insertion_of_existent_key_attempt_exception(
                tkey const &key);

    public:

        tkey const &get_key() const noexcept;

    };

    class obtaining_of_nonexistent_key_attempt_exception final:
            public std::logic_error
    {

    private:

        tkey _key;

    public:

        explicit obtaining_of_nonexistent_key_attempt_exception(
                tkey const &key);

    public:

        tkey const &get_key() const noexcept;

    };

    class disposal_of_nonexistent_key_attempt_exception final:
            public std::logic_error
    {

    private:

        tkey _key;

    public:

        explicit disposal_of_nonexistent_key_attempt_exception(
                tkey const &key);

    public:

        tkey const &get_key() const noexcept;

    };

    // endregion target operations associated exception types

    // region template methods definition

protected:

    class template_method_basics:
            public logger_guardant
    {
        friend class binary_search_tree<tkey, tvalue>;
    protected:

        binary_search_tree<tkey, tvalue> *_tree;

    public:

        explicit template_method_basics(
                binary_search_tree<tkey, tvalue> *tree);

    protected:

        std::stack<node **> find_path(
                tkey const &key) const
        {
            std::stack<node **> result_path;

            node **path_finder = &(_tree->_root);
            auto const &comparer = _tree->_keys_comparer;

            while (true)
            {
                result_path.push(path_finder);

                if (*path_finder == nullptr)
                {
                    break;
                }

                auto comparison_result = comparer(key, (*path_finder)->key);
                if (comparison_result == 0)
                {
                    break;
                }

                path_finder = comparison_result < 0
                              ? &((*path_finder)->left_subtree)
                              : &((*path_finder)->right_subtree);
            }

            return result_path;
        }

        virtual void balance(
                std::stack<node **> &path)
        {
        }

    protected:

        [[nodiscard]] logger *get_logger() const noexcept final;

    };

    class insertion_template_method:
            public template_method_basics,
            public allocator_guardant
    {

    private:

        binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy _insertion_strategy;

    public:

        explicit insertion_template_method(
                binary_search_tree<tkey, tvalue> *tree,
                typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy);

    public:

        void insert(
                tkey const &key,
                tvalue const &value);

        void insert(
                tkey const &key,
                tvalue &&value);

        void set_insertion_strategy(
                typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept;

    protected:
        void *allocate_with_guard_my(
                size_t value_size,
                size_t values_count) const
        {
            allocator *target_allocator = get_allocator();
            return target_allocator == nullptr
                   ? ::operator new(value_size * values_count)
                   : target_allocator->allocate(value_size, values_count);
        }
        [[nodiscard]] allocator *get_allocator() const noexcept final
        {
            return this->_tree->get_allocator();
        }

    };

    class obtaining_template_method:
            public template_method_basics
    {

    public:

        explicit obtaining_template_method(
                binary_search_tree<tkey, tvalue> *tree);

    public:

        tvalue const &obtain(
                tkey const &key);

        std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
                tkey const &lower_bound,
                tkey const &upper_bound,
                bool lower_bound_inclusive,
                bool upper_bound_inclusive)
        {
            std::vector<typename associative_container<tkey, tvalue>::key_value_pair> range;

            std::stack<node *> path;
            node *current = this->_tree->_root;
            while (true)
            {
                if (current == nullptr)
                {
                    break;
                }

                auto comparison_result = this->_tree->_keys_comparer(lower_bound, current->key);
                path.push(current);

                if (comparison_result == 0)
                {
                    if (lower_bound_inclusive)
                    {
                        break;
                    }

                    current = current->right_subtree;
                }
                else if (comparison_result < 0)
                {
                    current = current->left_subtree;
                }
                else
                {
                    current = current->right_subtree;
                }

                if (current == nullptr && this->_tree->_keys_comparer(path.top()->key, lower_bound) < (lower_bound_inclusive
                                                                                                       ? 0
                                                                                                       : 1))
                {
                    path = std::move(std::stack<node *>());
                }
            }

            auto it = infix_iterator(this->_tree, path, this->_tree->create_iterator_data());
            while ((it != this->_tree->end_infix()) && (this->_tree->_keys_comparer(upper_bound, (*it)->get_key()) > (upper_bound_inclusive
                                                                                                                      ? -1
                                                                                                                      : 0)))
            {
                range.push_back(std::move(typename associative_container<tkey, tvalue>::key_value_pair((*it)->get_key(), (*it)->get_value())));
                ++it;
            }

            return range;
        }

    };

    class disposal_template_method:
            public template_method_basics,
            public allocator_guardant
    {

    private:

        binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy _disposal_strategy;

    public:

        explicit disposal_template_method(
                binary_search_tree<tkey, tvalue> *tree,
                typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy);

    public:

        virtual tvalue dispose(
                tkey const &key);

        void set_disposal_strategy(
                typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept;

    private:

        template<
                typename T>
        inline void swap(
                T &&one,
                T &&another)
        {
            T temp = std::move(one);
            one = std::move(another);
            another = std::move(temp);
        }

    private:

        [[nodiscard]] allocator *get_allocator() const noexcept final;

    };

    // endregion template methods definition

protected:

    node *_root;

protected:

    insertion_template_method *_insertion_template;

    obtaining_template_method *_obtaining_template;

    disposal_template_method *_disposal_template;

protected:

    explicit binary_search_tree(
            typename binary_search_tree<tkey, tvalue>::insertion_template_method *insertion_template,
            typename binary_search_tree<tkey, tvalue>::obtaining_template_method *obtaining_template,
            typename binary_search_tree<tkey, tvalue>::disposal_template_method *disposal_template,
            std::function<int(tkey const &, tkey const &)>,
            allocator *allocator,
            logger *logger);

public:

    explicit binary_search_tree(
            std::function<int(tkey const &, tkey const &)> comparer = std::less<tkey>(),
            allocator *allocator = nullptr,
            logger *logger = nullptr,
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:

    binary_search_tree(
            binary_search_tree<tkey, tvalue> const &other);

    binary_search_tree(
            binary_search_tree<tkey, tvalue> &&other) noexcept;

    binary_search_tree<tkey, tvalue> &operator=(
            binary_search_tree<tkey, tvalue> const &other);

    binary_search_tree<tkey, tvalue> &operator=(
            binary_search_tree<tkey, tvalue> &&other) noexcept;

    ~binary_search_tree() override;

private:

    void clear(
            node *&subtree_root)
    {
        if (subtree_root == nullptr)
        {
            return;
        }

        clear(subtree_root->left_subtree);
        clear(subtree_root->right_subtree);
        subtree_root->~node();
        this->deallocate_with_guard(subtree_root);

        subtree_root = nullptr;
    }

    node *copy(
            node const *subtree_root)
    {
        if (subtree_root == nullptr)
        {
            return nullptr;
        }

        node *subtree_root_copied = reinterpret_cast<node *>(this->allocate_with_guard_my(get_node_size(), 1));
        call_node_constructor(subtree_root_copied, subtree_root->key, subtree_root->value);
        subtree_root_copied->left_subtree = copy(subtree_root->left_subtree);
        subtree_root_copied->right_subtree = copy(subtree_root->right_subtree);

        return subtree_root_copied;
    }
    void *allocate_with_guard_my(
            size_t value_size,
            size_t values_count) const
    {
        allocator *target_allocator = template_method_basics::get_allocator();
        return target_allocator == nullptr
               ? ::operator new(value_size * values_count)
               : target_allocator->allocate(value_size, values_count);
    }
    virtual size_t get_node_size() const noexcept
    {
        return sizeof(typename binary_search_tree<tkey, tvalue>::node);
    }

    virtual void call_node_constructor(
            node *raw_space,
            tkey const &key,
            tvalue const &value)
    {
        allocator::construct(raw_space, key, value);
    }

    // TODO: should it be here or inside insertion template method?!
    virtual void call_node_constructor(
            node *raw_space,
            tkey const &key,
            tvalue &&value)
    {
        allocator::construct(raw_space, key, std::move(value));
    }

    virtual void inject_additional_data(iterator_data *destination, node *source)
    {

    }

    virtual iterator_data *create_iterator_data() const
    {
        return new iterator_data();
    }

public:

    void insert(
            tkey const &key,
            tvalue const &value) final;

    void insert(
            tkey const &key,
            tvalue &&value) final;

    tvalue const &obtain(
            tkey const &key) final;

    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
            tkey const &lower_bound,
            tkey const &upper_bound,
            bool lower_bound_inclusive,
            bool upper_bound_inclusive) final;

    tvalue dispose(
            tkey const &key) final;

public:

    void set_insertion_strategy(
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept;

    void set_removal_strategy(
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept;

public:

    // region iterators requests definition

    prefix_iterator begin_prefix() const noexcept;

    prefix_iterator end_prefix() const noexcept;

    prefix_const_iterator cbegin_prefix() const noexcept;

    prefix_const_iterator cend_prefix() const noexcept;

    prefix_reverse_iterator rbegin_prefix() const noexcept;

    prefix_reverse_iterator rend_prefix() const noexcept;

    prefix_const_reverse_iterator crbegin_prefix() const noexcept;

    prefix_const_reverse_iterator crend_prefix() const noexcept;

    infix_iterator begin_infix() const noexcept;

    infix_iterator end_infix() const noexcept;

    infix_const_iterator cbegin_infix() const noexcept;

    infix_const_iterator cend_infix() const noexcept;

    infix_reverse_iterator rbegin_infix() const noexcept;

    infix_reverse_iterator rend_infix() const noexcept;

    infix_const_reverse_iterator crbegin_infix() const noexcept;

    infix_const_reverse_iterator crend_infix() const noexcept;

    postfix_iterator begin_postfix() const noexcept;

    postfix_iterator end_postfix() const noexcept;

    postfix_const_iterator cbegin_postfix() const noexcept;

    postfix_const_iterator cend_postfix() const noexcept;

    postfix_reverse_iterator rbegin_postfix() const noexcept;

    postfix_reverse_iterator rend_postfix() const noexcept;

    postfix_const_reverse_iterator crbegin_postfix() const noexcept;

    postfix_const_reverse_iterator crend_postfix() const noexcept;

    // endregion iterators requests definition

protected:

    // region subtree rotations definition

    void small_left_rotation(
            typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
            bool validate = true) const;

    void small_right_rotation(
            typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
            bool validate = true) const;

    void big_left_rotation(
            typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
            bool validate = true) const;

    void big_right_rotation(
            typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
            bool validate = true) const;

    void double_left_rotation(
            typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
            bool at_grandparent_first,
            bool validate = true) const;

    void double_right_rotation(
            typename binary_search_tree<tkey, tvalue>::node *&subtree_root,
            bool at_grandparent_first,
            bool validate = true) const;

    // endregion subtree rotations definition

};

// region binary_search_tree<tkey, tvalue>::node methods implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::node::node(
        tkey const &key,
        tvalue const &value):
        key(key),
        value(value),
        left_subtree(nullptr),
        right_subtree(nullptr)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::node::node(
        tkey const &key,
        tvalue &&value):
        key(key),
        value(std::move(value)),
        left_subtree(nullptr),
        right_subtree(nullptr)
{

}

// endregion binary_search_tree<tkey, tvalue>::node methods implementation

// region iterators implementation

// region iterator data implementation



// endregion iterator data implementation

// region prefix_iterator implementation

template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_iterator::operator==(
        typename binary_search_tree<tkey, tvalue>::prefix_iterator const &other) const noexcept
{
    if (_state.empty() && other._state.empty())
    {
        return true;
    }
    if (_state.empty() ^ other._state.empty())
    {
        return false;
    }
    return _state.top() == other._state.top();
}

template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_iterator::operator!=(
        typename binary_search_tree<tkey, tvalue>::prefix_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator &binary_search_tree<tkey, tvalue>::prefix_iterator::operator++()
{
    if (_state.empty())
    {
        // undefined behavior
        return *this;
    }

    if (_state.top()->left_subtree != nullptr)
    {
        _state.push(_state.top()->left_subtree);
        assign_data();
    }
    else if (_state.top()->right_subtree != nullptr)
    {
        _state.push(_state.top()->right_subtree);
        assign_data();
    }
    else
    {
        do
        {
            node *current = _state.top();
            _state.pop();
            if (_state.empty())
            {
                return *this;
            }
            if (_state.top()->left_subtree == current)
            {
                if (_state.top()->right_subtree != nullptr)
                {
                    _state.push(_state.top()->right_subtree);
                    assign_data();
                    return *this;
                }
            }
        } while (true);


    }
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator const binary_search_tree<tkey, tvalue>::prefix_iterator::operator++(
        int not_used)
{
    auto it = *this;
    ++*this;
    return it;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::prefix_iterator::operator*() const
{
    if (_state.empty())
    {
        throw std::logic_error("can't get data from iterator!!1!1");
    }

    return _data;
}

// endregion prefix_iterator implementation

// region prefix_const_iterator implementation


template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator==(
        typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const &other) const noexcept
{
    if (_state.empty() && other._state.empty())
    {
        return true;
    }
    if (_state.empty() ^ other._state.empty())
    {
        return false;
    }
    return _state.top() == other._state.top();
}

template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator!=(
        typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator &binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator++()
{
    if (_state.empty())
    {
        // undefined behavior
        return *this;
    }

    if (_state.top()->left_subtree != nullptr)
    {
        _state.push(_state.top()->left_subtree);
        assign_data();
    }
    else if (_state.top()->right_subtree != nullptr)
    {
        _state.push(_state.top()->right_subtree);
        assign_data();
    }
    else
    {
        do
        {
            node *current = _state.top();
            _state.pop();
            if (_state.empty())
            {
                return *this;
            }
            if (_state.top()->left_subtree == current)
            {
                if (_state.top()->right_subtree != nullptr)
                {
                    _state.push(_state.top()->right_subtree);
                    assign_data();
                    return *this;
                }
            }
        } while (true);


    }
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator++(
        int not_used)
{
    auto it = *this;
    ++*this;
    return it;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator*() const
{
    if (_state.empty())
    {
        throw std::logic_error("can't get data from iterator!!1!1");
    }

    return _data;
}
// endregion prefix_const_iterator implementation

// region prefix_reverse_iterator implementation

template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator==(
        typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const &other) const noexcept
{
    if (_state.empty() && other._state.empty())
    {
        return true;
    }
    if (_state.empty() ^ other._state.empty())
    {
        return false;
    }
    return _state.top() == other._state.top();
}

template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator!=(
        typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator++()
{
    if (_state.empty())
    {
        // undefined behavior plk
        return *this;
    }
    node* current = _state.top();
    _state.pop();
    if (_state.empty())
    {
        return *this;
    }
    node* parent = _state.top();
    if (parent->right_subtree == current)
    {
        do
        {
            if (_state.top()->left_subtree != nullptr)
            {
                while (_state.top()->right_subtree != nullptr || _state.top()->left_subtree != nullptr)
                {
                    if (_state.top()->left_subtree != nullptr)
                    {
                        _state.push(_state.top()->left_subtree);
                        while (_state.top()->right_subtree != nullptr)
                        {
                            _state.push(_state.top()->right_subtree);
                        }
                    }
                }
                assign_data();
                return *this;
            }
            else
            {
                assign_data();
                return *this;
            }
        } while (true);

    }
    else
    {
        assign_data();
        return *this;
    }
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator++(
        int not_used)
{
    auto it = *this;
    ++*this;
    return it;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator*() const
{
    if (_state.empty())
    {
        throw std::logic_error("can't get data from iterator!!1!1");
    }

    return _data;
}

// endregion prefix_reverse_iterator implementation

// region prefix_const_reverse_iterator implementation


template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator==(
        typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const &other) const noexcept
{
    if (_state.empty() && other._state.empty())
    {
        return true;
    }
    if (_state.empty() ^ other._state.empty())
    {
        return false;
    }
    return _state.top() == other._state.top();
}

template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator!=(
        typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator++()
{
    if (_state.empty())
    {
        // undefined behavior plk
        return *this;
    }
    node* current = _state.top();
    _state.pop();
    if (_state.empty())
    {
        return *this;
    }
    node* parent = _state.top();
    if (parent->right_subtree == current)
    {
        do
        {
            if (_state.top()->left_subtree != nullptr)
            {
                while (_state.top()->right_subtree != nullptr || _state.top()->left_subtree != nullptr)
                {
                    if (_state.top()->left_subtree != nullptr)
                    {
                        _state.push(_state.top()->left_subtree);
                        while (_state.top()->right_subtree != nullptr)
                        {
                            _state.push(_state.top()->right_subtree);
                        }
                    }
                }
                assign_data();
                return *this;
            }
            else
            {
                assign_data();
                return *this;
            }
        } while (true);

    }
    else
    {
        assign_data();
        return *this;
    }
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator++(
        int not_used)
{
    auto it = *this;
    ++*this;
    return it;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator*() const
{
    if (_state.empty())
    {
        throw std::logic_error("can't get data from iterator!!1!1");
    }

    return _data;
}

// endregion prefix_const_reverse_iterator implementation

// region infix_iterator implementation

template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_iterator::operator==(
        typename binary_search_tree<tkey, tvalue>::infix_iterator const &other) const noexcept
{
    if (_state.empty() && other._state.empty())
    {
        return true;
    }
    if (_state.empty() ^ other._state.empty())
    {
        return false;
    }
    return _state.top() == other._state.top();
}

template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_iterator::operator!=(
        typename binary_search_tree<tkey, tvalue>::infix_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator &binary_search_tree<tkey, tvalue>::infix_iterator::operator++()
{
    if (_state.empty())
    {
        // undefined behavior
        return *this;
    }
    //logger* lg = get_logger(); todo
    if (_state.top()->right_subtree != nullptr)
    {
        _state.push(_state.top()->right_subtree);
        while (_state.top()->left_subtree != nullptr)
        {
            _state.push(_state.top()->left_subtree);
        }
        assign_data();
    }
    else
    {
        node *current = nullptr;
        do
        {
            current = _state.top();
            _state.pop();
            if (_state.empty())
            {
                return *this;
            }
            if (_state.top()->left_subtree == current)
            {
                assign_data();
                return *this;
            }
        }
        while (true);
    }
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator const binary_search_tree<tkey, tvalue>::infix_iterator::operator++(
        int not_used)
{
    auto it = *this;
    ++*this;
    return it;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::infix_iterator::operator*() const
{
    if (_state.empty())
    {
        throw std::logic_error("can't get data from iterator!!1!1");
    }

    return _data;
}

// endregion infix_iterator implementation

// region infix_const_iterator implementation

template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_iterator::operator==(
        typename binary_search_tree<tkey, tvalue>::infix_const_iterator const &other) const noexcept
{
    if (_state.empty() && other._state.empty())
    {
        return true;
    }
    if (_state.empty() ^ other._state.empty())
    {
        return false;
    }
    return _state.top() == other._state.top();
}

template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_iterator::operator!=(
        typename binary_search_tree<tkey, tvalue>::infix_const_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator &binary_search_tree<tkey, tvalue>::infix_const_iterator::operator++()
{
    if (_state.empty())
    {
        // undefined behavior
        return *this;
    }

    if (_state.top()->right_subtree != nullptr)
    {
        _state.push(_state.top()->right_subtree);
        while (_state.top()->left_subtree != nullptr)
        {
            _state.push(_state.top()->left_subtree);
        }

        assign_data();
    }
    else
    {
        node *current = nullptr;
        do
        {
            current = _state.top();
            _state.pop();
            if (_state.empty())
            {
                return *this;
            }
            if (_state.top()->left_subtree == current)
            {
                assign_data();
                return *this;
            }
        }
        while (true);
    }
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator const binary_search_tree<tkey, tvalue>::infix_const_iterator::operator++(
        int not_used)
{
    auto it = *this;
    ++*this;
    return it;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::infix_const_iterator::operator*() const
{
    if (_state.empty())
    {
        throw std::logic_error("can't get data from iterator!!1!1");
    }

    return _data;
}

// endregion infix_const_iterator implementation

// region infix_reverse_iterator implementation


template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator==(
        typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const &other) const noexcept
{
    if (_state.empty() && other._state.empty())
    {
        return true;
    }
    if (_state.empty() ^ other._state.empty())
    {
        return false;
    }
    return _state.top() == other._state.top();
}

template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator!=(
        typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator++()
{
    if (_state.empty())
    {
        // undefined behavior
        return *this;
    }

    if (_state.top()->left_subtree != nullptr)
    {
        _state.push(_state.top()->left_subtree);
        while (_state.top()->right_subtree != nullptr)
        {
            _state.push(_state.top()->right_subtree);
        }

        assign_data();
    }
    else
    {
        node *current = nullptr;
        do
        {
            current = _state.top();
            _state.pop();
            if (_state.empty())
            {
                return *this;
            }
            if (_state.top()->right_subtree == current)
            {
                assign_data();
                return *this;
            }
        }
        while (true);
    }
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator++(
        int not_used)
{
    auto it = *this;
    ++*this;
    return it;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator*() const
{
    if (_state.empty())
    {
        throw std::logic_error("can't get data from iterator!!1!1");
    }

    return _data;
}
// endregion infix_reverse_iterator implementation

// region infix_const_reverse_iterator implementation


template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator==(
        typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const &other) const noexcept
{
    if (_state.empty() && other._state.empty())
    {
        return true;
    }
    if (_state.empty() ^ other._state.empty())
    {
        return false;
    }
    return _state.top() == other._state.top();
}

template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator!=(
        typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator&  binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator++()
{
    if (_state.empty())
    {
        // undefined behavior
        return *this;
    }

    if (_state.top()->left_subtree != nullptr)
    {
        _state.push(_state.top()->left_subtree);
        while (_state.top()->right_subtree != nullptr)
        {
            _state.push(_state.top()->right_subtree);
        }

        assign_data();
    }
    else
    {
        node *current = nullptr;
        do
        {
            current = _state.top();
            _state.pop();
            if (_state.empty())
            {
                return *this;
            }
            if (_state.top()->right_subtree == current)
            {
                assign_data();
                return *this;
            }
        }
        while (true);
    }
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator++(
        int not_used)
{
    auto it = *this;
    ++*this;
    return it;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator*() const
{
    if (_state.empty())
    {
        throw std::logic_error("can't get data from iterator!!1!1");
    }

    return _data;
}
// endregion infix_const_reverse_iterator implementation

// region postfix_iterator implementation

template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_iterator::operator==(
        typename binary_search_tree<tkey, tvalue>::postfix_iterator const &other) const noexcept
{
    if (_state.empty() && other._state.empty())
    {
        return true;
    }
    if (_state.empty() ^ other._state.empty())
    {
        return false;
    }
    return _state.top() == other._state.top();
}

template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_iterator::operator!=(
        typename binary_search_tree<tkey, tvalue>::postfix_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator &binary_search_tree<tkey, tvalue>::postfix_iterator::operator++()
{
    if (_state.empty())
    {
        // undefined behavior
        return *this;
    }
    node* current = _state.top();
    _state.pop();
    if (_state.empty())
    {
        // undefined behavior
        return *this;
    }
    node* parent = _state.top();
    if (parent->left_subtree == current)
    {
        do
        {
            if (_state.top()->right_subtree != nullptr)
            {
                while (_state.top()->left_subtree != nullptr || _state.top()->right_subtree != nullptr)
                {
                    if (_state.top()->right_subtree != nullptr)
                    {
                        _state.push(_state.top()->right_subtree);
                        while (_state.top()->left_subtree != nullptr)
                        {
                            _state.push(_state.top()->left_subtree);
                        }
                    }
                }
                assign_data();
                return *this;
            }
            else
            {
                assign_data();
                return *this;
            }
        } while (true);
    }
    else
    {
        assign_data();
        return *this;
    }
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator const binary_search_tree<tkey, tvalue>::postfix_iterator::operator++(
        int not_used)
{
    auto it = *this;
    ++*this;
    return it;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::postfix_iterator::operator*() const
{
    if (_state.empty())
    {
        throw std::logic_error("can't get data from iterator!!1!1");
    }

    return _data;
}

// endregion postfix_iterator implementation

// region postfix_const_iterator implementation

template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator==(
        typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const &other) const noexcept
{
    if (_state.empty() && other._state.empty())
    {
        return true;
    }
    if (_state.empty() ^ other._state.empty())
    {
        return false;
    }
    return _state.top() == other._state.top();
}

template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator!=(
        typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator &binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator++()
{//TODO check
    if (_state.empty())
    {
        // undefined behavior
        return *this;
    }
    node* current = _state.top();
    _state.pop();
    if (_state.empty())
    {
        // undefined behavior
        return *this;
    }
    node* parent = _state.top();
    if (parent->left_subtree == current)
    {
        do
        {
            if (_state.top()->right_subtree != nullptr)
            {
                while (_state.top()->left_subtree != nullptr || _state.top()->right_subtree != nullptr)
                {
                    if (_state.top()->right_subtree != nullptr)
                    {
                        _state.push(_state.top()->right_subtree);
                        while (_state.top()->left_subtree != nullptr)
                        {
                            _state.push(_state.top()->left_subtree);
                        }
                    }
                }
                assign_data();
                return *this;
            }
            else
            {
                assign_data();
                return *this;
            }
        } while (true);
    }
    else
    {
        assign_data();
        return *this;
    }
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator++(
        int not_used)
{
    auto it = *this;
    ++*this;
    return it;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator*() const
{
    if (_state.empty())
    {
        throw std::logic_error("can't get data from iterator!!1!1");
    }

    return _data;
}

// endregion postfix_const_iterator implementation

// region postfix_reverse_iterator implementation

template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator==(
        typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const &other) const noexcept
{
    if (_state.empty() && other._state.empty())
    {
        return true;
    }
    if (_state.empty() ^ other._state.empty())
    {
        return false;
    }
    return _state.top() == other._state.top();
}

template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator!=(
        typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator++()
{
    if (_state.empty())
    {
        // undefined behavior
        return *this;
    }
    node* current = _state.top();
    _state.pop();
    if (_state.empty())
    {
        // undefined behavior
        return *this;
    }
    node* parent = _state.top();
    if (parent->left_subtree == current)
    {
        do
        {
            if (_state.top()->right_subtree != nullptr)
            {
                while (_state.top()->left_subtree != nullptr || _state.top()->right_subtree != nullptr)
                {
                    if (_state.top()->right_subtree != nullptr)
                    {
                        _state.push(_state.top()->right_subtree);
                        while (_state.top()->left_subtree != nullptr)
                        {
                            _state.push(_state.top()->left_subtree);
                        }
                    }
                }
                assign_data();
                return *this;
            }
            else
            {
                assign_data();
                return *this;
            }
        } while (true);
    }
    else
    {
        assign_data();
        return *this;
    }
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator++(
        int not_used)
{
    auto it = *this;
    ++*this;
    return it;
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator*() const
{
    if (_state.empty())
    {
        throw std::logic_error("can't get data from iterator!!1!1");
    }

    return _data;
}


// endregion postfix_reverse_iterator implementation

// region postfix_const_reverse_iterator implementation

template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator==(
        typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const &other) const noexcept
{
    if (_state.empty() && other._state.empty())
    {
        return true;
    }
    if (_state.empty() ^ other._state.empty())
    {
        return false;
    }
    return _state.top() == other._state.top();
}

template<
        typename tkey,
        typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator!=(
        typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator++()
{
    if (_state.empty())
    {
        // undefined behavior
        return *this;
    }

    if (_state.top()->right_subtree != nullptr)
    {
        _state.push(_state.top()->right_subtree);
        assign_data();
    }
    else if (_state.top()->left_subtree != nullptr)
    {
        _state.push(_state.top()->left_subtree);
        assign_data();
    }
    else
    {
        do
        {
            node *current = _state.top();
            _state.pop();
            if (_state.empty())
            {
                return *this;
            }
            if (_state.top()->right_subtree == current)
            {
                if (_state.top()->left_subtree != nullptr)
                {
                    _state.push(_state.top()->left_subtree);
                    assign_data();
                    return *this;
                }
            }
        } while (true);

    }
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator++(
        int not_used)
{
    auto it = *this;
    ++*this;
    return it;
}
template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator*() const
{
    if (_state.empty())
    {
        throw std::logic_error("can't get data from iterator!!1!1");
    }

    return _data;
}

// endregion postfix_const_reverse_iterator implementation

// endregion iterators implementation

// region target operations associated exception types implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception::insertion_of_existent_key_attempt_exception(
        tkey const &key):
        std::logic_error("Attempt to insert already existing key inside the tree.")
{

}

template<
        typename tkey,
        typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception::obtaining_of_nonexistent_key_attempt_exception(
        tkey const &key):
        std::logic_error("Attempt to obtain a value by non-existing key from the tree.")
{

}

template<
        typename tkey,
        typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception::disposal_of_nonexistent_key_attempt_exception(
        tkey const &key):
        std::logic_error("Attempt to dispose a value by non-existing key from the tree.")
{

}

template<
        typename tkey,
        typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

// endregion target operations associated exception types implementation

// region template methods implementation

// region binary_search_tree<tkey, tvalue>::template_method_basics implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(
        binary_search_tree<tkey, tvalue> *tree)
        : _tree(tree)
{

}

template<
        typename tkey,
        typename tvalue>
[[nodiscard]] inline logger *binary_search_tree<tkey, tvalue>::template_method_basics::get_logger() const noexcept
{
    return get_logger();
}

// endregion binary_search_tree<tkey, tvalue>::template_method_basics implementation

// region search_tree<tkey, tvalue>::insertion_template_method implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::insertion_template_method::insertion_template_method(
        binary_search_tree<tkey, tvalue> *tree,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy):
        binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(tree),
        _insertion_strategy(insertion_strategy)
{

}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::insert(
        tkey const &key,
        tvalue const &value)
{
    auto path = this->find_path(key);
    if (*(path.top()) != nullptr)
    {
        switch (_insertion_strategy)
        {
            case insertion_of_existent_key_attempt_strategy::throw_an_exception:
                throw insertion_of_existent_key_attempt_exception(key);
            case insertion_of_existent_key_attempt_strategy::update_value:
                (*(path.top()))->value = value;
                break;
        }

        return;
    }
    this->_tree->call_node_constructor(*(path.top()) = reinterpret_cast<node*>(allocate_with_guard_my(this->_tree->get_node_size(), 1)), key, value);
    this->balance(path);
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::insert(
        tkey const &key,
        tvalue &&value)
{
    auto path = this->find_path(key);
    if (*(path.top()) != nullptr)
    {
        switch (_insertion_strategy)
        {
            case insertion_of_existent_key_attempt_strategy::throw_an_exception:
                throw insertion_of_existent_key_attempt_exception(key);
            case insertion_of_existent_key_attempt_strategy::update_value:
                (*(path.top()))->value = std::move(value);
                break;
        }

        return;
    }

    this->_tree->call_node_constructor(*(path.top()) = reinterpret_cast<node *>(allocate_with_guard_my(this->_tree->get_node_size(), 1)), key, value);

    this->balance(path);
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::insertion_template_method::set_insertion_strategy(
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept
{
    _insertion_strategy = insertion_strategy;
}


// endregion search_tree<tkey, tvalue>::insertion_template_method implementation

// region search_tree<tkey, tvalue>::obtaining_template_method implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::obtaining_template_method::obtaining_template_method(
        binary_search_tree<tkey, tvalue> *tree):
        binary_search_tree<tkey, tvalue>::template_method_basics::template_method_basics(tree)
{

}

template<
        typename tkey,
        typename tvalue>
tvalue const &binary_search_tree<tkey, tvalue>::obtaining_template_method::obtain(
        tkey const &key)
{
    auto path = this->find_path(key);
    if (*(path.top()) == nullptr)
    {
        throw obtaining_of_nonexistent_key_attempt_exception(key);
    }

    tvalue const &got_value = (*(path.top()))->value;

    this->balance(path);

    return got_value;
}

// endregion search_tree<tkey, tvalue>::obtaining_template_method implementation

// region search_tree<tkey, tvalue>::disposal_template_method implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::disposal_template_method::disposal_template_method(
        binary_search_tree<tkey, tvalue> *tree,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
        binary_search_tree<tkey, tvalue>::template_method_basics(tree)
{

}

template<
        typename tkey,
        typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::disposal_template_method::dispose(
        tkey const &key)
{
    auto path = this->find_path(key);
    if (*(path.top()) == nullptr)
    {
        switch (_disposal_strategy)
        {
            case disposal_of_nonexistent_key_attempt_strategy::throw_an_exception:
                throw disposal_of_nonexistent_key_attempt_exception(key);
            case disposal_of_nonexistent_key_attempt_strategy::do_nothing:
                return tvalue();
        }
    }

    if ((*(path.top()))->left_subtree != nullptr && (*(path.top()))->right_subtree != nullptr)
    {
        auto *target_to_swap = *(path.top());
        auto **current = &((*(path.top()))->left_subtree);

        while (*current != nullptr)
        {
            path.push(current);
            current = &((*current)->right_subtree);
        }

        swap(std::move(target_to_swap->key), std::move((*(path.top()))->key));
        swap(std::move(target_to_swap->value), std::move((*(path.top()))->value));
    }

    tvalue value = std::move((*(path.top()))->value);
    //tvalue value = (*(path.top()))->value;
    node *leftover_subtree = (*(path.top()))->left_subtree == nullptr
                             ? (*(path.top()))->right_subtree
                             : (*(path.top()))->left_subtree;
    allocator::destruct(*(path.top()));
    deallocate_with_guard(*(path.top()));
    *(path.top()) = leftover_subtree;
    this->balance(path);
    return value;
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::disposal_template_method::set_disposal_strategy(
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept
{
    _disposal_strategy = disposal_strategy;
}

template<
        typename tkey,
        typename tvalue>
[[nodiscard]] inline allocator *binary_search_tree<tkey, tvalue>::disposal_template_method::get_allocator() const noexcept
{
    return this->_tree->get_allocator();
}

// endregion search_tree<tkey, tvalue>::disposal_template_method implementation

// endregion template methods

// region construction, assignment, destruction implementation

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
        typename binary_search_tree<tkey, tvalue>::insertion_template_method *insertion_template,
        typename binary_search_tree<tkey, tvalue>::obtaining_template_method *obtaining_template,
        typename binary_search_tree<tkey, tvalue>::disposal_template_method *disposal_template,
        std::function<int(tkey const &, tkey const &)> comparer,
        allocator *allocator,
        logger *logger):
        search_tree<tkey, tvalue>(comparer, logger, allocator),
        _root(nullptr),
        _insertion_template(insertion_template),
        _obtaining_template(obtaining_template),
        _disposal_template(disposal_template)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
        std::function<int(tkey const &, tkey const &)> keys_comparer,
        allocator *allocator,
        logger *logger,
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
        binary_search_tree(
                new binary_search_tree<tkey, tvalue>::insertion_template_method(this, insertion_strategy),
                new binary_search_tree<tkey, tvalue>::obtaining_template_method(this),
                new binary_search_tree<tkey, tvalue>::disposal_template_method(this, disposal_strategy),
                keys_comparer,
                allocator,
                logger)
{

}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
        binary_search_tree<tkey, tvalue> const &other):
        search_tree<tkey, tvalue>(other._keys_comparer, other.get_logger(), other.get_allocator()),
        _insertion_template(new binary_search_tree<tkey, tvalue>::insertion_template_method(this, binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception)),
        _obtaining_template(new binary_search_tree<tkey, tvalue>::obtaining_template_method(this)),
        _disposal_template(new binary_search_tree<tkey, tvalue>::disposal_template_method(this, binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception))
{
    _root = copy(other._root);
    // TODO: handle strategy copying!!1!1
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
        binary_search_tree<tkey, tvalue> &&other) noexcept:
        search_tree<tkey, tvalue>(other._keys_comparer, other.get_logger(), other.get_allocator())
{
    // other._logger = nullptr;
    // other._allocator = nullptr;

    _insertion_template = other._insertion_template;
    other._insertion_template = nullptr;

    _obtaining_template = other._obtaining_template;
    other._obtaining_template = nullptr;

    _disposal_template = other._disposal_template;
    other._disposal_template = nullptr;

    _root = other._root;
    other._root = nullptr;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(
        binary_search_tree<tkey, tvalue> const &other)
{
    if (this != &other)
    {
        // TODO: assign template methods or not?!
        clear(_root);
        this->_allocator = other._allocator;
        this->_logger = other._logger;
        this->_keys_comparer = other._keys_comparer;
        _root = copy(other._root);
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(
        binary_search_tree<tkey, tvalue> &&other) noexcept
{
    if (this != &other)
    {
        clear(_root);
        this->_keys_comparer = other._keys_comparer;

        this->_logger = other._logger;
        other._logger = nullptr;

        this->_allocator = other._allocator;
        other._allocator = nullptr;

        _insertion_template = other._insertion_template;
        other._insertion_template = nullptr;

        _obtaining_template = other._obtaining_template;
        other._obtaining_template = nullptr;

        _disposal_template = other._disposal_template;
        other._disposal_template = nullptr;

        _root = other._root;
        other._root = nullptr;
    }

    return *this;
}

template<
        typename tkey,
        typename tvalue>
binary_search_tree<tkey, tvalue>::~binary_search_tree()
{
    delete _insertion_template;
    delete _obtaining_template;
    delete _disposal_template;
    clear(_root);
}

// endregion construction, assignment, destruction implementation

// region associative_container<tkey, tvalue> contract implementation

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::insert(
        tkey const &key,
        tvalue const &value)
{
    _insertion_template->insert(key, value);
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::insert(
        tkey const &key,
        tvalue &&value)
{
    _insertion_template->insert(key, std::move(value));
}

template<
        typename tkey,
        typename tvalue>
tvalue const &binary_search_tree<tkey, tvalue>::obtain(
        tkey const &key)
{
    return _obtaining_template->obtain(key);
}

template<
        typename tkey,
        typename tvalue>
std::vector<typename associative_container<tkey, tvalue>::key_value_pair> binary_search_tree<tkey, tvalue>::obtain_between(
        tkey const &lower_bound,
        tkey const &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive)
{
    return _obtaining_template->obtain_between(lower_bound, upper_bound, lower_bound_inclusive, upper_bound_inclusive);
}

template<
        typename tkey,
        typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::dispose(
        tkey const &key)
{
    return _disposal_template->dispose(key);
}

// endregion associative_containers contract implementations

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::set_insertion_strategy(
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept
{
    this->_insertion_template->_insertion_strategy = insertion_strategy;
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::set_removal_strategy(
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept
{
    this->_disposal_template->_disposal_strategy = disposal_strategy;
}

// region iterators requesting implementation

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator binary_search_tree<tkey, tvalue>::begin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_iterator(this, _root, create_iterator_data());
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator binary_search_tree<tkey, tvalue>::end_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_iterator(this, nullptr, create_iterator_data());
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_iterator(this, _root, create_iterator_data());
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator binary_search_tree<tkey, tvalue>::cend_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_iterator(this, nullptr, create_iterator_data());
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_reverse_iterator(this, _root, create_iterator_data());
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_reverse_iterator(this, nullptr, create_iterator_data());
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator(this, _root, create_iterator_data());
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_prefix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator(this, nullptr, create_iterator_data());
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator binary_search_tree<tkey, tvalue>::begin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_iterator(this, _root, create_iterator_data());
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator binary_search_tree<tkey, tvalue>::end_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_iterator(this, nullptr, create_iterator_data());
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_iterator(this, _root, create_iterator_data());
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator binary_search_tree<tkey, tvalue>::cend_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_iterator(this, nullptr, create_iterator_data());
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_reverse_iterator(dynamic_cast<typename binary_search_tree<tkey, tvalue>::node *>(_root));
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_reverse_iterator(this, nullptr, create_iterator_data());
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator(this, _root, create_iterator_data());
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_infix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator(this, nullptr, create_iterator_data());
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator binary_search_tree<tkey, tvalue>::begin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_iterator(this, _root, create_iterator_data());
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator binary_search_tree<tkey, tvalue>::end_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_iterator(this, nullptr, create_iterator_data());
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_iterator(this, _root, create_iterator_data());
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator binary_search_tree<tkey, tvalue>::cend_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_iterator(this, nullptr, create_iterator_data());
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_reverse_iterator(this, _root, create_iterator_data());
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_reverse_iterator(this, nullptr, create_iterator_data());
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator(this, _root, create_iterator_data());
}

template<
        typename tkey,
        typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_postfix() const noexcept
{
    return binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator(this, nullptr, create_iterator_data());
}

// endregion iterators request implementation

// region subtree rotations implementation

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::small_left_rotation(
        binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate) const
{
    if(validate && (subtree_root == nullptr || subtree_root->right_subtree == nullptr)) {
        throw std::logic_error("can't rotate");
    }

    node *new_root = subtree_root->right_subtree;
    subtree_root->right_subtree = new_root->left_subtree;
    new_root->left_subtree = subtree_root;
    subtree_root = new_root;

}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::small_right_rotation(
        binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate) const
{
    if(validate && (subtree_root == nullptr || subtree_root->left_subtree == nullptr)) {
        throw std::logic_error("can't rotate");
    }

    node *new_root = subtree_root->left_subtree;
    subtree_root->left_subtree = new_root->right_subtree;
    new_root->right_subtree = subtree_root;
    subtree_root = new_root;
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::big_left_rotation(
        binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate) const
{
    if (validate && (subtree_root == nullptr || subtree_root->right_subtree == nullptr || subtree_root->right_subtree->left_subtree == nullptr)) {
        throw std::logic_error("can't rotate");
    }

    node *right_child = subtree_root->right_subtree;
    node *right_left_child = right_child->left_subtree;

    right_child->left_subtree = right_left_child->right_subtree;
    right_left_child->right = right_child;
    subtree_root->right = right_left_child;

    // node *temporary_root = subtree_root;
    // subtree_root = right_left_child;

    small_left_rotation(subtree_root, validate);
}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::big_right_rotation(
        binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool validate) const
{//малый левый левого ребенка - малый правый корня
    if (validate && (subtree_root == nullptr || subtree_root->left_subtree == nullptr || subtree_root->left_subtree->right_subtree == nullptr)) {
        throw std::logic_error("can't rotate");
    }
    node* left_child = subtree_root->left_subtree;
    node* left_right_child = left_child->right_subtree;

    subtree_root->left_subtree->right_subtree = left_right_child->left_subtree;
    left_right_child->left_subtree = left_child;
    subtree_root->left_subtree = left_right_child;

    small_right_rotation(subtree_root, validate);

}

template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::double_left_rotation(
        binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool at_grandparent_first,
        bool validate) const
{

    if (at_grandparent_first)
    {
        if (validate && (subtree_root == nullptr || subtree_root->left_subtree == nullptr || subtree_root->left_subtree->right_subtree == nullptr))
        {
            throw std::logic_error("can't rotate");
        }

        node *left_child = subtree_root->left_subtree;
        node *left_right_child = left_child->right_subtree;

        left_child->right_subtree = left_right_child->left_subtree;
        left_right_child->left_subtree = left_child;
        subtree_root->left_subtree = left_right_child;

        // node *temporary_root = subtree_root;
        // subtree_root = left_right_child;

        small_left_rotation(subtree_root, validate);
    }
    else
    {
        small_left_rotation(subtree_root, validate);
        if (validate && (subtree_root == nullptr || subtree_root->left_subtree == nullptr || subtree_root->left_subtree->right_subtree == nullptr))
        {
            throw std::logic_error("can't rotate");
        }

        node *left_child = subtree_root->left_subtree;
        node *left_right_child = left_child->right_subtree;

        left_child->right_subtree = left_right_child->left_subtree;
        left_right_child->left_subtree = left_child;
        subtree_root->left_subtree = left_right_child;
    }
}
template<
        typename tkey,
        typename tvalue>
void binary_search_tree<tkey, tvalue>::double_right_rotation(
        binary_search_tree<tkey, tvalue>::node *&subtree_root,
        bool at_grandparent_first,
        bool validate) const
{//малый правый правого ребенка - малый правый корня
    if (subtree_root == nullptr || subtree_root->right_subtree == nullptr || subtree_root->right_subtree->left_subtree == nullptr) {
        throw std::logic_error("can't rotate");
    }
    node *right_child = subtree_root->right_subtree;
    node *right_left_child = right_child->left_subtree;

    right_child->left_subtree = right_left_child->right_subtree;
    right_left_child->right = right_child;
    subtree_root->right = right_left_child;

    small_right_rotation(subtree_root, validate);
}



// endregion subtree rotations implementation

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H