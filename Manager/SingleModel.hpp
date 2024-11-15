#pragma once
#include<memory>

namespace hjdd
{
	namespace manager
	{
		template<class _Ty>
		class SingleModel
		{
		public:
			SingleModel(const SingleModel<_Ty>&) = delete;
			SingleModel& operator=(const SingleModel<_Ty>&) = delete;
			virtual ~SingleModel(){}
			static std::shared_ptr<_Ty> instance()
			{
				if (SingleModel<_Ty>::_instance == nullptr)
				{
					_instance = std::shared_ptr<_Ty>(new _Ty);
				}
				return SingleModel<_Ty>::_instance;
			}

		protected:
			SingleModel(){}
		private:
			static std::shared_ptr<_Ty> _instance;

		};
		template<class _Ty>
		std::shared_ptr<_Ty> SingleModel<_Ty>::_instance = nullptr;
	}
}