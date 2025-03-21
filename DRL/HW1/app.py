from flask import Flask, render_template, request, redirect, url_for
import numpy as np

app = Flask(__name__)

# Store the current grid state
grid_state = {
    "size": 5,
    "start": None,
    "end": None,
    "obstacles": [],
    "obstacles_left": 3
}

@app.route('/', methods=['GET', 'POST'])
def index():
    global grid_state
    
    # Initialize with proper defaults if needed
    if grid_state["size"] is None:
        grid_state["size"] = 5
    
    # Always ensure obstacles_left is calculated properly
    grid_state["obstacles_left"] = grid_state["size"] - 2
    
    if request.method == 'POST':
        action = request.form.get('action')
        
        if action == 'create_grid':
            # Parse and validate grid size
            try:
                size = int(request.form.get('grid_size', 5))
                grid_state['size'] = max(5, min(9, size))  # Ensure size is between 5-9
            except ValueError:
                grid_state['size'] = 5
                
            # Reset grid state
            grid_state['start'] = None
            grid_state['end'] = None
            grid_state['obstacles'] = []
            grid_state['obstacles_left'] = grid_state['size'] - 2
            
        elif action == 'reset_grid':
            # Reset grid state but keep the size
            grid_state['start'] = None
            grid_state['end'] = None
            grid_state['obstacles'] = []
            grid_state['obstacles_left'] = grid_state['size'] - 2
            
        elif action == 'cell_click':
            row = int(request.form.get('row'))
            col = int(request.form.get('col'))
            
            # Create cell_pos as a proper list to ensure consistent comparison
            cell_pos = [int(row), int(col)]
            
            # Validate that this cell is not already assigned
            already_assigned = False
            if grid_state['start'] is not None and grid_state['start'][0] == cell_pos[0] and grid_state['start'][1] == cell_pos[1]:
                already_assigned = True
            elif grid_state['end'] is not None and grid_state['end'][0] == cell_pos[0] and grid_state['end'][1] == cell_pos[1]:
                already_assigned = True
            elif any(obs[0] == cell_pos[0] and obs[1] == cell_pos[1] for obs in grid_state['obstacles']):
                already_assigned = True
                
            if already_assigned:
                return redirect(url_for('index'))
            
            if grid_state['start'] is None:
                # Set start cell
                grid_state['start'] = cell_pos
            elif grid_state['end'] is None:
                # Set end cell
                grid_state['end'] = cell_pos
            elif grid_state['obstacles_left'] > 0:
                # Set obstacle
                grid_state['obstacles'].append(cell_pos)
                grid_state['obstacles_left'] -= 1
    
    # Debug - print the current state to verify
    print(f"Current grid state: size={grid_state['size']}, start={grid_state['start']}, end={grid_state['end']}")
    
    # Render template with current grid state
    return render_template('index.html', 
                           grid_size=grid_state['size'],
                           start=grid_state['start'],
                           end=grid_state['end'],
                           obstacles=grid_state['obstacles'],
                           obstacles_left=grid_state['obstacles_left'])

if __name__ == '__main__':
    app.run(debug=True)

