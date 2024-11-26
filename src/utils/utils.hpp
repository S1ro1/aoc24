#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <type_traits>
#include <map>
#include <optional>

namespace utils
{
  inline std::string trim(std::string_view str,
                          const std::optional<std::string_view> &whitespace = std::nullopt)
  {
    const auto chars = whitespace.value_or(" \t");
    const auto start = str.find_first_not_of(chars);
    if (start == std::string_view::npos)
    {
      return "";
    }
    const auto end = str.find_last_not_of(chars);
    return std::string(str.substr(start, end - start + 1));
  }

  template <typename T>
  T parse_element(const std::string &line)
  {
    std::stringstream converter(line);
    T value;
    if (converter >> value)
    {
      return value;
    }
    throw std::runtime_error("Unable to parse line: " + line);
  }

  inline std::string read_file_content(const std::string &filename, const std::string &delimiter = "\n")
  {
    std::ifstream file(filename);
    if (!file.is_open())
    {
      throw std::runtime_error("Unable to open file: " + filename);
    }

    std::string content;
    std::string line;
    while (std::getline(file, line))
    {
      content += line + delimiter;
    }
    return content;
  }

  inline auto split_string = [](std::string_view content, std::string_view delimiter)
  {
    std::vector<std::string> tokens;
    size_t pos = 0;
    while ((pos = content.find(delimiter)) != std::string_view::npos)
    {
      auto token = content.substr(0, pos);
      if (!token.empty())
      {
        tokens.emplace_back(token);
      }
      content = content.substr(pos + delimiter.length());
    }
    if (!content.empty())
    {
      tokens.emplace_back(content);
    }
    return tokens;
  };

  template <typename T>
  std::vector<T> parse_and_split(const std::string &filename,
                                 const std::string &delimiter1 = "\n",
                                 const std::string &delimiter2 = "")
  {
    const auto content = read_file_content(filename, delimiter1);
    std::vector<T> result;

    const auto process_token = [&result](const std::string &token)
    {
      if (!token.empty())
      {
        result.push_back(parse_element<T>(token));
      }
    };

    if (delimiter2.empty())
    {
      std::ranges::for_each(split_string(content, delimiter1), process_token);
    }
    else
    {
      for (const auto &line : split_string(content, delimiter1))
      {
        std::ranges::for_each(split_string(line, delimiter2), process_token);
      }
    }

    return result;
  }

  template <typename T>
  std::vector<std::vector<T>> parse_and_split_2d(const std::string &filename,
                                                 const std::string &delimiter1 = "\n",
                                                 const std::string &delimiter2 = ",")
  {
    const auto content = read_file_content(filename, delimiter1);
    std::vector<std::vector<T>> result;

    const auto process_line = [&delimiter2](const std::string &line)
    {
      std::vector<T> row;
      for (const auto &token : split_string(line, delimiter2))
      {
        if (!token.empty())
        {
          row.push_back(parse_element<T>(token));
        }
      }
      return row;
    };

    for (const auto &line : split_string(content, delimiter1))
    {
      if (auto row = process_line(line); !row.empty())
      {
        result.push_back(std::move(row));
      }
    }

    return result;
  }

  template <typename K, typename V>
  std::map<K, V> parse_to_map(const std::string &filename,
                              const std::string &pair_delimiter = "\n",
                              const std::string &kv_delimiter = ":")
  {
    const auto content = read_file_content(filename, pair_delimiter);
    std::map<K, V> result;

    const auto process_pair = [&kv_delimiter](const std::string &pair)
        -> std::optional<std::pair<std::string, std::string>>
    {
      if (auto delim_pos = pair.find(kv_delimiter); delim_pos != std::string::npos)
      {
        auto key = trim(std::string_view(pair.substr(0, delim_pos)));
        auto value = trim(std::string_view(pair.substr(delim_pos + kv_delimiter.length())));
        if (!key.empty() && !value.empty())
        {
          return std::make_pair(key, value);
        }
      }
      return std::nullopt;
    };

    for (const auto &pair : split_string(content, pair_delimiter))
    {
      if (auto kv = process_pair(pair))
      {
        auto &[key_str, value_str] = *kv;
        K key = parse_element<K>(key_str);
        V value = parse_element<V>(value_str);
        result.emplace(std::move(key), std::move(value));
      }
    }

    return result;
  }

} // namespace utils
