<?php

/**
 * Class Ted_Form_Abstract
 * @author: Lucas Gomes
 */
abstract class Ted_Form_Abstract extends Zend_Form
{
    /**
     * @var string
     */
    private $_encoding = 'ISO-8859-1';

    /**
     * @var Zend_View
     */
    private $view = null;

    /**
     *
     */
    const PATH_VIEW_FORM = 'ted/modulos/principal/forms';

    /**
     * @param null $options
     */
    public function __construct($options = null)
    {
        parent::__construct($options);
        $this->init();
    }

    /**
     *
     */
    public function init()
    {
        parent::init();
        $this->view = new Zend_View();
        //$this->_loadDefaultSets();
    }

    /**
     *
     */
    protected function _loadDefaultSets()
    {
        $this->view->setEncoding($this->_encoding);
        $this->view->setScriptPath(APPRAIZ . self::PATH_VIEW_FORM);

        foreach ($this as $item) {
            $item->setView($this->view);
        }
    }

    /**
     * @return string
     */
    public function showForm()
    {
        return $this->render($this->view);
    }

    /**
     * @return bool|string
     */
    public function getEncoding()
    {
        if ($this->_encoding) {
            return $this->_encoding;
        }

        return false;
    }
}