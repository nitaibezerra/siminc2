<?php

import('models.Par.Demandatipo');

class Par_IndexController extends Simec_Controller_Action
{
    public function indexAction()
    {
    }

    public function formularioAction()
    {
        $model = new Par_Model_Demandatipo();
    }
}