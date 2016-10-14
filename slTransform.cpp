#include "slTransform.H"
#include "slIO.H"

std::istream &operator>>(std::istream &strm, SlTransform &m) {
  std::ios::fmtflags orgFlags = strm.setf(std::ios::skipws);
	eatChar('[',strm);

	register int r,c;

	for(r=0;r<4;r++) {
		eatChar('[',strm);
		for(c=0;c<4;c++) {
			strm >> m(r,c);
			if(c!=3) eatChar(',',strm);
		}
		eatChar(']',strm);
	}

	eatChar(']',strm);
	strm.flags(orgFlags);
	return strm;
}

std::ostream &operator<<(std::ostream &strm,const SlTransform &m) {
	strm << "[";
	register int r,c;

	for(r=0;r<4;r++) {
		strm << "[ ";
		for(c=0;c<4;c++) {
			strm << m(r,c);
			if(c!=3) strm << ",";
		}
		strm << " ]";
		if(r!=3) strm << "\n ";
	}

	strm << " ]\n";

	return strm;
}


//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
