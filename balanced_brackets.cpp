#include <stack>
#include <string>

bool is_opening_bracket(char ch)
{
    return ch == '('
        || ch == '{'
        || ch == '[';
}

bool is_closing_bracket(char ch)
{
    return ch == ')'
        || ch == ']'
        || ch == '}';
}

bool is_corresponding_brackets(char left, char right)
{
    return left == '(' && right == ')'
        || left == '{' && right == '}'
        || left == '[' && right == ']';
}

bool balancedBrackets(const std::string)
{
    std::stack<char> stack;

    for (char ch : str) {
        if (is_opening_bracket(ch)) {
            stack.push(ch);
            continue;
        }

        if (is_closing_bracket(ch)) {
            if (stack.empty())
                return false; // there is no opening corresponding bracket in the first place

            char top = stack.top();
            stack.pop();

            if (!is_corresponding_brackets(top, ch))
                return false; // top = left because it is the actual previous bracket that needs closing

            continue;
        }
    }

    return s.empty();
}
