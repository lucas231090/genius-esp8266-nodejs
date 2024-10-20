const express = require("express");
const WebSocket = require("ws");

const app = express();
const port = 3000;

app.use(express.static("public"));

const wss = new WebSocket.Server({ noServer: true });

let currentSequence = [];
let gameState = {
  level: 0,
  started: false,
};

function generateSequence(level) {
  const colors = ["red", "green", "blue"];
  let sequence = [];
  for (let i = 0; i < level; i++) {
    const randomColor = colors[Math.floor(Math.random() * colors.length)];
    sequence.push(randomColor);
  }
  return sequence;
}

wss.on("connection", (ws) => {
  console.log("Nova conexão");

  ws.on("message", (message) => {
    const data = JSON.parse(message);

    if (data.type === "start") {
      gameState.level = 1;
      currentSequence = generateSequence(gameState.level);
      ws.send(JSON.stringify({ type: "sequence", sequence: currentSequence }));
    }

    if (data.type === "userInput") {
      const { userSequence } = data;
      const isCorrect = userSequence.every(
        (color, index) => color === currentSequence[index]
      );
      if (isCorrect && userSequence.length === currentSequence.length) {
        gameState.level += 1;
        currentSequence = generateSequence(gameState.level);
        ws.send(
          JSON.stringify({ type: "nextLevel", sequence: currentSequence })
        );
      } else if (!isCorrect) {
        gameState.level = 0;
        ws.send(JSON.stringify({ type: "gameOver" }));
      }
    }
  });
});

const server = app.listen(port, () => {
  console.log(`Servidor rodando na porta ${port}`);
});

server.on("upgrade", (request, socket, head) => {
  wss.handleUpgrade(request, socket, head, (ws) => {
    wss.emit("connection", ws, request);
  });
});
