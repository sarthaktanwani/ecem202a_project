import pandas as pd
import os

scenario_1 = "../../data/logs/ble/scenario1"
scenario_2 = "../../data/logs/ble/scenario2"
directory_list = [scenario_1, scenario_2]

for directory in directory_list:
    if "scenario1" in directory:
        avg_data = pd.DataFrame(columns = ['GazeboRead', 'NS3Link1', 'ReadFile', 'NS3Link2', 'GazeboPublish', 'E2E_Latency'])
        for file in os.scandir(directory):
            data = pd.read_csv(file, index_col=False)
            avg_data = avg_data.append({'GazeboRead':data['GazeboRead'].mean(), 'NS3Link1':data['NS3Link1'].mean(), 'ReadFile':data['ReadFile'].mean(), 'NS3Link2':data['NS3Link2'].mean(), 'GazeboPublish':data['GazeboPublish'].mean(), 'E2E_Latency':data['E2E_Latency'].sum()}, ignore_index=True)
    else:
        avg_data = pd.DataFrame(columns = ['FileTransfer', 'GazeboRead', 'ReadFile', 'GazeboPublish', 'E2E_Latency'])
        for file in os.scandir(directory):
            data = pd.read_csv(file, index_col=False)
            avg_data = avg_data.append({'FileTransfer':data['FileTransfer'].mean(), 'GazeboRead':data['GazeboRead'].mean(), 'ReadFile':data['ReadFile'].mean(), 'GazeboPublish':data['GazeboPublish'].mean(), 'E2E_Latency':data['E2E latency'].sum()}, ignore_index=True)
    avg_data.to_csv('{0}'.format(directory) + '/ble_processed.csv', index=False)
