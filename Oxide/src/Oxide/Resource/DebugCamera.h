#pragma once

#include "Oxide/Core/Base.h"
#include "Oxide/Renderer/Renderer.h"
#include <cmath>

namespace Oxide {

    class DebugCamera : public Actor {

    public:

        void OnStart() override {
            
            camera = (PerspectiveCamera*)Game->ActiveScene->camera.get();
            movement = glm::vec3(0);
            movementDir = &Game->ActiveScene->camera->GetLookAt();
            glfwGetCursorPos(Game->window.GetGLFWWindow(), &lastx, &lasty);

            Game->window.eventhandler->OnKeyEvent([&] (OxideEvent key, OxideEvent action) -> void {
                if (action == OxideEvent::Pressed) {
                    switch (key) {
                        case OxideEvent::KEY_W:
                            movement.z = movementspeed;
                            break;
                        case OxideEvent::KEY_A:
                            movement.x = -movementspeed;
                            break;
                        case OxideEvent::KEY_S:
                            movement.z = -movementspeed;
                            break;
                        case OxideEvent::KEY_D:
                            movement.x = movementspeed;
                            break;
                        case OxideEvent::KEY_LEFT_SHIFT:
                            movement.y = -movementspeed;
                            break;
                        case OxideEvent::KEY_SPACE:
                            movement.y = movementspeed;
                            break;
                    }
                } else if (action == OxideEvent::Released) {
                    switch (key) {
                        case OxideEvent::KEY_W:
                            movement.z = 0;
                            break;
                        case OxideEvent::KEY_A:
                            movement.x = 0;
                            break;
                        case OxideEvent::KEY_S:
                            movement.z = 0;
                            break;
                        case OxideEvent::KEY_D:
                            movement.x = 0;
                            break;
                        case OxideEvent::KEY_LEFT_SHIFT:
                            movement.y = 0;
                            break;
                        case OxideEvent::KEY_SPACE:
                            movement.y = 0;
                            break;
                        case OxideEvent::KEY_LEFT_ALT:
                            glPolygonMode(GL_FRONT_AND_BACK, (wireframe) ? GL_FILL : GL_LINE);
                            wireframe = !wireframe;
                        case OxideEvent::KEY_I:
                            movementspeed += 0.3;
                            break;
                        case OxideEvent::KEY_O:
                            movementspeed -= 0.3;
                            break;

                        case OxideEvent::KEY_G:
                            printf("xangle: %f\nyangle: %f\nLookingDir: {%f, %f, %f}\n", xangle, yangle, 
                                    (*movementDir).x, (*movementDir).y, (*movementDir).z);
                            break;
                        case OxideEvent::KEY_Z:
                            FOV -= 2.0;
                            camera->SetFOV(FOV);
                            break;
                        case OxideEvent::KEY_X:
                            FOV += 2.0;
                            camera->SetFOV(FOV);
                            break;
                    }
                }
            });

            Game->window.eventhandler->OnMouseButtonEvent([&] (OxideEvent button, OxideEvent action) -> void {
                GLFWwindow* window = Game->window.GetGLFWWindow();
                if (action == OxideEvent::Pressed) {
                    
                }
                if (action == OxideEvent::Released) {
                    switch (button) {
                        case OxideEvent::MOUSE_BUTTON_1:
                            activemouse = 1 - activemouse;
                            glfwSetInputMode(window, GLFW_CURSOR, (activemouse) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
                            glfwGetCursorPos(window, &lastx, &lasty);
                            break;
                    }
                }
            });

            Game->window.eventhandler->OnMouseEvent([&] (double xpos, double ypos) -> void {

                if (activemouse) return;
                double part = 2.0 * PI;
                double xdiff = (xpos - lastx);
                double ydiff = -(ypos - lasty);
                xangle = xangle + xdiff * sensitivity;
                xangle = (xangle < 0) ? part - xangle : fmod(xangle, part);
                yangle = fmin(fmax(yangle + ydiff * sensitivity, -PI/2.0 + 0.2), PI/2.0 - 0.2);
                lastx = xpos;
                lasty = ypos;
            });

            FOV = camera->GetFOV();
        }

        void EachFrame() {
            glm::vec3 forwardDir = *(movementDir);
            forwardDir.y = 0;
            forwardDir = glm::normalize(forwardDir);
            glm::vec3 transformed = (constant * movement).x * cross(*movementDir, {0, 1, 0}) + (constant * movement).y * glm::vec3(0, 1, 0) + (constant * movement).z * (forwardDir);
            Game->ActiveScene->camera->Move(transformed);
            if (!activemouse) {
                if (oldDir == *movementDir) {
                    glm::vec3 newdir = {cos(xangle) * cos(yangle), sin(yangle), sin(xangle) * cos(yangle)};
                    Game->ActiveScene->camera->LookAt(newdir, true);
                    oldDir = *movementDir;
                } else {
                    oldDir = *(movementDir);
                    glm::vec2 normvec = glm::normalize(glm::vec2(oldDir.z, oldDir.x));
                    xangle = ((oldDir.z < 0) ? PI - asin(normvec.y) : asin(normvec.y));
                    yangle = asin(oldDir.y);
                }
            }
        }

        const std::string TypeName = "DebugCamera";

        glm::vec3 movement;
        const glm::vec3* movementDir;
        glm::vec3 oldDir = {0, 0, 0};
        float movementspeed = 1.0f;
        const float constant = 0.01f;
        bool activemouse = true;

        float FOV = 90.0f;

        double sensitivity = 2.0 * PI/2560.0;
        double lastx, lasty = 0.0f;
        float xangle, yangle = 0.0f;
        bool lookingChanged = true;
        bool wireframe = false;

        PerspectiveCamera* camera;

    ACTOR_ESSENTIALS(DebugCamera);

    };

}