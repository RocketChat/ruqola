/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

/**
 * @class SessionKeyDistributionTest
 * @brief Autotest for E2EE session key management and distribution in Rocket.Chat.
 *
 * This test verifies:
 *  - Session key encryption for multiple users using their public keys.
 *
 *  - Distribution of encrypted session keys via the Rocket.Chat API.
 *
 *  - Decryption of suggested keys with private keys.
 *
 *  - Accepting and rejecting suggested group keys.
 *
 *  - Proper assignment and verification of session keys.
 *
 * Prerequisites:
 *
 *  - The .env file must contain credentials for at least two users (USERNAME1, PASSWORD1, USERNAME2, PASSWORD2).
 *
 *  - The test room must exist and be accessible by both users.
 */
class SessionKeyDistributionTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testSessionKeyDistribution();
};