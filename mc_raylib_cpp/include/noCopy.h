#ifndef NO_COPY_GUARD
#define NO_COPY_GUARD


class NoCopy {
	public:
		NoCopy() = default;
		NoCopy(const NoCopy&) = delete;
		NoCopy& operator=(const NoCopy&) = delete;
};



#endif