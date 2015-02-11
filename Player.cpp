#include "SDL.h"
#include "Player.h"
#include "pieces.h"

Player::Player(Field *field)
{
    m_field = field;
}

Player::~Player()
{
}

void
Player::setContext(Piece *piece, Piece *nextPiece)
{
    m_piece.setPiece(piece->getPiece());
    m_piece.setRotation(piece->getRotation());

    m_nextPiece.setPiece(nextPiece->getPiece());
    m_nextPiece.setRotation(nextPiece->getRotation());

    calculateDest();
}

int
Player::getNextAction(int posX, int posY, int rotation)
{
    if (rotation !=  m_destRotation) {
        return ACTION_ROTATE;
    }

    if (posX < m_destPosX) {
        return ACTION_MOVE_RIGHT;
    }

    if (posX > m_destPosX) {
        return ACTION_MOVE_LEFT;
    }

    return ACTION_DROP;
}

void
Player::calculateDest()
{
    int bestFit = -1000;
    int bestX = -1000, bestR = -1000;

    for (int x=0; x<FIELD_WIDTH; x++) {
        int maxRotation = 4;
        // reduce number of rotations for symmetric pieces
        if (m_piece.getPiece() == PIECE_O) {
            maxRotation = 1;
        } else if (
            m_piece.getPiece() == PIECE_S
            || m_piece.getPiece() == PIECE_Z
            || m_piece.getPiece() == PIECE_I
        ) {
            maxRotation = 2;
        }
        for (int r=0; r<maxRotation; r++) { // rotations
            m_piece.setRotation(r);
            int fit = checkXR(x, r);
            if (fit > bestFit) {
                bestFit = fit;
                bestX = x;
                bestR = r;
            }
        }
    }

    m_destPosX = bestX;
    m_destRotation = bestR;
}

int
Player::checkXR(int posX, int r)
{
    int x, y, posY = 0;
    int joins = 0, blanks = 0;
    int collisions;

    // drop piece to find posY
    for (;;) {
        collisions = m_field->checkCollisions(
            &m_piece,
            posX,
            posY+1,
            Field::COLLIDED_HEAP|Field::COLLIDED_LEFT|Field::COLLIDED_RIGHT
        );
        if (collisions) {
            break;
        }
        posY++;
    }

    // out of field
    if (
        collisions&Field::COLLIDED_LEFT
        || collisions&Field::COLLIDED_RIGHT
    ) {
        return 0;
    }

    m_piece.resetBlocksEnum();
    while (m_piece.iterateBlocksEnum(&x, &y)) {
        int blockX = posX + x - PIECE_SIZE/2;
        int blockY = posY + y - PIECE_SIZE/2;

        if (
            blockX == 0
            || blockX == FIELD_WIDTH - 1
        ) {
            joins++;
        }

        // touches the heap with bottom
        if (
            blockY == FIELD_HEIGHT - 1
            || m_field->checkNotBlank(blockX, blockY + 1)
        ) {
            joins++;
        }

        // no blocks from below - bad
        if (
            blockY < FIELD_HEIGHT - 1
            && !m_field->checkNotBlank(blockX, blockY + 1)
            && (
                y >= PIECE_SIZE - 1
                || !m_piece.checkHasBlock(x, y+1)
            )
        ) {
            blanks++;
        }

        // touches the heap with left side
        if (
            blockX > 0
            && m_field->checkNotBlank(blockX - 1, blockY)
        ) {
            joins++;
        }

        // touches the heap with right side
        if (
            blockX < FIELD_WIDTH - 1
            && m_field->checkNotBlank(blockX + 1, blockY)
        ) {
            joins++;
        }
    }

    // we don't like blanks twice stronger than we like joins
    // and we prefer to put pieces as low we can
    return (joins - blanks * 2) * posY;
}
