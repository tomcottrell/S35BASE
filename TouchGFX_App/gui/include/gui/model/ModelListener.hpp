#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

// Forward declaration
class Model;

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    void bindModel(Model* m)
    {
        model = m;
    }

    // System state change notification
    virtual void systemStateChanged() {}

protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
