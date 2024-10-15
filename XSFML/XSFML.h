#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include <string>


namespace XSFML {
	typedef const char* CSTR;

	struct XSFMLToken {
		enum TokenType {
			UNKNOWN,
			KEYWORD,
			BLOCK_DEFINITION,
			ESCOPE_START,
			ESCOPE_END,
			ATTRIBUTE_SETTER,
			STRING,
			ENDLINE,
		};

		TokenType type;
		std::string value = "";

	};

	struct XSFMLBlock {
		std::string name = "";
		std::unordered_map<std::string, std::string> attributes;

		bool isAnonymous = false;

		unsigned int _id = 0;
	};

	struct XSFMLDocument {
		std::vector<XSFMLToken> tokens;
		std::vector<XSFMLBlock> blocks;

		void GetBlocksByKeywordValue(std::vector<XSFMLBlock>& vector, CSTR keyword, CSTR value, bool needsValue = true) {
			for (std::vector<XSFMLBlock>::iterator it = blocks.begin(); it != blocks.end(); it++) {
				
				for (std::unordered_map<std::string, std::string>::iterator _it = it->attributes.begin();
					_it != it->attributes.end(); _it++) {

					if (_it->first.compare(keyword) == 0) {
						if ((needsValue && _it->second.compare(value) == 0) || (needsValue == false)) {
							vector.push_back(*it);
						}
					}

				}

			}
		}
	};

	inline void XSFML_LOAD(XSFMLDocument& document, CSTR source) {
		unsigned int INDEX = 0;

		bool STRING_STARTED = false;
		bool BLOCK_STARTED = false;

		std::string CWORD = "";

		XSFMLToken TOKEN;
		TOKEN.type = XSFMLToken::UNKNOWN;
		TOKEN.value = "";

		while (source[INDEX] != '\0') {
			char& CHAR = (char&)source[INDEX];

			if (STRING_STARTED) {
				if (CHAR == '"') {
					XSFMLToken STRING_TOKEN;
					STRING_TOKEN.type = XSFMLToken::STRING;
					STRING_TOKEN.value = CWORD;

					document.tokens.push_back(STRING_TOKEN);

					CWORD = "";
					
					STRING_STARTED = false;

					INDEX++;
					continue;
				}
			}
			else {
				/*
				if (CHAR == '\n') {
					TOKEN.type = XSFMLToken::ENDLINE;
				}
				*/

				if (CHAR == '"') {
					STRING_STARTED = true;

					INDEX++;
					continue;
				}

				if (CHAR == '@') {
					TOKEN.type = XSFMLToken::BLOCK_DEFINITION;
				}

				if (CHAR == '(') {
					if (CWORD.length() > 0) {
						XSFMLToken WORD_TOKEN;
						WORD_TOKEN.type = XSFMLToken::KEYWORD;
						WORD_TOKEN.value = CWORD;

						document.tokens.push_back(WORD_TOKEN);

						CWORD = "";
					}

					TOKEN.type = XSFMLToken::ESCOPE_START;
				}

				if (CHAR == ')') {
					TOKEN.type = XSFMLToken::ESCOPE_END;
				}

				if (CHAR == ':') {
					if (CWORD.length() > 0) {
						XSFMLToken WORD_TOKEN;
						WORD_TOKEN.type = XSFMLToken::KEYWORD;
						WORD_TOKEN.value = CWORD;

						document.tokens.push_back(WORD_TOKEN);

						CWORD = "";
					}

					TOKEN.type = XSFMLToken::ATTRIBUTE_SETTER;
				}

				if (CHAR == ' ' || CHAR == '\t') {
					if (CWORD.length() > 0) {
						XSFMLToken WORD_TOKEN;
						WORD_TOKEN.type = XSFMLToken::KEYWORD;
						WORD_TOKEN.value = CWORD;

						document.tokens.push_back(WORD_TOKEN);

						CWORD = "";
					}
				}
			}
			
			if ((STRING_STARTED == false && (CHAR != ' ' && CHAR != '\n' && CHAR != '\t')) || STRING_STARTED == true)
				CWORD.push_back(source[INDEX]);

			if (TOKEN.type != XSFMLToken::UNKNOWN) {
				document.tokens.push_back(TOKEN);

				TOKEN.type = TOKEN.UNKNOWN;
				TOKEN.value = "";

				CWORD = "";
			}

			INDEX++;
		}

		printf("Exited with %zu tokens parseds!\n", document.tokens.size());
	};
	
	inline void XSFML_PARSE(XSFMLDocument& document, size_t index = 0) {
		bool IsBlock = false;
		bool InBlock = false;
		
		XSFMLBlock block;
		block.isAnonymous = false;

		bool IsSubBlock = false;
		bool InSubBlock = false;

		XSFMLBlock subBlock;
		subBlock.isAnonymous = true;

		unsigned int SubBlocksCount = 0;

		for (size_t i = index; i < document.tokens.size(); i++) {
			XSFMLToken& token = (XSFMLToken&)document.tokens[i];

			if (IsBlock) {
				if (InBlock) {

					if (IsSubBlock) {
						if (InSubBlock) {
							if (token.type == XSFMLToken::ESCOPE_END) {
								IsSubBlock = false;
								InSubBlock = false;

								document.blocks.push_back(subBlock);

								subBlock.name = "";
								subBlock.attributes.clear();
								subBlock._id = 0;

								SubBlocksCount++;

								continue;
							}

							if (token.type == XSFMLToken::KEYWORD && i + 2 < document.tokens.size() && document.tokens[i + 1].type == XSFMLToken::ATTRIBUTE_SETTER && document.tokens[i + 2].type == XSFMLToken::STRING) {
								subBlock.attributes[document.tokens[i].value] = document.tokens[i + 2].value;
							}
						}
						else {
							if (token.type == XSFMLToken::ESCOPE_START) {
								InSubBlock = true;
							}
						}


						continue;
					}
					else {
						if (token.type == XSFMLToken::BLOCK_DEFINITION) {
							IsSubBlock = true;
							subBlock.name = block.name + "@" + std::to_string(SubBlocksCount);
							subBlock._id = block._id;
							continue;
						}

						
					}

					if (token.type == XSFMLToken::ESCOPE_END || (i == document.tokens.size() - 1)) {
						InBlock = false;
						IsBlock = false;

						document.blocks.push_back(block);

						block.name = "";
						block.attributes.clear();
						block._id = 0;

						SubBlocksCount = 0;

						continue;
					}

					if (token.type == XSFMLToken::KEYWORD && i + 2 < document.tokens.size() && document.tokens[i + 1].type == XSFMLToken::ATTRIBUTE_SETTER && document.tokens[i + 2].type == XSFMLToken::STRING) {
						block.attributes[document.tokens[i].value] = document.tokens[i + 2].value;
					}
				}
				else {
					if (token.type == XSFMLToken::ESCOPE_START) {
						InBlock = true;
					}
				}
			}
			else {
				if (token.type == XSFMLToken::BLOCK_DEFINITION && i + 1 < document.tokens.size() && document.tokens[i + 1].type == XSFMLToken::KEYWORD) {
					IsBlock = true;

					block.name = document.tokens[i + 1].value;
					block._id = document.blocks.size();
				}
			}
		}
	};

	inline void XSFML_CLEAR(XSFMLDocument& document) {
		document.tokens.clear();
		document.blocks.clear();
	};
};



