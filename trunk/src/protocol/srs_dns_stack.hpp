//
// Copyright (c) 2013-2021 The SRS Authors
//
// SPDX-License-Identifier: MIT or MulanPSL-2.0
//
#ifndef SRS_PROTOCOL_DNS_HPP
#define SRS_PROTOCOL_DNS_HPP

#include <srs_core.hpp>
#include <string>
#include <vector>

#define SRS_DNS_TYPE_A        1
#define SRS_DNS_TYPE_CNAME    5
#define SRS_DNS_TYPE_PTR      12
#define SRS_DNS_TYPE_MX       15
#define SRS_DNS_TYPE_TXT      16
#define SRS_DNS_TYPE_AAAA     28
#define SRS_DNS_TYPE_SRV      33
#define SRS_DFNS_TYPE_DNAME   39

#define SRS_DNS_CLASS_IN      1

#define SRS_RESOLVE_FORMERR   1
#define SRS_RESOLVE_SERVFAIL  2
#define SRS_RESOLVE_NXDOMAIN  3
#define SRS_RESOLVE_NOTIMP    4
#define SRS_RESOLVE_REFUSED   5

/* -- Define DNS message format -- */
/* Header section format */
/**                                 1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      ID                       |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    QDCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ANCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    NSCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ARCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
*/
class SrsDnsHeader
{
private:
    uint16_t    id;
    uint16_t    flags;
    uint16_t    question;
    uint16_t    answer;
    uint16_t    authority;
    uint16_t    additional;
}

/* Question section format */
/**                                 1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                     QNAME                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QTYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QCLASS                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
*/
class SrsDnsQuestion
{
public:
    std::string     qname;
	uint16_t        qtype;
	uint16_t        qclass;
}

class SrsDnsRdata
{

}

class SrsDnsRdataSOA: public SrsDnsRdata
{
public:
    std::string     ns;
    std::string     mbox;
    uint32_t        serial;
    uint32_t        refresh;
    uint32_t        retry;
    uint32_t        expire;
    uint32_t        minttl;
}

class SrsDnsRdataMX: public SrsDnsRdata
{
public:
    uint16_t        preference;
    ngx_str_t       mx;
}

/* Resource record format */
/**                                 1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                                               /
    /                      NAME                     /
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      TYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     CLASS                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      TTL                      |
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                   RDLENGTH                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
    /                     RDATA                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
*/

class SrsDnsRR
{
public:
    std::string     name;
	uint16_t        rtype;
	uint16_t        rclass;
	uint16_t        rttl;
	uint16_t        rdlength;
    SrsDnsRdata     *rdata;
}

class SrsDnsMessage
{
public:
    SrsDnsHeader            *header;
    SrsDnsQuestion          *question;
    std::vector<SrsDnsRR*>   answers;
    std::vector<SrsDnsRR*>   authorities;
    std::vector<SrsDnsRR*>   additionals;

public:
    SrsDnsMessage();
    virtual ~SrsDnsMessage();
    /**
     * read dns message from stream.
     */
    virtual srs_error_t read(SrsBuffer* stream);
    /**
     * write dns message to stream.
     */
    virtual srs_error_t write(SrsBuffer* stream);
    virtual SrsDnsMessage* copy();
    virtual srs_error_t add_request(std::string data, int dns_type);
}

#endif