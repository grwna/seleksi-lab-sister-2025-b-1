// SPDX-License-Identifier: UNLICENSED
pragma solidity ^0.8.13;

interface IBoss {
    function lifePoints() external returns (uint);
    function attack(uint256 damage) external;
    function loot() external;
}

contract Boss {
    IBoss immutable enemy;

    constructor(address target) payable{
        enemy = IBoss(target);
    }

    function win() external {
        enemy.attack(enemy.lifePoints());
    }
}
