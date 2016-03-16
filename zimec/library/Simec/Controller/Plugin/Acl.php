<?php

class Simec_Controller_Plugin_Acl extends Zend_Controller_Plugin_Abstract
{

    /**
     * Called before an action is dispatched by Zend_Controller_Dispatcher.
     *
     * This callback allows for proxy or filter behavior.  By altering the
     * request and resetting its dispatched flag (via
     * {@link Zend_Controller_Request_Abstract::setDispatched() setDispatched(false)}),
     * the current action may be skipped.
     *
     * @param  Zend_Controller_Request_Abstract $request
     * @return void
     */
    public function preDispatch(Zend_Controller_Request_Abstract $request)
    {
        return true;
        $oAuth = Zend_Auth::getInstance();

        $acl = new Zend_Acl();
        $acl->addRole('public');
        $acl->addRole('private', array('public'));

        $role = 'usucpf_';
        $acl->addRole($role, array('private'));

        $sResource = $request->module . '/' . $request->controller . '/' . $request->action;

        $resource = 'seguranca/usuario/formulario';

        $acl->addResource(new Zend_Acl_Resource($resource));
        $acl->allow($role, $resource);

        if ($acl->isAllowed('public', $sResource)) {
            ver('publico', d);
            return true;
        }

        if (!$acl->isAllowed($role, $sResource)) {
            ver('não permitido', d);
        }

        ver($_SESSION,$acl, $oAuth->hasIdentity(), $request, $oAuth, d);
        return true;
    }

}
