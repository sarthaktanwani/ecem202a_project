import pandas as pd
import os

scenario_1 = "../logs/Scenario_1"
scenario_2 = "../logs/Scenario_2"
directory_list = [scenario_1, scenario_2]

for directory in directory_list:
    if "1" in directory:
        avg_data = pd.DataFrame(columns = ['GazeboRead', 'NS3Link1', 'ReadFile', 'NS3Link2', 'GazeboPublish', 'SumLatency'])
        for file in os.scandir(directory):
            data = pd.read_csv(file, index_col=False)
            avg_data = avg_data.append({'GazeboRead':data['GazeboRead'].mean(), 'NS3Link1':data['NS3Link1'].mean(), 'ReadFile':data['ReadFile'].mean(), 'NS3Link2':data['NS3Link2'].mean(), 'GazeboPublish':data['GazeboPublish'].mean(), 'SumLatency':data['SumLatency'].mean()}, ignore_index=True)
    else:
        avg_data = pd.DataFrame(columns = ['GazeboRead', 'ReadFile', 'GazeboPublish', 'SumLatency'])
        for file in os.scandir(directory):
            data = pd.read_csv(file, index_col=False)
            avg_data = avg_data.append({'GazeboRead':data['GazeboRead'].mean(), 'ReadFile':data['ReadFile'].mean(), 'GazeboPublish':data['GazeboPublish'].mean(), 'SumLatency':data['SumLatency'].mean()}, ignore_index=True)
    avg_data.to_csv('{0}'.format(directory) + '/processed.csv', index=False)
