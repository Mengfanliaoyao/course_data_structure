int totalMoney(int n) {
  const static int whole_week_base = 1 + 2 + 3 + 4 + 5 + 6 + 7;
  int total_week = n/7;
  int total_week_increment = 0;
  for(int i = 0; i < total_week; i++)
  {
    total_week_increment += i;
  }
  int total_money = whole_week_base * total_week + 7 * total_week_increment;
  int imcomplete_weekday = n%7;
  for(int i = 1; i <= imcomplete_weekday; i++)
  {
    total_money += (i + total_week);
  }
  return total_money;
}
