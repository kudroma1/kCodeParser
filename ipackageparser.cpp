#include "ipackageparser.hpp"

using namespace kudroma::code_parser;

boost::regex IPackageParser::packageRegex_ = boost::regex("\\s*pk [a-zA-Z_0-9]*\\s*");

std::string IPackageParser::parseItemName(const std::string& line)
{
    auto local = line;
    std::vector<std::string> parts;
    boost::split(parts, line, boost::is_any_of(" "));
    std::vector<std::string> words;
    words.reserve(parts.size());
    for (auto&& p : parts)
    {
        if (!p.empty())
            words.emplace_back(std::move(p));
    }
    if (words.size() >= 2)
        return words[1];
    else
        BOOST_LOG_TRIVIAL(debug) << boost::str(boost::format("Line \"%1%\" invalid package line '%2%'") % __FUNCTION__ % line);

    return std::string();
}

std::shared_ptr<PackageDesc> IPackageParser::tryParse(const std::string& line)
{
    if (boost::regex_match(line, packageRegex_))
        return std::make_shared<PackageDesc>(parseItemName(line));
    else
        return nullptr;
}