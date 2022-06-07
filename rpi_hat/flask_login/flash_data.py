# -*- coding: utf-8 -*-
# use with python3
import configparser

def ReadConfigFile (start_empty=False):
    global pulses_total_list
    global pulses_hour_list_m1
    global pulses_hour_list_m2
    global pulses_hour_list_m3
    global pulses_hour_list_m4
    global pulses_week_list_m1
    global pulses_week_list_m2
    global pulses_week_list_m3
    global pulses_week_list_m4
    global pulses_month_list_m1
    global pulses_month_list_m2
    global pulses_month_list_m3
    global pulses_month_list_m4

    if start_empty:
        pulses_total_list = [0 for x in range(4)]
        pulses_hour_list_m1 = [0 for x in range(24)]
        pulses_hour_list_m2 = [0 for x in range(24)]
        pulses_hour_list_m3 = [0 for x in range(24)]
        pulses_hour_list_m4 = [0 for x in range(24)]
        pulses_week_list_m1 = [0 for x in range(7)]
        pulses_week_list_m2 = [0 for x in range(7)]
        pulses_week_list_m3 = [0 for x in range(7)]
        pulses_week_list_m4 = [0 for x in range(7)]
        pulses_month_list_m1 = [0 for x in range(30)]
        pulses_month_list_m2 = [0 for x in range(30)]
        pulses_month_list_m3 = [0 for x in range(30)]
        pulses_month_list_m4 = [0 for x in range(30)]
        return
        

    config = configparser.RawConfigParser()
    config.read('flash_data.txt')
    
    pulses_read = config.get('pulses', 'pulses_total')
    pulses_total_list = [int(x) for x in list(pulses_read.split(','))]
    print(pulses_total_list)

    
    pulses_read = config.get('pulses', 'pulses_hour_m1')
    pulses_hour_list_m1 = [int(x) for x in list(pulses_read.split(','))]
    print(pulses_hour_list_m1)

    pulses_read = config.get('pulses', 'pulses_hour_m2')
    pulses_hour_list_m2 = [int(x) for x in list(pulses_read.split(','))]
    print(pulses_hour_list_m2)

    pulses_read = config.get('pulses', 'pulses_hour_m3')
    pulses_hour_list_m3 = [int(x) for x in list(pulses_read.split(','))]
    print(pulses_hour_list_m3)

    pulses_read = config.get('pulses', 'pulses_hour_m4')
    pulses_hour_list_m4 = [int(x) for x in list(pulses_read.split(','))]
    print(pulses_hour_list_m4)

    
    pulses_read = config.get('pulses', 'pulses_week_m1')
    pulses_week_list_m1 = [int(x) for x in list(pulses_read.split(','))]
    print(pulses_week_list_m1)

    pulses_read = config.get('pulses', 'pulses_week_m2')
    pulses_week_list_m2 = [int(x) for x in list(pulses_read.split(','))]
    print(pulses_week_list_m2)

    pulses_read = config.get('pulses', 'pulses_week_m3')
    pulses_week_list_m3 = [int(x) for x in list(pulses_read.split(','))]
    print(pulses_week_list_m3)

    pulses_read = config.get('pulses', 'pulses_week_m4')
    pulses_week_list_m4 = [int(x) for x in list(pulses_read.split(','))]
    print(pulses_week_list_m4)

    
    pulses_read = config.get('pulses', 'pulses_month_m1')
    pulses_month_list_m1 = [int(x) for x in list(pulses_read.split(','))]
    print(pulses_month_list_m1)

    pulses_read = config.get('pulses', 'pulses_month_m2')
    pulses_month_list_m2 = [int(x) for x in list(pulses_read.split(','))]
    print(pulses_month_list_m2)

    pulses_read = config.get('pulses', 'pulses_month_m3')
    pulses_month_list_m3 = [int(x) for x in list(pulses_read.split(','))]
    print(pulses_month_list_m3)

    pulses_read = config.get('pulses', 'pulses_month_m4')
    pulses_month_list_m4 = [int(x) for x in list(pulses_read.split(','))]
    print(pulses_month_list_m4)
    
        
        
def WriteConfigFile ():
    config = configparser.RawConfigParser()

    config.add_section('pulses')

    to_save_list = str(pulses_total_list)
    to_save_str_bound = to_save_list.replace('[', '')
    to_save_str_no_bounds = to_save_str_bound.replace(']', '')       
    config.set('pulses', 'pulses_total', to_save_str_no_bounds)

    
    to_save_list = str(pulses_hour_list_m1)
    to_save_str_bound = to_save_list.replace('[', '')
    to_save_str_no_bounds = to_save_str_bound.replace(']', '')       
    config.set('pulses', 'pulses_hour_m1', to_save_str_no_bounds)

    to_save_list = str(pulses_hour_list_m2)
    to_save_str_bound = to_save_list.replace('[', '')
    to_save_str_no_bounds = to_save_str_bound.replace(']', '')       
    config.set('pulses', 'pulses_hour_m2', to_save_str_no_bounds)

    to_save_list = str(pulses_hour_list_m3)
    to_save_str_bound = to_save_list.replace('[', '')
    to_save_str_no_bounds = to_save_str_bound.replace(']', '')       
    config.set('pulses', 'pulses_hour_m3', to_save_str_no_bounds)

    to_save_list = str(pulses_hour_list_m4)
    to_save_str_bound = to_save_list.replace('[', '')
    to_save_str_no_bounds = to_save_str_bound.replace(']', '')       
    config.set('pulses', 'pulses_hour_m4', to_save_str_no_bounds)


    to_save_list = str(pulses_week_list_m1)
    to_save_str_bound = to_save_list.replace('[', '')
    to_save_str_no_bounds = to_save_str_bound.replace(']', '')       
    config.set('pulses', 'pulses_week_m1', to_save_str_no_bounds)

    to_save_list = str(pulses_week_list_m2)
    to_save_str_bound = to_save_list.replace('[', '')
    to_save_str_no_bounds = to_save_str_bound.replace(']', '')       
    config.set('pulses', 'pulses_week_m2', to_save_str_no_bounds)

    to_save_list = str(pulses_week_list_m3)
    to_save_str_bound = to_save_list.replace('[', '')
    to_save_str_no_bounds = to_save_str_bound.replace(']', '')       
    config.set('pulses', 'pulses_week_m3', to_save_str_no_bounds)

    to_save_list = str(pulses_week_list_m4)
    to_save_str_bound = to_save_list.replace('[', '')
    to_save_str_no_bounds = to_save_str_bound.replace(']', '')       
    config.set('pulses', 'pulses_week_m4', to_save_str_no_bounds)

    to_save_list = str(pulses_month_list_m1)
    to_save_str_bound = to_save_list.replace('[', '')
    to_save_str_no_bounds = to_save_str_bound.replace(']', '')       
    config.set('pulses', 'pulses_month_m1', to_save_str_no_bounds)

    to_save_list = str(pulses_month_list_m2)
    to_save_str_bound = to_save_list.replace('[', '')
    to_save_str_no_bounds = to_save_str_bound.replace(']', '')       
    config.set('pulses', 'pulses_month_m2', to_save_str_no_bounds)

    to_save_list = str(pulses_month_list_m3)
    to_save_str_bound = to_save_list.replace('[', '')
    to_save_str_no_bounds = to_save_str_bound.replace(']', '')       
    config.set('pulses', 'pulses_month_m3', to_save_str_no_bounds)

    to_save_list = str(pulses_month_list_m4)
    to_save_str_bound = to_save_list.replace('[', '')
    to_save_str_no_bounds = to_save_str_bound.replace(']', '')       
    config.set('pulses', 'pulses_month_m4', to_save_str_no_bounds)
    
    
    # Writing our configuration file
    with open('flash_data.txt', 'w') as configfile:
        config.write(configfile)


# pulses_total_list = [0 for x in range(4)]
# pulses_hour_list_m1 = [0 for x in range(24)]
# pulses_hour_list_m2 = [0 for x in range(24)]
# pulses_hour_list_m3 = [0 for x in range(24)]
# pulses_hour_list_m4 = [0 for x in range(24)]
# pulses_week_list_m1 = [0 for x in range(7)]
# pulses_week_list_m2 = [0 for x in range(7)]
# pulses_week_list_m3 = [0 for x in range(7)]
# pulses_week_list_m4 = [0 for x in range(7)]
# pulses_month_list_m1 = [0 for x in range(30)]
# pulses_month_list_m2 = [0 for x in range(30)]
# pulses_month_list_m3 = [0 for x in range(30)]
# pulses_month_list_m4 = [0 for x in range(30)]
hour_cntr = 0
week_day_cntr = 0
month_day_cntr = 0

def hourUpdate (pulses_last_hour_list):
    global pulses_total_list
    global pulses_hour_list_m1
    global pulses_hour_list_m2
    global pulses_hour_list_m3
    global pulses_hour_list_m4
    global pulses_week_list_m1
    global pulses_week_list_m2
    global pulses_week_list_m3
    global pulses_week_list_m4
    global pulses_month_list_m1
    global pulses_month_list_m2
    global pulses_month_list_m3
    global pulses_month_list_m4
    global hour_cntr
    global week_day_cntr
    global month_day_cntr

    pulses_total_list[0] += pulses_last_hour_list[0]
    pulses_total_list[1] += pulses_last_hour_list[1]
    pulses_total_list[2] += pulses_last_hour_list[2]
    pulses_total_list[3] += pulses_last_hour_list[3]
    
    pulses_hour_list_m1[hour_cntr] = pulses_last_hour_list[0]
    pulses_hour_list_m2[hour_cntr] = pulses_last_hour_list[1]
    pulses_hour_list_m3[hour_cntr] = pulses_last_hour_list[2]
    pulses_hour_list_m4[hour_cntr] = pulses_last_hour_list[3]

    if hour_cntr < 23:
        hour_cntr += 1
    else:
        hour_cntr = 0

        pulses_week_list_m1[week_day_cntr] = sum(pulses_hour_list_m1)
        pulses_week_list_m2[week_day_cntr] = sum(pulses_hour_list_m2)
        pulses_week_list_m3[week_day_cntr] = sum(pulses_hour_list_m3)
        pulses_week_list_m4[week_day_cntr] = sum(pulses_hour_list_m4)
        
        if week_day_cntr < 6:
            week_day_cntr += 1
        else:
            week_day_cntr = 0

        pulses_month_list_m1[month_day_cntr] = sum(pulses_hour_list_m1)
        pulses_month_list_m2[month_day_cntr] = sum(pulses_hour_list_m2)
        pulses_month_list_m3[month_day_cntr] = sum(pulses_hour_list_m3)
        pulses_month_list_m4[month_day_cntr] = sum(pulses_hour_list_m4)
        
        if month_day_cntr < 29:
            month_day_cntr += 1
        else:
            month_day_cntr = 0

    # write backup after the update
    WriteConfigFile()


def getMeterTotals():
    global pulses_total_list
    
    return pulses_total_list


def getMeterHours():
    global pulses_hour_list_m1
    global pulses_hour_list_m2
    global pulses_hour_list_m3
    global pulses_hour_list_m4
    
    return (pulses_hour_list_m1, pulses_hour_list_m2, pulses_hour_list_m3, pulses_hour_list_m4)


def getMeterWeeks():
    global pulses_week_list_m1
    global pulses_week_list_m2
    global pulses_week_list_m3
    global pulses_week_list_m4
    
    return (pulses_week_list_m1, pulses_week_list_m2, pulses_week_list_m3, pulses_week_list_m4)


def getMeterMonths():
    global pulses_month_list_m1
    global pulses_month_list_m2
    global pulses_month_list_m3
    global pulses_month_list_m4
    
    return (pulses_month_list_m1, pulses_month_list_m2, pulses_month_list_m3, pulses_month_list_m4)




    
    #####################
    # Testing Functions #
    #####################
def Tests_For_Pulses ():
    hours = 3000

    days = int(hours / 24)
    weeks = int (days / 7)
    months = int (days / 30)
    print(f'Testing {hours} hours, days: {days}, weeks: {weeks} months: {months}')

    ###############
    # For Meter 1 #
    ###############    
    pulses_list = [1, 0, 0, 0]
    print('meters pulses ' + str(pulses_list))
    
    for i in range(hours):
        hourUpdate(pulses_list)
        
    print('results:')
    print('totals: ' + str(pulses_total_list))
    print('')
    print('hour m1: ' + str(pulses_hour_list_m1))
    print('hour m2: ' + str(pulses_hour_list_m2))
    print('hour m3: ' + str(pulses_hour_list_m3))
    print('hour m4: ' + str(pulses_hour_list_m4))
    print('')
    print('week m1: ' + str(pulses_week_days_list_m1))
    print('week m2: ' + str(pulses_week_days_list_m2))
    print('week m3: ' + str(pulses_week_days_list_m3))
    print('week m4: ' + str(pulses_week_days_list_m4))
    print('')
    print('months m1: ' + str(pulses_month_days_list_m1))
    print('months m2: ' + str(pulses_month_days_list_m2))
    print('months m3: ' + str(pulses_month_days_list_m3))
    print('months m4: ' + str(pulses_month_days_list_m4))

    ###############
    # For Meter 2 #
    ###############    
    pulses_list = [0, 1, 0, 0]
    print('meters pulses ' + str(pulses_list))
    
    for i in range(hours):
        hourUpdate(pulses_list)
        
    print('results:')
    print('totals: ' + str(pulses_total_list))
    print('')
    print('hour m1: ' + str(pulses_hour_list_m1))
    print('hour m2: ' + str(pulses_hour_list_m2))
    print('hour m3: ' + str(pulses_hour_list_m3))
    print('hour m4: ' + str(pulses_hour_list_m4))
    print('')
    print('week m1: ' + str(pulses_week_days_list_m1))
    print('week m2: ' + str(pulses_week_days_list_m2))
    print('week m3: ' + str(pulses_week_days_list_m3))
    print('week m4: ' + str(pulses_week_days_list_m4))
    print('')
    print('months m1: ' + str(pulses_month_days_list_m1))
    print('months m2: ' + str(pulses_month_days_list_m2))
    print('months m3: ' + str(pulses_month_days_list_m3))
    print('months m4: ' + str(pulses_month_days_list_m4))

    ###############
    # For Meter 3 #
    ###############    
    pulses_list = [0, 0, 1, 0]
    print('meters pulses ' + str(pulses_list))
    
    for i in range(hours):
        hourUpdate(pulses_list)
        
    print('results:')
    print('totals: ' + str(pulses_total_list))
    print('')
    print('hour m1: ' + str(pulses_hour_list_m1))
    print('hour m2: ' + str(pulses_hour_list_m2))
    print('hour m3: ' + str(pulses_hour_list_m3))
    print('hour m4: ' + str(pulses_hour_list_m4))
    print('')
    print('week m1: ' + str(pulses_week_days_list_m1))
    print('week m2: ' + str(pulses_week_days_list_m2))
    print('week m3: ' + str(pulses_week_days_list_m3))
    print('week m4: ' + str(pulses_week_days_list_m4))
    print('')
    print('months m1: ' + str(pulses_month_days_list_m1))
    print('months m2: ' + str(pulses_month_days_list_m2))
    print('months m3: ' + str(pulses_month_days_list_m3))
    print('months m4: ' + str(pulses_month_days_list_m4))

    ###############
    # For Meter 4 #
    ###############    
    pulses_list = [0, 0, 0, 1]
    print('meters pulses ' + str(pulses_list))
    
    for i in range(hours):
        hourUpdate(pulses_list)
        
    print('results:')
    print('totals: ' + str(pulses_total_list))
    print('')
    print('hour m1: ' + str(pulses_hour_list_m1))
    print('hour m2: ' + str(pulses_hour_list_m2))
    print('hour m3: ' + str(pulses_hour_list_m3))
    print('hour m4: ' + str(pulses_hour_list_m4))
    print('')
    print('week m1: ' + str(pulses_week_days_list_m1))
    print('week m2: ' + str(pulses_week_days_list_m2))
    print('week m3: ' + str(pulses_week_days_list_m3))
    print('week m4: ' + str(pulses_week_days_list_m4))
    print('')
    print('months m1: ' + str(pulses_month_days_list_m1))
    print('months m2: ' + str(pulses_month_days_list_m2))
    print('months m3: ' + str(pulses_month_days_list_m3))
    print('months m4: ' + str(pulses_month_days_list_m4))


def Test_Read_Config_File ():
    ReadConfigFile()

    
def Test_Write_Config_File ():
    global pulses_total_list
    global pulses_hour_list_m1
    global pulses_hour_list_m2
    global pulses_hour_list_m3
    global pulses_hour_list_m4
    global pulses_week_list_m1
    global pulses_week_list_m2
    global pulses_week_list_m3
    global pulses_week_list_m4
    global pulses_month_list_m1
    global pulses_month_list_m2
    global pulses_month_list_m3
    global pulses_month_list_m4

    pulses_total_list[0] = 1
    pulses_total_list[1] = 2
    pulses_total_list[2] = 3
    pulses_total_list[3] = 4

    pulses_hour_list_m1 = [x for x in range(24)]
    pulses_hour_list_m2 = [x for x in range(24)]
    pulses_hour_list_m3 = [x for x in range(24)]
    pulses_hour_list_m4 = [x for x in range(24)]
    pulses_week_list_m1 = [x for x in range(7)]
    pulses_week_list_m2 = [x for x in range(7)]
    pulses_week_list_m3 = [x for x in range(7)]
    pulses_week_list_m4 = [x for x in range(7)]
    pulses_month_list_m1 = [x for x in range(30)]
    pulses_month_list_m2 = [x for x in range(30)]
    pulses_month_list_m3 = [x for x in range(30)]
    pulses_month_list_m4 = [x for x in range(30)]
    
    WriteConfigFile()
    

def Test_Create_Empty_File ():
    ReadConfigFile(True)
    WriteConfigFile()
    print('flash_data.txt empty created!')
    
    
##############
# Main Tests #
##############
if __name__ == "__main__":
    # Tests_For_Pulses()

    # Test_Write_Config_File()

    # Test_Read_Config_File()

    Test_Create_Empty_File()
