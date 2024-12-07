module;

#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <ranges>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <queue>


export module utils;

export namespace utils {
  /**
   * @brief Trim a string from both sides
   * @param str The string to trim
   * @param whitespace The whitespace characters to trim (default is " \t")
   * @return The trimmed string
   */
  inline std::string trim(std::string_view str,
                          const std::optional<std::string_view> &whitespace = std::nullopt) {
    const auto chars = whitespace.value_or(" \t");
    const auto start = str.find_first_not_of(chars);
    if (start == std::string_view::npos) {
      return "";
    }
    const auto end = str.find_last_not_of(chars);
    return std::string(str.substr(start, end - start + 1));
  }

  template<typename T>
  struct is_vector : std::false_type {};

  template<typename T, typename A>
  struct is_vector<std::vector<T, A>> : std::true_type {};

  /**
   * @brief Parse an element from a string
   * @param line The string to parse
   * @return The parsed element
   */
  template<typename T>
  T parse_element(const std::string &line) {
    std::stringstream converter(line);
    T value;

    if constexpr (std::is_same_v<T, std::string>) {
      std::string result;
      std::getline(converter, result);
      return result;
    } else if constexpr (is_vector<T>::value) {
      T result;
      typename T::value_type element;
      while (converter >> element) {
        result.push_back(element);
      }
      if (result.empty()) {
        throw std::runtime_error("Unable to parse line into vector: " + line);
      }
      return result;
    } else {
      if (converter >> value) {
        return value;
      }
      throw std::runtime_error("Unable to parse line: " + line);
    }
  }

  /**
   * @brief Read the content of a file
   * @param filename The filename to read
   * @param delimiter The delimiter to use (default is "\n")
   * @return The content of the file
   */
  inline std::string read_file_content(const std::string &filename, const std::string &delimiter = "\n") {
    std::ifstream file(filename);
    if (!file.is_open()) {
      throw std::runtime_error("Unable to open file: " + filename);
    }

    std::string content;
    std::string line;
    while (std::getline(file, line)) {
      content += line + delimiter;
    }
    return content;
  }

  /**
   * @brief Split a string into tokens
   * @param content The string to split
   * @param delimiter The delimiter to use
   * @return The tokens
   */
  inline auto split_string = [](std::string_view content, std::string_view delimiter) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    while ((pos = content.find(delimiter)) != std::string_view::npos) {
      auto token = content.substr(0, pos);
      if (!token.empty()) {
        tokens.emplace_back(token);
      }
      content = content.substr(pos + delimiter.length());
    }
    if (!content.empty()) {
      tokens.emplace_back(content);
    }
    return tokens;
  };

  /**
   * @brief Parse and split a file or content into a vector of elements
   * @param input The filename or content to process
   * @param delimiter1 The delimiter to use for the first split (default is "\n")
   * @param delimiter2 The delimiter to use for the second split (default is "")
   * @return The vector of elements
   */
  template<typename T, bool IsFilename = true>
  std::vector<T> parse_and_split(const std::string &input,
                                 const std::string &delimiter1 = "\n",
                                 const std::string &delimiter2 = "") {
    const auto content = IsFilename ? read_file_content(input, delimiter1) : input;
    std::vector<T> result;

    const auto process_token = [&result](const std::string &token) {
      if (!token.empty()) {
        result.push_back(parse_element<T>(token));
      }
    };

    if (delimiter2.empty()) {
      std::ranges::for_each(split_string(content, delimiter1), process_token);
    } else {
      for (const auto &line: split_string(content, delimiter1)) {
        std::ranges::for_each(split_string(line, delimiter2), process_token);
      }
    }

    return result;
  }

  /**
   * @brief Parse and split a file or content into a 2D vector of elements
   * @param input The filename or content to process
   * @param delimiter1 The delimiter to use for the first split (default is "\n")
   * @param delimiter2 The delimiter to use for the second split (default is ",")
   * @return The 2D vector of elements
   */
  template<typename T, bool IsFilename = true>
  std::vector<std::vector<T> > parse_and_split_2d(const std::string &input,
                                                  const std::string &delimiter1 = "\n",
                                                  const std::string &delimiter2 = ",") {
    const auto content = IsFilename ? read_file_content(input, delimiter1) : input;
    std::vector<std::vector<T> > result;

    const auto process_line = [&delimiter2](const std::string &line) {
      std::vector<T> row;
      for (const auto &token: split_string(line, delimiter2)) {
        if (!token.empty()) {
          row.push_back(parse_element<T>(token));
        }
      }
      return row;
    };

    for (const auto &line: split_string(content, delimiter1)) {
      if (auto row = process_line(line); !row.empty()) {
        result.push_back(std::move(row));
      }
    }

    return result;
  }

  /**
   * @brief Parse and split a file or content into a map
   * @param input The filename or content to process
   * @param pair_delimiter The delimiter to use for the pair (default is "\n")
   * @param kv_delimiter The delimiter to use for the key-value pair (default is ":")
   * @return The map
   */
  template<typename K, typename V, bool IsFilename = true, bool Unique = true>
  auto parse_to_map(const std::string &input,
                    const std::string &pair_delimiter = "\n",
                    const std::string &kv_delimiter = ":") {
    const auto content = IsFilename ? read_file_content(input, pair_delimiter) : input;

    using ResultType = std::conditional_t<Unique,
      std::map<K, V>,
      std::vector<std::tuple<K, V> > >;
    ResultType result;

    const auto process_pair = [&kv_delimiter](const std::string &pair)
      -> std::optional<std::pair<std::string, std::string> > {
      if (auto delim_pos = pair.find(kv_delimiter); delim_pos != std::string::npos) {
        auto key = trim(std::string_view(pair.substr(0, delim_pos)));
        auto value = trim(std::string_view(pair.substr(delim_pos + kv_delimiter.length())));
        if (!key.empty() && !value.empty()) {
          return std::make_pair(key, value);
        }
      }
      return std::nullopt;
    };

    for (const auto &pair: split_string(content, pair_delimiter)) {
      if (auto kv = process_pair(pair)) {
        auto &[key_str, value_str] = *kv;
        K key = parse_element<K>(key_str);
        V value = parse_element<V>(value_str);
        if constexpr (Unique) {
          result.emplace(std::move(key), std::move(value));
        } else {
          result.emplace_back(std::move(key), std::move(value));
        }
      }
    }

    return result;
  }

  /**
   * @brief Perform topological sort on the input specified by the set of rules
   * @param rules set of ordered rules (k, v) where k preceedes v
   * @return Sorted graph as a vector<T>
   */
  template<typename T>
  std::vector<T> topological_sort(std::vector<std::tuple<T, T> > rules) {
    std::unordered_map<int, std::vector<int> > adjList;
    std::unordered_map<int, int> inDegree;

    for (auto [k, v]: rules) {
      adjList[k].push_back(v);
      inDegree[v]++;
      if (inDegree.find(k) == inDegree.end()) {
        inDegree[k] = 0;
      }
    }

    std::queue<int> q;
    for (const auto &[node, degree]: inDegree) {
      if (degree == 0) {
        q.push(node);
      }
    }

    std::vector<int> sortedSeq;
    while (!q.empty()) {
      int node = q.front();
      q.pop();
      sortedSeq.push_back(node);

      for (int neighbor: adjList[node]) {
        inDegree[neighbor]--;
        if (inDegree[neighbor] == 0) {
          q.push(neighbor);
        }
      }
    }
    return sortedSeq;
  }
} // namespace utils
