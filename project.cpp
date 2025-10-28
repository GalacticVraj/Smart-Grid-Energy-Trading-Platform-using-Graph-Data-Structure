#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <cstdlib>

using namespace std;

// ==================== DATA STRUCTURES ====================

struct Transaction {
    string id;
    string sellerId;
    string buyerId;
    double energy;
    double price;
    time_t timestamp;

    Transaction(string sid, string bid, double e, double p) 
        : sellerId(sid), buyerId(bid), energy(e), price(p) {
        timestamp = time(nullptr);
        id = "TXN" + to_string(timestamp);
    }
};

struct User {
    string id;
    string name;
    double surplus;
    double demand;
    double balance;

    User() : id(""), name(""), surplus(0), demand(0), balance(0) {}
    
    User(string uid, string n, double s, double d, double b)
        : id(uid), name(n), surplus(s), demand(d), balance(b) {}
};

// ==================== PLATFORM ====================

class TradingPlatform {
private:
    map<string, User> users;
    vector<Transaction> transactions;

    void openBrowser(const string& filename) {
        #ifdef _WIN32
            string command = "start \"\" \"" + filename + "\"";
            system(command.c_str());
        #elif __APPLE__
            string command = "open \"" + filename + "\"";
            system(command.c_str());
        #elif __linux__
            string command = "xdg-open \"" + filename + "\"";
            system(command.c_str());
        #else
            cout << "Please open: " << filename << endl;
        #endif
    }

public:
    void addUser(User user) {
        users[user.id] = user;
    }

    void launchWebApp() {
        string filename = "energy_trading_platform.html";
        ofstream file(filename);
        
        if (!file.is_open()) {
            cout << "Error creating file\n";
            return;
        }
        
        file << "<!DOCTYPE html>\n<html lang='en'>\n<head>\n";
        file << "<meta charset='UTF-8'>\n";
        file << "<meta name='viewport' content='width=device-width, initial-scale=1.0'>\n";
    file << "<title>⚡ NEXUS ENERGY</title>\n";
        
        // CSS Styles - Dark Tech Theme
        file << "<style>\n";
        file << "* { margin: 0; padding: 0; box-sizing: border-box; }\n";
    file << "body { font-family: 'Courier New', 'SF Mono', Monaco, monospace; background: #071228; color: #00aaff; padding: 20px; min-height: 100vh; overflow-x: hidden; }\n";
    file << "body::before { content: ''; position: fixed; top: 0; left: 0; width: 100%; height: 100%; background: linear-gradient(rgba(0,170,255,0.03) 1px, transparent 1px), linear-gradient(90deg, rgba(0,170,255,0.03) 1px, transparent 1px); background-size: 50px 50px; pointer-events: none; z-index: -1; }\n";
    file << "h1 { text-align: center; margin: 20px 0; font-size: 2.8em; text-shadow: 0 0 10px #00aaff, 0 0 20px #00aaff; letter-spacing: 3px; }\n";
    file << ".subtitle { text-align: center; color: #0077cc; margin-bottom: 30px; font-size: 1.1em; letter-spacing: 2px; }\n";
        file << ".container { max-width: 1400px; margin: 0 auto; }\n";
    file << ".card { background: rgba(5, 12, 25, 0.9); padding: 30px; margin: 20px 0; border-radius: 8px; border: 1px solid #00aaff; box-shadow: 0 0 20px rgba(0,170,255,0.25), inset 0 0 20px rgba(0,170,255,0.06); position: relative; overflow: hidden; }\n";
    file << ".card::before { content: ''; position: absolute; top: 0; left: -100%; width: 100%; height: 100%; background: linear-gradient(90deg, transparent, rgba(0,170,255,0.1), transparent); animation: scan 3s linear infinite; }\n";
        file << "@keyframes scan { 0% { left: -100%; } 100% { left: 100%; } }\n";
    file << "h2 { color: #00aaff; margin-bottom: 20px; font-size: 1.6em; text-shadow: 0 0 5px #00aaff; border-bottom: 1px solid #0077cc; padding-bottom: 10px; }\n";
    file << ".tabs { display: flex; gap: 5px; margin-bottom: 20px; border-bottom: 2px solid #002244; background: rgba(0,10,20,0.8); padding: 5px; border-radius: 6px; }\n";
    file << ".tab { padding: 12px 25px; cursor: pointer; border: none; background: transparent; font-family: inherit; font-size: 14px; color: #0077cc; border-radius: 4px; transition: all 0.3s; letter-spacing: 1px; position: relative; overflow: hidden; }\n";
        file << ".tab::before { content: ''; position: absolute; top: 0; left: -100%; width: 100%; height: 100%; background: linear-gradient(90deg, transparent, rgba(0,255,65,0.3), transparent); transition: left 0.5s; }\n";
        file << ".tab:hover::before { left: 100%; }\n";
        file << ".tab:hover { color: #00ff41; text-shadow: 0 0 10px #00ff41; }\n";
    file << ".tab.active { background: rgba(0,170,255,0.08); color: #00aaff; text-shadow: 0 0 10px #00aaff; border: 1px solid #00aaff; }\n";
        file << ".tab-content { display: none; animation: fadeIn 0.5s ease-in; }\n";
        file << "@keyframes fadeIn { from { opacity: 0; transform: translateY(10px); } to { opacity: 1; transform: translateY(0); } }\n";
        file << ".tab-content.active { display: block; }\n";
        file << ".form-grid { display: grid; grid-template-columns: 1fr 1fr; gap: 20px; margin: 20px 0; }\n";
        file << ".form-group { margin: 15px 0; }\n";
        file << "label { display: block; margin-bottom: 8px; font-weight: 600; color: #00cc33; text-transform: uppercase; font-size: 0.9em; letter-spacing: 1px; }\n";
        file << "input, select { width: 100%; padding: 12px; background: rgba(0,10,5,0.8); border: 1px solid #003300; border-radius: 4px; font-size: 15px; color: #00ff41; font-family: inherit; transition: all 0.3s; }\n";
        file << "input:focus, select:focus { outline: none; border-color: #00ff41; box-shadow: 0 0 10px rgba(0,255,65,0.5); background: rgba(0,20,10,0.9); }\n";
        file << "input::placeholder { color: #006622; }\n";
        file << "button { background: linear-gradient(135deg, #003300 0%, #00cc33 100%); color: #001100; padding: 14px 30px; border: none; border-radius: 4px; cursor: pointer; font-size: 14px; font-weight: bold; transition: all 0.3s; text-transform: uppercase; letter-spacing: 1px; font-family: inherit; position: relative; overflow: hidden; }\n";
        file << "button::before { content: ''; position: absolute; top: 0; left: -100%; width: 100%; height: 100%; background: linear-gradient(90deg, transparent, rgba(255,255,255,0.3), transparent); transition: left 0.5s; }\n";
        file << "button:hover::before { left: 100%; }\n";
        file << "button:hover { transform: translateY(-2px); box-shadow: 0 5px 15px rgba(0,255,65,0.4); color: #000; }\n";
        file << ".btn-reset { background: linear-gradient(135deg, #330000 0%, #cc3300 100%); margin-left: 10px; }\n";
    file << ".success { background: rgba(0,80,140,0.12); padding: 15px; color: #00aaff; border-radius: 4px; margin: 15px 0; display: none; font-weight: bold; border: 1px solid #0077cc; text-shadow: 0 0 5px #00aaff; }\n";
    file << ".error { background: rgba(140,0,0,0.12); padding: 15px; color: #ff8080; border-radius: 4px; margin: 15px 0; display: none; font-weight: bold; border: 1px solid #cc3333; text-shadow: 0 0 5px rgba(255,128,128,0.8); }\n";
        file << ".stats { display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 20px; margin: 20px 0; }\n";
        file << ".stat-box { background: rgba(0,20,10,0.8); padding: 25px; border-radius: 6px; text-align: center; border: 1px solid #003300; position: relative; overflow: hidden; }\n";
    file << ".stat-box::before { content: ''; position: absolute; top: 0; left: 0; width: 100%; height: 3px; background: linear-gradient(90deg, #00aaff, #0077cc); }\n";
        file << ".stat-box h3 { font-size: 12px; margin-bottom: 10px; opacity: 0.8; text-transform: uppercase; letter-spacing: 1px; color: #00cc33; }\n";
        file << ".stat-box .value { font-size: 28px; font-weight: bold; color: #00ff41; text-shadow: 0 0 10px rgba(0,255,65,0.5); }\n";
        file << "table { width: 100%; border-collapse: collapse; margin-top: 10px; border: 1px solid #003300; }\n";
        file << "th, td { padding: 12px; text-align: left; border-bottom: 1px solid #003300; }\n";
        file << "th { background: rgba(0,50,25,0.6); color: #00ff41; font-weight: 600; text-transform: uppercase; letter-spacing: 1px; font-size: 0.9em; }\n";
        file << "tr { background: rgba(0,15,8,0.5); transition: all 0.3s; }\n";
        file << "tr:hover { background: rgba(0,255,65,0.1); }\n";
    file << ".badge { display: inline-block; padding: 4px 10px; border-radius: 12px; font-size: 0.8em; font-weight: bold; text-transform: uppercase; letter-spacing: 1px; }\n";
    file << ".badge-green { background: rgba(0,170,255,0.12); color: #00aaff; border: 1px solid #0077cc; }\n";
    file << ".badge-blue { background: rgba(0,100,255,0.12); color: #0088ff; border: 1px solid #0066cc; }\n";
    file << ".badge-orange { background: rgba(255,100,0,0.2); color: #ff6600; border: 1px solid #cc5500; }\n";
    file << ".badge-gray { background: rgba(100,100,100,0.12); color: #888; border: 1px solid #444; }\n";
        file << "#tree-canvas { width: 100%; height: 500px; border: 1px solid #003300; border-radius: 6px; background: rgba(0,10,5,0.8); }\n";
        file << ".tree-legend { display: flex; gap: 20px; justify-content: center; margin: 15px 0; flex-wrap: wrap; }\n";
        file << ".legend-item { display: flex; align-items: center; gap: 8px; }\n";
        file << ".legend-color { width: 20px; height: 20px; border-radius: 50%; border: 2px solid #003300; }\n";
        file << ".path-finder { background: rgba(0,30,15,0.6); padding: 20px; border-radius: 6px; margin: 20px 0; border: 1px solid #004400; }\n";
        file << ".path-result { background: rgba(0,15,8,0.8); padding: 15px; border-radius: 4px; margin-top: 15px; font-family: monospace; font-size: 14px; border-left: 4px solid #00ff41; color: #00ff41; }\n";
        file << ".terminal-text { color: #00ff41; font-family: monospace; }\n";
        file << ".glow { text-shadow: 0 0 10px currentColor; }\n";
        file << ".cyber-border { border: 1px solid #00ff41; box-shadow: 0 0 10px rgba(0,255,65,0.3); }\n";
        file << "@media (max-width: 768px) { .form-grid { grid-template-columns: 1fr; } .tabs { flex-wrap: wrap; } }\n";
        file << "</style>\n";
        
        // JavaScript
        file << "<script>\n";
        file << "let users = [];\n";
        file << "let transactions = [];\n";
        file << "let network = {};\n\n";
        
        // Initial data
        file << "const initialUsers = [\n";
        for (map<string, User>::iterator it = users.begin(); it != users.end(); ++it) {
            User& u = it->second;
            file << "  {id: '" << u.id << "', name: '" << u.name << "', surplus: " << u.surplus 
                 << ", demand: " << u.demand << ", balance: " << u.balance << "},\n";
        }
        file << "];\n\n";
        
        // Initialize data
        file << "function initData() {\n";
        file << "  users = JSON.parse(JSON.stringify(initialUsers));\n";
        file << "  transactions = [];\n";
        file << "  network = {};\n";
        file << "}\n\n";
        
        file << "function resetData() {\n";
        file << "  if (confirm('WARNING: This will purge all transaction data. Continue?')) {\n";
        file << "    initData();\n";
    file << "    showMessage('Data cleared.', 'success');\n";
        file << "    updateAll();\n";
        file << "  }\n";
        file << "}\n\n";
        
        // Tab switching
        file << "function switchTab(tabName) {\n";
        file << "  document.querySelectorAll('.tab').forEach(t => t.classList.remove('active'));\n";
        file << "  document.querySelectorAll('.tab-content').forEach(t => t.classList.remove('active'));\n";
        file << "  document.getElementById('tab-' + tabName).classList.add('active');\n";
        file << "  document.getElementById('content-' + tabName).classList.add('active');\n";
        file << "  if (tabName === 'network') setTimeout(drawTree, 100);\n";
        file << "}\n\n";
        
        // Execute trade
        file << "function executeTrade() {\n";
        file << "  const seller = document.getElementById('seller').value;\n";
        file << "  const buyer = document.getElementById('buyer').value;\n";
        file << "  const energy = parseFloat(document.getElementById('energy').value);\n";
        file << "  const price = parseFloat(document.getElementById('price').value);\n";
        file << "  \n";
        file << "  if (!seller || !buyer || !energy || !price) {\n";
    file << "    showMessage('All fields required.', 'error');\n";
        file << "    return;\n";
        file << "  }\n";
        file << "  \n";
        file << "  if (seller === buyer) {\n";
    file << "    showMessage('Invalid trade.', 'error');\n";
        file << "    return;\n";
        file << "  }\n";
        file << "  \n";
        file << "  const s = users.find(u => u.id === seller);\n";
        file << "  const b = users.find(u => u.id === buyer);\n";
        file << "  const cost = energy * price;\n";
        file << "  \n";
        file << "  if (s.surplus < energy) {\n";
    file << "    showMessage('Insufficient surplus: ' + s.surplus.toFixed(2) + ' kWh', 'error');\n";
        file << "    return;\n";
        file << "  }\n";
        file << "  \n";
        file << "  if (b.demand < energy) {\n";
    file << "    showMessage('Exceeds demand: ' + b.demand.toFixed(2) + ' kWh', 'error');\n";
        file << "    return;\n";
        file << "  }\n";
        file << "  \n";
        file << "  if (b.balance < cost) {\n";
    file << "    showMessage('Insufficient balance: ₹' + cost.toFixed(2), 'error');\n";
        file << "    return;\n";
        file << "  }\n";
        file << "  \n";
        file << "  s.surplus -= energy;\n";
        file << "  b.demand -= energy;\n";
        file << "  s.balance += cost;\n";
        file << "  b.balance -= cost;\n";
        file << "  \n";
        file << "  transactions.push({ id: 'TXN' + Date.now(), seller, buyer, energy, price });\n";
        file << "  \n";
        file << "  if (!network[seller]) network[seller] = [];\n";
        file << "  if (!network[buyer]) network[buyer] = [];\n";
        file << "  if (!network[seller].includes(buyer)) network[seller].push(buyer);\n";
        file << "  if (!network[buyer].includes(seller)) network[buyer].push(seller);\n";
        file << "  \n";
    file << "  showMessage('Trade executed: ' + energy.toFixed(2) + ' kWh for ₹' + cost.toFixed(2), 'success');\n";
        file << "  updateAll();\n";
        file << "  document.getElementById('tradeForm').reset();\n";
        file << "}\n\n";
        
        // BFS path finding
        file << "function findPath() {\n";
        file << "  const start = document.getElementById('pathStart').value;\n";
        file << "  const end = document.getElementById('pathEnd').value;\n";
        file << "  \n";
        file << "  if (!start || !end) {\n";
        file << "    showMessage('ERROR: Select both nodes.', 'error');\n";
        file << "    return;\n";
        file << "  }\n";
        file << "  \n";
        file << "  if (start === end) {\n";
        file << "    document.getElementById('pathResult').innerHTML = '<span class=\"terminal-text\">PATH: ' + start + '</span><br><span class=\"terminal-text\">LENGTH: 0 hops</span>';\n";
        file << "    return;\n";
        file << "  }\n";
        file << "  \n";
        file << "  const queue = [start];\n";
        file << "  const parent = { [start]: null };\n";
        file << "  \n";
        file << "  while (queue.length > 0) {\n";
        file << "    const curr = queue.shift();\n";
        file << "    if (curr === end) {\n";
        file << "      const path = [];\n";
        file << "      let node = end;\n";
        file << "      while (node !== null) {\n";
        file << "        path.unshift(node);\n";
        file << "        node = parent[node];\n";
        file << "      }\n";
        file << "      document.getElementById('pathResult').innerHTML = '<span class=\"terminal-text\">PATH FOUND: ' + path.join(' → ') + '</span><br><span class=\"terminal-text\">LENGTH: ' + (path.length - 1) + ' hops</span>';\n";
        file << "      return;\n";
        file << "    }\n";
        file << "    \n";
        file << "    if (network[curr]) {\n";
        file << "      for (const neighbor of network[curr]) {\n";
        file << "        if (!(neighbor in parent)) {\n";
        file << "          parent[neighbor] = curr;\n";
        file << "          queue.push(neighbor);\n";
        file << "        }\n";
        file << "      }\n";
        file << "    }\n";
        file << "  }\n";
        file << "  \n";
        file << "  document.getElementById('pathResult').innerHTML = '<span class=\"terminal-text\">NO PATH: Cannot route between ' + start + ' and ' + end + '</span>';\n";
        file << "}\n\n";
        
        // Draw tree visualization
        file << "function drawTree() {\n";
        file << "  const canvas = document.getElementById('tree-canvas');\n";
        file << "  const ctx = canvas.getContext('2d');\n";
        file << "  canvas.width = canvas.offsetWidth;\n";
        file << "  canvas.height = 500;\n";
        file << "  \n";
        file << "  ctx.clearRect(0, 0, canvas.width, canvas.height);\n";
        file << "  \n";
        file << "  if (Object.keys(network).length === 0) {\n";
        file << "    ctx.font = '16px \"Courier New\", monospace';\n";
        file << "    ctx.fillStyle = '#003300';\n";
        file << "    ctx.textAlign = 'center';\n";
        file << "    ctx.fillText('NETWORK OFFLINE - Execute trades to establish connections', canvas.width / 2, canvas.height / 2);\n";
        file << "    return;\n";
        file << "  }\n";
        file << "  \n";
        file << "  const nodes = users.map(u => u.id);\n";
        file << "  const positions = {};\n";
        file << "  const radius = Math.min(canvas.width, canvas.height) * 0.35;\n";
        file << "  const centerX = canvas.width / 2;\n";
        file << "  const centerY = canvas.height / 2;\n";
        file << "  \n";
        file << "  nodes.forEach((node, i) => {\n";
        file << "    const angle = (i / nodes.length) * 2 * Math.PI - Math.PI / 2;\n";
        file << "    positions[node] = {\n";
        file << "      x: centerX + radius * Math.cos(angle),\n";
        file << "      y: centerY + radius * Math.sin(angle)\n";
        file << "    };\n";
        file << "  });\n";
        file << "  \n";
        file << "  ctx.strokeStyle = '#00ff41';\n";
        file << "  ctx.lineWidth = 1;\n";
        file << "  ctx.setLineDash([5, 5]);\n";
        file << "  for (const node in network) {\n";
        file << "    for (const neighbor of network[node]) {\n";
        file << "      if (node < neighbor) {\n";
        file << "        ctx.beginPath();\n";
        file << "        ctx.moveTo(positions[node].x, positions[node].y);\n";
        file << "        ctx.lineTo(positions[neighbor].x, positions[neighbor].y);\n";
        file << "        ctx.stroke();\n";
        file << "      }\n";
        file << "    }\n";
        file << "  }\n";
        file << "  ctx.setLineDash([]);\n";
        file << "  \n";
        file << "  nodes.forEach(node => {\n";
        file << "    const user = users.find(u => u.id === node);\n";
        file << "    const pos = positions[node];\n";
        file << "    \n";
        file << "    const gradient = ctx.createRadialGradient(pos.x, pos.y, 0, pos.x, pos.y, 35);\n";
        file << "    if (user.surplus > 0) {\n";
        file << "      gradient.addColorStop(0, '#00ff41');\n";
        file << "      gradient.addColorStop(1, '#003300');\n";
        file << "    } else if (user.demand > 0) {\n";
        file << "      gradient.addColorStop(0, '#0088ff');\n";
        file << "      gradient.addColorStop(1, '#002244');\n";
        file << "    } else {\n";
        file << "      gradient.addColorStop(0, '#666666');\n";
        file << "      gradient.addColorStop(1, '#222222');\n";
        file << "    }\n";
        file << "    \n";
        file << "    ctx.fillStyle = gradient;\n";
        file << "    ctx.beginPath();\n";
        file << "    ctx.arc(pos.x, pos.y, 35, 0, 2 * Math.PI);\n";
        file << "    ctx.fill();\n";
        file << "    \n";
        file << "    ctx.strokeStyle = user.surplus > 0 ? '#00ff41' : (user.demand > 0 ? '#0088ff' : '#444444');\n";
        file << "    ctx.lineWidth = 3;\n";
        file << "    ctx.stroke();\n";
        file << "    \n";
        file << "    ctx.fillStyle = '#001100';\n";
        file << "    ctx.font = 'bold 14px \"Courier New\", monospace';\n";
        file << "    ctx.textAlign = 'center';\n";
        file << "    ctx.textBaseline = 'middle';\n";
        file << "    ctx.fillText(node, pos.x, pos.y);\n";
        file << "    \n";
        file << "    ctx.fillStyle = user.surplus > 0 ? '#00cc33' : (user.demand > 0 ? '#0066cc' : '#666666');\n";
        file << "    ctx.font = '12px \"Courier New\", monospace';\n";
        file << "    ctx.fillText(user.name, pos.x, pos.y + 50);\n";
        file << "  });\n";
        file << "}\n\n";
        
        // Show message
        file << "function showMessage(msg, type) {\n";
        file << "  const successDiv = document.getElementById('successMsg');\n";
        file << "  const errorDiv = document.getElementById('errorMsg');\n";
        file << "  \n";
        file << "  if (type === 'success') {\n";
        file << "    successDiv.innerHTML = '<span class=\"terminal-text\">' + msg + '</span>';\n";
        file << "    successDiv.style.display = 'block';\n";
        file << "    errorDiv.style.display = 'none';\n";
        file << "    setTimeout(() => successDiv.style.display = 'none', 5000);\n";
        file << "  } else {\n";
        file << "    errorDiv.innerHTML = '<span class=\"terminal-text\">' + msg + '</span>';\n";
        file << "    errorDiv.style.display = 'block';\n";
        file << "    successDiv.style.display = 'none';\n";
        file << "    setTimeout(() => errorDiv.style.display = 'none', 5000);\n";
        file << "  }\n";
        file << "}\n\n";
        
        // Update all displays
        file << "function updateAll() {\n";
        file << "  const totalEnergy = transactions.reduce((sum, t) => sum + t.energy, 0);\n";
        file << "  const totalRevenue = transactions.reduce((sum, t) => sum + (t.energy * t.price), 0);\n";
        file << "  const avgPrice = transactions.length > 0 ? totalRevenue / totalEnergy : 0;\n";
        file << "  \n";
        file << "  document.getElementById('totalEnergy').textContent = totalEnergy.toFixed(2) + ' kWh';\n";
        file << "  document.getElementById('totalRevenue').textContent = '₹' + totalRevenue.toFixed(2);\n";
        file << "  document.getElementById('totalTrades').textContent = transactions.length;\n";
        file << "  document.getElementById('avgPrice').textContent = '₹' + avgPrice.toFixed(2) + '/kWh';\n";
        file << "  \n";
        file << "  let usersHTML = '';\n";
        file << "  users.forEach(u => {\n";
        file << "    const surplusBadge = u.surplus > 0 ? `<span class='badge badge-green'>${u.surplus.toFixed(2)} kWh</span>` : `<span class='badge badge-gray'>0 kWh</span>`;\n";
        file << "    const demandBadge = u.demand > 0 ? `<span class='badge badge-blue'>${u.demand.toFixed(2)} kWh</span>` : `<span class='badge badge-gray'>0 kWh</span>`;\n";
        file << "    usersHTML += `<tr><td>${u.id}</td><td>${u.name}</td><td>${surplusBadge}</td><td>${demandBadge}</td><td>₹${u.balance.toFixed(2)}</td></tr>`;\n";
        file << "  });\n";
        file << "  document.getElementById('usersTable').innerHTML = usersHTML;\n";
        file << "  \n";
        file << "  let txnHTML = '';\n";
        file << "  transactions.forEach((t, i) => {\n";
        file << "    txnHTML += `<tr><td>${i + 1}</td><td>${t.id}</td><td>${t.seller}</td><td>${t.buyer}</td><td>${t.energy.toFixed(2)} kWh</td><td>₹${t.price.toFixed(2)}</td><td>₹${(t.energy * t.price).toFixed(2)}</td></tr>`;\n";
        file << "  });\n";
        file << "  document.getElementById('txnTable').innerHTML = txnHTML || `<tr><td colspan='7' style='text-align:center; color:#003300;'>No transactions logged</td></tr>`;\n";
        file << "  \n";
        file << "  const sellerSelect = document.getElementById('seller');\n";
        file << "  const buyerSelect = document.getElementById('buyer');\n";
        file << "  const pathStartSelect = document.getElementById('pathStart');\n";
        file << "  const pathEndSelect = document.getElementById('pathEnd');\n";
        file << "  \n";
        file << "  sellerSelect.innerHTML = `<option value=''>SELECT SELLER</option>`;\n";
        file << "  buyerSelect.innerHTML = `<option value=''>SELECT BUYER</option>`;\n";
        file << "  pathStartSelect.innerHTML = `<option value=''>SELECT START NODE</option>`;\n";
        file << "  pathEndSelect.innerHTML = `<option value=''>SELECT END NODE</option>`;\n";
        file << "  \n";
        file << "  users.filter(u => u.surplus > 0).forEach(u => {\n";
        file << "    sellerSelect.innerHTML += `<option value='${u.id}'>${u.name} (${u.id}) - ${u.surplus.toFixed(2)} kWh</option>`;\n";
        file << "  });\n";
        file << "  \n";
        file << "  users.filter(u => u.demand > 0).forEach(u => {\n";
        file << "    buyerSelect.innerHTML += `<option value='${u.id}'>${u.name} (${u.id}) - Needs ${u.demand.toFixed(2)} kWh</option>`;\n";
        file << "  });\n";
        file << "  \n";
        file << "  users.forEach(u => {\n";
        file << "    pathStartSelect.innerHTML += `<option value='${u.id}'>${u.name} (${u.id})</option>`;\n";
        file << "    pathEndSelect.innerHTML += `<option value='${u.id}'>${u.name} (${u.id})</option>`;\n";
        file << "  });\n";
        file << "  \n";
        file << "  drawTree();\n";
        file << "}\n\n";
        
        file << "window.onload = function() {\n";
        file << "  initData();\n";
        file << "  updateAll();\n";
        file << "  \n";
        file << "  setInterval(() => {\n";
        file << "    const scanElements = document.querySelectorAll('.card');\n";
        file << "    scanElements.forEach(el => {\n";
        file << "      const rect = el.getBoundingClientRect();\n";
        file << "      if (rect.top < window.innerHeight && rect.bottom > 0) {\n";
        file << "        el.style.animation = 'none';\n";
        file << "        el.offsetHeight;\n";
        file << "        el.style.animation = null;\n";
        file << "      }\n";
        file << "    });\n";
        file << "  }, 3000);\n";
        file << "};\n";
        file << "</script>\n";
        file << "</head>\n<body>\n";
        
        // HTML Body
        file << "<div class='container'>\n";
    file << "  <h1>⚡ NEXUS ENERGY</h1>\n";
    file << "  <div class='subtitle'>Energy Trading</div>\n";
        
        // Statistics
        file << "  <div class='stats'>\n";
        file << "    <div class='stat-box'><h3>Total Energy</h3><div class='value' id='totalEnergy'>0 kWh</div></div>\n";
        file << "    <div class='stat-box'><h3>Total Revenue</h3><div class='value' id='totalRevenue'>₹0</div></div>\n";
        file << "    <div class='stat-box'><h3>Total Trades</h3><div class='value' id='totalTrades'>0</div></div>\n";
        file << "    <div class='stat-box'><h3>Avg Price</h3><div class='value' id='avgPrice'>₹0/kWh</div></div>\n";
        file << "  </div>\n";
        
        // Tabs
        file << "  <div class='card'>\n";
        file << "    <div class='tabs'>\n";
    file << "      <button class='tab active' id='tab-trade' onclick='switchTab(\"trade\")'>Trade</button>\n";
    file << "      <button class='tab' id='tab-network' onclick='switchTab(\"network\")'>Network</button>\n";
    file << "      <button class='tab' id='tab-path' onclick='switchTab(\"path\")'>Path</button>\n";
    file << "      <button class='tab' id='tab-data' onclick='switchTab(\"data\")'>Data</button>\n";
        file << "    </div>\n";
        
        // Trade Tab
        file << "    <div class='tab-content active' id='content-trade'>\n";
        file << "      <div id='successMsg' class='success'></div>\n";
        file << "      <div id='errorMsg' class='error'></div>\n";
        file << "      <form id='tradeForm' onsubmit='event.preventDefault(); executeTrade();'>\n";
        file << "        <div class='form-grid'>\n";
        file << "          <div class='form-group'><label>Seller Node</label><select id='seller' required></select></div>\n";
        file << "          <div class='form-group'><label>Buyer Node</label><select id='buyer' required></select></div>\n";
        file << "          <div class='form-group'><label>Energy (kWh)</label><input type='number' id='energy' step='0.01' min='0.01' placeholder='Enter energy amount' required></div>\n";
        file << "          <div class='form-group'><label>Price (₹/kWh)</label><input type='number' id='price' step='0.01' min='0.01' placeholder='Enter price per unit' required></div>\n";
        file << "        </div>\n";
    file << "        <button type='submit'>Execute</button>\n";
    file << "        <button type='button' class='btn-reset' onclick='resetData()'>Reset</button>\n";
        file << "      </form>\n";
        file << "    </div>\n";
        
        // Network Tree Tab
        file << "    <div class='tab-content' id='content-network'>\n";
        file << "      <h2>Network Topology Visualization</h2>\n";
        file << "      <div class='tree-legend'>\n";
        file << "        <div class='legend-item'><div class='legend-color' style='background:linear-gradient(#00ff41, #003300);'></div><span>Supplier Node</span></div>\n";
        file << "        <div class='legend-item'><div class='legend-color' style='background:linear-gradient(#0088ff, #002244);'></div><span>Consumer Node</span></div>\n";
        file << "        <div class='legend-item'><div class='legend-color' style='background:linear-gradient(#666666, #222222);'></div><span>Balanced Node</span></div>\n";
        file << "      </div>\n";
        file << "      <canvas id='tree-canvas'></canvas>\n";
        file << "    </div>\n";
        
        // Path Finder Tab
        file << "    <div class='tab-content' id='content-path'>\n";
        file << "      <h2>Network Path Analysis</h2>\n";
        file << "      <div class='path-finder'>\n";
        file << "        <div class='form-grid'>\n";
        file << "          <div class='form-group'><label>Start Node</label><select id='pathStart'></select></div>\n";
        file << "          <div class='form-group'><label>End Node</label><select id='pathEnd'></select></div>\n";
        file << "        </div>\n";
        file << "        <button onclick='findPath()'>Analyze Path</button>\n";
        file << "        <div id='pathResult' class='path-result'></div>\n";
        file << "      </div>\n";
        file << "    </div>\n";
        
        // Data View Tab
        file << "    <div class='tab-content' id='content-data'>\n";
        file << "      <h2>Node Registry</h2>\n";
        file << "      <table><thead><tr><th>Node ID</th><th>Designation</th><th>Surplus</th><th>Demand</th><th>Balance</th></tr></thead><tbody id='usersTable'></tbody></table>\n";
        file << "      <h2 style='margin-top:30px;'>Transaction Log</h2>\n";
        file << "      <table><thead><tr><th>#</th><th>Transaction ID</th><th>Seller</th><th>Buyer</th><th>Energy</th><th>Price</th><th>Total</th></tr></thead><tbody id='txnTable'></tbody></table>\n";
        file << "    </div>\n";
        file << "  </div>\n";
        
        file << "</div>\n";
        file << "</body>\n</html>";
        
        file.close();
        
        cout << "\n🚀 SYSTEM INITIALIZED\n";
        cout << "🌐 Launching NEXUS-ENERGY Platform...\n\n";
        
        openBrowser(filename);
        
        cout << "✅ Platform online and operational\n";
        cout << "💾 Data stored in secure memory cache\n";
        cout << "🎮 Dark theme activated - Enjoy the cyber experience!\n\n";
    }
};

// ==================== MAIN ====================

int main() {
    TradingPlatform platform;

    cout << "\n==============================================\n";
    cout << "  NEXUS-ENERGY Trading Platform v2.0\n";
    cout << "        Dark Tech Edition\n";
    cout << "==============================================\n\n";

    platform.addUser(User("S1", "Solar Farm Alpha", 100, 0, 5000));
    platform.addUser(User("S2", "Wind Turbine Beta", 150, 0, 6000));
    platform.addUser(User("B1", "Factory Complex X", 0, 80, 8000));
    platform.addUser(User("B2", "Residential Sector Y", 0, 120, 10000));
    platform.addUser(User("M1", "Energy Storage Hub", 50, 30, 7000));

    cout << "📡 Initializing network nodes...\n";
    cout << "⚡ Powering up trading core...\n";
    cout << "🔒 Securing transaction protocols...\n\n";

    platform.launchWebApp();

    return 0;
}