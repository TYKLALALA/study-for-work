class Solution {
public:
	vector<int> twoSum(vector<int>& nums, int target) {
		vector<int> result;

		int m = 0;
		int flag = 0;
		for (auto i = nums.begin(); i<nums.end() - 1; i++)
		{
			int n = m + 1;
			for (auto j = i + 1; j<nums.end(); j++)
			{

				if (*i + *j == target)
				{
					flag = 1;
					result.push_back(m);
					result.push_back(n);
					break;
				}
				n++;

			}
			m++;
			if (flag == 1)
				break;
		}
		return result;
	}
};