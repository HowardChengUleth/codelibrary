import java.util.*;

public class GoToNearestGoldPlayer {
    /*
     * Sizes of the game field
     */
    private static final int FIELD_COLUMN_COUNT = 25;
    private static final int FIELD_ROW_COUNT = 16;

    /*
     * Values of cells
     */
    private static final char REMOVED_BRICK = '-';
    private static final char LADDER = 'H';
    private static final char BRICK = '=';
    private static final char EMPTY = '.';
    private static final char GOLD = '*';

    /*
     * Contains previous positions used by BFS
     */
    private static Position prevPosition[][] = new Position[FIELD_ROW_COUNT][FIELD_COLUMN_COUNT];

    /*
     * Contains previous moves used by BFS
     */
    private static Move prevMove[][] = new Move[FIELD_ROW_COUNT][FIELD_COLUMN_COUNT];

    /*
     * The game field
     */
    private static char[][] field = new char[FIELD_COLUMN_COUNT][FIELD_ROW_COUNT];

    /*
     * Position queue used by BFS
     */
    private static Queue<Position> queue = new LinkedList<Position>();

    /*
     * Programs of the enemies
     */
    private static String[] enemyPrograms;

    /*
     * Positions of the enemies
     */
    private static Position[] enemies;

    /*
     * Masters of the enemies
     */
    private static int[] masterOfEnemy;

    /*
     * Positions of the runners, our runner is on the first place
     */
    private static Position[] runners;

    /*
     * Some information about the runners
     */
    private static int[] scores;
    private static int[] delays;

    /*
     * Scanner used for data reading
     */
    private static Scanner in;

    /*
     * Entry point of player
     */
    public static void main(String[] args) {
        try {
            in = new Scanner(System.in);

            // Read initial state of the world
            int turns = in.nextInt();
            readFirstDescription();

            // Pass through turns
            for(int turnIndex = 0; turnIndex < turns; turnIndex++) {
                int turn = in.nextInt();
                if (turn == -1) {
                    break;
                }

                // Read the current state of the world and make a move
                readStateDescription();
                System.out.println(makeMove());
                System.out.flush();
            }
        } catch (Exception e) {
            System.err.println(e.getMessage());
            System.exit(-1);
        }
    }

    /*
     * Calculates the next move for our runner
     */
    private static Move makeMove() {
        // Is our runner died?
        if (!runners[0].correct()) {
            return Move.NONE;
        }

        // Clearing arrays for BFS
        for (int x = 0; x < FIELD_ROW_COUNT; x++) {
            for (int y = 0; y < FIELD_COLUMN_COUNT; y++) {
                prevPosition[x][y] = null;
                prevMove[x][y] = null;
            }
        }

        // BFS
        queue.clear();
        Position target = null;

        // Start BFS from our runner's position to all cells with gold
        update(runners[0], null, Move.NONE);
        while (!queue.isEmpty()) {
            Position position = queue.poll();
            if (getCell(position) == GOLD) {
                target = position;
                break;
            }

            // Iterate through possible moves in order (LEFT, RIGHT, TOP, BOTTOM)
            for (Move move : Move.values()) {
                Position newPosition = move(position, move);
                update(newPosition, position, move);
            }
        }

        // If there is no available gold on the field, then do nothing
        if (target == null) {
            return Move.NONE;
        }

        // Returning back through previous positions from the closest gold
        Position current = target, previous = target;
        while (!current.equals(runners[0])) {
            previous = current;
            current = prevPosition[current.getRow()][current.getColumn()];
        }

        // Make a move to the closest gold
        return prevMove[previous.getRow()][previous.getColumn()];
    }

    /*
     * Returns the next position if a one makes the given move from the given position
     */
    private static Position move(Position currentPosition, Move move) {
        // Move ignoring trapped enemies
        Position newPosition = moveIgnoringEnemies(currentPosition, move, true);

        // Checks for a enemy below in cell with removed brick
        if (!newPosition.equals(currentPosition) && getCell(newPosition) == REMOVED_BRICK) {
            for (Position enemy : enemies) {
                if (enemy.equals(newPosition)) {
                    // Move without fall down
                    Position position = moveIgnoringEnemies(currentPosition, move, false);
                    if (position.equals(newPosition) && move == Move.BOTTOM) {
                        return currentPosition; // Cannot move below to the cell with enemy
                    } else {
                        return position;
                    }
                }
            }
        }

        return newPosition;
    }

    /*
     * Returns the next position if a one makes the given move from the
     * given position, but ignores enemies in cells with removed bricks
     */
    private static Position moveIgnoringEnemies(Position position, Move move, boolean canFly) {
        char currentCell = getCell(position);
        Position shiftedTopPosition = position.shift(Move.TOP);
        Position shiftedRightPosition = position.shift(Move.RIGHT);
        Position shiftedBottomPosition = position.shift(Move.BOTTOM);
        Position shiftedLeftPosition = position.shift(Move.LEFT);

        // Is flying?
        if (isEmptyLike(currentCell) && isEmptyLike(getCell(shiftedBottomPosition)) && canFly) {
            return shiftedBottomPosition;
        }

        if (move == Move.NONE) {
            return position;
        }

        switch (move) {
            case TOP:
                if (currentCell == LADDER && getCell(shiftedTopPosition) != BRICK) {
                    return shiftedTopPosition;
                }
                break;
            case RIGHT:
                if (getCell(shiftedRightPosition) != BRICK) {
                    return shiftedRightPosition;
                }
                break;
            case BOTTOM:
                if ((currentCell == LADDER || isEmptyLike(currentCell))
                        && (getCell(shiftedBottomPosition) == LADDER || isEmptyLike(getCell(shiftedBottomPosition)))) {
                    return shiftedBottomPosition;
                }
                break;
            case LEFT:
                if (getCell(shiftedLeftPosition) != BRICK) {
                    return shiftedLeftPosition;
                }
                break;
        }

        return position;
    }

    /*
     * Returns true iff the cell looks like empty
     */
    private static boolean isEmptyLike(char cell) {
        return cell == EMPTY || cell == GOLD || cell == REMOVED_BRICK;
    }

    /*
     * Returns cell iff the position is inside the game field and BRICK otherwise.
     */
    private static char getCell(Position position) {
        if (0 <= position.getRow() && position.getRow() < FIELD_ROW_COUNT) {
            if (0 <= position.getColumn() && position.getColumn() < FIELD_COLUMN_COUNT) {
                return field[position.getRow()][position.getColumn()];
            }
        }

        // The given position is out of the field
        return BRICK;
    }

    /*
     * Updates values in BFS
     */
    private static void update(Position position, Position prev, Move move) {
        if (prevMove[position.getRow()][position.getColumn()] == null) {
            prevPosition[position.getRow()][position.getColumn()] = prev;
            prevMove[position.getRow()][position.getColumn()] = move;
            queue.add(position);
        }
    }

    /*
     * Reads per-turn game state
     */
    private static void readStateDescription() {
        // Read the game field
        readGameField();

        // Read descriptions of the runners
        for(int i = 0; i < 2; i++) {
            runners[i] = readPosition();
            scores[i] = in.nextInt();
            delays[i] = in.nextInt();
        }

        // Read descriptions of the enemies
        for(int i = 0; i < enemies.length; i++) {
            enemies[i] = readPosition();
            masterOfEnemy[i] = in.nextInt();
        }
    }

    /*
     * Reads one-time world description
     */
    private static void readFirstDescription() {
        // Read the game field
        readGameField();
        runners = new Position[2];
        scores = new int[2];
        delays = new int[2];

        // Read descriptions of the runners
        readCharactersPositions(runners);

        // Read descriptions of the enemies
        int enemyCount = in.nextInt();
        enemies = new Position[enemyCount];
        enemyPrograms = new String[enemyCount];
        masterOfEnemy = new int[enemyCount];
        for (int i = 0; i < enemies.length; i++) {
            enemies[i] = readPosition();
            enemyPrograms[i] = in.next();
        }
    }

    /*
     * Reads the game field from the scanner
     */
    private static void readGameField() {
        for (int i = 0; i < FIELD_ROW_COUNT; i++) {
            String line = in.next();
            for (int j = 0; j < FIELD_COLUMN_COUNT; j++) {
                field[i] = line.toCharArray();
            }
        }
    }

    /*
     * Reads array of positions from the scanner
     */
    private static void readCharactersPositions(Position[] positions) {
        for (int i = 0; i < positions.length; i++) {
            positions[i] = readPosition();
        }
    }

    /*
     * Reads position from the scanner
     */
    private static Position readPosition() {
        int row = in.nextInt();
        int column = in.nextInt();
        return new Position(row, column);
    }

    /*
     * Class for positions of runners and enemies on the game field
     */
    private static class Position {
        private final int row;
        private final int column;

        private Position(int row, int column) {
            this.row = row;
            this.column = column;
        }

        public int getRow() {
            return row;
        }

        public int getColumn() {
            return column;
        }

        // Correct position isn't equal to "-1 -1"
        public boolean correct() {
            return row >= 0 && column >= 0;
        }

        // Shift position to specific direction
        public Position shift(Move move) {
            if (move == Move.LEFT) {
                return new Position(row, column - 1);
            }

            if (move == Move.RIGHT) {
                return new Position(row, column + 1);
            }

            if (move == Move.TOP) {
                return new Position(row - 1, column);
            }

            if (move == Move.BOTTOM) {
                return new Position(row + 1, column);
            }

            return new Position(row, column);
        }

        @Override
        public String toString() {
            return "(" + row + ", " + column + ")";
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (!(o instanceof Position)) return false;

            Position position = (Position) o;
            return row == position.row && column == position.column;
        }

        @Override
        public int hashCode() {
            int result = row;
            result = 31 * result + column;
            return result;
        }
    }

    /*
     * Possible moves of runner
     */
    private static enum Move {
        NONE,
        LEFT,
        RIGHT,
        TOP,
        BOTTOM
    }
}
