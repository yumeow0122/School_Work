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

@app.route('/api/improve', methods=['POST'])
def improve_policy():
    """Improve the current policy using evaluation.
    
    Returns:
        JSON object containing the updated policy and corresponding state values.
    """
    payload = request.get_json()
    g_size = payload.get('size')
    start_pt = payload.get('start')
    end_pt = payload.get('end')
    obs = payload.get('obstacles', [])
    pol = payload.get('policy')

    updated_policy, new_values = policy_improvement(g_size, start_pt, end_pt, obs, pol)
    return jsonify({'policy': updated_policy, 'values': new_values})

@app.route('/api/value-iteration', methods=['POST'])
def run_value_iteration():
    """Calculate the optimal policy and state values using value iteration.
    
    Returns:
        JSON object with the optimal policy and state values.
    """
    payload = request.get_json()
    g_size = payload.get('size')
    start_pt = payload.get('start')
    end_pt = payload.get('end')
    obs = payload.get('obstacles', [])

    opt_policy, opt_values = value_iteration(g_size, start_pt, end_pt, obs)
    return jsonify({'policy': opt_policy, 'values': opt_values})

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

def policy_improvement(size, start, end, obstacles, policy):
    """Improve the current policy via policy evaluation.
    
    Args:
        size (int): Size of the grid.
        start (dict): Starting position.
        end (dict): Goal position.
        obstacles (list): List of obstacles.
        policy (list): Existing policy grid.
        
    Returns:
        tuple: A tuple containing the improved policy and the state values.
    """
    GAMMA = 0.9
    THETA = 0.0001
    REWARD_STEP = -0.04
    REWARD_GOAL = 1.0

    if start:
        start = [start['row'], start['col']]
    if end:
        end = [end['row'], end['col']]
    obs_coords = [[o['row'], o['col']] for o in obstacles]

    V = [[0 for _ in range(size)] for _ in range(size)]
    actions = ['up', 'down', 'left', 'right']
    effects = {'up': (-1, 0), 'down': (1, 0), 'left': (0, -1), 'right': (0, 1)}

    def valid(i, j):
        return 0 <= i < size and 0 <= j < size and [i, j] not in obs_coords

    def next_state(state, act):
        i, j = state
        di, dj = effects[act]
        ni, nj = i + di, j + dj
        return (ni, nj) if valid(ni, nj) else (i, j)

    while True:
        delta = 0
        for i in range(size):
            for j in range(size):
                if [i, j] == end:
                    V[i][j] = REWARD_GOAL
                    continue
                if [i, j] in obs_coords:
                    V[i][j] = 0
                    continue
                prev_value = V[i][j]
                act = policy[i][j]
                ni, nj = next_state((i, j), act)
                V[i][j] = (REWARD_GOAL if [ni, nj] == end else REWARD_STEP) + GAMMA * V[ni][nj]
                delta = max(delta, abs(V[i][j] - prev_value))
        if delta < THETA:
            break

    new_policy = [row[:] for row in policy]
    for i in range(size):
        for j in range(size):
            if [i, j] == end or [i, j] in obs_coords:
                continue
            current_act = policy[i][j]
            act_values = {}
            for act in actions:
                ni, nj = next_state((i, j), act)
                act_values[act] = (REWARD_GOAL if [ni, nj] == end else REWARD_STEP) + GAMMA * V[ni][nj]
            best_act = max(act_values, key=act_values.get)
            new_policy[i][j] = best_act
    return new_policy, V

def value_iteration(size, start, end, obstacles):
    """Compute the optimal policy and values using value iteration.
    
    Args:
        size (int): Grid size.
        start (dict): Starting position.
        end (dict): Goal position.
        obstacles (list): List of obstacles.
        
    Returns:
        tuple: A tuple (optimal_policy, state_values).
    """
    GAMMA = 0.9
    THETA = 0.0001
    REWARD_STEP = -0.04
    REWARD_GOAL = 1.0

    if start:
        start = [start['row'], start['col']]
    if end:
        end = [end['row'], end['col']]
    obs_coords = [[o['row'], o['col']] for o in obstacles]

    V = [[0 for _ in range(size)] for _ in range(size)]
    actions = ['up', 'down', 'left', 'right']
    effects = {'up': (-1, 0), 'down': (1, 0), 'left': (0, -1), 'right': (0, 1)}

    def valid(i, j):
        return 0 <= i < size and 0 <= j < size and [i, j] not in obs_coords

    def next_state(state, act):
        i, j = state
        di, dj = effects[act]
        ni, nj = i + di, j + dj
        return (ni, nj) if valid(ni, nj) else (i, j)

    while True:
        diff = 0
        for i in range(size):
            for j in range(size):
                if [i, j] == end:
                    V[i][j] = REWARD_GOAL
                    continue
                if [i, j] in obs_coords:
                    V[i][j] = 0
                    continue
                prev_val = V[i][j]
                best_val = float('-inf')
                for act in actions:
                    ni, nj = next_state((i, j), act)
                    value = (REWARD_GOAL if [ni, nj] == end else REWARD_STEP) + GAMMA * V[ni][nj]
                    best_val = max(best_val, value)
                V[i][j] = best_val
                diff = max(diff, abs(V[i][j] - prev_val))
        if diff < THETA:
            break

    optimal_policy = [['' for _ in range(size)] for _ in range(size)]
    for i in range(size):
        for j in range(size):
            if [i, j] == end or [i, j] in obs_coords:
                optimal_policy[i][j] = 'none'
            else:
                act_dict = {}
                for act in actions:
                    ni, nj = next_state((i, j), act)
                    act_dict[act] = (REWARD_GOAL if [ni, nj] == end else REWARD_STEP) + GAMMA * V[ni][nj]
                optimal_policy[i][j] = max(act_dict, key=act_dict.get)
    return optimal_policy, V

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
