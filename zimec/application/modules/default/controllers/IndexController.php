<?php

class Default_IndexController extends Simec_Controller_Action
{
    public function indexAction()
    {
    	$this->_helper->layout->setLayout('login');
    }
}