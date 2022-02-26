//
// Copyright (c) 2013-2021 The SRS Authors
//
// SPDX-License-Identifier: MIT or MulanPSL-2.0
//

#include <srs_dns_stack.hpp>
#include <srs_kernel_log.hpp>
#include <srs_kernel_error.hpp>

#include <string>

SrsDnsMessage::SrsDnsMessage()
{

}

SrsDnsMessage::~SrsDnsMessage()
{
    srs_freep(header);
    srs_freep(question);

    for (int i = 0; i < (int)answers.size(); i++) {
        SrsDnsRR* answer = answers.at(i);
        srs_freep(answer);
    }

    for (int i = 0; i < (int)authorities.size(); i++) {
        SrsDnsRR* authority = authorities.at(i);
        srs_freep(authority);
    }

    for (int i = 0; i < (int)additionals.size(); i++) {
        SrsDnsRR* additional = additionals.at(i);
        srs_freep(additional);
    }
}

srs_error_t SrsDnsMessage::add_request(std::string host, uint dns_type)
{
    srs_error_t err = srs_success;

    switch (dns_type)
    {
    case SRS_DNS_TYPE_A:
        break;
    case SRS_DNS_TYPE_AAAA:
        break;
    default:
        return srs_error_new(ERROR_DNS_REQUEST, "not support dns type");
    }

    if (host.empty()) {
        return srs_error_new(ERROR_DNS_REQUEST, "host empty");
    }

    SrsDnsQuestion* question = new SrsDnsQuestion();
    question->qclass = SRS_DNS_CLASS_IN;
    question->qtype = dns_type;
    question->qname = host;

    return err;
}

