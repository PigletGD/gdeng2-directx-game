#pragma once
#include <iostream>
#include <vector>
#include <stack>

class EditorAction;
class AGameObject;

class ActionHistory
{
public:
	typedef std::stack<EditorAction*> ActionStack;

	static ActionHistory* getInstance();
	static void initialize();
	static void destroy();

	void recordAction(AGameObject* gameObject);
	bool hasRemainingUndoActions();
	bool hasRemainingRedoActions();
	EditorAction* undoAction();
	EditorAction* redoAction();
	void clear();

private:
	ActionHistory();
	~ActionHistory();
	ActionHistory(ActionHistory const&) {};             // copy constructor is private
	ActionHistory& operator=(ActionHistory const&) {};  // assignment operator is private*/

	static ActionHistory* m_shared_instance;

	ActionStack m_actions_performed;
	ActionStack m_actions_cancelled;
};