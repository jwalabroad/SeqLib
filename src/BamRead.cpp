#include "SnowTools/BamRead.h"

namespace SnowTools {

  BamRead::BamRead() {
    b = bam_init1();
  }

  BamRead::~BamRead() {
    bam_destroy1(b);
  }

  void BamRead::SetQname(std::string n)
  {
    // copy out the non-qname data
    size_t nonq_len = b->l_data - b->core.l_qname;
    uint8_t* nonq = (uint8_t*)malloc(nonq_len);
    memcpy(nonq, b->data + b->core.l_qname, nonq_len);

    // clear the old data and alloc the new amount 
    free(b->data);
    b->data = (uint8_t*)calloc(nonq_len + n.length() + 1, 1);
    
    // add in the new qname
    memcpy(b->data, (uint8_t*)n.c_str(), n.length() + 1); // +1 for \0

    // update the sizes
    b->l_data = b->l_data - b->core.l_qname + n.length() + 1;
    b->core.l_qname = n.length() + 1;    
    
    // copy over the old data
    memcpy(b->data + b->core.l_qname, nonq, nonq_len);
    free(nonq);
  }

  void BamRead::SetSequence(std::string s)
  {

    // change the size to accomodate new sequence. Clear the quality string
    std::cout << "osize " << b->l_data << " calcsize " << (b->core.l_qseq + b->core.l_qname + (b->core.n_cigar<<2)) << std::endl;
    b->data = (uint8_t*)realloc(b->data, b->core.l_qname + s.length() + (b->core.n_cigar<<2));
    
    // copy in the new sequence
    memcpy(b->data + b->core.l_qname + (b->core.n_cigar<<2), (uint8_t*)s.c_str(), s.length());

  }

  /*  std::string BamRead::toSam(bam_hdr_t* h) const 
  {
    kstring_t *str;
    sam_format1(h, b, str);
    return std::string(str->s);
    }*/

  std::ostream& operator<<(std::ostream& out, const BamRead &r)
  {
    //kstring_t *str;
    //sam_format1(hdr, b, str);
    //out << str->s;
    return out;

    out << r.QnameChar()  << "\t" << r.AlignmentFlag()  << "\t"
	<< r.ChrID() << "\t" 
	<< r.InsertSize() << "\t" << r.AlignmentFlag() << "\t" 
	<< r.MapQuality() << "\t" << r.Sequence();
    return out;
      
    
  }
  
}
