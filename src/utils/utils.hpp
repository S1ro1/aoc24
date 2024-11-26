#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <type_traits>
#include <map>

namespace utils
{

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

  inline std::string read_file_content(const std::string& filename, const std::string& delimiter = "\n")
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

  template <typename T>
  std::vector<T> parse_and_split(const std::string &filename,
                                 const std::string &delimiter1 = "\n",
                                 const std::string &delimiter2 = "")
  {
    std::string content = read_file_content(filename, delimiter1);
    std::vector<T> result;
    
    if (delimiter2.empty())
    {
      std::stringstream ss(content);
      std::string token;
      while (std::getline(ss, token, delimiter1[0]))
      {
        if (token.empty())
          continue;
        result.push_back(parse_element<T>(token));
      }
    }
    else
    {
      // Two delimiters mode
      size_t pos = 0;
      while ((pos = content.find(delimiter1)) != std::string::npos)
      {
        std::string line = content.substr(0, pos);
        content.erase(0, pos + delimiter1.length());

        size_t inner_pos = 0;
        while ((inner_pos = line.find(delimiter2)) != std::string::npos)
        {
          std::string token = line.substr(0, inner_pos);
          if (!token.empty())
          {
            result.push_back(parse_element<T>(token));
          }
          line.erase(0, inner_pos + delimiter2.length());
        }
        if (!line.empty())
        {
          result.push_back(parse_element<T>(line));
        }
      }
    }

    return result;
  }

  template <typename T>
  std::vector<std::vector<T>> parse_and_split_2d(const std::string &filename,
                                                 const std::string &delimiter1 = "\n",
                                                 const std::string &delimiter2 = ",")
  {
    std::string content = read_file_content(filename, delimiter1);
    std::vector<std::vector<T>> result;
    
    // Split into rows
    size_t pos = 0;
    while ((pos = content.find(delimiter1)) != std::string::npos)
    {
      std::string line = content.substr(0, pos);
      content.erase(0, pos + delimiter1.length());

      if (line.empty())
        continue;

      // Process each row
      std::vector<T> row;
      size_t inner_pos = 0;
      while ((inner_pos = line.find(delimiter2)) != std::string::npos)
      {
        std::string token = line.substr(0, inner_pos);
        if (!token.empty())
        {
          row.push_back(parse_element<T>(token));
        }
        line.erase(0, inner_pos + delimiter2.length());
      }
      // Handle the last element in the row
      if (!line.empty())
      {
        row.push_back(parse_element<T>(line));
      }

      if (!row.empty())
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
    std::string content = read_file_content(filename, pair_delimiter);
    std::map<K, V> result;
    
    // Split into pairs
    size_t pos = 0;
    while ((pos = content.find(pair_delimiter)) != std::string::npos)
    {
      std::string pair = content.substr(0, pos);
      content.erase(0, pos + pair_delimiter.length());

      if (pair.empty())
        continue;

      // Find the key-value delimiter
      size_t delim_pos = pair.find(kv_delimiter);
      if (delim_pos == std::string::npos)
      {
        throw std::runtime_error("Invalid pair format, missing delimiter: " + pair);
      }

      // Split into key and value
      std::string key_str = pair.substr(0, delim_pos);
      std::string value_str = pair.substr(delim_pos + kv_delimiter.length());

      // Trim whitespace if needed
      key_str.erase(0, key_str.find_first_not_of(" \t"));
      key_str.erase(key_str.find_last_not_of(" \t") + 1);
      value_str.erase(0, value_str.find_first_not_of(" \t"));
      value_str.erase(value_str.find_last_not_of(" \t") + 1);

      if (!key_str.empty() && !value_str.empty())
      {
        K key = parse_element<K>(key_str);
        V value = parse_element<V>(value_str);
        result.insert({std::move(key), std::move(value)});
      }
    }

    return result;
  }

} // namespace utils
