#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <windows.h>


enum class Priority { LOW, MEDIUM, HIGH };

struct Task {
    int id;
    std::string name;
    Priority priority;
};


template <typename Iterator, typename Predicate>
class FilterIterator {
private:
    Iterator current;
    Iterator end;
    Predicate pred;

    void satisfy_predicate() {
        while (current != end && !pred(*current)) {
            ++current;
        }
    }

public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = typename std::iterator_traits<Iterator>::value_type;
    using reference = typename std::iterator_traits<Iterator>::reference;
    using pointer = typename std::iterator_traits<Iterator>::pointer;
    using difference_type = typename std::iterator_traits<Iterator>::difference_type;

    FilterIterator(Iterator begin, Iterator end, Predicate p)
        : current(begin), end(end), pred(p) {
        satisfy_predicate();
    }

    reference operator*() const { return *current; }

    pointer operator->() const { return &(*current); }

    FilterIterator& operator++() {
        if (current != end) {
            ++current;
            satisfy_predicate();
        }
        return *this;
    }

    FilterIterator operator++(int) {
        FilterIterator temp = *this;
        ++(*this);
        return temp;
    }

    bool operator==(const FilterIterator& other) const { return current == other.current; }
    bool operator!=(const FilterIterator& other) const { return current != other.current; }
};

template <typename Iterator, typename Predicate>
FilterIterator<Iterator, Predicate> make_filter_iterator(Iterator begin, Iterator end, Predicate p) {
    return FilterIterator<Iterator, Predicate>(begin, end, p);
}


int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::vector<Task> tasks = {
        {1, "Исправить опечатку в документации", Priority::LOW},
        {2, "Устранить критический сбой сервера", Priority::HIGH},
        {3, "Обновить интерфейс профиля пользователя", Priority::MEDIUM},
        {4, "Закрыть уязвимость безопасности", Priority::HIGH},
        {5, "Провести рефакторинг модуля логирования", Priority::LOW}
    };

    auto is_high_priority = [](const Task& t) {
        return t.priority == Priority::HIGH;
    };

    auto filter_begin = make_filter_iterator(tasks.begin(), tasks.end(), is_high_priority);
    auto filter_end = make_filter_iterator(tasks.end(), tasks.end(), is_high_priority);

    std::cout << "--- Задачи с ВЫСОКИМ приоритетом ---\n";
    for (auto it = filter_begin; it != filter_end; ++it) {
        std::cout << "ID: " << it->id << " | Задача: " << it->name << "\n";
    }

    return 0;
}