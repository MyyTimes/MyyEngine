#pragma once

class ISystem
{
public:
	virtual ~ISystem() = default;

	virtual bool Init() = 0;
	virtual void Shutdown() = 0;
	virtual void Update(float) = 0;
	virtual void Render() = 0;
};