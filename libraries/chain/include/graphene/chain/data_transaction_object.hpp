/*
 * Copyright (c) 2015 Cryptonomex, Inc., and contributors.
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once

#include <graphene/chain/protocol/operations.hpp>
#include <graphene/db/generic_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <vector>
namespace graphene {
    namespace chain {

    using namespace std;
    class database;
    class league_object;

        struct  data_transaction_datasource_status_object
        {
            account_id_type     datasource;
            uint8_t             status = 0;
        };

        /**
         * 查询数据对象
         * @brief
         *
         *
         */
        class data_transaction_object : public graphene::db::abstract_object<data_transaction_object> {
        public:
            static const uint8_t space_id = protocol_ids;
            static const uint8_t type_id = data_transaction_object_type;

            string                                  request_id;
            object_id_type                          product_id;
            string                                  version;
            string                                  params;
            // 跟踪查询的处理状态
            uint8_t                                 status;
            // 查询数据的创建时间
            time_point_sec                          create_date_time;
            // 查询的请求者
            account_id_type                         requester;
            fc::optional<league_id_type>            league_id = fc::optional<league_id_type>();
            string                                  memo;
            vector<data_transaction_datasource_status_object>             datasources_status;
        };

        // data_transaction_object 的排序函数
        struct sort_data_transaction_object_by_create_date_time {
            bool operator() (const data_transaction_object &l, const data_transaction_object &r) {
                return l.create_date_time > r.create_date_time;
            }
        };

        class data_transaction_search_results_object : public graphene::db::abstract_object<data_transaction_search_results_object> {
        public:
            static const uint8_t space_id = implementation_ids;
            static const uint8_t type_id = impl_data_transaction_search_results_object_type;

            uint64_t total;
            vector <data_transaction_object> data;
        };

        /**
         * @ingroup object_index
         */
        struct by_request_id {};
        struct by_requester {};
        using data_transaction_multi_index_type = multi_index_container<
            data_transaction_object,
            indexed_by<
                ordered_unique< tag<by_id>, member< object, object_id_type, &object::id > >,
                ordered_unique< tag<by_request_id>, member<data_transaction_object, string, &data_transaction_object::request_id> >,
                ordered_non_unique< tag<by_requester>, 
                    composite_key<
                        data_transaction_object,
                         member<data_transaction_object, account_id_type, &data_transaction_object::requester>
                     >
                >
              >
            >;
        /**
         * @ingroup object_index
         */
        using data_transaction_index = generic_index<data_transaction_object, data_transaction_multi_index_type>;




    }
}

FC_REFLECT( graphene::chain::data_transaction_datasource_status_object,
            (datasource)
            (status)
)

FC_REFLECT_DERIVED(graphene::chain::data_transaction_object,
                   (graphene::db::object),
                   (request_id)
                   (product_id)
                   (version)
                   (params)
                   (status)
                   (create_date_time)
                   (requester)
                   (league_id)
                   (memo)
                   (datasources_status))
FC_REFLECT_DERIVED(graphene::chain::data_transaction_search_results_object,
                   (graphene::db::object),
                   (total)
                   (data))
