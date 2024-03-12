#include <iostream>
#include <fstream>
#include <map>

class TabularData
{
public:
  TabularData(const std::string filename)
  {
    initialize_map(filename);
  }

  void table_head(const unsigned int n_rows = 10);
  double operator()(const double t);
  void output_interpolated_data(const std::string filename,
                                const double time_start,
                                const double time_end,
                                const double delta_t,
                                const std::string y_label = "");

private:
  std::map<double,double> table_values;
  void initialize_map(const std::string filename);
};

// Read data and store it in map
void TabularData::initialize_map(const std::string filename)
{
  std::ifstream infile(filename);
  double x, y;
  while (infile >> x >> y)
    table_values.insert({x, y});
}

// Display first 10 rows in table
void TabularData::table_head(const unsigned int n_rows)
{
  unsigned int counter = 0;
  std::cout << "----------------\n"
            << "x\tf(x)\n" 
            << "----------------\n";
  
  for (auto const& elem : table_values)
  {
    if (counter < n_rows)
    {
      std::cout << elem.first << "\t" << elem.second << "\n";
      counter++;
    }
    else
      break;
  }

  std::cout << "----------------" << std::endl;
}

double TabularData::operator()(const double t)
  {
    double out = 1000000; /*Bogus value*/
    auto iter_t = table_values.find(t);
    
    if (iter_t == table_values.end()) /* Value not found: interpolate! */
    {
      auto t1 = table_values.upper_bound(t);
      auto t0 = std::prev(t1);
      out = ((t1->second - t0->second)/(t1->first - t0->first)) * (t - t0->first) + t0->second;
    }
    else /* Value found! Return this value */
      out = iter_t->second;

    // To do: raise an exception if t is out of data range.

    return out;
  }

// This is a function to test the output of the program.
void TabularData::output_interpolated_data(const std::string filename,
                                           const double time_start,
                                           const double time_end,
                                           const double delta_t,
                                           const std::string y_label)
{
  std::ofstream output;
  output.open(filename);
  
  double t = time_start;

  if (y_label.length() > 0)
    output << "time [s]" << "," << y_label << "\n"; 

  while (t <= time_end)
  {
    output << t << "," << (*this)(t) << "\n";
    t += delta_t;
  }

  if (t != time_end)
    output << time_end << "," << (*this)(time_end) << "\n";
}

int main()
{
  TabularData activation("linear_ramp.txt");
  activation.table_head(9);
  std::cout << activation(0.00) << "\n"
            << activation(0.005) << "\n"
            << activation(0.01) << "\n"
            << activation(0.25) << "\n"
            << activation(0.255) << "\n"
            << activation(0.26) << std::endl;
  activation.output_interpolated_data("linear_ramp_output.csv",
                                      0.0,
                                      0.5,
                                      0.001);
  return 0;
}