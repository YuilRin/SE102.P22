﻿#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "../GameObject.h"
#include <vector>
#include "../../Animation/Animation.h"
#include "../Weapons/Weapon.h"
#include "Info.h"
#include "../../Tilemap/Collider.h"

#include <unordered_map>
enum class PlayerState {
    Idle, Walking, Jumping, SitDown, Stand_Hit,
    Climbing, Attacking, TakingDamage, Dead, PickingUpItem, Falling

};


class Player : public GameObject {
private:
    float speed;
    float velocityY,
        velocityX;
    float _gravity = 9.8f;

    bool facingLeft;

    bool isOnGround, isOnStair, isOnMovingPlatform;
    bool isHolding, isAttacking, isJumping;
    bool _isDead;


    int health;
    int lives;
    int whipLevel = 1;

    bool hasCross; ///??

    ID3D11Device* device;
    ID3D11DeviceContext* deviceContext;

    


    PlayerState state;

    std::map<PlayerState, Animation> animations;

    
    std::unordered_map<WeaponType, Weapon*> weaponPool;
    Weapon* currentWeapon;

    float attackTimer = 0.0f;
    const float attackDuration = 0.91f;

    int currentStage;
    bool isChangingStage;

    Info* _info;
    std::vector<Collider> groundColliders;
	//std::vector<Collider> stairColliders;

public:
    void HandleWeaponUpdate(float elapsedTime);
    void HandleAxeUpdate();

    Player(float x, float y, std::map<PlayerState, Animation> anims, ID3D11Device* device);
    void SetGroundColliders(const std::vector<Collider>& colliders);

    void onKeyPressed(WPARAM key);
    void onKeyReleased(WPARAM key);
    void unhookinputevent();

    void Update(float elapsedTime) override;
    void Render(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) override;

    /// Move
    void MoveLeft();
    void MoveRight();
    void Jump();
    void Fall();
    void SitDown();
    void StandUp();
    void ClimbUp();
    void ClimbDown();

    /// Attack
    void Attack();
    void TakeDamage(int damage);
    void Revive();
    void ChangeState(PlayerState newState);
    void CheckCollision(GameObject* object);

    /// Weapon
    void ChangeWeapon(WeaponType newType);
    void AddWeapon(Weapon* newWeapon);
    void RemoveWeapon(WeaponType type);
    void UseWeapon(WeaponType type);

    /// Stage
    void SetStage(int stageID);
    int GetStage();
    bool IsChangingStage();
    void TransitionToNextStage();

    void Reset();

    void UpgradeWhip();

};

#endif

