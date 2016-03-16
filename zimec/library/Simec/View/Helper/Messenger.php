<?php
/**
* Helper for print messages in screen
*
* @uses	   Zend_View_Helper_Abstract
* @package	Core_View_Helper_Messenger
* @subpackage View\Helper
*/

class Simec_View_Helper_Messenger extends Zend_View_Helper_Abstract 
{
	/**
	 * @param  array  $messages
	 * @param  string $title
	 * @return string
	 */
	public function notify() 
	{
		$html = null;
		$viewMessages = $this->view->messages;
		$flashMessages = Zend_Controller_Action_HelperBroker::getStaticHelper('FlashMessenger')->getMessages();
		
		$messages = $this->_view();
		$messages.= $this->_flash();
		
		if ($messages)
		{
			$html .= '<div class="message-content">';
			$html .= '<ul>';
			$html .= $messages;
			$html .= '</ul>';
			$html .= '</div>';
		}

		if ($html) 
			$html = '<div id="messages">' . $html . '</div>';
		
		return $html;
	}
	
	/**
	 * @param  array  $messages
	 * @param  string $title
	 * @return string
	 */
	public function messenger()
	{
		$html = null;
		$viewMessages = $this->view->messages;
		$flashMessages = Zend_Controller_Action_HelperBroker::getStaticHelper('FlashMessenger')->getMessages();
	
		$messages = $this->_view();
		$messages.= $this->_flash();
	
		if ($messages)
		{
			$html .= '<div class="message-content">';
			$html .= '<ul>';
			$html .= $messages;
			$html .= '</ul>';
			$html .= '</div>';
		}
	
		if ($html)
			$html = '<div id="messages" style="display: none;">' . $html . '</div>';
	
		return $html;
	}
	
	private function _view()
	{
		$htmlView = null;
		$viewMessages = $this->view->messages;
		
		if (count($viewMessages))
		{
			foreach ($viewMessages as $namespace => $messages)
			{
				$count = count($messages);
		
				if ($count > 0)
				{
					foreach ($messages as $message)
					{
						$htmlView .= '<li data-tipo="' . $namespace . '" class="alerter ' . $namespace . '">';
						$htmlView .= $message;
						$htmlView .= '</li>';
					}
				}
			}
		
			return $htmlView;
		}		
	}
	
	private function _flash()
	{
		$htmlFlash = null;
		$flashMessages = Zend_Controller_Action_HelperBroker::getStaticHelper('FlashMessenger')->getMessages();
		
		if (count($flashMessages)) 
		{
			foreach ($flashMessages as $messages) 
			{
				$count = count($messages);
				
				if ($count > 0) 
				{
					foreach ($messages as $namespace => $message) 
					{
						$htmlFlash .= '<li data-tipo="' . $namespace . '" class="alerter ' . $namespace . '">';
						$htmlFlash .= $message;
						$htmlFlash .= '</li>';
					}
				}
			}
		}
		
		return $htmlFlash;
	}
}
