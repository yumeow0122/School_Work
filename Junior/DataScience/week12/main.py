import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('C:/Users/yumeow/Desktop/PROJECT/School_Work/Junior/DataScience/week12/data.csv')

# create a figure and four axes
fig, axs = plt.subplots(nrows=2, ncols=2, figsize=(16, 16))

# plot the first subplot: line chart of male, female, and total population
axs[0, 0].plot(df['Township'], df['Male'], label="Male")
axs[0, 0].plot(df['Township'], df['Female'], label="Female")
axs[0, 0].plot(df['Township'], df['Total(Male+Female)'], label="Total")
axs[0, 0].set_title("Population by Gender and Township")
axs[0, 0].set_xlabel("Township")
axs[0, 0].set_ylabel("Population")
axs[0, 0].legend()

# plot the second subplot: line chart of immigration and emigration
axs[0, 1].plot(df['Township'], df['Immigration'], label="Immigration")
axs[0, 1].plot(df['Township'], df['Emigration'], label="Emigration")
axs[0, 1].set_title("Immigration and Emigration by Township")
axs[0, 1].set_xlabel("Township")
axs[0, 1].set_ylabel("Number of People")
axs[0, 1].legend()

# plot the third subplot: bar chart of household number
axs[1, 0].bar(df['Township'], df['Number_of_Households'])
axs[1, 0].set_title("Household Number by Township")
axs[1, 0].set_xlabel("Township")
axs[1, 0].set_ylabel("Number of Number_of_Households")

# plot the fourth subplot: line chart of births and Death
axs[1, 1].plot(df['Township'], df['Birth'], label="Birth")
axs[1, 1].plot(df['Township'], df['Death'], label="Death")
axs[1, 1].set_title("Birth and Death by Township")
axs[1, 1].set_xlabel("Township")
axs[1, 1].set_ylabel("Number of People")
axs[1, 1].legend()

# adjust the spacing between subplots
fig.subplots_adjust(hspace=0.4, wspace=0.3)

# display the plot
plt.show()