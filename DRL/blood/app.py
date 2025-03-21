from flask import Flask, render_template, request, jsonify
import numpy as np
import random

app = Flask(__name__)

@app.route('/')
def index():
    """Render the main page."""
    return render_template('index.html')

@app.route('/api/grid', methods=['POST'])
def create_grid():
    """Create grid configuration with a random policy.
    
    Returns:
        JSON object containing grid size, start/end positions, obstacles, a random policy, and value grid.
    """
    payload = request.get_json()
    grid_size = payload.get('n', 5)  # Use default size if not provided

    grid_config = {
        'size': grid_size,
        'start': None,
        'end': None,
        'obstacles': [],
        'policy': generate_random_policy(grid_size),
        'values': [[0 for _ in range(grid_size)] for _ in range(grid_size)]
    }
    return jsonify(grid_config)

@app.route('/api/evaluate', methods=['POST'])
def evaluate_policy():
    """Evaluate the given policy and compute state values.
    
    Returns:
        JSON object with the evaluated state values.
    """
    payload = request.get_json()
    g_size = payload.get('size')
    start_pt = payload.get('start')
    end_pt = payload.get('end')
    obs = payload.get('obstacles', [])
    pol = payload.get('policy')

    computed_values = policy_evaluation(g_size, start_pt, end_pt, obs, pol)
    return jsonify({'values': computed_values})

def generate_random_policy(n):
    """Generate a random policy grid.
    
    Args:
        n (int): Size of the grid.
        
    Returns:
        list: A 2D array of randomly selected actions ('up', 'down', 'left', 'right').
    """
    actions = ['up', 'down', 'left', 'right']
    rand_policy = []
    for _ in range(n):
        row = [random.choice(actions) for _ in range(n)]
        rand_policy.append(row)
    return rand_policy

def policy_evaluation(size, start, end, obstacles, policy):
    """Evaluate the provided policy to compute state values.
    
    Args:
        size (int): Size of the grid.
        start (dict): Starting position.
        end (dict): Goal position.
        obstacles (list): List of obstacles.
        policy (list): Current policy grid.
        
    Returns:
        list: A 2D array of evaluated state values.
    """
    GAMMA = 0.9
    THETA = 0.0001
    REWARD_STEP = -0.04
    REWARD_GOAL = 1.0

    V = [[0 for _ in range(size)] for _ in range(size)]
    if start:
        start = [start['row'], start['col']]
    if end:
        end = [end['row'], end['col']]
    obs_coords = [[o['row'], o['col']] for o in obstacles]

    def next_state(state, act):
        i, j = state
        if act == 'up':
            new_i, new_j = i - 1, j
        elif act == 'down':
            new_i, new_j = i + 1, j
        elif act == 'left':
            new_i, new_j = i, j - 1
        elif act == 'right':
            new_i, new_j = i, j + 1
        else:
            new_i, new_j = i, j
        if new_i < 0 or new_i >= size or new_j < 0 or new_j >= size or [new_i, new_j] in obs_coords:
            return (i, j)
        return (new_i, new_j)

    while True:
        delta = 0
        new_V = [[0 for _ in range(size)] for _ in range(size)]
        for i in range(size):
            for j in range(size):
                if [i, j] == end:
                    new_V[i][j] = REWARD_GOAL
                    continue
                if [i, j] in obs_coords:
                    new_V[i][j] = 0
                    continue
                act = policy[i][j]
                ni, nj = next_state((i, j), act)
                reward = REWARD_GOAL if [ni, nj] == end else REWARD_STEP
                new_V[i][j] = reward + GAMMA * V[ni][nj]
                delta = max(delta, abs(new_V[i][j] - V[i][j]))
        V = new_V
        if delta < THETA:
            break
    return V

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
