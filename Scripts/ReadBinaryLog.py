import struct
import pandas as pd

def remove_spaces(filepath):
    with open(filepath,'r') as f:
        lines = f.readlines() # Get all lines in file
    
    # Remove spaces
    lines = [line.replace(' ','') for line in lines] 

    with open(filepath,'w') as f:
        f.writelines(lines) # Write new lines to file
        
def convert_log(format, lengths, labels, filepath):
    remove_spaces(filepath)

    df = pd.DataFrame(columns=labels) # Create blank dataframe
    lineIdx = 0
    with open(filepath,'r') as f:
        while True:
            line = [0] * len(lengths)
            for i in range(len(lengths)):
                data = f.read(lengths[i] * 2) # Read struct entry
                if data == '': break
                data = int(data,16) # Convert to int from hex
                
                # Reverse byte order, convert to proper data type
                data = struct.unpack('<' + format[i], struct.pack('>I', data))[0] 
                
                # Append to line array
                line[i] = data
            if data == '': break
            # Store line in data frame
            df.loc[lineIdx] = line

            lineIdx += 1
            
        # Start at t = 0
        df['t'] = df['t'] - df['t'][0]
        
        return df
