<?php
/**
 * Abstract class for extension
 */
require_once 'Zend/View/Helper/FormElement.php';


class Simec_View_Helper_FormInput extends Simec_View_Helper_FormElement
{
    public function formInput($name, $label = null, $value = null, $attribs = null, $config = array())
    {
        $id = isset($attribs['id']) ? $attribs['id'] : $name;
        $type = isset($attribs['type']) ? $attribs['type'] : 'text';
        $required = is_array($config) && in_array('required', $config) ? 'required="required"' : '';
        $class = isset($attribs['class']) ? 'form-control ' .  $attribs['class'] : 'form-control';
        $config['label-for'] = isset($config['label-for']) ? $config['label-for'] : $id;

        unset($attribs['id'], $attribs['type'], $attribs['class']);

        // Construindo o input
        $xhtml = '<input'
                . ' name="' . $this->view->escape($name) . '"'
                . ' id="' . $this->view->escape($id) . '"'
                . ' type="' . $type . '"'
                . ' value="' . $value . '"'
                . ' class="' . $class . '"'
                . ' ' . $required . ' '
                . $this->_htmlAttribs($attribs)
                . " />";

        return $this->buildField($xhtml, $label, $config);
    }
}
