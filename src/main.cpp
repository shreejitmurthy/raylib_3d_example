#include "raylib.h"
#include "reactphysics3d/reactphysics3d.h"

void QuaternionToEuler(const reactphysics3d::Quaternion& q, float& pitch, float& yaw, float& roll) {
    // Convert quaternion to Euler angles (pitch, yaw, roll)
    float ysqr = q.y * q.y;

    // Roll (x-axis rotation)
    float t0 = +2.0f * (q.w * q.x + q.y * q.z);
    float t1 = +1.0f - 2.0f * (q.x * q.x + ysqr);
    roll = atan2(t0, t1);

    // Pitch (y-axis rotation)
    float t2 = +2.0f * (q.w * q.y - q.z * q.x);
    t2 = t2 > 1.0f ? 1.0f : t2;
    t2 = t2 < -1.0f ? -1.0f : t2;
    pitch = asin(t2);

    // Yaw (z-axis rotation)
    float t3 = +2.0f * (q.w * q.z + q.x * q.y);
    float t4 = +1.0f - 2.0f * (ysqr + q.z * q.z);
    yaw = atan2(t3, t4);
}

int main() {
    const int screenWidth = 800, screenHeight = 600;

    reactphysics3d::PhysicsCommon physicsCommon;

    // Create a physics world
    reactphysics3d::PhysicsWorld* world = physicsCommon.createPhysicsWorld();
    world->enableSleeping(true);

    // Set gravity
    world->setGravity(reactphysics3d::Vector3(0, -9.81f, 0));

    // Create a rigid body in the world
    reactphysics3d::Vector3 position(0, 5, 0);
    reactphysics3d::Quaternion orientation = reactphysics3d::Quaternion::identity();
    reactphysics3d::Transform transform(position, orientation);
    reactphysics3d::RigidBody* body = world->createRigidBody(transform);

    // Create collision shape for the body
    reactphysics3d::BoxShape* boxShape = physicsCommon.createBoxShape(reactphysics3d::Vector3(0.5f, 0.5f, 0.5f));
    reactphysics3d::Transform shapeTransform = reactphysics3d::Transform::identity();
    reactphysics3d::Collider* collider = body->addCollider(boxShape, shapeTransform);

    // Set material properties for bounciness
    reactphysics3d::Material& material = collider->getMaterial();
    material.setBounciness(0.1);  // Set bounciness (0.0 = no bounce, 1.0 = perfect bounce)

    // Set the inertia tensor to affect rotation
    body->updateMassPropertiesFromColliders();
    reactphysics3d::Vector3 inertiaTensorLocal = body->getLocalInertiaTensor();
    inertiaTensorLocal *= 2.0f; // Increase the rotational inertia
    body->setLocalInertiaTensor(inertiaTensorLocal);

    // Create platform
    reactphysics3d::Vector3 platform_pos(0, 0, 0);
    reactphysics3d::Quaternion platform_orientation = reactphysics3d::Quaternion::identity();
    reactphysics3d::Transform platform_transform(platform_pos, platform_orientation);
    reactphysics3d::RigidBody* platform_body = world->createRigidBody(platform_transform);
    platform_body->setType(reactphysics3d::BodyType::STATIC);

    // Create collision shape for the platform
    reactphysics3d::BoxShape* platformShape = physicsCommon.createBoxShape(reactphysics3d::Vector3(5.0f, 0.f, 5.0f));
    platform_body->addCollider(platformShape, reactphysics3d::Transform::identity());

    const float timeStep = 1.0f / 60.0f;

    InitWindow(screenWidth, screenHeight, "");
    SetTargetFPS(60);

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 10.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Model cube = LoadModel("../resources/cube.obj");

    DisableCursor();

    while (!WindowShouldClose()) {

        SetWindowTitle(TextFormat("FPS: %d", GetFPS()));

        world->update(timeStep);

        // Get the updated position and orientation of the body
        transform = body->getTransform();
        position = transform.getPosition();
        orientation = transform.getOrientation();

        platform_transform = platform_body->getTransform();
        platform_pos = platform_transform.getPosition();

        // Convert the orientation quaternion to Euler angles
        float pitch, yaw, roll;
        QuaternionToEuler(orientation, pitch, yaw, roll);

        UpdateCameraPro(&camera,
                        (Vector3){
                                (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))*0.1f -
                                (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))*0.1f,
                                (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))*0.1f -
                                (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))*0.1f,
                                (IsKeyDown(KEY_LEFT_SHIFT)*-0.1f + (IsKeyDown(KEY_SPACE))*0.1f
                                )
                        },
                        (Vector3){
                                GetMouseDelta().x*0.05f,
                                GetMouseDelta().y*0.05f,
                                0.0f
                        },
                        GetMouseWheelMove()*2.0f);

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        // Draw the cube with its updated position and rotation
        DrawModelEx(cube,
                    (Vector3){position.x, position.y, position.z},
                    (Vector3){1.0f, 0.0f, 0.0f},
                    roll * RAD2DEG,
                    (Vector3){0.5f, 0.5f, 0.5f},
                    WHITE);
        DrawPlane((Vector3){ platform_pos.x, platform_pos.y, platform_pos.z }, {10.f, 10.f}, GRAY);

//        DrawGrid(20, 1.0f);

        EndMode3D();
        DrawText(TextFormat("Position: %f, %f, %f", position.x, position.y, position.z), 10, 10, 20, BLACK);
        DrawText(TextFormat("Sleeping: %s", body->isSleeping() ? "true" : "false"), 10, 30, 20, BLACK);

        EndDrawing();
    }

    CloseWindow();
    physicsCommon.destroyPhysicsWorld(world);

    return 0;
}
