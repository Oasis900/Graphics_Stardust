#pragma once
#ifndef INTERFACES_H
#define INTERFACES_H

struct IRenderable
{
    virtual void Render() = 0;
};

struct IUpdatable
{
    virtual void Update(const float& delta_time) = 0;
};
#endif
