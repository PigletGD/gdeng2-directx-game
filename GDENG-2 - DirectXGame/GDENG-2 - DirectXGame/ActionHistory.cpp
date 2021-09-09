#include "ActionHistory.h"
#include "EditorAction.h"
#include "EngineBackend.h"

ActionHistory* ActionHistory::m_shared_instance = NULL;

ActionHistory::ActionHistory()
{
}

ActionHistory::~ActionHistory()
{
	clear();
}

ActionHistory* ActionHistory::getInstance()
{
	return m_shared_instance;
}

void ActionHistory::initialize()
{
	m_shared_instance = new ActionHistory();
}

void ActionHistory::destroy()
{
	delete m_shared_instance;
}

void ActionHistory::recordAction(AGameObject* gameObject)
{
	if (EngineBackend::getInstance()->getMode() == EngineBackend::EditorMode::EDITOR) {
		EditorAction* editorAction = new EditorAction(gameObject);
		m_actions_performed.push(editorAction);

		std::cout << "Stored action " << gameObject->getName() << "\n";
	}
}

bool ActionHistory::hasRemainingUndoActions()
{
	return !m_actions_performed.empty();
}

bool ActionHistory::hasRemainingRedoActions()
{
	return !m_actions_cancelled.empty();
}


EditorAction* ActionHistory::undoAction()
{
	//do not undo actions during play mode.
	if (EngineBackend::getInstance()->getMode() != EngineBackend::EditorMode::EDITOR) {
		std::cout << "Cannot perform any undo during play. \n";
		return NULL;
	}

	if (this->hasRemainingUndoActions()) {
		EditorAction* action = m_actions_performed.top();
		m_actions_performed.pop();
		m_actions_cancelled.push(action);
		return action;
	}
	else {
		std::cout << "No more actions remaining. \n";
		return NULL;
	}
}

EditorAction* ActionHistory::redoAction()
{
	//do not undo actions during play mode.
	if (EngineBackend::getInstance()->getMode() != EngineBackend::EditorMode::EDITOR) {
		std::cout << "Cannot perform any redo during play. \n";
		return NULL;
	}

	if (this->hasRemainingRedoActions()) {
		EditorAction* action = m_actions_cancelled.top();
		m_actions_cancelled.pop();
		m_actions_performed.push(action);
		return action;
	}
	else {
		std::cout << "No more actions remaining. \n";
		return NULL;
	}
}

void ActionHistory::clear()
{
	while (this->hasRemainingUndoActions()) {
		EditorAction* action = m_actions_performed.top();
		delete action;
		m_actions_performed.pop();
	}

	while (this->hasRemainingRedoActions()) {
		EditorAction* action = m_actions_cancelled.top();
		delete action;
		m_actions_cancelled.pop();
	}
}