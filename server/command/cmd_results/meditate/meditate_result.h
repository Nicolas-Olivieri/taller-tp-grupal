#ifndef MEDITATE_RESULT_H
#define MEDITATE_RESULT_H


enum class MeditateStatus { SUCCESS, GHOST_FAIL, ARCHETYPE_FAIL, MUST_NOT_NOTIFY };


struct MeditateResult {
    MeditateStatus status;

    MeditateResult();

    explicit MeditateResult(const MeditateStatus& status);
};


#endif  // MEDITATE_RESULT_H
