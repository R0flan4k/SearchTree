#include "search_tree.h"
#include "gtest/gtest.h"

#include <iterator>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using It = typename RangeQueries::search_tree_t<int>::NodeIt;

TEST(SearchTree, BalanceInsert)
{
    RangeQueries::search_tree_t<int> st{}, st1{}, st2{};
    It n1 = st.insert(10), n2 = st.insert(15), n3 = st.insert(20),
       n4 = st1.insert(15), n5 = st1.insert(4), n6 = st1.insert(5);

    std::vector<int> input2{15, 4, 5, 13, 2, 18, 14, 10, 7};
    for (auto i : input2)
        st2.insert(i);

    EXPECT_EQ(n1->parent, n2);
    EXPECT_EQ(n1->left, st.nil());
    EXPECT_EQ(n1->right, st.nil());
    EXPECT_EQ(n2->parent, st.nil());
    EXPECT_EQ(n2->left, n1);
    EXPECT_EQ(n2->right, n3);
    EXPECT_EQ(n3->parent, n2);
    EXPECT_EQ(n3->right, st.nil());
    EXPECT_EQ(n3->left, st.nil());

    EXPECT_EQ(n4->parent, n6);
    EXPECT_EQ(n4->right, st1.nil());
    EXPECT_EQ(n4->left, st1.nil());
    EXPECT_EQ(n5->parent, n6);
    EXPECT_EQ(n5->right, st1.nil());
    EXPECT_EQ(n5->left, st1.nil());
    EXPECT_EQ(n6->parent, st1.nil());
    EXPECT_EQ(n6->right, n4);
    EXPECT_EQ(n6->left, n5);
}

TEST(SearchTree, Bounds)
{
    RangeQueries::search_tree_t<int> st{}, st1{}, st2{};
    std::vector<int> input{15, 4, 5, 13, 2, 18, 14, 10, 7}, input1{10, 20},
        input2{0, 44, 45, 1, 4, 5, 49, 57, 61, 72, 84, 99, 21, 25, 36, 46};
    std::unordered_map<int, It> nodes, nodes1, nodes2;
    for (auto i : input)
        nodes.emplace(i, st.insert(i));
    for (auto i : input1)
        nodes1.emplace(i, st1.insert(i));
    for (auto i : input2)
        nodes2.emplace(i, st2.insert(i));

    EXPECT_EQ(st.lower_bound(6), nodes.find(7)->second);
    EXPECT_EQ(st.upper_bound(16), nodes.find(18)->second);
    EXPECT_EQ(st.lower_bound(5), nodes.find(5)->second);

    EXPECT_EQ(st1.lower_bound(8), nodes1.find(10)->second);
    EXPECT_EQ(st1.upper_bound(31), st1.nil());

    EXPECT_EQ(st2.lower_bound(21), nodes2.find(21)->second);
    EXPECT_EQ(st2.lower_bound(-1), nodes2.find(0)->second);
    EXPECT_EQ(st2.lower_bound(43), nodes2.find(44)->second);
    EXPECT_EQ(st2.lower_bound(44), nodes2.find(44)->second);
    EXPECT_EQ(st2.lower_bound(100), st2.nil());
    EXPECT_EQ(st2.lower_bound(1), nodes2.find(1)->second);
    EXPECT_EQ(st2.lower_bound(2), nodes2.find(4)->second);
    EXPECT_EQ(st2.lower_bound(5), nodes2.find(5)->second);
    EXPECT_EQ(st2.lower_bound(6), nodes2.find(21)->second);
    EXPECT_EQ(st2.lower_bound(25), nodes2.find(25)->second);
    EXPECT_EQ(st2.lower_bound(35), nodes2.find(36)->second);
    EXPECT_EQ(st2.lower_bound(45), nodes2.find(45)->second);
    EXPECT_EQ(st2.lower_bound(46), nodes2.find(46)->second);
    EXPECT_EQ(st2.lower_bound(48), nodes2.find(49)->second);
    EXPECT_EQ(st2.lower_bound(59), nodes2.find(61)->second);
    EXPECT_EQ(st2.lower_bound(72), nodes2.find(72)->second);
    EXPECT_EQ(st2.lower_bound(83), nodes2.find(84)->second);
    EXPECT_EQ(st2.upper_bound(44), nodes2.find(45)->second);
    EXPECT_EQ(st2.upper_bound(99), st2.nil());
    EXPECT_EQ(st2.upper_bound(43), nodes2.find(44)->second);
}

TEST(SearchTree, GetDistance)
{
    RangeQueries::search_tree_t<int> st{}, st1{};
    std::vector<int> input{15, 4, 5, 13, 2, 18, 14, 10, 7},
        input1{0, 44, 45, 1, 4, 5, 49, 57, 61, 72, 84, 99, 21, 25, 36, 46};
    for (auto i : input)
        st.insert(i);
    for (auto i : input1)
        st1.insert(i);

    EXPECT_EQ(st.get_distance(st.lower_bound(6), st.upper_bound(16)), 5);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(21), st1.upper_bound(43)), 3);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(-1), st1.upper_bound(100)),
              input1.size());
    EXPECT_EQ(st1.get_distance(st1.lower_bound(43), st1.upper_bound(72)), 7);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(0), st1.upper_bound(1)), 2);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(0), st1.upper_bound(0)), 1);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(0), st1.upper_bound(44)), 8);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(1), st1.upper_bound(4)), 2);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(1), st1.upper_bound(5)), 3);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(1), st1.upper_bound(25)), 5);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(1), st1.upper_bound(45)), 8);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(1), st1.upper_bound(46)), 9);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(1), st1.upper_bound(61)), 12);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(1), st1.upper_bound(99)), 15);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(1), st1.upper_bound(100)), 15);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(99), st1.upper_bound(99)), 1);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(44), st1.upper_bound(100)), 9);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(-2), st1.upper_bound(-1)), 0);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(100), st1.upper_bound(101)), 0);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(-1), st1.upper_bound(-1)), 0);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(0), st1.upper_bound(0)), 1);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(1), st1.upper_bound(1)), 1);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(2), st1.upper_bound(2)), 0);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(4), st1.upper_bound(4)), 1);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(5), st1.upper_bound(5)), 1);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(7), st1.upper_bound(7)), 0);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(21), st1.upper_bound(21)), 1);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(22), st1.upper_bound(22)), 0);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(21), st1.upper_bound(22)), 1);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(25), st1.upper_bound(25)), 1);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(30), st1.upper_bound(30)), 0);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(30), st1.upper_bound(31)), 0);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(36), st1.upper_bound(36)), 1);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(37), st1.upper_bound(37)), 0);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(44), st1.upper_bound(44)), 1);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(44), st1.upper_bound(45)), 2);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(45), st1.upper_bound(45)), 1);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(46), st1.upper_bound(46)), 1);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(48), st1.upper_bound(48)), 0);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(49), st1.upper_bound(49)), 1);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(55), st1.upper_bound(55)), 0);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(57), st1.upper_bound(57)), 1);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(58), st1.upper_bound(58)), 0);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(61), st1.upper_bound(61)), 1);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(65), st1.upper_bound(65)), 0);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(72), st1.upper_bound(72)), 1);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(75), st1.upper_bound(75)), 0);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(84), st1.upper_bound(84)), 1);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(86), st1.upper_bound(86)), 0);
    EXPECT_EQ(st1.get_distance(st1.lower_bound(99), st1.upper_bound(99)), 1);
}

TEST(SearchTree, SetRangeQueries)
{
    std::set<int> s;
    RangeQueries::search_tree_t<int> st{};
    std::basic_string<char> str =
        "q 159 253 k 89 k 139 q 194 188 q 0 107 q 277 278 k 260 k 181 k 132 q "
        "257 280 q 95 270 q 44 64 k 284 q 45 124 k 45 q 249 90 k 188 q 189 21 "
        "k 77 q 205 172 k 285 k 30 q 33 32 k 258 q 162 112 q 168 3 k 142 k 5 q "
        "14 175 k 71 q 206 265 q 216 80 q 43 252 q 193 293 k 5 k 19 q 266 23 q "
        "77 264 k 218 k 92 q 193 56 q 10 63 k 230 k 147 k 125 k 1 q 28 119 k "
        "67 q 236 168 k 229 k 41 k 129 q 161 172 k 56 k 45 q 99 79 q 264 169 q "
        "31 157 q 207 126 q 157 54 k 192 k 127 k 71 q 107 145 q 25 155 q 30 "
        "189 k 139 k 59 k 153 q 289 15 q 179 53 q 212 18 q 158 86 k 178 k 177 "
        "k 183 q 222 280 k 205 k 170 k 273 q 73 152 k 29 k 275 k 9 q 5 16 k "
        "172 k 121 q 57 5 q 31 181 k 41 k 200 k 255 q 286 12 k 41 q 4 109 q 19 "
        "273 q 147 182 k 38 k 60 q 73 33 k 160 q 39 39 k 44 q 68 82 q 33 230 k "
        "70 k 186 k 205 k 159 q 98 202 q 172 116 k 96 k 106 k 160 q 92 188 q "
        "208 11 q 208 221 k 177 q 246 219 q 227 245 k 131 k 153 k 52 q 132 65 "
        "k 229 q 164 67 k 32 k 76 k 105 k 98 q 116 288 q 78 163 k 63 q 46 227 "
        "q 0 215 k 8 k 106 q 291 88 k 47 q 223 98 q 186 33 q 26 234 q 146 81 q "
        "100 276 k 108 k 262 k 40 k 148 q 216 46 k 49 k 196 q 42 159 q 35 138 "
        "q 57 95 k 44 k 206 k 289 q 143 50 q 149 188 k 152 k 107 k 110 q 65 92 "
        "k 189 k 96 q 19 234 q 139 275 q 24 37 k 218 q 223 223 k 49 k 72 k 137 "
        "q 103 106 k 22 q 83 240 k 265 q 155 166 q 256 265 q 214 148 k 118 k "
        "63 k 210 q 223 255 q 94 210 k 227 q 217 144 q 167 261 q 127 184 k 54 "
        "k 1 k 233 q 155 297 q 218 151 k 85 q 274 53 q 186 134 q 79 189 k 142 "
        "k 141 q 30 204 k 165 k 160 k 292 q 243 283 q 90 12 q 72 90 q 266 191 "
        "k 3 k 58 k 73 q 46 32 k 32 q 8 165 k 195 q 73 251 k 228 q 57 19 k 114 "
        "k 261 q 241 33 q 277 122 k 80 k 137 q 207 18 k 19 q 124 295 q 243 98 "
        "q 199 283 q 49 58 k 269 q 108 131 k 120 k 23 q 148 230 k 102 q 97 266 "
        "k 123 k 196 k 122 k 204 k 119 k 104 k 91 k 80 k 104 k 72 q 44 154 q "
        "73 190 q 78 106 q 37 245 q 124 191 k 214 k 110 q 180 261 q 141 38 k "
        "278 q 74 153 k 249 q 253 9 q 286 6 q 73 30 k 46 k 290 k 176 k 125 q "
        "281 246 q 132 33 k 255 k 101 q 109 46 k 151 k 71 q 299 238 k 189 k "
        "176 q 39 132 k 149 q 105 294 k 217 k 134 q 94 25 k 79 q 176 205 k 289 "
        "q 105 115 k 129 k 101 k 40 k 136 k 11 q 259 275 q 231 56 k 244 q 4 70 "
        "k 193 q 218 16 k 259 q 112 259 q 247 0 q 147 135 q 271 104 k 107 q "
        "279 79 q 27 30 k 256 k 289 q 217 53 q 94 286 q 26 96 k 77 k 82 q 129 "
        "115 k 91 q 149 276 q 284 5 q 290 21 q 17 258 k 150 q 190 200 k 73 q "
        "252 73 q 276 158 k 120 k 174 k 86 q 165 193 q 19 112 q 45 209 k 284 k "
        "128 q 11 130 k 47 k 288 k 220 k 201 k 85 k 32 q 115 85 q 159 180 k 40 "
        "q 187 53 k 41 k 45 q 191 214 q 100 115 q 82 190 q 158 8 q 103 25 k "
        "258 q 178 23 q 5 176 q 5 72 q 186 90 k 50 q 215 208 k 294 k 163 k 263 "
        "q 66 109 k 48 q 157 66 q 258 259 k 177 q 235 83 q 165 44 q 139 81 q "
        "126 291 k 76 k 115 k 143 k 85 k 169 q 197 53 q 33 113 q 33 273 k 57 k "
        "120 k 192 k 186 q 145 275 q 296 111 q 248 64 q 71 199 k 41 k 89 q 108 "
        "63 k 79 q 186 252 q 274 231 k 186 q 98 285 q 177 241 q 274 147 q 10 "
        "52 k 151 q 188 189 q 158 11 k 40 k 264 q 108 204 k 223 k 280 k 186 k "
        "57 k 283 k 15 q 226 76 k 170 q 159 156 q 196 100 k 169 q 143 257 k "
        "133 q 284 270 q 179 282 k 270 k 9 k 215 k 45 k 126 q 18 155 q 85 166 "
        "q 92 189 k 209 k 146 k 150 q 62 191 q 297 97 k 73 k 264 q 141 130 q "
        "91 11 q 91 52 q 45 123 q 207 70 k 173 k 300 q 70 238 k 78 q 1 246 q "
        "177 114 q 168 225 q 287 130 q 193 13 q 204 83 q 142 76 k 38 q 138 137 "
        "q 177 143 q 64 247 k 163 q 176 152 q 114 284 q 30 180 k 116 q 111 205 "
        "k 45 q 256 211 k 297 q 16 22 q 223 122 q 212 7 k 270 k 34 q 277 207 k "
        "123 k 31 q 162 44 k 63 k 132 q 177 17 q 267 242 k 5 q 291 261 k 145 q "
        "36 259 k 213 q 41 58 q 80 188 q 247 98 k 190 k 228 k 145 q 252 74 q 4 "
        "257 k 150 q 102 139 q 292 60 k 225 q 140 179 k 97 k 15 q 203 110 q "
        "206 196 k 221 k 109 k 27 q 213 49 q 173 268 k 34 q 108 284 k 241 k 60 "
        "k 80 q 112 287 q 98 214 k 147 q 90 11 k 144 k 208 q 89 3 q 164 204 q "
        "36 165 q 119 296 q 71 185 q 174 75 k 241 q 184 200 q 93 202 k 203 q 7 "
        "154 q 33 207 k 61 q 182 241 k 110 k 250 k 259 q 183 13 k 19 q 290 248 "
        "q 86 249 k 288 k 184 q 28 253 q 71 45 q 249 145 q 116 175 k 44 k 39 k "
        "103 k 13 q 160 3 q 298 48 k 284 k 13 q 165 160 k 282 k 63 q 266 68 q "
        "256 78 k 208 k 125 q 128 49 q 15 225 q 173 139 k 265 q 38 88 q 210 "
        "251 q 184 151 q 287 127 k 222 k 227 k 140 k 242 k 263 q 92 251 k 91 q "
        "94 152 k 148 k 46 q 127 97 k 153 k 269 q 171 49 k 235 k 113 q 132 166 "
        "k 161 k 18 q 25 153 k 98 k 65 q 107 166 q 110 26 k 132 k 252 k 94 q "
        "169 160 q 172 100 q 14 220 k 122 k 216 k 10 q 191 143 k 156 q 279 230 "
        "q 300 19 q 254 151 q 157 84 e";
    std::basic_stringstream<char> stream{str};

    char request;
    for (stream >> request; request != 'e'; stream >> request)
    {
        switch (request)
        {
        case 'q':
        {
            int b1, b2;
            stream >> b1 >> b2;
            if (b1 > b2)
                break;
            using ConstIt =
                typename RangeQueries::search_tree_t<int>::NodeConstIt;
            ConstIt st_left_it = st.lower_bound(b1),
                    st_right_it = st.upper_bound(b2);
            auto s_left_it = s.lower_bound(b1), s_right_it = s.upper_bound(b2);
            EXPECT_EQ(st.get_distance(st_left_it, st_right_it),
                      std::distance(s_left_it, s_right_it));
            break;
        }
        case 'k':
        {
            int key;
            stream >> key;
            s.insert(key);
            st.insert(key);
            break;
        }
        default:
            assert(0 && "UNREACHABLE");
            break;
        }
    }
}